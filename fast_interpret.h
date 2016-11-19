struct _vliw {
    int opcode;
    double *outptr;
    double *m1ptr;
    double *m2ptr;
};

void interpret_calltable(struct _vliw *vliw);
void interpret_switch(struct _vliw *vliw);
void interpret_direct(double *out, double m2, int len);
