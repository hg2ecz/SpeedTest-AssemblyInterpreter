#include "fast_interpret.h"

void interpret_direct(double *out, double m2, int len) {
    for (int i=0; i<len; i++) *out += m2;
}
