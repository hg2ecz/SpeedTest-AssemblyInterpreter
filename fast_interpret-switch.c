#include "fast_interpret.h"

static struct _vcpu {
    int progct;
    _Bool run;
} vcpu;

void interpret_switch(struct _vliw *vliw, int len) {
    vcpu.progct=0;
    vcpu.run=1;
    while (vcpu.run) {
	switch(vliw[vcpu.progct].opcode) {
	    case 0: break; // nop
	    case 1: {       // and
		int m1_i = *vliw[vcpu.progct].m1ptr;
		int m2_i = *vliw[vcpu.progct].m2ptr;
		*vliw[vcpu.progct].outptr = m1_i & m2_i;
		} break;
	    case 2: {        // or
		int m1_i = *vliw[vcpu.progct].m1ptr;
		int m2_i = *vliw[vcpu.progct].m2ptr;
		*vliw[vcpu.progct].outptr = m1_i | m2_i;
		} break;
	    case 3: {        // xor
		int m1_i = *vliw[vcpu.progct].m1ptr;
		int m2_i = *vliw[vcpu.progct].m2ptr;
		*vliw[vcpu.progct].outptr = m1_i ^ m2_i;
		} break;
	    case 4: {        // not
		int m1_i = *vliw[vcpu.progct].m1ptr;
		*vliw[vcpu.progct].outptr = ~m1_i;
		} break;

	    case 5:        // add
		*vliw[vcpu.progct].outptr = *vliw[vcpu.progct].m1ptr + *vliw[vcpu.progct].m2ptr;
		break;

	    case 6:        // sub
		*vliw[vcpu.progct].outptr = *vliw[vcpu.progct].m1ptr - *vliw[vcpu.progct].m2ptr;
		break;

	    case 7:        // mul
		*vliw[vcpu.progct].outptr = *vliw[vcpu.progct].m1ptr * *vliw[vcpu.progct].m2ptr;
		break;

	    case 8:        // div
		*vliw[vcpu.progct].outptr = *vliw[vcpu.progct].m1ptr / *vliw[vcpu.progct].m2ptr;
		break;

	    case 9: break; // nop
	    case 10: break; // nop
	    case 11: break; // nop
	    case 12: break; // nop
	    case 13: break; // nop

	    case 14:
		if (*vliw[vcpu.progct].m1ptr) vcpu.progct+=*vliw[vcpu.progct].m2ptr-1;
		break;
	    case 15:
		vcpu.run=0;
		break;

	    default: break;
	}
	vcpu.progct++;
    }
}
