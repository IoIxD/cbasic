#include "compiler.hpp"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>

void Compiler::push_clear(int linenum) {
  line_num_header(linenum);

  mIRBuilder->CreateCall(mFunctions["cb_clear_variables"]);
};

void Compiler::push_assign_null(int linenum, std::string key) {
  line_num_header(linenum);

  mIRBuilder->CreateCall(
      mFunctions["cb_variable_assign_null"],
      {mIRBuilder->CreateGlobalStringPtr(key, "", 0, mModule.get())});
};

void Compiler::push_assign_number(int linenum, std::string key, double val) {
  line_num_header(linenum);

  auto str = mIRBuilder->CreateGlobalStringPtr(key, "", 0, mModule.get());
  llvm::Value *v =
      llvm::ConstantFP::get(mIRBuilder->getDoubleTy(), llvm::APFloat(val));
  mIRBuilder->CreateCall(mFunctions["cb_variable_assign_number"], {str, v});
};

void Compiler::push_assign_boolean(int linenum, std::string key, bool val) {
  line_num_header(linenum);

  auto str = mIRBuilder->CreateGlobalStringPtr(key, "", 0, mModule.get());
  llvm::Value *v = llvm::ConstantInt::get(mIRBuilder->getInt1Ty(), val);
  mIRBuilder->CreateCall(mFunctions["cb_variable_assign_boolean"], {str, v});
};

void Compiler::push_assign_string(int linenum, std::string key,
                                  std::string val) {
  line_num_header(linenum);

  auto str = mIRBuilder->CreateGlobalStringPtr(key, "", 0, mModule.get());
  auto v = mIRBuilder->CreateGlobalStringPtr(val, "", 0, mModule.get());
  mIRBuilder->CreateCall(mFunctions["cb_variable_assign_string"], {str, v});
};
