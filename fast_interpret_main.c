#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "fast_interpret.h"


static double gout=123456, gm2=1;


static double msec(struct timespec gstart, struct timespec gend) {
    unsigned long long nsec;
    nsec = 1000ULL*1000*1000*(gend.tv_sec - gstart.tv_sec) + gend.tv_nsec - gstart.tv_nsec;
    double msec = nsec/1000./1000.;
    return msec;
}


int main() {
    double loopct;
    double loopdec1=1;
    double reljump=-DATANUM;
    static double plus3=3;

    printf("Datanum: %d, Repeat: %d, gout: %f, gm2: %f\n", DATANUM, REPEAT, gout, gm2);
    static struct timespec t1, t2;
    struct _vliw *vliw = malloc((DATANUM+1)*sizeof(struct _vliw)); // +1 exit
    for (int i=0; i<DATANUM-2; i++) {
	vliw[i].opcode = 5; // add
	vliw[i].outptr = &gout;
	vliw[i].m1ptr = &gout;
	vliw[i].m2ptr = &gm2;
    }
    vliw[DATANUM-3].m2ptr = &plus3; // 1+2 ... result comp. (sub; jnz)

    vliw[DATANUM-2].opcode = 6; // sub
    vliw[DATANUM-2].outptr = &loopct;
    vliw[DATANUM-2].m1ptr = &loopct;
    vliw[DATANUM-2].m2ptr = &loopdec1;

    vliw[DATANUM-1].opcode = 14; // jnz
    vliw[DATANUM-1].outptr = &loopct;
    vliw[DATANUM-1].m1ptr = &loopct;   // compared value
    vliw[DATANUM-1].m2ptr = &reljump;  // jump

    vliw[DATANUM+0].opcode = 15; // +1 exit
    vliw[DATANUM+0].outptr = &loopct;
    vliw[DATANUM+0].m1ptr = &loopct;
    vliw[DATANUM+0].m2ptr = &loopdec1;

    loopct=REPEAT; // reinit loopct
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_calltable(vliw);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("calltable --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

    loopct=REPEAT; // reinit loopct
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_switch(vliw);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("switch    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_direct(&gout, gm2, REPEAT*DATANUM);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("direkt    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

}
