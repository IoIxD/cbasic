#include "runtime.h"
#include "stb_ds.h"

bool cb_eval_variable_eq(const char *key, const char * key2) {
    if(shgeti(cb_variables, key) == -1 || shgeti(cb_variables, key2) == -1) {
        return false;
    } else {
        /* for an eq comparison we can just compare strings essentially */
        char *strbuf_1 = malloc(1024 + 1);
        char *strbuf_2 = malloc(1024 + 1);
        cb_var_value val1 = shget(cb_variables, key);
        cb_var_value val2 = shget(cb_variables, key2);
        cb_val_as_string(val1, strbuf_1);
        cb_val_as_string(val2, strbuf_2);
        bool v = strcmp(strbuf_1, strbuf_2) == 0;

        return v;
    }
}
bool cb_eval_variable_gt(const char *key, const char * key2) {
    if(shgeti(cb_variables, key) == -1 || shgeti(cb_variables, key2) == -1) {
        return false;
    } else {
        cb_var_value val1 = shget(cb_variables, key);
        cb_var_value val2 = shget(cb_variables, key2);
        double d1 = cb_val_as_number(val1);
        double d2 = cb_val_as_number(val2);
        return d1 > d2;
    }
}
bool cb_eval_variable_lt(const char *key, const char * key2) {
    if(shgeti(cb_variables, key) == -1 || shgeti(cb_variables, key2) == -1) {
        return false;
    } else {
        cb_var_value val1 = shget(cb_variables, key);
        cb_var_value val2 = shget(cb_variables, key2);
        double d1 = cb_val_as_number(val1);
        double d2 = cb_val_as_number(val2);
        return d1 < d2;
    }
}
bool cb_eval_variable_ge(const char *key, const char * key2) {
    if(shgeti(cb_variables, key) == -1 || shgeti(cb_variables, key2) == -1) {
        return false;
    } else {
        cb_var_value val1 = shget(cb_variables, key);
        cb_var_value val2 = shget(cb_variables, key2);
        double d1 = cb_val_as_number(val1);
        double d2 = cb_val_as_number(val2);
        return d1 >= d2;
    }
}
bool cb_eval_variable_le(const char *key, const char * key2) {
    if(shgeti(cb_variables, key) == -1 || shgeti(cb_variables, key2) == -1) {
        return false;
    } else {
        cb_var_value val1 = shget(cb_variables, key);
        cb_var_value val2 = shget(cb_variables, key2);
        double d1 = cb_val_as_number(val1);
        double d2 = cb_val_as_number(val2);
        return d1 <= d2;
    }
}
