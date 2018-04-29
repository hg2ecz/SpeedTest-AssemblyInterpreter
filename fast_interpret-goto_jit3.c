#include <limits.h>
#include <stdlib.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_goto_jit3(const struct _vliw *vliw, int len) {
    int callstack[CALLSTACKMAX] = {INT_MIN};
    unsigned int callstackptr = 0;

    static void *instruction_table[] = {
	&&instr_and, // *outmem = *op1mem & *op2mem;
	&&instr_or,  // *outmem = *op1mem | *op2mem;
	&&instr_xor, // *outmem = *op1mem ^ *op2mem;
	&&instr_shl, // *outmem = (unsigned long long)*op1mem << (int)*op2mem;

	&&instr_shr, // *outmem = (unsigned long long)*op1mem >> (int)*op2mem;
	&&instr_add, // *outmem = *op1mem + *op2mem;
	&&instr_sub, // *outmem = *op1mem - *op2mem;
	&&instr_mul, // *outmem = *op1mem * *op2mem;

	&&instr_div, // *outmem = *op1mem / *op2mem;
	&&instr_mod, // *outmem = (long long)*op1mem % (int)*op2mem;
	&&instr_incjl, // *outmem++; if (*outmem < *op2mem) progct+=*op1mem;
	&&instr_decjge,// *outmem--; if (*outmem >= *op2mem) progct+=*op1mem;

	&&instr_je,  // if (*outmem == *op2mem) progct+=*op1mem;
	&&instr_jne, // if (*outmem != *op2mem) progct+=*op1mem;
	&&instr_call,// progct+=*op1mem; ++stack = *op2mem; (=relative return address)
	&&instr_ret, // progct+=stack--; stack[0] = INT_MIN ---> exit
    };

    struct _vliw_jit {
	void *instr;
	double *outptr;
	double *m1ptr;
	double *m2ptr;
    };

    struct _vliw_jit *vliw_jit = malloc((len+1)*sizeof(struct _vliw_jit)); // +1 exit
    for (int i=0; i<len; i++) {
	vliw_jit[i].instr = instruction_table[vliw[i].opcode & 0x0f];
	vliw_jit[i+1].outptr = vliw[i].outptr;
	vliw_jit[i+1].m1ptr = vliw[i].m1ptr;
	vliw_jit[i+1].m2ptr = vliw[i].m2ptr;
    }

    int progct = 0;
    goto *vliw_jit[progct++].instr;

instr_and:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr & (long long)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_or:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr | (long long)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_xor:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr ^ (long long)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_shl:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr << (unsigned int)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_shr:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr >> (unsigned int)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_add:
    *vliw_jit[progct].outptr = *vliw_jit[progct].m1ptr + *vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_sub:
    *vliw_jit[progct].outptr = *vliw_jit[progct].m1ptr - *vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_mul:
    *vliw_jit[progct].outptr = *vliw_jit[progct].m1ptr * *vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_div:
    *vliw_jit[progct].outptr = *vliw_jit[progct].m1ptr / *vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_mod:
    *vliw_jit[progct].outptr = (long long)*vliw_jit[progct].m1ptr % (int)*vliw_jit[progct].m2ptr;
    goto *vliw_jit[progct++].instr;

instr_incjl:
    progct += ++*vliw_jit[progct].outptr <  *vliw_jit[progct].m2ptr ? *vliw_jit[progct].m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_decjge:
    progct += --*vliw_jit[progct].outptr >= *vliw_jit[progct].m2ptr ? *vliw_jit[progct].m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_je:
    progct += *vliw_jit[progct].outptr == *vliw_jit[progct].m2ptr ? *vliw_jit[progct].m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_jne:
    progct += *vliw_jit[progct].outptr != *vliw_jit[progct].m2ptr ? *vliw_jit[progct].m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_call:
    if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
    callstack[++callstackptr] = *vliw_jit[progct].m2ptr;
    progct += *vliw_jit[progct].m1ptr;
    goto *vliw_jit[progct++].instr;

instr_ret:
    progct += callstack[callstackptr--];
    if (progct >= 0) goto *vliw_jit[progct++].instr; // speedup: end return check here   (last return --> exit)

    free(vliw_jit);
}
