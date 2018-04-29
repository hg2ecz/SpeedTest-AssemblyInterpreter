#include <limits.h>
#include <stdlib.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_goto_jit_mod(const struct _vliw *vliw, int len) {
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

    void **instr = malloc(len*sizeof(void *)); // +1 exit
    for (int i=0; i<len; i++) {
	instr[i] = instruction_table[vliw[i].opcode & 0x0f];
    }
    struct _vliw v;

    int progct = 0;
    goto *instr[progct];

instr_and:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr & (long long)*v.m2ptr;
    goto *instr[progct];

instr_or:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr | (long long)*v.m2ptr;
    goto *instr[progct];

instr_xor:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr ^ (long long)*v.m2ptr;
    goto *instr[progct];

instr_shl:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr << (unsigned int)*v.m2ptr;
    goto *instr[progct];

instr_shr:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr >> (unsigned int)*v.m2ptr;
    goto *instr[progct];

instr_add:
    v = vliw[progct++];
    *v.outptr = *v.m1ptr + *v.m2ptr;
    goto *instr[progct];

instr_sub:
    v = vliw[progct++];
    *v.outptr = *v.m1ptr - *v.m2ptr;
    goto *instr[progct];

instr_mul:
    v = vliw[progct++];
    *v.outptr = *v.m1ptr * *v.m2ptr;
    goto *instr[progct];

instr_div:
    v = vliw[progct++];
    *v.outptr = *v.m1ptr / *v.m2ptr;
    goto *instr[progct];

instr_mod:
    v = vliw[progct++];
    *v.outptr = (long long)*v.m1ptr % (int)*v.m2ptr;
    goto *instr[progct];

instr_incjl:
    v = vliw[progct++];
    progct += ++*v.outptr <  *v.m2ptr ? *v.m1ptr : 0;
    goto *instr[progct];

instr_decjge:
    v = vliw[progct++];
    progct += --*v.outptr >= *v.m2ptr ? *v.m1ptr : 0;
    goto *instr[progct];

instr_je:
    v = vliw[progct++];
    progct += *v.outptr == *v.m2ptr ? *v.m1ptr : 0;
    goto *instr[progct];

instr_jne:
    v = vliw[progct++];
    progct += *v.outptr != *v.m2ptr ? *v.m1ptr : 0;
    goto *instr[progct];

instr_call:
    v = vliw[progct++];
    if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
    callstack[++callstackptr] = *v.m2ptr;
    progct += *v.m1ptr;
    goto *instr[progct];

instr_ret:
    progct += callstack[callstackptr--];
    if (progct >= 0) goto *instr[++progct]; // speedup: end return check here   (last return --> exit)

    free(instr);
}
