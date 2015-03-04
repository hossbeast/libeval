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

#ifndef _EVAL_AST
#define _EVAL_AST

#include <stdint.h>

#include "yyutil.h"

#define AST_TYPE_TABLE(x)									\
	_AST_TYPE(EXPR	, 0x01	, x)						\
	_AST_TYPE(INT		, 0x02	, x)						\
	_AST_TYPE(FLOAT	, 0x04	, x)						\
	_AST_TYPE(VAR		, 0x05	, x)

enum {
#define _AST_TYPE(a, b, x) AST_TYPE_ ## a = b,
AST_TYPE_TABLE(0)
#undef _AST_TYPE
};

#define _AST_TYPE(a, b, x) x == b ? QUOTE(a) :
#define AST_TYPE_STR(x) AST_TYPE_TABLE(x) "UNKNOWN"

#define AST_ARITY_TABLE(x)		\
	_AST_ARITY(UNARY	, 0x01	, x)			\
	_AST_ARITY(BINARY	, 0x02	, x)

enum {
#define _AST_ARITY(a, b, x) AST_ARITY_ ## a = b,
AST_ARITY_TABLE(0)
#undef _AST_ARITY
};

#define _AST_ARITY(a, b, x) x == b ? QUOTE(a) :
#define AST_ARITY_STR(x) AST_ARITY_TABLE(x) "UNKNOWN"

#define AST_OPERATOR_TABLE(x, y)																					\
	_AST_OPERATOR(ADDINV		, 0x01	, "-"		, AST_ARITY_UNARY		, x	, y)		\
	_AST_OPERATOR(BITINV		, 0x02	, "~"		, AST_ARITY_UNARY		, x	, y)		\
	_AST_OPERATOR(LOGINV		, 0x03	, "!"		, AST_ARITY_UNARY		, x	, y)		\
	_AST_OPERATOR(LOGOR			, 0x04	, "||"	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(LOGAND		, 0x05	, "&&"	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(BITOR			, 0x06	, "|"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(BITAND		, 0x07	, "&"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(BITXOR		, 0x08	, "^"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(DIV				, 0x09	, "/"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(EQ				, 0x0a	, "=="	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(NE				, 0x0b	, "!="	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(GE				, 0x0c	, ">="	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(GT				, 0x0d	, ">"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(LE				, 0x0e	, "<="	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(LT				, 0x0f	, "<"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(MINUS			, 0x10	, "-"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(PLUS			, 0x11	, "+"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(MOD				, 0x12	, "%"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(MUL				, 0x13	, "*"		, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(SHL				, 0x14	, "<<"	, AST_ARITY_BINARY	, x	, y)		\
	_AST_OPERATOR(SHR				, 0x15	, ">>"	, AST_ARITY_BINARY	, x	, y)

enum {
#define _AST_OPERATOR(a, b, c, d, x, y) AST_OPERATOR_ ## a = b,
AST_OPERATOR_TABLE(0, 0)
#undef _AST_OPERATOR
};

#define _AST_OPERATOR(a, b, c, d, x, y) y == 1 && x == b ? QUOTE(a) : y == 2 && x == b ? c :
#define AST_OPERATOR_STR(x) AST_OPERATOR_TABLE(x, 1) "UNKNOWN"
#define AST_OPERATOR_NAM(x) AST_OPERATOR_TABLE(x, 2) "UNKNOWN"

typedef yyu_location ast_location;

struct ast_storage;
typedef struct ast_storage ast_storage;

typedef struct ast
{
	uint32_t			type;
	ast_location	loc;

	union
	{
		int32_t		i;					// INT
		float			f;					// FLOAT
		char *		n;					// VAR

		struct								// EXPR
		{
			int	operator;

			union
			{
				struct
				{
					struct ast *	op;		// operand
				};

				struct
				{
					struct ast *	lop;	// left operand
					struct ast *	rop;	// right operand
				};
			};
		};
	};
} ast;

#define restrict __restrict

/// ast_storage_alloc
//
// SUMMARY
//  
//
int ast_storage_alloc(ast_storage ** restrict js)
	__attribute__((nonnull));

void ast_storage_free(ast_storage * restrict js);
void ast_storage_xfree(ast_storage ** const restrict js)
	__attribute__((nonnull));

/// ast_mknode
//
// SUMMARY
//  
//
int ast_mk_int(ast_storage * const restrict js, ast_location * const restrict loc, int32_t v, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_float(ast_storage * const restrict js, ast_location * const restrict loc, float v, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_var(ast_storage * const restrict js, ast_location * const restrict loc, char * v, ast ** restrict j)
	__attribute__((nonnull));

/// ast_mknode
//
// SUMMARY
//  
//
int ast_mk_unary(ast_storage * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict op, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_binary(ast_storage * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
	__attribute__((nonnull));

void ast_free(ast * restrict a);
void ast_xfree(ast ** const restrict a)
	__attribute__((nonnull));

#endif
