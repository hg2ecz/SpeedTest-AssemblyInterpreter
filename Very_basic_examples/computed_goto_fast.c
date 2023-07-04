// Assembly code: https://c.godbolt.org/z/Ksj8sb6z6
#include <stddef.h>
#include <stdint.h>

#define PROGLEN 0x10000

// Opcodes need prefiltering between 0..7, so no runtime check here.
int32_t interpret_example(unsigned char opcodes[PROGLEN]) {
    size_t instrptr=0;
    int32_t reg=0;

    static void *inst_t[] = {
       &&add_133,
       &&sub_133,
       &&add_155,
       &&sub_155,
       &&add_177,
       &&sub_177,
       &&reg_to_pc,
       &&ret,
    };

    add_133:
                reg += 133;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    sub_133:
                reg -= 133;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    add_155:
                reg += 155;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    sub_155:
                reg -= 155;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    add_177:
                reg += 177;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    sub_177:
                reg -= 177;
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    reg_to_pc:
                instrptr = reg;                    // JMP
                instrptr %= PROGLEN;
                goto *inst_t[opcodes[instrptr++]];
    ret:
                return reg;
}
