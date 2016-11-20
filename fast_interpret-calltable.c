#include <limits.h>
#include "fast_interpret.h"

static int instr_and(double *out, double m1, double m2) { *out = (long long)m1 & (long long)m2; return 0; }
static int instr_or (double *out, double m1, double m2) { *out = (long long)m1 | (long long)m2; return 0; }
static int instr_xor(double *out, double m1, double m2) { *out = (long long)m1 ^ (long long)m2; return 0; }
static int instr_shl(double *out, double m1, double m2) { *out = (long long)m1 << (unsigned int)m2; return 0; }
static int instr_shr(double *out, double m1, double m2) { *out = (long long)m1 >> (unsigned int)m2; return 0; }
static int instr_add(double *out, double m1, double m2) {*out = m1 + m2; return 0; }
static int instr_sub(double *out, double m1, double m2) {*out = m1 - m2; return 0; }
static int instr_mul(double *out, double m1, double m2) {*out = m1 * m2; return 0; }
static int instr_div(double *out, double m1, double m2) {*out = m1 / m2; return 0; }
static int instr_mod(double *out, double m1, double m2) {*out = (long long)m1 % (int)m2; return 0; }

static int instr_incjl (double *out, double m1, double m2) { return ++*out < m2 ? m1 : 0; }
static int instr_decjge(double *out, double m1, double m2) { return --*out >= m2 ? m1 : 0; }
static int instr_je    (double *out, double m1, double m2) { return *out == m2 ? m1 : 0; }
static int instr_jne   (double *out, double m1, double m2) { return *out != m2 ? m1 : 0; }

#define CALLSTACKMAX 1024
static int callstack[CALLSTACKMAX];
static unsigned int callstackptr;
static int instr_call(double *out, double m1, double m2) {
    if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
    callstack[++callstackptr]=m2;
    return m1;
}

static int instr_ret(double *out, double m1, double m2) { return callstack[callstackptr--]; }

static int (*instructions[])(double *out, double m1, double m2) = {
    instr_and, // *outmem = *op1mem & *op2mem;
    instr_or,  // *outmem = *op1mem | *op2mem;
    instr_xor, // *outmem = *op1mem ^ *op2mem;
    instr_shl, // *outmem = (unsigned long long)*op1mem << (int)*op2mem;

    instr_shr, // *outmem = (unsigned long long)*op1mem >> (int)*op2mem;
    instr_add, // *outmem = *op1mem + *op2mem;
    instr_sub, // *outmem = *op1mem - *op2mem;
    instr_mul, // *outmem = *op1mem * *op2mem;

    instr_div, // *outmem = *op1mem / *op2mem;
    instr_mod, // *outmem = (long long)*op1mem % (int)*op2mem;
    instr_incjl, // *outmem++; if (*outmem < *op2mem) progct+=*op1mem;
    instr_decjge,// *outmem--; if (*outmem >= *op2mem) progct+=*op1mem;

    instr_je,  // if (*outmem == *op2mem) progct+=*op1mem;
    instr_jne, // if (*outmem != *op2mem) progct+=*op1mem;
    instr_call,// progct+=*op1mem; ++stack = *op2mem; (=relative return address)
    instr_ret, // progct+=stack--; stack[0] = INT_MIN ---> exit
};

void interpret_calltable(struct _vliw *vliw) {
    callstack[0]=INT_MIN;
    callstackptr=0;
    int progct=0;
    while (progct >= 0) {
	progct+=instructions[vliw[progct].opcode & 0x0f](
		vliw[progct].outptr,
		*vliw[progct].m1ptr,
		*vliw[progct].m2ptr
	);
	progct++;
    }
}
