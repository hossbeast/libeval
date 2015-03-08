#ifndef _EVAL_ERRTAB_H
#define _EVAL_ERRTAB_H
#include "xapi.h"
#define ETABLE_EVAL \
_E(1, SYNTAX, "syntax error") \
_E(1, ILLBYTE, "unexpected byte sequence") \

enum {
#define _E(a, b, c) EVAL_ ## b = a,
ETABLE_EVAL
#undef _E
};
#define ERRMIN_EVAL 1
#define ERRMAX_EVAL 1
extern etable * perrtab_EVAL;
#endif
