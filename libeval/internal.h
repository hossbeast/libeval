/* Copyright (c) 2015 Todd Freed <todd.freed@gmail.com>

   This file is part of fab.
   
   fab is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   fab is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with fab.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _EVAL_INTERNAL_H
#define _EVAL_INTERNAL_H

// api
#include "eval.h"
#include "compile/compile.h"

// internal
#include "compile/x86_64.h"
#include "ast/ast.h"
#include "ast/ast.parse.h"
#include "error/EVAL.errtab.h"
#include "storage/storage.internal.h"

#if XAPI_PROVIDE_BACKTRACE
#include "error/EVAL.errtab.h"
#endif

#define API __attribute__((visibility("protected")))
#define APIDATA

#undef restrict
#endif
