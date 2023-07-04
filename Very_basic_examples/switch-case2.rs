// Assembly code: https://rust.godbolt.org/z/M9Pj3Kvja
const PROGLEN: usize = 0x10_000;

enum OpCode {
    Add133,
    Sub133,
    Add155,
    Sub155,
    Add177,
    Sub177,
    RegToPc,
    Ret,
}

fn interpret_example(opcodes: &[OpCode; PROGLEN]) -> i32 {
    let mut instrptr = 0;
    let mut reg: i32 = 0;

    loop {
        instrptr %= PROGLEN;
        match opcodes[instrptr] {
            OpCode::Add133 => reg += 133,
            OpCode::Sub133 => reg -= 133,
            OpCode::Add155 => reg += 155,
            OpCode::Sub155 => reg -= 155,
            OpCode::Add177 => reg += 177,
            OpCode::Sub177 => reg -= 177,
            OpCode::RegToPc => instrptr = reg as usize, // JMP
            OpCode::Ret => return reg,
        }
        instrptr += 1;
    }
}
