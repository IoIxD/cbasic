#include "compiler.hpp"
#include <format>

void Compiler::push_clear(int linenum) {
  line_num_header(linenum);

  append_instructions("\tcall cb_clear_variables");
};

void Compiler::push_assign_null(int linenum, std::string key) {
  line_num_header(linenum);

  append_instructions(
      std::format("\tlea rdi, [rip + {}]", translate_string(key)),
      "\tcall cb_variable_assign_null");
};

void Compiler::push_assign_number(int linenum, std::string key, double val) {
  line_num_header(linenum);

  append_instructions(
      std::format("\tlea rdi, [rip + {}]", translate_string(key)),
      std::format("\tmov rax, 0x{:016X}", std::bit_cast<uint64_t>(val)),
      "\tmovq xmm0, rax",
      "\tcall cb_variable_assign_number");
};

void Compiler::push_assign_boolean(int linenum, std::string key, bool val) {
  line_num_header(linenum);

  append_instructions(
      std::format("\tlea rdi, [rip + {}]", translate_string(key)),
      std::format("\tmov rsi, {}", val ? 1 : 0),
      "\tcall cb_variable_assign_boolean");
};

void Compiler::push_assign_string(int linenum, std::string key,
                                  std::string val) {
  line_num_header(linenum);

  append_instructions(
      std::format("\tlea rdi, [rip + {}]", translate_string(key)),
      std::format("\tlea rsi, [rip + {}]", translate_string(key)),
      "\tcall cb_variable_assign_string");
};
