#ifndef __CB_core_H__
#define __CB_core_H__

#include <stdbool.h>
#include <stddef.h>

typedef enum cb_var_type_t {
  CB_VAR_NULL = 0,
  CB_VAR_NUMBER,
  CB_VAR_BOOLEAN,
  CB_VAR_STRING,
  CB_VAR_ARRAY,
} cb_var_type;

typedef struct cb_var_value_t {
  union {
    void *null;
    double number;
    bool boolean;
    char *string;
    struct cb_array_t *array;
  } var;
  cb_var_type type;
} cb_var_value;

typedef struct cb_var_key_t {
  const char *key;
  cb_var_value value;
} cb_var_key;

typedef struct cb_array_t {
  cb_var_value *elements;
  size_t len;
} cb_array;

void cb_clear_variables(void);
void cb_variable_assign_null(const char *key);
void cb_variable_assign_number(const char *key, double val);
void cb_variable_assign_boolean(const char *key, bool val);
void cb_variable_assign_string(const char *key, const char *val);

bool cb_eval_variable_true(const char *key);
bool cb_eval_variable_false(const char *key);
bool cb_eval_variable_eq(const char *key, const char *key2);
bool cb_eval_variable_gt(const char *key, const char *key2);
bool cb_eval_variable_lt(const char *key, const char *key2);
bool cb_eval_variable_ge(const char *key, const char *key2);
bool cb_eval_variable_le(const char *key, const char *key2);

void cb_val_as_string(cb_var_value val, char *strbuf);
double cb_val_as_number(cb_var_value val);
bool cb_val_as_boolean(cb_var_value val);

void cb_create_array(const char *key, size_t sz);

extern cb_var_key *cb_variables;
#endif
