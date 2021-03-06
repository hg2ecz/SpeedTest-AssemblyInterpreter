struct _vliw {
    int opcode;
    double *outptr;
    double *m1ptr;
    double *m2ptr;
};

void interpret_direct(double *out, double m2, int len);
void interpret_calltable(const struct _vliw *vliw);
void interpret_switch(const struct _vliw *vliw);
void interpret_goto(const struct _vliw *vliw);
void interpret_goto(const struct _vliw *vliw);
void interpret_goto_jit(const struct _vliw *vliw, int len);
void interpret_goto_jit_mod(const struct _vliw *vliw, int len);
void interpret_goto_jit2(const struct _vliw *vliw, int len);
void interpret_goto_jit3(const struct _vliw *vliw, int len);
void interpret_goto_jit3_mod(const struct _vliw *vliw, int len);
