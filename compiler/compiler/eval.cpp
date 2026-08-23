#include "compiler.hpp"
#include <format>

void Compiler::push_bool_eval_goto_code(std::string key1, std::string key2,
                                        int gotonum, std::string def,
                                        std::string cmpfunc, bool isgoto,
                                        bool issingular) {
  std::string skipline = std::format("__line_{}_skip", mDummyCounter++);
  append_instructions(
      std::format("\tlea rdi, [rip + {}]", translate_string(key1)),
      issingular ? ""
                 : std::format("\tlea rsi, [rip + {}]", translate_string(key2)),
      std::format("\tcall {}", cmpfunc),
      "\ttest al, al",
      isgoto ? "" : std::format("\tjz {}", skipline),
      isgoto ? std::format("\tjnz __ABASIC_LINE_{}", gotonum)
             : std::format("\tcall __ABASIC_SUB_{}", def),
      isgoto ? "" : std::format("{}:", skipline));
};

void Compiler::push_bool_eval_goto_if_true(int linenum, std::string key,
                                           int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", gotonum, "", "cb_eval_variable_true", true, true);
};

void Compiler::push_bool_eval_goto_sub_if_true(int linenum, std::string key,
                                               std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", -1, def, "cb_eval_variable_true", false, true);
};

void Compiler::push_bool_eval_goto_if_false(int linenum, std::string key,
                                            int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", gotonum, "", "cb_eval_variable_false", false, true);
};

void Compiler::push_bool_eval_goto_sub_if_false(int linenum, std::string key,
                                                std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, "", -1, def, "cb_eval_variable_false", false, true);
};

void Compiler::push_bool_eval_goto_if_eq(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_eq", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_eq(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_eq", false, false);
};

void Compiler::push_bool_eval_goto_if_gt(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_gt", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_gt(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_gt", false, false);
};

void Compiler::push_bool_eval_goto_if_lt(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_lt", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_lt(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_lt", false, false);
};
void Compiler::push_bool_eval_goto_if_ge(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_ge", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_ge(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_ge", false, false);
};

void Compiler::push_bool_eval_goto_if_le(int linenum, std::string key,
                                         std::string key2, int gotonum) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, gotonum, "", "cb_eval_variable_le", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_le(int linenum, std::string key,
                                             std::string key2,
                                             std::string def) {
  line_num_header(linenum);
  push_bool_eval_goto_code(
      key, key2, -1, def, "cb_eval_variable_le", false, false);
};
