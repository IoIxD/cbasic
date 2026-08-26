#include "cb_core.h"
#include "stb_ds.h"
#include <stdlib.h>

void cb_create_array(const char *key, size_t sz) {
  cb_var_value v;
  cb_array *arr = malloc(sizeof(cb_array));
  arr->elements = malloc(sz * sizeof(cb_var_value));
  arr->len = sz;

  v.type = CB_VAR_ARRAY;
  v.var.array = arr;
  shput(cb_variables, key, v);
};
