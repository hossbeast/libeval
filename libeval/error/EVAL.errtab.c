#include "EVAL.errtab.h"
#define APIDATA
etable errtab_EVAL = {
   .tag = "EVAL"
 , .min = ERRMIN_EVAL
 , .max = ERRMAX_EVAL
 , .v =  (typeof(((etable*)0)->v[0])[]) {
#define _E(a, b, c) [ a + (ERRMIN_EVAL * -1) ] = { .name = #b, .desc = c },
ETABLE_EVAL
#undef _E
}};
etable * APIDATA perrtab_EVAL = &errtab_EVAL;
