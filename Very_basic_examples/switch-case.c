// Assembly code: https://c.godbolt.org/z/4bjK8adoj
#include <stddef.h>
#include <stdint.h>

int32_t interpret_example(unsigned char *opcodes) {
    size_t instrptr=0;
    int32_t reg=0;

    while (1) {
        switch (opcodes[instrptr++] & 0x07) {
            case 0x00: reg += 133; break;
            case 0x01: reg -= 133; break;
            case 0x02: reg += 155; break;
            case 0x03: reg -= 155; break;
            case 0x04: reg += 177; break;
            case 0x05: reg -= 177; break;
            case 0x06: instrptr = reg; break; // JMP
            default: return reg; break;       // case 0x07: less step on gcc-13? Why?
        }
    }
}
