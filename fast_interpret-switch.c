#include "fast_interpret.h"

void interpret_switch(struct _vliw *vliw, int len) {
    int i=0;
    while (i<len) {
	switch(vliw[i].opcode) {
	    case 0: break; // nop
	    case 1: {       // and
		int m1_i = *vliw[i].m1ptr;
		int m2_i = *vliw[i].m2ptr;
		*vliw[i].outptr = m1_i & m2_i;
		} break;
	    case 2: {        // or
		int m1_i = *vliw[i].m1ptr;
		int m2_i = *vliw[i].m2ptr;
		*vliw[i].outptr = m1_i | m2_i;
		} break;
	    case 3: {        // xor
		int m1_i = *vliw[i].m1ptr;
		int m2_i = *vliw[i].m2ptr;
		*vliw[i].outptr = m1_i ^ m2_i;
		} break;
	    case 4: {        // not
		int m1_i = *vliw[i].m1ptr;
		*vliw[i].outptr = ~m1_i;
		} break;

	    case 5:        // add
		*vliw[i].outptr = *vliw[i].m1ptr + *vliw[i].m2ptr;
		break;

	    case 6:        // sub
		*vliw[i].outptr = *vliw[i].m1ptr - *vliw[i].m2ptr;
		break;

	    case 7:        // mul
		*vliw[i].outptr = *vliw[i].m1ptr * *vliw[i].m2ptr;
		break;

	    case 8:        // div
		*vliw[i].outptr = *vliw[i].m1ptr / *vliw[i].m2ptr;
		break;

	    default: break;
	}
	i++;
    }
}
