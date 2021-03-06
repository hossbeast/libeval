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

#include <stdint.h>

#if XAPI_PROVIDE_BACKTRACE
#include "xapi/XAPI.errtab.h"
#include "xlinux/XLINUX.errtab.h"
#include "internal.h"

static etable * tab[4];

void __attribute__((constructor)) init()
{
	tab[0] = perrtab_SYS;
	tab[0]->id = 1;

	tab[1] = perrtab_XAPI;
	tab[1]->id = 2;

	tab[2] = perrtab_XLINUX;
	tab[2]->id = 3;

	tab[3] = perrtab_EVAL;
	tab[3]->id = 4;
}

const char * eval_errname(const int code)
{
	int16_t rt = code >> 16;			// table index
	int16_t rc = code & 0xFFFF;		// code index

	if(rt < 1 || rt > 3)
		return 0;

	return tab[rt]->v[rc + (tab[rt]->min * -1)].name;
}

const char * eval_errdesc(const int code)
{
	int16_t rt = code >> 16;			// table index
	int16_t rc = code & 0xFFFF;		// code index

	if(rt < 1 || rt > 3)
		return 0;

	return tab[rt]->v[rc + (tab[rt]->min * -1)].desc;
}

const char * eval_errstr(const int code)
{
	int16_t rt = code >> 16;			// table index
	int16_t rc = code & 0xFFFF;		// code index

	if(rt < 1 || rt > 3)
		return 0;

	return tab[rt]->v[rc + (tab[rt]->min * -1)].str;
}

const etable * eval_errtab(const int code)
{
	int16_t rt = code >> 16;			// table index

	if(rt < 1 || rt > 3)
		return 0;

	return tab[rt];
}

int eval_errcode(const int code)
{
	int16_t rt = code >> 16;			// table index
	int16_t rc = code & 0xFFFF;		// code index

	if(rt < 1 || rt > 3)
		return 0;

	return rc + (tab[rt]->min * -1);
}
#endif
