#include "compiler/compiler.hpp"
#include "lexer/lexer.hpp"

int main() {
  Compiler compiler;
  Lexer lexer;

  compiler.push_subroutine_def("true_func");
  compiler.push_print(-1, "Comparison was true!");
  compiler.push_subroutine_ret("true_func");

  compiler.push_subroutine_def("false_func");
  compiler.push_print(-1, "Comparison was false (expected)!");
  compiler.push_subroutine_ret("false_func");

  compiler.push_subroutine_def("tests_func");
  compiler.push_print(-1, "number1 == number2?");
  compiler.push_bool_eval_goto_sub_if_eq(-1, "number1", "number2", "true_func");
  compiler.push_print(-1, "number1 > number2?");
  compiler.push_bool_eval_goto_sub_if_gt(-1, "number1", "number2", "true_func");
  compiler.push_print(-1, "number1 < number2?");
  compiler.push_bool_eval_goto_sub_if_lt(-1, "number1", "number2", "true_func");
  compiler.push_print(-1, "number1 >= number2?");
  compiler.push_bool_eval_goto_sub_if_ge(-1, "number1", "number2", "true_func");
  compiler.push_print(-1, "number1 <= number2?");
  compiler.push_bool_eval_goto_sub_if_le(-1, "number1", "number2", "true_func");
  compiler.push_subroutine_ret("tests_func");

  compiler.push_main_start();

  compiler.push_assign_boolean(-1, "bool_test", true);
  compiler.push_bool_eval_goto_sub_if_true(-1, "bool_test", "true_func");

  compiler.push_assign_boolean(-1, "bool_test", false);
  compiler.push_bool_eval_goto_sub_if_false(-1, "bool_test", "false_func");

  compiler.push_assign_number(-1, "number1", 1.01);
  compiler.push_assign_number(-1, "number2", 0);
  compiler.push_goto_sub(-1, "tests_func");

  compiler.push_assign_number(-1, "number1", 0);
  compiler.push_assign_number(-1, "number2", 2);
  compiler.push_goto_sub(-1, "tests_func");

  compiler.push_main_end();

  compiler.finish();

  return 0;
}
