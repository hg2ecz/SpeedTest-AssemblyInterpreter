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
    double repeatnum=REPEAT;
    double reljump=-DATANUM;
    double plus2=2;

    double loopct;

    printf("Datanum: %d, Repeat: %d, gout: %f, gm2: %f\n", DATANUM, REPEAT, gout, gm2);
    static struct timespec t1, t2;
    struct _vliw *vliw = malloc((DATANUM+1)*sizeof(struct _vliw)); // +1 exit
    for (int i=0; i<DATANUM-1; i++) {
	vliw[i].opcode = 5; // add
	vliw[i].outptr = &gout;
	vliw[i].m1ptr = &gout;
	vliw[i].m2ptr = &gm2;
    }
    vliw[DATANUM-2].m2ptr = &plus2; // 1+2 ... result comp. (sub; jnz)

    vliw[DATANUM-1].opcode = 10;       // incjl
    vliw[DATANUM-1].outptr = &loopct;
    vliw[DATANUM-1].m1ptr = &reljump;  // jump
    vliw[DATANUM-1].m2ptr = &repeatnum;// compared value

    vliw[DATANUM+0].opcode = 15; // +1 ret (exit)
    vliw[DATANUM+0].outptr = &loopct;
    vliw[DATANUM+0].m1ptr = &loopct;
    vliw[DATANUM+0].m2ptr = &repeatnum;

    double gouttest=0; interpret_direct(&gouttest, gm2, REPEAT*DATANUM); // cpu wake from sleep (ondemand)

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_direct(&gout, gm2, REPEAT*DATANUM);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("direkt    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));


    loopct=0; // reinit loopct
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_calltable(vliw);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("calltable --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

    loopct=0; // reinit loopct
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    interpret_switch(vliw);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("switch    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));


}
