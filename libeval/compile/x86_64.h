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

#ifndef _EVAL_X86_64_H
#define _EVAL_X86_64_H

#include <stdint.h>

struct eval_storage;		// storage.h
struct ast;							// ast.h

#define restrict __restrict

/// x86_64_compile
//
// SUMMARY
//
// PARAMETERS
//  es - 
//  as - 
//  fn - 
//
int x86_64_compile(struct eval_storage * const restrict es, const struct ast * const restrict as, void * restrict fn)
	__attribute__((nonnull));

#endif
