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

#include <stdarg.h>
#include <string.h>

#include "pstring.h"

#include "internal.h"

#include "memblk.h"
#include "macros.h"

#define restrict __restrict
#define emit(...) fatal(xemit, mem, NARGS(__VA_ARGS__), __VA_ARGS__)

// location types
#define LOC_REG 1
#define LOC_MEM 2

typedef struct
{
	uint8_t		t;			// one of LOC_*
	union
	{
		struct {				// LOC_REG
			uint16_t		x;		// register bit
		};

		struct {				// LOC_MEM
			uint16_t		off;	// offset from stack pointer
		};
	}
} loc;

//
// static
//

static char * locstr(char * dst, size_t sz, uint32_t nummodel, loc * l)
{
	if(l->t == LOC_REG)
	{
		if(nummodel == EVAL_NUMMODEL_UINT64)
		{
			static char * regs = (char*[]){ 
					[0] = "rax"
				, [1] = "rcx"
				, [2] = "rdx"
				, [3] = "rbx"
				, [4] = "rsp"
				, [5] = "rbp"
				, [6] = "rsi"
				, [7] = "rdi"
				, [8] = "r8"
				, [9] = "r9"
				, [10] = "r10"
				, [11] = "r11"
				, [12] = "r12"
				, [13] = "r13"
				, [14] = "r14"
				, [15] = "r15"
			};

			snprintf(dst, sz, "%s", regs[l->x]);
		}
	}

	return dst;
}

static void dump(pstring * ps, char * fmt, ...)
{
	static int n;

	int x;
	for(x = n; x < ps->l; x++)
	{
		printf("%02x 0x%02x ", x, (uint8_t)ps->s[x]);
		if(ps->s[x] > 0x2 && ps->s[x] < 0x7e)
			printf("%c", ps->s[x]);
		if(x == n)
		{
			printf("     ");

			va_list va;
			va_start(va, fmt);
			vprintf(fmt, va);
			va_end(va);
		}
		printf("\n");
	}

	n = x;
}

static int xemit(pstring ** restrict mem, size_t l, ...)
{
	va_list va;
	va_start(va, l);

	int x;
	for(x = 0; x < l; x++)
	{
		char b = (char)va_arg(va, int);

		fatal(pscatw, mem, &b, 1);
	}

finally:
	va_end(va);
coda;
}

static int render(pstring ** restrict mem, uint32_t nummodel, uint16_t * regmap, const ast * const restrict a, loc * dst)
{
	char space[64];

	int x;

	if(a->type == AST_TYPE_EXPR)
	{
		if(a->operator == AST_OPERATOR_PLUS)
		{
			for(x = 0; x < a->opsl; x++)
			{
#if 0
					emit(0x66, 0x05);				// add ax, immediate 16-bit (64 bit mode)
					emit(0x05);							// add eax, immediate 32-bit
					emit(0x48, 0x05);				// add rax, immediate 64-bit
					emit(0x05);							// add eax, immediate 32-bit

					emit(0xb8);							// mov eax, immediate 32-bit
					emit(((char*)&a->i)[LSIGX(4, 0)]);
					emit(((char*)&a->i)[LSIGX(4, 1)]);
					emit(((char*)&a->i)[LSIGX(4, 2)]);
					emit(((char*)&a->i)[LSIGX(4, 3)]);
#endif

				if(nummodel == EVAL_NUMMODEL_UINT64)
				{
					if(a->ops[x]->type == AST_TYPE_INT)		// immediate operand
					{
						// allocate the next register
						~(*regmap) & -~(*regmap)
					}
				}
			}
		}
	}
	else if(a->type == AST_TYPE_INT)
	{
		if(nummodel == EVAL_NUMMODEL_UINT64)
		{
			if(dst->t == LOC_REG)
			{
				if(dst->x < 8)	// opcode : mov
					emit(0x48);
				else
					emit(0x49);

				// register
				emit(0xB8 + (dst->x % 8));

				// immediate value
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 0)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 1)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 2)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 3)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 4)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 5)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 6)]);
				emit(((char*)&a->ops[x]->i)[LSIGX(8, 7)]);

				dump(*mem, "mov %s, immediate 64-bit", locstr(space, sizeof(space), nummodel, dst));
			}
		}

		dump(*mem, "mov eax, immediate 32-bit");
	}

	finally : coda;
}

//
// public
//

int x86_64_compile(eval_storage * const restrict es, const ast * const restrict a, uint32_t nummodel, void * restrict fn)
{
	pstring * ps = 0;
	pstring ** mem = &ps;

	// emit the prologue
	emit(0x55);							// push rbp						; save the existing value of rbp
	emit(0x48, 0x89, 0xe5);	// mov rbp, rsp				; copy the value of rsp to rbp, thus setting up the local base pointer
	dump(*mem, "prologue");

	// setup the register map
	uint16_t regmap = 0;

	// setup the dest
	loc dst;
	if(nummodel == EVAL_NUMMODEL_UINT64)
		dst = { .t = LOC_REG, .x = 0 };		// rax

	// emit the body
	fatal(render, mem, nummodel, &regmap, a, &dst);

	// emit the epilogue
	emit(0x5d);							// pop rbp						; restore rbp
	emit(0xc3);							// ret
	dump(*mem, "epilogue");

	// copy to executable memory
	char * code = 0;
	fatal(memblk_alloc, es->exmem, &code, ps->l);
	memcpy(code, ps->s, ps->l);
	*(void**)fn = code;

finally:
	psfree(ps);
coda;
}
