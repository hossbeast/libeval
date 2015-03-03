#ifndef _EVAL_ERRTAB_H
#define _EVAL_ERRTAB_H
#include "xapi.h"
#define ETABLE_EVAL \
_E(1, DLERROR, "dynamic loader error") \
_E(2, FTWERROR, "filesystem traversal error") \
_E(3, LESS, "less data available than was expected") \

enum {
#define _E(a, b, c) EVAL_ ## b = a,
ETABLE_EVAL
#undef _E
};
#define ERRMIN_EVAL 1
#define ERRMAX_EVAL 3
extern etable * perrtab_EVAL;
#endif
