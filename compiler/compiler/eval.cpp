#include "compiler.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

void Compiler::push_bool_eval_goto_code(std::string key1, std::string key2,
                                        int gotonum, std::string def,
                                        std::string cmpfunc, bool isgoto,
                                        bool issingular) {
  auto k1 = mIRBuilder->CreateGlobalStringPtr(key1, "", 0, mModule.get());
  llvm::Value *val;
  if (issingular) {
    val = mIRBuilder->CreateCall(mFunctions[cmpfunc], {k1});
  } else {
    auto k2 = mIRBuilder->CreateGlobalStringPtr(key2, "", 0, mModule.get());
    val = mIRBuilder->CreateCall(mFunctions[cmpfunc], {k1, k2});
  }

  llvm::BasicBlock *thenBB =
      llvm::BasicBlock::Create(*mLLVM, "then", mCurFunction);
  llvm::BasicBlock *elseBB =
      llvm::BasicBlock::Create(*mLLVM, "else", mCurFunction);
  llvm::BasicBlock *mergeBB =
      llvm::BasicBlock::Create(*mLLVM, "merge", mCurFunction);
  mIRBuilder->CreateCondBr(val, thenBB, elseBB);

  mIRBuilder->SetInsertPoint(thenBB);
  if (isgoto) {
    push_goto(-1, gotonum);
  } else {
    push_goto_sub(-1, def);
  }
  mIRBuilder->CreateBr(elseBB);
  mIRBuilder->SetInsertPoint(elseBB);
  mIRBuilder->CreateBr(mergeBB);
  mIRBuilder->SetInsertPoint(mergeBB);
};

void Compiler::push_bool_eval_goto_if_true(int linenum, std::string key,
                                           int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", gotonum, "", "cb_eval_variable_true", true, true);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_true(int linenum, std::string key,
                                               std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", -1, def, "cb_eval_variable_true", false, true);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_if_false(int linenum, std::string key,
                                            int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", gotonum, "", "cb_eval_variable_false", false, true);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_false(int linenum, std::string key,
                                                std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", -1, def, "cb_eval_variable_false", false, true);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_if_eq(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_eq", true, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_eq(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_eq", false, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_if_gt(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_gt", true, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_gt(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_gt", false, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_if_lt(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_lt", true, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_lt(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_lt", false, false);
  line_num_footer(linenum);
};
void Compiler::push_bool_eval_goto_if_ge(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_ge", true, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_ge(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_ge", false, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_if_le(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_le", true, false);
  line_num_footer(linenum);
};

void Compiler::push_bool_eval_goto_sub_if_le(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_le", false, false);
  line_num_footer(linenum);
};
