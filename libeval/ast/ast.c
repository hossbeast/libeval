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

#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "xlinux.h"
#include "pstring.h"

#include "internal.h"

#include "ast/ast.tab.h"
#include "ast/ast.lex.h"
#include "ast/ast.tokens.h"
#include "ast/ast.states.h"

#include "macros.h"

struct ast_parser
{
	void *		p;				// scanner

	ast ** 		nodes;
	size_t		nodesl;
	size_t		nodesa;
};

#define restrict __restrict

//
// static
//
static int getnode(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t type, ast ** restrict j)
{
	if(js->nodesl == js->nodesa)
	{
		size_t ns = js->nodesa ?: 10;
		ns = ns * 2 + ns / 2;
		fatal(xrealloc, &js->nodes, sizeof(*js->nodes), ns, js->nodesa);

		int x;
		for(x = js->nodesa; x < ns; x++)
		{
			fatal(xmalloc, &js->nodes[x], sizeof(*js->nodes[0]));
		}

		js->nodesa = ns;
	}

	(*j) = js->nodes[js->nodesl++];
	(*j)->type = type;
	memcpy(&(*j)->loc, loc, sizeof(*loc));

	finally : coda;
}

static int reduce(int (*parser)(void*, parse_param*), parse_param * pp)
{
	int r;
	if((r = parser(pp->scanner, pp)) || pp->scanerr)
	{
    if(r == 2)
		{
		  // memory exhaustion error from the parser
		  tfail(perrtab_SYS, ENOMEM);
		}
		else if(pp->scanerr)
		{
		  // scanner error
		  fails(pp->scanerr, pp->error_str);
		}
		else if(pp->gramerr)
		{
		  // failure to reduce
		  fails(EVAL_SYNTAX, pp->error_str);
		}
		else
		{
		  // error thrown from a grammar rule (such as ENOMEM)
		  fail(0);
		}
	}

finally :
#if XAPI_PROVIDE_BACKTRACE
	if(XAPI_UNWINDING)
	{
	  if(pp->scanerr || pp->gramerr)
	  {
	    XAPI_INFOF("location", "[%d,%d - %d,%d]"
	      , pp->error_loc.f_lin + 1
	      , pp->error_loc.f_col + 1
	      , pp->error_loc.l_lin + 1
	      , pp->error_loc.l_col + 1
	    );
	
	    if(pp->gramerr)
	    {
	      XAPI_INFOS("token", pp->tokenstring);
	    }
	  }
	}
#endif
coda;
}

static int normal_psrender(const ast * const restrict a, pstring ** restrict ps, int level)
{
	if(a->type == AST_TYPE_INT)
	{
		fatal(pscatf, ps, "%"PRId32, a->i);
	}
	else if(a->type == AST_TYPE_FLOAT)
	{
		fatal(pscatf, ps, "3.6f%", a->f);
	}
	else if(a->type == AST_TYPE_VAR)
	{
		fatal(pscats, ps, a->n);
	}
	else if(a->type == AST_TYPE_EXPR)
	{
		if(level)
			fatal(pscatw, ps, "(", 1);

		if((a->operator & AST_OPERATOR_ARITY_OPT) == AST_ARITY_UNARY)
		{
			fatal(pscats, ps, AST_OPERATOR_STR(a->operator));
			fatal(normal_psrender, a->op, ps, level + 1);
		}
		else if((a->operator & AST_OPERATOR_ARITY_OPT) == AST_ARITY_BINARY)
		{
			fatal(normal_psrender, a->lop, ps, level + 1);
			fatal(pscatw, ps, " ", 1);
			fatal(pscats, ps, AST_OPERATOR_STR(a->operator));
			fatal(pscatw, ps, " ", 1);
			fatal(normal_psrender, a->rop, ps, level + 1);
		}
		else if((a->operator & AST_OPERATOR_ARITY_OPT) == AST_ARITY_POLYNARY)
		{
			int x;
			for(x = 0; x < a->opsl; x++)
			{
				if(x)
				{
					fatal(pscatw, ps, " ", 1);
					fatal(pscats, ps, AST_OPERATOR_STR(a->operator));
					fatal(pscatw, ps, " ", 1);
				}

				fatal(normal_psrender, a->ops[x], ps, level + 1);
			}
		}
		if(level)
			fatal(pscatw, ps, ")", 1);
	}

	finally : coda;
}

//
// public
//

