#include <limits.h>
#include <stdlib.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_goto_jit4(const struct _vliw *vliw, int len) {
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
    struct _vliw_jit v;

    int progct = 0;
    goto *vliw_jit[progct++].instr;

instr_and:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr & (long long)*v.m2ptr;
    goto *v.instr;

instr_or:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr | (long long)*v.m2ptr;
    goto *v.instr;

instr_xor:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr ^ (long long)*v.m2ptr;
    goto *v.instr;

instr_shl:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr << (unsigned int)*v.m2ptr;
    goto *v.instr;

instr_shr:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr >> (unsigned int)*v.m2ptr;
    goto *v.instr;

instr_add:
    v = vliw_jit[progct++];
    *v.outptr = *v.m1ptr + *v.m2ptr;
    goto *v.instr;

instr_sub:
    v = vliw_jit[progct++];
    *v.outptr = *v.m1ptr - *v.m2ptr;
    goto *v.instr;

instr_mul:
    v = vliw_jit[progct++];
    *v.outptr = *v.m1ptr * *v.m2ptr;
    goto *v.instr;

instr_div:
    v = vliw_jit[progct++];
    *v.outptr = *v.m1ptr / *v.m2ptr;
    goto *v.instr;

instr_mod:
    v = vliw_jit[progct++];
    *v.outptr = (long long)*v.m1ptr % (int)*v.m2ptr;
    goto *v.instr;

instr_incjl:
    v = vliw_jit[progct];
    progct += ++*v.outptr <  *v.m2ptr ? *v.m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_decjge:
    v = vliw_jit[progct];
    progct += --*v.outptr >= *v.m2ptr ? *v.m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_je:
    v = vliw_jit[progct];
    progct += *v.outptr == *v.m2ptr ? *v.m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_jne:
    v = vliw_jit[progct];
    progct += *v.outptr != *v.m2ptr ? *v.m1ptr : 0;
    goto *vliw_jit[progct++].instr;

instr_call:
    v = vliw_jit[progct];
    if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
    callstack[++callstackptr] = *v.m2ptr;
    progct += *v.m1ptr;
    goto *vliw_jit[progct++].instr;

instr_ret:
    progct += callstack[callstackptr--];
    if (progct >= 0) goto *vliw_jit[progct++].instr; // speedup: end return check here   (last return --> exit)

    // end of function...
    free(vliw_jit);
}
