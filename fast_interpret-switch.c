#include <limits.h>
#include "fast_interpret.h"

#define CALLSTACKMAX 1024

void interpret_switch(struct _vliw *vliw) {
    int callstack[CALLSTACKMAX] = {INT_MIN};
    unsigned int callstackptr = 0;
    int progct=0;
    while (progct >= 0) {
	switch(vliw[progct].opcode) {
	    case 0: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr & (long long)*vliw[progct].m2ptr; break;
	    case 1: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr | (long long)*vliw[progct].m2ptr; break;
	    case 2: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr ^ (long long)*vliw[progct].m2ptr; break;
	    case 3: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr << (unsigned int)*vliw[progct].m2ptr; break;
	    case 4: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr >> (unsigned int)*vliw[progct].m2ptr; break;
	    case 5: *vliw[progct].outptr = *vliw[progct].m1ptr + *vliw[progct].m2ptr; break;
	    case 6: *vliw[progct].outptr = *vliw[progct].m1ptr - *vliw[progct].m2ptr; break;
	    case 7: *vliw[progct].outptr = *vliw[progct].m1ptr * *vliw[progct].m2ptr; break;
	    case 8: *vliw[progct].outptr = *vliw[progct].m1ptr / *vliw[progct].m2ptr; break;
	    case 9: *vliw[progct].outptr = (long long)*vliw[progct].m1ptr % (int)*vliw[progct].m2ptr; break;

	    case 10: progct += ++*vliw[progct].outptr <  *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // incjl
	    case 11: progct += --*vliw[progct].outptr >= *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // decjge

	    case 12: progct += *vliw[progct].outptr == *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // cmpje
	    case 13: progct += *vliw[progct].outptr != *vliw[progct].m2ptr ? *vliw[progct].m1ptr : 0; break; // cmpjne

	    case 14:
		if (callstackptr >= CALLSTACKMAX-1) callstackptr=1;
		callstack[++callstackptr] = *vliw[progct].m2ptr;
		progct += *vliw[progct].m1ptr;
		break;
	    case 15: progct += callstack[callstackptr--];
	    default: break;
	}
	progct++;
    }
}
