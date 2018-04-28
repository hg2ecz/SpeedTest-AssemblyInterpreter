use interpreter_calltable::VliwStruct;

pub struct InterpretCalltable {
    instructions: Vec<fn(&mut InterpretCalltable, out:f64, m1: f64, m2: f64) -> (f64, usize)>,
    callstack: Vec<usize>,
}

impl InterpretCalltable {
    fn instr_and(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 & m2 as u64) as f64, 0) }
    fn instr_or (&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 | m2 as u64) as f64, 0) }
    fn instr_xor(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { ((m1 as u64 ^ m2 as u64) as f64, 0) }
    fn instr_shl(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (((m1 as u64) << m2 as u64) as f64, 0) }
    fn instr_shr(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (((m1 as u64) >> m2 as u64) as f64, 0) }
    fn instr_add(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 + m2, 0) }
    fn instr_sub(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 - m2, 0) }
    fn instr_mul(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 * m2, 0) }
    fn instr_div(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 / m2, 0) }
    fn instr_mod(&mut self, _out:f64, m1: f64, m2: f64) -> (f64, usize) { (m1 % m2, 0) }

    fn instr_incjl (&mut self, out:f64, m1: f64, m2: f64) -> (f64, usize) { let o = out+1.; if o < m2 { (o, m1 as isize as usize) } else {(o, 0)} }
    fn instr_decjge(&mut self, out:f64, m1: f64, m2: f64) -> (f64, usize) { let o = out-1.; if o >= m2 { (o, m1 as isize as usize) } else {(o, 0)} }
    fn instr_je    (&mut self, out:f64, m1: f64, m2: f64) -> (f64, usize) { if out == m2 { (out, m1 as isize as usize) } else { (out, 0)} }
    fn instr_jne   (&mut self, out:f64, m1: f64, m2: f64) -> (f64, usize) { if out != m2 { (out, m1 as isize as usize) } else { (out, 0)} }
    fn instr_call(&mut self, out:f64, m1: f64, m2: f64) -> (f64, usize) { self.callstack.push(m2 as usize); (out, m1 as usize) }
    fn instr_ret(&mut self, out:f64, _m1: f64, _m2: f64) -> (f64, usize) { (out, self.callstack.pop().unwrap()) }

    pub fn new() -> InterpretCalltable {
	let invalid = usize::max_value()/2;
	InterpretCalltable { callstack: vec![invalid],
		    instructions: vec![
			InterpretCalltable::instr_and,
			InterpretCalltable::instr_or,
			InterpretCalltable::instr_xor,
			InterpretCalltable::instr_shl,
			InterpretCalltable::instr_shr,
			InterpretCalltable::instr_add,
			InterpretCalltable::instr_sub,
			InterpretCalltable::instr_mul,
			InterpretCalltable::instr_div,
			InterpretCalltable::instr_mod,

			InterpretCalltable::instr_incjl,
			InterpretCalltable::instr_decjge,
			InterpretCalltable::instr_je,
			InterpretCalltable::instr_jne,
			InterpretCalltable::instr_call,
			InterpretCalltable::instr_ret
		    ],
	}
    }

    pub fn interpret_calltable(&mut self, vliw_struct: &Vec<VliwStruct>, mut reg: Vec<f64>) -> (Vec<f64>) {
	let invalid = usize::max_value()/2;
	self.callstack = vec![invalid];
	let mut progct: usize=0;

	while progct < invalid {
	    let res=self.instructions[vliw_struct[progct].0 & 0x0f](self, reg[vliw_struct[progct].1], reg[vliw_struct[progct].2], reg[vliw_struct[progct].3]);
	    reg[vliw_struct[progct].1] = res.0; progct+=res.1;
	    progct+=1;
	}
	return reg;
    }
}
