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

#include <string.h>
#include <stdlib.h>

#include "xlinux.h"

#include "internal.h"

struct ast_storage
{
	ast ** 		nodes;
	size_t		nodesl;
	size_t		nodesa;
};

#define restrict __restrict

//
// static
//
int getnode(ast_storage * const restrict js, ast_location * const restrict loc, uint32_t type, ast ** restrict j)
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

//
// public
//

int ast_storage_alloc(ast_storage ** restrict js)
{
	xproxy(xmalloc, js, sizeof(**js));
}

int ast_storage_free(ast_storage * restrict js)
{
	if(js)
	{
		int x;
		for(x = 0; x < js->nodesa; x++)
			ast_free(j->nodes[x]);
		free(js->nodes);
	}

	free(js);
}

int ast_storage_xfree(ast_storage ** const restrict js)
{
	ast_storage_free(*js);
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

int ast_mk_int(ast_storage * const restrict js, ast_location * const restrict loc, int32_t v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->i = v;
	
	finally : coda;
}

int ast_mk_float(ast_storage * const restrict js, ast_location * const restrict loc, float v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->f = v;
	
	finally : coda;
}

int ast_mk_var(ast_storage * const restrict js, ast_location * const restrict loc, char * v, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_INT, j);

	(*j)->n = v;
	
	finally : coda;
}

int ast_mk_unary(ast_storage * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict op, ast ** restrict j)
{
	fatal(getnode, js, loc, AST_TYPE_EXPR, j);

	finally : coda;
}

int ast_mk_binary(ast_storage * const restrict js, ast_location * const restrict loc, uint32_t operator, ast * const restrict lop, ast * const restrict rop, ast ** restrict j)
{

}
