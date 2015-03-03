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

#ifndef _EVAL_COMPILE_H
#define _EVAL_COMPILE_H

#include <stdint.h>

#define restrict __restrict

/// eval_compile
//
// SUMMARY
//
// PARAMETERS
//  es - 
//  e  - 
//  fn - 
//
int eval_compile(eval_storage * const restrict es, char * const restrict e, void * restrict fn)
	__attribute__((nonnull));

#endif
