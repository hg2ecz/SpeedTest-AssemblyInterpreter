// Assembly code: https://rust.godbolt.org/z/3sEfq3Wox
fn interpret_example(opcodes: &[u8]) -> i32 {
    let mut instrptr = 0;
    let mut reg: i32 = 0;

    loop {
        match opcodes[instrptr] & 0x07 {
            0x00 => reg += 133,
            0x01 => reg -= 133,
            0x02 => reg += 155,
            0x03 => reg -= 155,
            0x04 => reg += 177,
            0x05 => reg -= 177,
            0x06 => instrptr = reg as usize, // JMP
            _ => return reg,                 // 0x07: can't compile (Rust 1.70)
        }
        instrptr += 1;
    }
}
