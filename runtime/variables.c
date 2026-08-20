#include "runtime.h"
#include "stb_ds.h"
#include <math.h>
#include <stdio.h>

cb_var_key *cb_variables = NULL;

void cb_clear_variables(void) {
    shfree(cb_variables);
}

void cb_variable_assign_null(const char * key) {
    printf("[debug] assigning null to %s\n",key);

    cb_var_value v;
    v.type = CB_VAR_NULL;
    v.var.null = NULL;
    shput(cb_variables, key, v);
};
void cb_variable_assign_number(const char * key, double val) {
    printf("[debug] assigning %0.2f to %s\n", val, key);

    cb_var_value v;
    v.type = CB_VAR_NUMBER;
    v.var.number = val;
    shput(cb_variables, key, v);
};
void cb_variable_assign_boolean(const char * key, bool val) {
    printf("[debug] assigning %d to %s\n", val, key);

    cb_var_value v;
    v.type = CB_VAR_BOOLEAN;
    v.var.boolean = val;
    shput(cb_variables, key, v);
};
void cb_variable_assign_string(const char * key, const char * val) {
    printf("[debug] assigning %s to %s\n", val, key);

    cb_var_value v;
    v.type = CB_VAR_STRING;
    v.var.string = val;
    shput(cb_variables, key, v);
};

void cb_val_as_string(cb_var_value val, char * strbuf) {
    switch(val.type) {
    case CB_VAR_NULL:
        snprintf(strbuf, 1024, "(null)");
        break;
    case CB_VAR_NUMBER:
        snprintf(strbuf, 1024, "%0.2f", val.var.number);
        break;
    case CB_VAR_BOOLEAN:
        snprintf(strbuf, 1024, "%s", val.var.boolean ? "true" : "false");
        break;
    case CB_VAR_STRING:
        snprintf(strbuf, 1024, "%s", val.var.string);
        break;
    }
};

double cb_val_as_number(cb_var_value val) {
  switch (val.type) {
  case CB_VAR_NULL:
      return FP_NAN;
      break;
  case CB_VAR_NUMBER:
      return val.var.number;
  case CB_VAR_BOOLEAN:
      return val.var.boolean ? 1.0f : 0.0f;
  case CB_VAR_STRING:
      return atof(val.var.string);
  }
};

bool cb_val_as_boolean(cb_var_value val) {
  switch (val.type) {
  case CB_VAR_NULL:
      return false;
  case CB_VAR_NUMBER:
      return val.var.number != 0.0f;
  case CB_VAR_BOOLEAN:
      return val.var.boolean;
  case CB_VAR_STRING:
      return strcmp(val.var.string, "true") == 0;
  }
};

bool cb_eval_variable_true(const char *key) {
    if(shgeti(cb_variables, key) == -1) {
        return false;
    } else {
        return cb_val_as_boolean(shget(cb_variables, key)) == true;
    }
};
bool cb_eval_variable_false(const char *key) {
    if(shgeti(cb_variables, key) == -1) {
        return false;
    } else {
        return cb_val_as_boolean(shget(cb_variables, key)) == false;
    }
};
