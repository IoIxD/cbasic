#include "compiler.hpp"
#include <format>

std::vector<std::string> Compiler::mLinkedFunctions = {
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

void Compiler::push_header(std::vector<Literal> literals) {
    mInstructions.push_back("\t.global main");

    mInstructions.push_back("\n\t.data");
    for(auto lit : literals) {
        switch(lit.type()) {
            case Literal::Type::String:
                mInstructions.push_back(std::format("\n{}:\n\t.byte {}, 0",
                    translate_string(lit.get_string()),
                    escape_string(lit.get_string()))
                );
                break;
        }
    }

    mInstructions.push_back("");

    for(auto func : mLinkedFunctions) {
        mInstructions.push_back(std::format(".extern {}",func));
    }

    mInstructions.push_back("\n\t.text");
}

void Compiler::line_num_header(int linenum) {
    if(linenum != -1) {
        if(mCurSubroutine.has_value()) {
            mInstructions.push_back(std::format("\n__ABASIC_SUB_{}_LINE_{}:", mCurSubroutine.value(), linenum));
        } else {
            mInstructions.push_back(std::format("\n__ABASIC_LINE_{}:", linenum));
        }
    }
};

void Compiler::push_main_start() {
    mInstructions.push_back("\nmain:");
    mInstructions.push_back("\tpush rbp");
    mInstructions.push_back("\tmov rbp, rsp");
    mInstructions.push_back("\tsub rsp, 32");
}

void Compiler::push_print(int linenum, std::string msg) {
    line_num_header(linenum);

    mInstructions.push_back("\tmov rax, 1"); // sys_write
    mInstructions.push_back("\tmov rdi, 1"); // stdout
    mInstructions.push_back(std::format("\tlea rsi, [rip + {}]", translate_string(msg))); // msg
    mInstructions.push_back(std::format("\tmov rdx, {}", msg.length())); // len
    mInstructions.push_back("\tsyscall");
};

void Compiler::push_goto(int linenum, int gotonum) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tjmp __ABASIC_LINE_{}", gotonum));
};

void Compiler::push_subroutine_def(std::string def) {
    mCurSubroutine = def;
    // mInstructions.push_back(std::format("jmp __ABASIC_SUB_{}_END", def));
    mInstructions.push_back(std::format("\n__ABASIC_SUB_{}:", def));
    mInstructions.push_back("\tpush rbp");
    mInstructions.push_back("\tmov rbp, rsp");
    mInstructions.push_back("\tsub rsp, 32");
};
void Compiler::push_subroutine_ret(std::string def) {
    mCurSubroutine = {};
    mInstructions.push_back("\tmov rsp, rbp");
    mInstructions.push_back("\tpop rbp");
    mInstructions.push_back("\tret");
    // mInstructions.push_back(std::format("__ABASIC_SUB_{}_END:", def));
};

void Compiler::push_goto_sub(int linenum, std::string sub) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tcall __ABASIC_SUB_{}", sub));
};

void Compiler::push_clear(int linenum) {
    line_num_header(linenum);

    mInstructions.push_back("\tcall cb_clear_variables");
};


void Compiler::push_assign_null(int linenum, std::string key) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key)));
    mInstructions.push_back("\tcall cb_variable_assign_null");
};

void Compiler::push_assign_number(int linenum, std::string key, double val) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key)));
    mInstructions.push_back(std::format("\tmov rax, 0x{:016X}", std::bit_cast<uint64_t>(val)));
    mInstructions.push_back("\tmovq xmm0, rax");
    mInstructions.push_back("\tcall cb_variable_assign_number");
};

void Compiler::push_assign_boolean(int linenum, std::string key, bool val) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key)));
    mInstructions.push_back(std::format("\tmov rsi, {}", val ? 1 : 0));
    mInstructions.push_back("\tcall cb_variable_assign_boolean");
};

