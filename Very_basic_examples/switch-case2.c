// Assembly code: https://c.godbolt.org/z/3nMeP9fW9
#include <stddef.h>
#include <stdint.h>

enum OpCode {
    Add133,
    Sub133,
    Add155,
    Sub155,
    Add177,
    Sub177,
    RegToPc,
    Ret,
};

int32_t interpret_example(enum OpCode *opcodes) {
    size_t instrptr=0;
    int32_t reg=0;

    while (1) {
        switch (opcodes[instrptr++] & 0x07) {     // & 0x07 - faster!
            case Add133: reg += 133; break;
            case Sub133: reg -= 133; break;
            case Add155: reg += 155; break;
            case Sub155: reg -= 155; break;
            case Add177: reg += 177; break;
            case Sub177: reg -= 177; break;
            case RegToPc: instrptr = reg; break; // JMP
            case Ret: return reg; break;
        }
    }
}
