// Assembly code: https://c.godbolt.org/z/aoGhTfK5r
#include <stddef.h>
#include <stdint.h>

// Opcodes need prefiltering between 0..7, so no runtime check here.
int32_t interpret_goto_example(const uint8_t *opcodes) {
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
                goto *inst_t[opcodes[instrptr++]];
    sub_133:
                reg -= 133;
                goto *inst_t[opcodes[instrptr++]];
    add_155:
                reg += 155;
                goto *inst_t[opcodes[instrptr++]];
    sub_155:
                reg -= 155;
                goto *inst_t[opcodes[instrptr++]];
    add_177:
                reg += 177;
                goto *inst_t[opcodes[instrptr++]];
    sub_177:
                reg -= 177;
                goto *inst_t[opcodes[instrptr++]];
    reg_to_pc:
                instrptr = reg;                    // JMP
                goto *inst_t[opcodes[instrptr++]];
    ret:
                return reg;
}
