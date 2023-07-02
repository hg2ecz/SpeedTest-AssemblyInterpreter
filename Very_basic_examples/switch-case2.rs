// Assembly code: https://rust.godbolt.org/z/YxWG8rc6Y
pub enum OpCode {
    Add133,
    Sub133,
    Add155,
    Sub155,
    Add177,
    Sub177,
    RegToPc,
    Ret,
}

pub fn interpret_example(opcodes: &[OpCode]) -> i32 {
    let mut instrptr = 0;
    let mut reg: i32 = 0;

    loop {
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