int ast_parse(ast_parser * const p, char * buf, size_t bufsz, ast ** const restrict rv)
{
	pstring * ps = 0;
	int __attribute__((unused)) __r;

	const char * ast_tokenname(int token)
	{
		return ast_tokennames[token];
	};

	const char * ast_statename(int state)
	{
		return state >= 0 ? ast_statenames[state] : "";
	};

  int logwould(void * token, void * udata)
  {
		return 1;
  };

  void loglog(void * token, void * udata, const char * func, const char * file, int line, char * fmt, ...)
  {
    va_list va;
    va_start(va, fmt);
		vdprintf(1, fmt, va);
		__r = write(1, "\n", 1);
    va_end(va);
  };

	parse_param pp = {
		  .p						= p
		, .scanner			= p->p
		, .orig					= buf
		, .tokname			= ast_tokenname
		, .statename		= ast_statename

		, .token_would	= logwould
		, .token_log		= loglog
	};

	// create state for this parse
	void * state = 0;
	if((state = ast_yy_scan_bytes(buf, bufsz, p->p)) == 0)
		tfail(perrtab_SYS, ENOMEM);

	// make available to the lexer
	ast_yyset_extra(&pp, p->p);

	// invoke the parser
	fatal(reduce, ast_yyparse, &pp);

	if(pp.g)
	{
		fatal(ast_normal_pswrite, pp.g, &ps);
		__r = write(1, ps->s, ps->l);
		__r = write(1, "\n", 1);
	}

	if(rv)
		(*rv) = pp.g;

finally:
	ast_yy_delete_buffer(state, p->p);
	yyu_extra_destroy(&pp);
	psfree(ps);
coda;
}

//
// public 
//

int ast_parser_alloc(ast_parser ** restrict js)
{
	fatal(xmalloc, js, sizeof(**js));
	tfatalize(perrtab_SYS, ENOMEM, ast_yylex_init, &(*js)->p);

	finally : coda;
}

void ast_parser_free(ast_parser * restrict js)
{
	if(js)
	{
		int x;
		for(x = 0; x < js->nodesa; x++)
			ast_free(js->nodes[x]);
		free(js->nodes);
	}

	free(js);
}

void ast_parser_xfree(ast_parser ** const restrict js)
{
	ast_parser_free(*js);
	*js = 0;
}

void ast_free(ast * restrict a)
{
	if(a)
	{
		
	}

	free(a);
}

void ast_xfree(ast ** const restrict a)
{
	ast_free(*a);
	*a = 0;
}

//
// public node creation
//

int ast_mk_int(ast_parser * const restrict js, ast_location * const restrict loc, int32_t v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->i = v;
	
	finally : coda;
}

int ast_mk_float(ast_parser * const restrict js, ast_location * const restrict loc, float v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->f = v;
	
	finally : coda;
}

int ast_mk_var(ast_parser * const restrict js, ast_location * const restrict loc, char * v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->n = v;
	
	finally : coda;
}

int ast_mk_unary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict op, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_EXPR, j);

	(*j)->operator = operator;
	(*j)->op = op;

	finally : coda;
}

int ast_mk_binary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_EXPR, j);

	(*j)->operator = operator;
	(*j)->lop = lop;
	(*j)->rop = rop;

	finally : coda;
}

int ast_mk_polynary(ast_parser * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
{
	if(lop->type != AST_TYPE_EXPR || lop->operator != operator)
	{
		fatal(getnode, js, loc, AST_TYPE_EXPR, j);
		(*j)->operator = operator;

		// append the left operand
		if((*j)->opsl == (*j)->opsa)
		{
			size_t ns = (*j)->opsa ?: 3;
			ns = ns * 2 + ns / 2;
			fatal(xrealloc, &(*j)->ops, sizeof(*(*j)->ops), ns, (*j)->opsa);
			(*j)->opsa = ns;
		}
		(*j)->ops[(*j)->opsl++] = lop;
	}

	// append the right operand
	if((*j)->opsl == (*j)->opsa)
	{
		size_t ns = (*j)->opsa ?: 3;
		ns = ns * 2 + ns / 2;
		fatal(xrealloc, &(*j)->ops, sizeof(*(*j)->ops), ns, (*j)->opsa);
		(*j)->opsa = ns;
	}

	(*j)->ops[(*j)->opsl++] = rop;

	// update the location
	memcpy(&(*j)->loc, loc, sizeof(*loc));

	finally : coda;
}

//
// public node manipulation
//

int ast_normal_psrender(const ast * const restrict a, pstring ** restrict ps)
{
	xproxy(normal_psrender, a, ps, 0);
}

int ast_normal_pswrite(const ast * const restrict a, pstring ** restrict ps)
{
	fatal(psclear, ps);
	fatal(ast_normal_psrender, a, ps);

	finally : coda;
}

int ast_structure_psrender(const ast * const restrict a, pstring ** restrict ps)
{
	return 0;
}

int ast_structure_pswrite(const ast * const restrict a, pstring ** restrict ps)
{
	fatal(psclear, ps);
	fatal(ast_structure_psrender, a, ps);

	finally : coda;
}
