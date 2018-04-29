#include <limits.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_goto(const struct _vliw *vliw) {
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

    int progct = 0;
    goto *instruction_table[vliw[progct].opcode & 0x0f];

instr_and:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr & (long long)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_or:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr | (long long)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_xor:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr ^ (long long)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_shl:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr << (unsigned int)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_shr:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr >> (unsigned int)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_add:
    *vliw[progct].outptr = *vliw[progct].m1ptr + *vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_sub:
    *vliw[progct].outptr = *vliw[progct].m1ptr - *vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_mul:
    *vliw[progct].outptr = *vliw[progct].m1ptr * *vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_div:
    *vliw[progct].outptr = *vliw[progct].m1ptr / *vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_mod:
    *vliw[progct].outptr = (long long)*vliw[progct].m1ptr % (int)*vliw[progct].m2ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_incjl:
    progct += ++*vliw[progct].outptr <  *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_decjge:
    progct += --*vliw[progct].outptr >= *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_je:
    progct += *vliw[progct].outptr == *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_jne:
    progct += *vliw[progct].outptr != *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_call:
    if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
    callstack[++callstackptr] = *vliw[progct].m2ptr;
    progct += *vliw[progct].m1ptr;
    goto *instruction_table[vliw[++progct].opcode & 0x0f];

instr_ret:
    progct += callstack[callstackptr--];
    if (progct >= 0) goto *instruction_table[vliw[++progct].opcode & 0x0f]; // speedup: end return check here   (last return --> exit)
}
