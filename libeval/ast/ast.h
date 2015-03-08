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

struct pstring;			// pstring.h

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

#define AST_ARITY_TABLE(x)								\
	_AST_ARITY(UNARY		, 0x01 << 0x08, x)	\
	_AST_ARITY(BINARY		, 0x02 << 0x08, x)	\
	_AST_ARITY(POLYNARY	, 0x03 << 0x08, x)

enum {
#define _AST_ARITY(a, b, x) AST_ARITY_ ## a = b,
AST_ARITY_TABLE(0)
#undef _AST_ARITY
};

#define _AST_ARITY(a, b, x) x == b ? QUOTE(a) :
#define AST_ARITY_STR(x) AST_ARITY_TABLE(x) "UNKNOWN"

#define AST_OPERATOR_TABLE(x, y)																					\
	_AST_OPERATOR(ADDINV		, 0x01	, "-"		, AST_ARITY_UNARY			, x	, y)	\
	_AST_OPERATOR(BITINV		, 0x02	, "~"		, AST_ARITY_UNARY			, x	, y)	\
	_AST_OPERATOR(LOGINV		, 0x03	, "!"		, AST_ARITY_UNARY			, x	, y)	\
	_AST_OPERATOR(DIV				, 0x04	, "/"		, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(EQ				, 0x05	, "=="	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(NE				, 0x06	, "!="	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(GE				, 0x07	, ">="	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(GT				, 0x08	, ">"		, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(LE				, 0x09	, "<="	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(LT				, 0x0a	, "<"		, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(MINUS			, 0x0b	, "-"		, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(MOD				, 0x0c	, "%"		, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(SHL				, 0x0d	, "<<"	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(SHR				, 0x0e	, ">>"	, AST_ARITY_BINARY		, x	, y)	\
	_AST_OPERATOR(LOGOR			, 0x0f	, "||"	, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(LOGAND		, 0x10	, "&&"	, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(BITOR			, 0x11	, "|"		, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(BITAND		, 0x12	, "&"		, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(BITXOR		, 0x13	, "^"		, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(MUL				, 0x14	, "*"		, AST_ARITY_POLYNARY	, x	, y)	\
	_AST_OPERATOR(PLUS			, 0x15	, "+"		, AST_ARITY_POLYNARY	, x	, y)

enum {
#define _AST_OPERATOR(a, b, c, d, x, y) AST_OPERATOR_ ## a = (b | d),
AST_OPERATOR_TABLE(0, 0)
#undef _AST_OPERATOR
};

#define AST_OPERATOR_ARITY_OPT	0xFF00

#define _AST_OPERATOR(a, b, c, d, x, y) y == 1 && x == (b | d) ? QUOTE(a) : y == 2 && x == (b | d) ? c :
#define AST_OPERATOR_NAME(x) AST_OPERATOR_TABLE(x, 1) "UNKNOWN"
#define AST_OPERATOR_STR(x) AST_OPERATOR_TABLE(x, 2) "UNKNOWN"

// reusable parser storage ; opaque type
struct ast_parser;
typedef struct ast_parser ast_parser;

// ast node location
typedef yyu_location ast_location;

// ast node
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
			uint32_t operator;

			union
			{
				struct						// UNARY
				{
					struct ast *	op;		// operand
				};

				struct						// BINARY
				{
					struct ast *	lop;	// left operand
					struct ast *	rop;	// right operand
				};

				struct						// POLYNARY
				{
					struct ast **	ops;	// operands
					size_t				opsl;
					size_t				opsa;
				};
			};
		};
	};
} ast;

#define restrict __restrict

/// ast_parse
//
// SUMMARY
//  parse the specified buffer to an ast
//
// PARAMETERS
//  p     - reusable ast parser
//  buf   - buffer to parse
//  bufsz - size of buffer
//  [rv]  - (returns) parsed ast
//
// RETURNS
//  0 on success, check *rv to determine whether the parse was successful
//
int ast_parse(ast_parser * const p, char * buf, size_t bufsz, ast ** const restrict rv)
	__attribute__((nonnull(1, 2)));

/*
** public ast_parser
*/

/// ast_mkparser
//
// creates an instance of a ast parser which may be passed repeatedly to parse
//
int ast_parser_alloc(ast_parser ** const restrict p)
  __attribute__((nonnull));

/// ast_freeparser
//
// free a ast parser returned from mkparser with free semantics
//
void ast_parser_free(ast_parser* const restrict);

/// ast_xfreeparser
//
// free a ast parser returned from mkparser with xfree semantics
//
void ast_parser_xfree(ast_parser ** const restrict)
  __attribute__((nonnull));

/*
** public ast node
*/

/// ast_mknode
//
// SUMMARY
//  create an ast value node
//
int ast_mk_int(ast_parser * const restrict js, ast_location * const restrict loc, int32_t v, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_float(ast_parser * const restrict js, ast_location * const restrict loc, float v, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_var(ast_parser * const restrict js, ast_location * const restrict loc, char * v, ast ** restrict j)
	__attribute__((nonnull));

/// ast_mknode
//
// SUMMARY
//  create an ast expression node
//
int ast_mk_unary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict op, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_binary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
	__attribute__((nonnull));

int ast_mk_polynary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
	__attribute__((nonnull));

/// ast_free
//
// SUMMARY
//  free an ast node with free semantics
//
void ast_free(ast * restrict a);

/// ast_xfree
//
// SUMMARY
//  free an ast node with xfree semantics
//
void ast_xfree(ast ** const restrict a)
	__attribute__((nonnull));

/// ast_normal_psrender
//
// SUMMARY
//  append a normalized representation of the ast to a dynamically resizing pstring
//
int ast_normal_psrender(const ast * const restrict a, struct pstring ** restrict ps)
	__attribute__((nonnull));

/// ast_normal_pswrite
//
// SUMMARY
//  write a normalized representation of the ast to a dynamically resizing pstring
// 
int ast_normal_pswrite(const ast * const restrict a, struct pstring ** restrict ps)
	__attribute__((nonnull));

/// ast_structure_psrender
//
// SUMMARY
//  append a structured representation of the ast to a dynamically resizing pstring
//
int ast_structure_psrender(const ast * const restrict a, struct pstring ** restrict ps)
	__attribute__((nonnull));

/// ast_structure_pswrite
//
// SUMMARY
//  write a structured representation of the ast to a dynamically resizing pstring
// 
int ast_structure_pswrite(const ast * const restrict a, struct pstring ** restrict ps)
	__attribute__((nonnull));

#endif
