// Assembly code: https://rust.godbolt.org/z/dzWjqPfW9
const PROGLEN: usize = 0x10_000;

fn interpret_example(opcodes: &[u8; PROGLEN]) -> i32 {
    let mut instrptr = 0;
    let mut reg: i32 = 0;

    loop {
        instrptr %= PROGLEN;
        match opcodes[instrptr] & 0x07 {
            0x00 => reg += 133,
            0x01 => reg -= 133,
            0x02 => reg += 155,
            0x03 => reg -= 155,
            0x04 => reg += 177,
            0x05 => reg -= 177,
            0x06 => instrptr = reg as usize, // JMP
            0x07 => return reg,
            _ => (),                         // this syntax needs to 0x07 (Rust 1.70)
        }
        instrptr += 1;
    }
}
