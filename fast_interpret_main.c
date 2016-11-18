#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "fast_interpret.h"

double gout=42, gm2=1;

static double msec(struct timespec gstart, struct timespec gend) {
    unsigned long long nsec;
    nsec = 1000ULL*1000*1000*(gend.tv_sec - gstart.tv_sec) + gend.tv_nsec - gstart.tv_nsec;
    double msec = nsec/1000./1000.;
    return msec;
}


int main() {
    printf("Datanum: %d, Repeat: %d, gout: %f, gm2: %f\n", DATANUM, REPEAT, gout, gm2);
    static struct timespec t1, t2;
    struct _vliw *vliw = malloc(DATANUM*sizeof(struct _vliw));
    for (int i=0; i<DATANUM; i++) {
	vliw[i].opcode = 5; // add
	vliw[i].outptr = &gout;
	vliw[i].m1ptr = &gout;
	vliw[i].m2ptr = &gm2;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    for (int i=0; i<REPEAT; i++) interpret_calltable(vliw, DATANUM);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("calltable --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    for (int i=0; i<REPEAT; i++) interpret_switch(vliw, DATANUM);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("switch    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);
    for (int i=0; i<REPEAT; i++) interpret_direct(&gout, gm2, DATANUM);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
    printf("direkt    --> gout: %f msec: %f, megainstrpersec: %f\n", gout, msec(t1, t2), 0.001*DATANUM*REPEAT/msec(t1, t2));

}
