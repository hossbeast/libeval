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

%code top {
	#include <stdio.h>
	#include <stdint.h>
	#include <string.h>
	#include <stdlib.h>

	#include "xapi.h"

	#include "internal.h"

	// defined in ast.lex.o
	int ast_yylex(void* yylvalp, void* yylloc, void* scanner);
}

%define api.pure
%error-verbose
%locations
%name-prefix "ast_yy"
%parse-param { void* scanner }
%parse-param { parse_param* parm }
%lex-param { void* scanner }

/* zero based lines and columns */
%initial-action { memset(&@$, 0, sizeof(@$)); }

%union {
	int						operator;
	int32_t				i32;
	float					fp;
	char *				var;
	ast *					ast;
}

/* terminals */
%token <i32> INT
%token <fp> FLOAT
%token <var> VAR

%token LOGAND		"&&"
%token LOGOR		"||"
%token SHL			"<<"
%token SHR			">>"

/* nonterminals */
%type <operator> unary_operator multiplicative_operator additive_operator shift_operator relational_operator equality_operator
 /*
%type <operator> logor logand bitor bitand bitxor div eq ne ge gt le lt minus plus mod mul shl shr
 */
%type <ast> expr logical_or_expr logical_and_expr inclusive_or_expr exclusive_or_expr and_expr
%type <ast> equality_expr relational_expr shift_expr additive_expr multiplicative_expr unary_expr operand

%%
utterance
	: expr
	{
		parm->g = $1;
	}
	;

expr
	: logical_or_expr
	;

logical_or_expr
	: logical_and_expr
	| logical_or_expr LOGOR logical_and_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, AST_OPERATOR_LOGOR, $1, $3, &$$);
	}
	;

logical_and_expr
	: inclusive_or_expr
	| logical_and_expr LOGAND inclusive_or_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, AST_OPERATOR_LOGAND, $1, $3, &$$);
	}
	;

inclusive_or_expr
	: exclusive_or_expr
	| inclusive_or_expr '|' exclusive_or_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, AST_OPERATOR_BITOR, $1, $3, &$$);
	}
	;

exclusive_or_expr
	: and_expr
	| exclusive_or_expr '^' and_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, AST_OPERATOR_BITXOR, $1, $3, &$$);
	}
	;

and_expr
	: equality_expr
	| and_expr '&' equality_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, AST_OPERATOR_BITAND, $1, $3, &$$);
	}
	;

equality_expr
	: relational_expr
	| equality_expr equality_operator relational_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, $2, $1, $3, &$$);
	}
	;

relational_expr
	: shift_expr
	| relational_expr relational_operator shift_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, $2, $1, $3, &$$);
	}
	;

shift_expr
	: additive_expr
	| shift_expr shift_operator additive_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, $2, $1, $3, &$$);
	}
	;

additive_expr
	: multiplicative_expr
	| additive_expr additive_operator multiplicative_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, $2, $1, $3, &$$);
	}
	;

multiplicative_expr
	: unary_expr
	| multiplicative_expr multiplicative_operator unary_expr
	{
		YYU_FATAL(ast_mk_binary, parm->js, &@$, $2, $1, $3, &$$);
	}
	;

unary_expr
	: operand
	| unary_operator unary_expr
	{
		YYU_FATAL(ast_mk_unary, parm->js, &@$, $1, $2, &$$);
	}
	;

operand
	: '(' expr ')'
	{
		$$ = $2;
	}
	| INT
	{
		YYU_FATAL(ast_mk_int, parm->js, &@$, $1, &$$);
	}
	| FLOAT
	{
		YYU_FATAL(ast_mk_float, parm->js, &@$, $1, &$$);
	}
	| VAR
	{
		YYU_FATAL(ast_mk_var, parm->js, &@$, $1, &$$);
	}
	;

unary_operator
	: '-'
	{
		$$ = AST_OPERATOR_ADDINV;
	}
	| '~'
	{
		$$ = AST_OPERATOR_BITINV;
	}
	| '!'
	{
		$$ = AST_OPERATOR_LOGINV;
	}
	;

multiplicative_operator
	: '/'
	{
		$$ = AST_OPERATOR_DIV;
	}
	| '%'
	{
		$$ = AST_OPERATOR_MOD;
	}
	| '*'
	{
		$$ = AST_OPERATOR_MUL;
	}
	;

additive_operator
	: '-'
	{
		$$ = AST_OPERATOR_MINUS;
	}
	| '+'
	{
		$$ = AST_OPERATOR_PLUS;
	}
	;

shift_operator
	: SHL
	{
		$$ = AST_OPERATOR_SHL;
	}
	| SHR
	{
		$$ = AST_OPERATOR_SHR;
	}
	;

relational_operator
	: '>' '='
	{
		$$ = AST_OPERATOR_GE;
	}
	| '>'
	{
		$$ = AST_OPERATOR_GT;
	}
	| '<' '='
	{
		$$ = AST_OPERATOR_LE;
	}
	| '<'
	{
		$$ = AST_OPERATOR_LT;
	}
	;

equality_operator
	: '=' '='
	{
		$$ = AST_OPERATOR_EQ;
	}
	| '!' '='
	{
		$$ = AST_OPERATOR_NE;
	}
	;