void Compiler::push_assign_string(int linenum, std::string key, std::string val) {
    line_num_header(linenum);

    mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key)));
    mInstructions.push_back(std::format("\tlea rsi, [rip + {}]", translate_string(key)));
    mInstructions.push_back("\tcall cb_variable_assign_string");
};

void Compiler::push_bool_eval_goto_code(std::string key1, std::string key2, int gotonum, std::string def, std::string cmpfunc, bool isgoto, bool issingular) {
    if(isgoto) {
        mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key1)));
        if(!issingular) mInstructions.push_back(std::format("\tlea rsi, [rip + {}]", translate_string(key2)));
        mInstructions.push_back(std::format("\tcall {}",cmpfunc));
        mInstructions.push_back("\ttest rax, rax");
        mInstructions.push_back(std::format("\tjnz __ABASIC_LINE_{}", gotonum));
    } else {
        std::string skipline = std::format("__line_{}_skip",mDummyCounter++);

        mInstructions.push_back(std::format("\tlea rdi, [rip + {}]", translate_string(key1)));
        if(!issingular) mInstructions.push_back(std::format("\tlea rsi, [rip + {}]", translate_string(key2)));
        mInstructions.push_back(std::format("\tcall {}",cmpfunc));
        mInstructions.push_back("\ttest al, al");
        mInstructions.push_back(std::format("\tjz {}",skipline));
        mInstructions.push_back(std::format("\tcall __ABASIC_SUB_{}", def));
        mInstructions.push_back(std::format("{}:",skipline));
    }
};

void Compiler::push_bool_eval_goto_if_true(int linenum, std::string key, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, "", gotonum, "", "cb_eval_variable_true", true, true);
};

void Compiler::push_bool_eval_goto_sub_if_true(int linenum, std::string key, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, "", -1, def, "cb_eval_variable_true", false, true);
};

void Compiler::push_bool_eval_goto_if_false(int linenum, std::string key, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, "", gotonum, "", "cb_eval_variable_false", false, true);
};

void Compiler::push_bool_eval_goto_sub_if_false(int linenum, std::string key, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, "", -1, def, "cb_eval_variable_false", false, true);
};

void Compiler::push_bool_eval_goto_if_eq(int linenum, std::string key, std::string key2, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, gotonum, "", "cb_eval_variable_eq", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_eq(int linenum, std::string key, std::string key2, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, -1, def, "cb_eval_variable_eq", false, false);
};


void Compiler::push_bool_eval_goto_if_gt(int linenum, std::string key, std::string key2, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, gotonum, "", "cb_eval_variable_gt", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_gt(int linenum, std::string key, std::string key2, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, -1, def, "cb_eval_variable_gt", false, false);
};

void Compiler::push_bool_eval_goto_if_lt(int linenum, std::string key, std::string key2, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, gotonum, "", "cb_eval_variable_lt", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_lt(int linenum, std::string key, std::string key2, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, -1, def, "cb_eval_variable_lt", false, false);
};
void Compiler::push_bool_eval_goto_if_ge(int linenum, std::string key, std::string key2, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, gotonum, "", "cb_eval_variable_ge", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_ge(int linenum, std::string key, std::string key2, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, -1, def, "cb_eval_variable_ge", false, false);
};

void Compiler::push_bool_eval_goto_if_le(int linenum, std::string key, std::string key2, int gotonum) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, gotonum, "", "cb_eval_variable_le", true, false);
};

void Compiler::push_bool_eval_goto_sub_if_le(int linenum, std::string key, std::string key2, std::string def) {
    line_num_header(linenum);
    push_bool_eval_goto_code(key, key2, -1, def, "cb_eval_variable_le", false, false);
};


void Compiler::push_footer(){
    mInstructions.push_back("");
    mInstructions.push_back("\tmov rsp, rbp");
    mInstructions.push_back("\tpop rbp");
    mInstructions.push_back("\tmov rax, 60"); // sys_exit
    mInstructions.push_back("\tmov rdi, 0"); // success
    mInstructions.push_back("\tsyscall");
}
