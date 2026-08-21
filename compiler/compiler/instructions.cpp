#include "compiler.hpp"
#include <format>

void Compiler::push_header(std::vector<Literal> literals) {
  append_instructions("\t.global main", "", "\t.data");
  for (auto lit : literals) {
    switch (lit.type()) {
    case Literal::Type::String:
      append_instructions(
          std::format("\n{}:", translate_string(lit.get_string())),
          std::format("\t.byte {}, 0", escape_string(lit.get_string()))
      );
      break;
    }
  }

  std::vector<std::string> linkedFunctions = {
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

  for (auto func : linkedFunctions) {
    mInstructions.push_back(std::format(".extern {}", func));
  }

  append_instructions("\n\t.text");
}

void Compiler::line_num_header(int linenum) {
  if (linenum != -1) {
    append_instructions(
        (mCurSubroutine.has_value())
            ? std::format(
                  "\n__ABASIC_SUB_{}_LINE_{}:", mCurSubroutine.value(), linenum
              )
            : std::format("\n__ABASIC_LINE_{}:", linenum)
    );
  }
};

void Compiler::push_main_start() {
  append_instructions(
      "\nmain:", "\tpush rbp", "\tmov rbp, rsp", "\tsub rsp, 32"
  );
}

void Compiler::push_print(int linenum, std::string msg) {
  line_num_header(linenum);

  append_instructions(
      "\tmov rax, 1",                                              // sys_write
      "\tmov rdi, 1",                                              // stdout
      std::format("\tlea rsi, [rip + {}]", translate_string(msg)), // msg
      std::format("\tmov rdx, {}", msg.length()),                  // len
      "\tsyscall"
  );
};

void Compiler::push_goto(int linenum, int gotonum) {
  line_num_header(linenum);

  append_instructions(std::format("\tjmp __ABASIC_LINE_{}", gotonum));
};

void Compiler::push_subroutine_def(std::string def) {
  mCurSubroutine = def;
  append_instructions(
      std::format("\n__ABASIC_SUB_{}:", def),
      "\tpush rbp",
      "\tmov rbp, rsp",
      "\tsub rsp, 32"
  );
};
void Compiler::push_subroutine_ret(std::string def) {
  mCurSubroutine = {};
  append_instructions("\tmov rsp, rbp", "\tpop rbp", "\tret");
};

void Compiler::push_goto_sub(int linenum, std::string sub) {
  line_num_header(linenum);

  append_instructions(std::format("\tcall __ABASIC_SUB_{}", sub));
};

void Compiler::push_footer() {
  append_instructions(
      "\tmov rsp, rbp", //
      "\tpop rbp",      //
      "\tmov rax, 60",  // sys_exit
      "\tmov rdi, 0",   // success
      "\tsyscall"
  );
}
