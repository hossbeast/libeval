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

#ifndef _EVAL_H
#define _EVAL_H

#include <stdint.h>
#include <sys/types.h>

/*
** 
*/
struct eval_storage;
typedef struct eval_storage eval_storage;

/*
**
*/
struct eval_expr;
typedef struct eval_expr eval_expr;

typedef struct val
{
	uint8_t t;
	union
	{
		int32_t		i;
		float			f;
	};
} val;

#define restrict __restrict

/// eval_storage_alloc
//
// SUMMARY
//  allocate an eval_storage
//
int eval_storage_alloc(eval_storage ** const restrict es)
	__attribute__((nonnull));

void eval_storage_free(eval_storage * es);
void eval_storage_xfree(eval_storage ** const restrict es)
	__attribute__((nonnull));

/// eval_prepare
//
// SUMMARY
//  prepare an expression
//
// PARAMETERS
//  es - an eval_storage
//  e  - expression
//  ex - (returns) parsed expression
//
int eval_prepare(eval_storage * restrict es, char * const restrict e, eval_expr ** const restrict ex)
	__attribute__((nonnull));

/// eval_exec
//
// SUMMARY
//  evaluate an expression
//
// PARAMETERS
//  ex     - 
//  [avec] - 
//  alen   - 
//  rv     - (returns) 
//
int eval_exec(eval_expr * const restrict ex, val * const avec, size_t alen, val * const rv)
	__attribute__((nonnull));

#undef restrict
#endif
