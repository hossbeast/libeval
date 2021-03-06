/* Copyright (c) 2012-2015 Todd Freed <todd.freed@gmail.com>

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

#ifndef _AST_PARSE_H
#define _AST_PARSE_H

#include <stdio.h>
#include <stdint.h>

#include "yyutil.h"

typedef struct
{
	/* yyu-defined xtra fields */
	yyu_extra;

	char *					orig;

	ast_parser *		p;						// parser
	ast * 					g;						// result node
	char						temp[256];		// temp space
} parse_param;

// defined in ast.tab.o
int ast_yyparse(void*, parse_param*);

/// ast_yyerror
//
// SUMMARY
//  1. flag the parse as failed by (setting pp->r = 0)
//  2. write the last error to listwise_error_fd
//  3. write the last token to listwise_error_fd
//
// DETAILS
//  called from tab.o and lex.o
//
static void ast_yyerror(void* loc, void* scanner, parse_param* pp, char const* err)
	__attribute__((weakref("yyu_grammar_error")));

#endif
