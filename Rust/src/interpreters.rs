// == Direct ==

pub fn interpret_direct(out: &mut f64, m2: f64) {
    *out += m2;
}

pub fn interpret_direct2(out: f64, m2: f64) -> f64 {
    out + m2
}

// == Switch ==

#[allow(dead_code)]
pub enum VliwEnum {
    And (usize, usize, usize), // 0x00
    Or  (usize, usize, usize), // 0x01
    Xor (usize, usize, usize), // 0x02
    Shl (usize, usize, usize), // 0x03
    Shr (usize, usize, usize), // 0x04
    Add (usize, usize, usize), // 0x05
    Sub (usize, usize, usize), // 0x06
    Mul (usize, usize, usize), // 0x07

    Div   (usize, usize, usize), // 0x08
    Mod   (usize, usize, usize), // 0x09
    Incjl (usize, usize, usize), // 0x0a
    Decjge(usize, usize, usize), // 0x0b
    Je    (usize, usize, usize), // 0x0c
    Jne   (usize, usize, usize), // 0x0d
    Call  (usize, usize, usize), // 0x0e
    Ret   (usize, usize, usize), // 0x0f
}

pub fn interpret_switch(vliw_enum: Vec<VliwEnum>, mut reg: Vec<f64>) -> Vec<f64> {
    let invalid = isize::min_value();
    let mut callstack: Vec<isize> = vec![invalid];
    let mut progct: isize=0;
    while progct >= 0 {
	let pct = progct as usize;
	match vliw_enum[pct] { // hint: NOP <== and 0, 0, 0  ( out = reg0 & reg0 )
	    VliwEnum::And (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 & reg[m2ptr] as u64) as f64,
	    VliwEnum::Or  (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 | reg[m2ptr] as u64) as f64,
	    VliwEnum::Xor (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 ^ reg[m2ptr] as u64) as f64,
	    VliwEnum::Shl (outptr, m1ptr, m2ptr)  => reg[outptr] = ((reg[m1ptr] as u64) << reg[m2ptr] as u64) as f64,
	    VliwEnum::Shr (outptr, m1ptr, m2ptr)  => reg[outptr] = ((reg[m1ptr] as u64) >> reg[m2ptr] as u64) as f64,
	    VliwEnum::Add (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] + reg[m2ptr],
	    VliwEnum::Sub (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] - reg[m2ptr],
	    VliwEnum::Mul (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] * reg[m2ptr],

	    VliwEnum::Div   (outptr, m1ptr, m2ptr)=> reg[outptr] = reg[m1ptr] / reg[m2ptr],
	    VliwEnum::Mod   (outptr, m1ptr, m2ptr)=> reg[outptr] = reg[m1ptr] % reg[m2ptr],
	    VliwEnum::Incjl (outptr, m1ptr, m2ptr)=> {reg[outptr]+=1.; if reg[outptr] <  reg[m2ptr] {progct+=reg[m1ptr] as isize}},
	    VliwEnum::Decjge(outptr, m1ptr, m2ptr)=> {reg[outptr]-=1.; if reg[outptr] >= reg[m2ptr] {progct+=reg[m1ptr] as isize}},

	    VliwEnum::Je    (outptr, m1ptr, m2ptr)=> if reg[outptr] == reg[m2ptr] {progct+=reg[m1ptr] as isize},
	    VliwEnum::Jne   (outptr, m1ptr, m2ptr)=> if reg[outptr] != reg[m2ptr] {progct+=reg[m1ptr] as isize},

	    VliwEnum::Call  (_outptr, m1ptr, m2ptr)=> {
		callstack.push(reg[m2ptr] as isize);
		progct += reg[m1ptr] as isize;
	    },
	    VliwEnum::Ret   (_outptr, _m1ptr, _m2ptr)=> progct += callstack.pop().unwrap(), // return (last return --> exit)
	}
	progct+=1;
    }
    reg
}

// == Calltable ==

fn instr_and(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { ((m1 as u64 & m2 as u64) as f64, 0) }
fn instr_or (_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { ((m1 as u64 | m2 as u64) as f64, 0) }
fn instr_xor(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { ((m1 as u64 ^ m2 as u64) as f64, 0) }
fn instr_shl(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (((m1 as u64) << m2 as u64) as f64, 0) }
fn instr_shr(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (((m1 as u64) >> m2 as u64) as f64, 0) }
fn instr_add(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (m1 + m2, 0) }
fn instr_sub(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (m1 - m2, 0) }
fn instr_mul(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (m1 * m2, 0) }
fn instr_div(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (m1 / m2, 0) }
fn instr_mod(_callstack: &mut Vec<isize>, _out:f64, m1: f64, m2: f64) -> (f64, isize) { (m1 % m2, 0) }

fn instr_incjl (_callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize) { let o = out+1.; if o < m2 { (o, m1 as isize) } else {(o, 0)} }
fn instr_decjge(_callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize) { let o = out-1.; if o >= m2 { (o, m1 as isize) } else {(o, 0)} }
fn instr_je    (_callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize) { if out == m2 { (out, m1 as isize) } else { (out, 0)} }
fn instr_jne   (_callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize) { if out != m2 { (out, m1 as isize) } else { (out, 0)} }
fn instr_call(callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize) { callstack.push(m2 as isize); (out, m1 as isize) }
fn instr_ret(callstack: &mut Vec<isize>, out:f64, _m1: f64, _m2: f64) -> (f64, isize) { (out, callstack.pop().unwrap()) }

pub struct VliwStruct (
    pub usize,
    pub usize,
    pub usize,
    pub usize
);

pub fn interpret_calltable(vliw_struct: Vec<VliwStruct>, mut reg: Vec<f64>) -> Vec<f64> {
    let instructions: Vec<fn(callstack: &mut Vec<isize>, out:f64, m1: f64, m2: f64) -> (f64, isize)> = vec![
	instr_and,
	instr_or,
	instr_xor,
	instr_shl,
	instr_shr,
	instr_add,
	instr_sub,
	instr_mul,
	instr_div,
	instr_mod,

	instr_incjl,
	instr_decjge,
	instr_je,
	instr_jne,
	instr_call,
	instr_ret
    ];

    let invalid = isize::min_value();
    let mut callstack: Vec<isize>=vec![invalid];
    let mut progct: isize=0;

    while progct >= 0 {
	let pct = progct as usize;
	let res=instructions[vliw_struct[pct].0 & 0x0f](&mut callstack, reg[vliw_struct[pct].1], reg[vliw_struct[pct].2], reg[vliw_struct[pct].3]);
	reg[vliw_struct[pct].1] = res.0; progct+=res.1;
	progct+=1;
    }
    return reg;
}
