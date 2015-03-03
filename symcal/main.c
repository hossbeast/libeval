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

#include <stdio.h>

#include "eval.h"
#include "eval/compile.h"

#include "xapi.h"

int main(int argc, char ** argv, char ** envp)
{
#if XAPI_PROVIDE_BACKTRACE
	char space[256];
#endif

	eval_storage * es = 0;

	fatal(eval_storage_alloc, &es);

	char * expr = "a + 5 + 10";
	int (*fn)(int) = 0;
	fatal(eval_compile, es, expr, &fn);

	int a = 7;
	int r = fn(a);

	printf("%s : a = %d : %d\n", expr, a, r);

finally:
	eval_storage_free(es);

#if XAPI_PROVIDE_BACKTRACE
	if(XAPI_UNWINDING)
	{
		size_t tracesz = xapi_trace_full(space, sizeof(space));
		dprintf(2, "%.*s\n", (int)tracesz, space);
	}
#endif
coda;
}
