#include <limits.h>
#include "fast_interpret.h"

static int instr_nop(double *out, double m1, double m2) { return 0; }

static int instr_and(double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i & m2_i;
    return 0;
}
static int instr_or (double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i | m2_i;
    return 0;
}
static int instr_xor(double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i ^ m2_i;
    return 0;
}
static int instr_not(double *out, double m1, double m2) {
    int m1_i = m1;
    *out = ~m1_i;
    return 0;
}

static int instr_add(double *out, double m1, double m2) {*out = m1 + m2; return 0; }
static int instr_sub(double *out, double m1, double m2) {*out = m1 - m2; return 0; }
static int instr_mul(double *out, double m1, double m2) {*out = m1 * m2; return 0; }
static int instr_div(double *out, double m1, double m2) {*out = m1 / m2; return 0; }

static int instr_jnz(double *out, double m1, double m2) {
    if (m1) return m2;
    return 0;
}

static int instr_exit(double *out, double m1, double m2) {
    return INT_MIN;
}


static int (*instructions[])(double *out, double m1, double m2) = {
    instr_nop,
    instr_and,
    instr_or,
    instr_xor,

    instr_not,
    instr_add,
    instr_sub,
    instr_mul,

    instr_div,
    instr_nop,
    instr_nop,
    instr_nop,

    instr_nop,
    instr_nop,
    instr_jnz, // jump not zero
    instr_exit
};

void interpret_calltable(struct _vliw *vliw, int len) {
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
