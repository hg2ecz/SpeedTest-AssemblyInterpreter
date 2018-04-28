fn instr_and(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 & m2 as u64) as f64, 0) }
fn instr_or (_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 | m2 as u64) as f64, 0) }
fn instr_xor(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 ^ m2 as u64) as f64, 0) }
fn instr_shl(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (((m1 as u64) << m2 as u64) as f64, 0) }
fn instr_shr(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (((m1 as u64) >> m2 as u64) as f64, 0) }
fn instr_add(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 + m2, 0) }
fn instr_sub(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 - m2, 0) }
fn instr_mul(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 * m2, 0) }
fn instr_div(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 / m2, 0) }
fn instr_mod(_callstack: &mut Vec<usize>, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 % m2, 0) }

fn instr_incjl (_callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize) { let o = out+1.; if o < m2 { (o, m1 as isize as usize) } else {(o, 0)} }
fn instr_decjge(_callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize) { let o = out-1.; if o >= m2 { (o, m1 as isize as usize) } else {(o, 0)} }
fn instr_je    (_callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize) { if out == m2 { (out, m1 as isize as usize) } else { (out, 0)} }
fn instr_jne   (_callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize) { if out != m2 { (out, m1 as isize as usize) } else { (out, 0)} }
fn instr_call(callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize) { callstack.push(m2 as usize); (out, m1 as usize) }
fn instr_ret(callstack: &mut Vec<usize>, out:f64, _m1: f64, _m2: f64) -> (f64, usize) { (out, callstack.pop().unwrap()) }

pub struct VliwStruct (
    pub usize,
    pub usize,
    pub usize,
    pub usize
);

pub fn interpret_calltable(vliw_struct: &Vec<VliwStruct>, mut reg: Vec<f64>) -> Vec<f64> {
    let instructions: Vec<fn(callstack: &mut Vec<usize>, out:f64, m1: f64, m2: f64) -> (f64, usize)> = vec![
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

    let invalid = usize::max_value()/2;
    let mut callstack: Vec<usize>=vec![invalid];
    let mut progct: usize=0;

    while progct < invalid {
	let res=instructions[vliw_struct[progct].0 & 0x0f](&mut callstack, reg[vliw_struct[progct].1], reg[vliw_struct[progct].2], reg[vliw_struct[progct].3]);
	reg[vliw_struct[progct].1] = res.0; progct+=res.1;
	progct+=1;
    }
    return reg;
}
