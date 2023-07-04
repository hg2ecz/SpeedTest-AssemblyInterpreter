// Assembly code: https://c.godbolt.org/z/K46bK1rbK
#include <stddef.h>
#include <stdint.h>

#define PROGLEN 0x10000

int32_t interpret_example(unsigned char opcodes[PROGLEN]) {
    size_t instrptr=0;
    int32_t reg=0;

    while (1) {
        instrptr %= PROGLEN;
        switch (opcodes[instrptr++] & 0x07) {
            case 0x00: reg += 133; break;
            case 0x01: reg -= 133; break;
            case 0x02: reg += 155; break;
            case 0x03: reg -= 155; break;
            case 0x04: reg += 177; break;
            case 0x05: reg -= 177; break;
            case 0x06: instrptr = reg; break; // JMP
            case 0x07: return reg; break;
        }
    }
}
