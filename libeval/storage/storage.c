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

struct eval_storage
{
	ast_storage *		as;					// subordinate ast_storage

	void *		exmem;						// pointer to memory region with execute permissions
	size_t		exmeml;						// used size
	size_t		exmema;						// allocated size
};

#define restrict __restrict

//
// api
//

int API eval_storage_alloc(eval_storage ** const restrict es)
{
	fatal(xmalloc, es, sizeof(**es));

	finally : coda;
}

void API eval_storage_free(eval_storage * es)
{
	if(es)
	{
		ast_storage_free(es->as);
		munmap(es->exmem, es->exmema);
	}
	free(es);
}
void API eval_storage_xfree(eval_storage ** const restrict es)
{
	eval_storage_free(*es);
	*es = 0;
}
