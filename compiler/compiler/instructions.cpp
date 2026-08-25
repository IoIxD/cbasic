#include "compiler.hpp"
#include <format>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

std::string Compiler::label_name(int linenum) {
  std::string labelName;
  if (mCurSubroutine.has_value()) {
    labelName =
        std::format("CBASIC_SUB_{}_LBL_{}", mCurSubroutine.value(), linenum);
  } else {
    labelName = std::format("CBASIC_LBL_{}", linenum);
  }
  return labelName;
}

void Compiler::line_num_header(int linenum) {
  if (linenum != -1) {
    auto lblName = label_name(linenum);
    auto label = llvm::BasicBlock::Create(*mLLVM, "label", mCurFunction);
    mLabels.insert_or_assign(lblName, label);
    mIRBuilder->CreateBr(label);
    mIRBuilder->SetInsertPoint(label);
  }
};

void Compiler::line_num_footer(int linenum) {
  if (linenum != -1) {
    // mTrailingLabel = mLabels[label_name(linenum)];
  }
}

void Compiler::push_main_start() {
  auto prototype = llvm::FunctionType::get(mIRBuilder->getInt32Ty(), false);
  mCurFunction = llvm::Function::Create(
      prototype, llvm::Function::ExternalLinkage, "main", mModule.get());
  llvm::BasicBlock *body =
      llvm::BasicBlock::Create(*mLLVM, "body", mCurFunction);
  mIRBuilder->SetInsertPoint(body);
}

void Compiler::push_main_end() {
  mIRBuilder->CreateRet(llvm::ConstantInt::get(mIRBuilder->getInt32Ty(), 0));
}

void Compiler::push_print(int linenum, std::string msg) {
  line_num_header(linenum);

  auto str = mIRBuilder->CreateGlobalStringPtr(msg, "", 0, mModule.get());
  mIRBuilder->CreateCall(mFunctions["puts"], {str});

  line_num_footer(linenum);
};

void Compiler::push_subroutine_def(std::string def) {
  mCurSubroutine = def;
  auto prototype = llvm::FunctionType::get(mIRBuilder->getVoidTy(), false);
  mCurFunction = llvm::Function::Create(
      prototype, llvm::Function::InternalLinkage, def, mModule.get());
  mCurInsertPoint = llvm::BasicBlock::Create(*mLLVM, def, mCurFunction);
  mIRBuilder->SetInsertPoint(mCurInsertPoint);
};
void Compiler::push_subroutine_ret(std::string def) {
  mCurSubroutine = {};
  mIRBuilder->CreateRetVoid();
  mIRBuilder->ClearInsertionPoint();
};

void Compiler::push_goto(int linenum, int gotonum) {
  line_num_header(linenum);

  mIRBuilder->CreateBr(mLabels[label_name(gotonum)]);

  line_num_footer(linenum);
};

void Compiler::push_goto_sub(int linenum, std::string sub) {
  line_num_header(linenum);

  auto prototype = llvm::FunctionType::get(mIRBuilder->getVoidTy(), false);
  auto fn = llvm::Function::Create(
      prototype, llvm::GlobalValue::InternalLinkage, sub);
  mIRBuilder->CreateCall(fn);

  line_num_footer(linenum);
};
