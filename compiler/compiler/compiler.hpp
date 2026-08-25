#pragma once

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Literal {
public:
  enum class Type { String, Array };

private:
  union {
    std::string *str;
    size_t arr;
  } mInner;
  Type mType;

public:
  Literal(std::string st)
      : mInner({.str = new std::string(st)}), mType(Type::String) {};
  Literal(size_t st) : mInner({.arr = st}), mType(Type::Array) {};

  std::string get_string() { return *mInner.str; }
  size_t get_arr() { return mInner.arr; }

  Type type() { return mType; }
};

class Compiler {
  std::unique_ptr<llvm::LLVMContext> mLLVM;
  std::unique_ptr<llvm::IRBuilder<>> mIRBuilder;
  std::unique_ptr<llvm::Module> mModule;

  std::unordered_map<std::string, llvm::Function *> mFunctions;
  std::unordered_map<std::string, llvm::BasicBlock *> mLabels;

  std::optional<std::string> mCurSubroutine = {};
  llvm::BasicBlock *mCurInsertPoint = nullptr;
  llvm::Function *mCurFunction = nullptr;

  int mDummyCounter = 0;

  std::string label_name(int linenum);
  void line_num_header(int linenum);
  void line_num_footer(int linenum);

  void push_bool_eval_goto_code(std::string key, std::string key2, int gotonum,
                                std::string def, std::string cmpfunc,
                                bool isgoto, bool issingular);

public:
  Compiler();

  void push_main_start();
  void push_main_end();

  /* functions */

  void push_subroutine_def(std::string def);
  void push_subroutine_ret(std::string def);
  void push_goto(int linenum, int gotonum);
  void push_goto_sub(int linenum, std::string sub);

  /* variables */

  void push_clear(int linenum);
  void push_assign_null(int linenum, std::string key);
  void push_assign_number(int linenum, std::string key, double number);
  void push_assign_boolean(int linenum, std::string key, bool boolean);
  void push_assign_string(int linenum, std::string key, std::string str);

  /* comparisons */

  void push_bool_eval_goto_if_true(int linenum, std::string key, int gotonum);
  void push_bool_eval_goto_if_false(int linenum, std::string key, int gotonum);
  void push_bool_eval_goto_sub_if_true(int linenum, std::string key,
                                       std::string def);
  void push_bool_eval_goto_sub_if_false(int linenum, std::string key,
                                        std::string def);
  void push_bool_eval_goto_if_eq(int linenum, std::string key, std::string key2,
                                 int gotonum);
  void push_bool_eval_goto_sub_if_eq(int linenum, std::string key,
                                     std::string key2, std::string def);
  void push_bool_eval_goto_if_gt(int linenum, std::string key, std::string key2,
                                 int gotonum);
  void push_bool_eval_goto_sub_if_gt(int linenum, std::string key,
                                     std::string key2, std::string def);
  void push_bool_eval_goto_if_lt(int linenum, std::string key, std::string key2,
                                 int gotonum);
  void push_bool_eval_goto_sub_if_lt(int linenum, std::string key,
                                     std::string key2, std::string def);
  void push_bool_eval_goto_if_ge(int linenum, std::string key, std::string key2,
                                 int gotonum);
  void push_bool_eval_goto_sub_if_ge(int linenum, std::string key,
                                     std::string key2, std::string def);
  void push_bool_eval_goto_if_le(int linenum, std::string key, std::string key2,
                                 int gotonum);
  void push_bool_eval_goto_sub_if_le(int linenum, std::string key,
                                     std::string key2, std::string def);

  /* other */

  void push_print(int linenum, std::string msg);

  const std::vector<std::string> linkedFunctions = {
      "cb_clear_variables",
      "cb_variable_assign_null",
      "cb_variable_assign_number",
      "cb_variable_assign_boolean",
      "cb_variable_assign_string",
      "cb_eval_variable_true",
      "cb_eval_variable_false",
      "cb_eval_variable_eq",
      "cb_eval_variable_gt",
      "cb_eval_variable_lt",
      "cb_eval_variable_ge",
      "cb_eval_variable_le",
  };

  void finish();

  // template <typename... Args> void append_instructions(Args &&...args) {
  // (mInstructions.push_back(std::forward<Args>(args)), ...);
  // }
};
