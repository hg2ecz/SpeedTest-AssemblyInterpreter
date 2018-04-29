#include <limits.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_switch(const struct _vliw *vliw) {
    int callstack[CALLSTACKMAX] = {INT_MIN};
    unsigned int callstackptr = 0;
    int progct=0;
    while (progct >= 0) {
	switch(vliw[progct].opcode) {
		    // hint: NOP <== and 0, 0, 0  ( out = reg0 & reg0 )
	    case 0: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr & (long long)*vliw[progct].m2ptr; break; // and
	    case 1: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr | (long long)*vliw[progct].m2ptr; break; // or
	    case 2: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr ^ (long long)*vliw[progct].m2ptr; break; // xor
	    case 3: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr << (unsigned int)*vliw[progct].m2ptr; break; // shl
	    case 4: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr >> (unsigned int)*vliw[progct].m2ptr; break; // shr
	    case 5: *vliw[progct].outptr = *vliw[progct].m1ptr + *vliw[progct].m2ptr; break; // add
	    case 6: *vliw[progct].outptr = *vliw[progct].m1ptr - *vliw[progct].m2ptr; break; // sub
	    case 7: *vliw[progct].outptr = *vliw[progct].m1ptr * *vliw[progct].m2ptr; break; // mul
	    case 8: *vliw[progct].outptr = *vliw[progct].m1ptr / *vliw[progct].m2ptr; break; // div
	    case 9: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr % (int)*vliw[progct].m2ptr; break; // mod

	    case 10: progct += ++*vliw[progct].outptr <  *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // incjl
	    case 11: progct += --*vliw[progct].outptr >= *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // decjge

	    case 12: progct += *vliw[progct].outptr == *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // cmpje
	    case 13: progct += *vliw[progct].outptr != *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // cmpjne

	    case 14:     // call
		if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
		callstack[++callstackptr] = *vliw[progct].m2ptr;
		progct += *vliw[progct].m1ptr;
		break;
	    case 15: progct += callstack[callstackptr--]; break; // return (last return --> exit)
	    default: break;
	}
	progct++;
    }
}
