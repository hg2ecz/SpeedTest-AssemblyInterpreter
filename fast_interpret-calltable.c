#include "fast_interpret.h"

static struct _vcpu {
    int progct;
    _Bool run;
} vcpu;

static void instr_nop(double *out, double m1, double m2) {}

static void instr_and(double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i & m2_i;
}
static void instr_or (double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i | m2_i;
}
static void instr_xor(double *out, double m1, double m2) {
    int m1_i = m1;
    int m2_i = m2;
    *out = m1_i ^ m2_i;
}
static void instr_not(double *out, double m1, double m2) {
    int m1_i = m1;
    *out = ~m1_i;
}

static void instr_add(double *out, double m1, double m2) {*out = m1 + m2; }
static void instr_sub(double *out, double m1, double m2) {*out = m1 - m2; }
static void instr_mul(double *out, double m1, double m2) {*out = m1 * m2; }
static void instr_div(double *out, double m1, double m2) {*out = m1 / m2; }

static void instr_jnz(double *out, double m1, double m2) {
    if (m1) vcpu.progct+=m2-1;
}

static void instr_exit(double *out, double m1, double m2) {
    vcpu.run=0;
}


static void (*instructions[])(double *out, double m1, double m2) = {
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
    vcpu.progct=0;
    vcpu.run=1;
    while (vcpu.run) {
	instructions[vliw[vcpu.progct].opcode & 0x0f](
		vliw[vcpu.progct].outptr,
		*vliw[vcpu.progct].m1ptr,
		*vliw[vcpu.progct].m2ptr
	);
	vcpu.progct++;
    }
}
