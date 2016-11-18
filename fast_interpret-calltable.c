#include "fast_interpret.h"

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
    instr_nop,
    instr_nop,
};

void interpret_calltable(struct _vliw *vliw, int len) {
    int i=0;
    while (i<len) {
	instructions[vliw[i].opcode & 0x0f](vliw[i].outptr, *vliw[i].m1ptr, *vliw[i].m2ptr);
	i++;
    }
}
