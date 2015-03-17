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

#include "internal.h"

#include "xlinux.h"

#include "memblk.h"

#define restrict __restrict

//
// api
//

int API eval_storage_alloc(eval_storage ** const restrict es)
{
	fatal(xmalloc, es, sizeof(**es));

	fatal(ast_parser_alloc, &(*es)->as);
	fatal(memblk_mk_mapped, &(*es)->exmem, PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS);

	finally : coda;
}

void API eval_storage_free(eval_storage * es)
{
	if(es)
	{
		ast_parser_free(es->as);
		memblk_free(es->exmem);
	}
	free(es);
}
void API eval_storage_xfree(eval_storage ** const restrict es)
{
	eval_storage_free(*es);
	*es = 0;
}
