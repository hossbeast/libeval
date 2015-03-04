/* Copyright (c) 2015 Todd Freed <todd.freed@gmail.com>

   This file is part of libeval.
   
   libeval is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   libeval is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with libeval.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _EVAL_EXPR_H
#define _EVAL_EXPR_H

#include <stdint.h>

#define restrict __restrict

#define EVAL_EXPR_INSPECT_ARGSSZ		1				/* number of parameters, ptr is pointer to int */
#define EVAL_EXPR_INSPECT_ISNATIVE	2				/* whether the expression was compiled, ptr is pointer to int */
#define EVAL_EXPR_INSPECT_FNPTR			3				/* pointer to the compiled function, ptr is pointer to function */

/// expr_inspect
//
// SUMMARY
//  interrogate an expression that has been prepared with eval_prepare
//
// PARAMETERS
//  ex  - 
//  cmd - 
//  ptr - 
//
// COMMANDS
//  
//
int eval_expr_inspect(eval_expr * const restrict ex, int cmd, void * ptr)
	__attribute__((nonnull));

#undef restrict
#endif
