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

struct eval_storage
{
	
};

#define restrict __restrict

//
// api
//

int API eval_storage_alloc(eval_storage ** const restrict es)
{
	return 0;
}

void API eval_storage_free(eval_storage * es)
{

}
void API eval_storage_xfree(eval_storage ** const restrict es)
{
	eval_storage_free(*es);
	*es = 0;
}
