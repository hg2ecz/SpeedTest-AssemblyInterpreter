#include <limits.h>
#include "fast_interpret.h"

void interpret_switch(struct _vliw *vliw) {
    int progct=0;
    _Bool run=1;
    while (run) {
	switch(vliw[progct].opcode) {
	    case 0: break; // nop
	    case 1: {       // and
		int m1_i = *vliw[progct].m1ptr;
		int m2_i = *vliw[progct].m2ptr;
		*vliw[progct].outptr = m1_i & m2_i;
		} break;
	    case 2: {        // or
		int m1_i = *vliw[progct].m1ptr;
		int m2_i = *vliw[progct].m2ptr;
		*vliw[progct].outptr = m1_i | m2_i;
		} break;
	    case 3: {        // xor
		int m1_i = *vliw[progct].m1ptr;
		int m2_i = *vliw[progct].m2ptr;
		*vliw[progct].outptr = m1_i ^ m2_i;
		} break;
	    case 4: {        // not
		int m1_i = *vliw[progct].m1ptr;
		*vliw[progct].outptr = ~m1_i;
		} break;

	    case 5:        // add
		*vliw[progct].outptr = *vliw[progct].m1ptr + *vliw[progct].m2ptr;
		break;

	    case 6:        // sub
		*vliw[progct].outptr = *vliw[progct].m1ptr - *vliw[progct].m2ptr;
		break;

	    case 7:        // mul
		*vliw[progct].outptr = *vliw[progct].m1ptr * *vliw[progct].m2ptr;
		break;

	    case 8:        // div
		*vliw[progct].outptr = *vliw[progct].m1ptr / *vliw[progct].m2ptr;
		break;

	    case 9: break; // nop
	    case 10: break; // nop
	    case 11: break; // nop
	    case 12: break; // nop
	    case 13: break; // nop

	    case 14:
		if (*vliw[progct].m1ptr) progct+=*vliw[progct].m2ptr;
		break;
	    case 15:
		run=0;
		break;

	    default: break;
	}
	progct++;
    }
}
