#include "compiler.hpp"
#include <format>

void Compiler::push_clear(int linenum) {
  line_num_header(linenum);

  append_instructions(
      "\tcall "
      "cb_"
      "clear_"
      "variabl"
      "es"
  );
};

void Compiler::push_assign_null(int linenum, std::string key) {
  line_num_header(linenum);

  append_instructions(
      std::format(
          "\tl"
          "ea "
          "rdi"
          ", "
          "[ri"
          "p "
          "+ "
          "{}"
          "]",
          translate_string(key)
      ),
      "\tcall "
      "cb_"
      "variabl"
      "e_"
      "assign_"
      "null"
  );
};

void Compiler::push_assign_number(int linenum, std::string key, double val) {
  line_num_header(linenum);

  append_instructions(
      std::format(
          "\tl"
          "ea "
          "rdi"
          ", "
          "[ri"
          "p "
          "+ "
          "{}"
          "]",
          translate_string(key)
      ),
      std::format(
          "\tm"
          "ov "
          "rax"
          ", "
          "0x{"
          ":01"
          "6X"
          "}",
          std::bit_cast<uint64_t>(val)
      ),
      "\tmovq "
      "xmm0, "
      "rax",
      "\tcall "
      "cb_"
      "variabl"
      "e_"
      "assign_"
      "numbe"
      "r"
  );
};

void Compiler::push_assign_boolean(int linenum, std::string key, bool val) {
  line_num_header(linenum);

  append_instructions(
      std::format(
          "\tl"
          "ea "
          "rdi"
          ", "
          "[ri"
          "p "
          "+ "
          "{}"
          "]",
          translate_string(key)
      ),
      std::format(
          "\tm"
          "ov "
          "rsi"
          ", "
          "{}",
          val ? 1 : 0
      ),
      "\tcall "
      "cb_"
      "variabl"
      "e_"
      "assign_"
      "boolea"
      "n"
  );
};

void Compiler::push_assign_string(
    int linenum, std::string key, std::string val
) {
  line_num_header(linenum);

  append_instructions(
      std::format(
          "\tl"
          "ea "
          "rdi"
          ", "
          "[ri"
          "p "
          "+ "
          "{}"
          "]",
          translate_string(key)
      ),
      std::format(
          "\tl"
          "ea "
          "rsi"
          ", "
          "[ri"
          "p "
          "+ "
          "{}"
          "]",
          translate_string(key)
      ),
      "\tcall "
      "cb_"
      "variabl"
      "e_"
      "assign_"
      "strin"
      "g"
  );
};
