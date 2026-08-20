#pragma once

#include <optional>
#include <string>
#include <vector>

class Literal {
public:
    enum class Type {
        String
    };
private:
    union {
        std::string *str;
    } mInner;
    Type mType;
public:
    Literal(std::string st): mInner({.str = new std::string(st)}), mType(Type::String) {};

    std::string get_string() {
        return *mInner.str;
    }
    Type type() {
        return mType;
    }
};


class Compiler {
    std::vector<std::string> mInstructions;
    std::optional<std::string> mCurSubroutine;

    std::string translate_string(std::string str);
    std::string escape_string(std::string str);

    void line_num_header(int linenum);

    static std::vector<std::string> mLinkedFunctions;

    int mDummyCounter = 0;

    void push_bool_eval_goto_code(std::string key, std::string key2, int gotonum, std::string def, std::string cmpfunc, bool isgoto, bool issingular);

public:
    void push_header(std::vector<Literal> literals);

    void push_main_start();

    void push_print(int linenum, std::string msg);
    void push_goto(int linenum, int gotonum);

    void push_subroutine_def(std::string def);
    void push_subroutine_ret(std::string def);
    void push_goto_sub(int linenum, std::string sub);

    void push_clear(int linenum);
    void push_assign_null(int linenum, std::string key);
    void push_assign_number(int linenum, std::string key, double number);
    void push_assign_boolean(int linenum, std::string key, bool boolean);
    void push_assign_string(int linenum, std::string key, std::string str);

    void push_bool_eval_goto_if_true(int linenum, std::string key, int gotonum);
    void push_bool_eval_goto_if_false(int linenum, std::string key, int gotonum);
    void push_bool_eval_goto_sub_if_true(int linenum, std::string key, std::string def);
    void push_bool_eval_goto_sub_if_false(int linenum, std::string key, std::string def);
    void push_bool_eval_goto_if_eq(int linenum, std::string key, std::string key2, int gotonum);
    void push_bool_eval_goto_sub_if_eq(int linenum, std::string key, std::string key2, std::string def);
    void push_bool_eval_goto_if_gt(int linenum, std::string key, std::string key2, int gotonum);
    void push_bool_eval_goto_sub_if_gt(int linenum, std::string key, std::string key2, std::string def);
    void push_bool_eval_goto_if_lt(int linenum, std::string key, std::string key2, int gotonum);
    void push_bool_eval_goto_sub_if_lt(int linenum, std::string key, std::string key2, std::string def);
    void push_bool_eval_goto_if_ge(int linenum, std::string key, std::string key2, int gotonum);
    void push_bool_eval_goto_sub_if_ge(int linenum, std::string key, std::string key2, std::string def);
    void push_bool_eval_goto_if_le(int linenum, std::string key, std::string key2, int gotonum);
    void push_bool_eval_goto_sub_if_le(int linenum, std::string key, std::string key2, std::string def);


    void push_footer();

    void print();
    void assemble();


};
