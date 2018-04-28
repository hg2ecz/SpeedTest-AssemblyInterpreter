use interpreter_calltable::VliwStruct;

pub struct InterpretCalltable {
    instructions: Vec<fn(&mut InterpretCalltable, out: usize, m1: usize, m2: usize) -> usize>,
    reg: Vec<f64>,
    callstack: Vec<usize>,
}

impl InterpretCalltable {
    fn instr_and(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = (self.reg[m1] as u64 & self.reg[m2] as u64) as f64; 0 }
    fn instr_or (&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = (self.reg[m1] as u64 | self.reg[m2] as u64) as f64; 0 }
    fn instr_xor(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = (self.reg[m1] as u64 ^ self.reg[m2] as u64) as f64; 0 }
    fn instr_shl(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = ((self.reg[m1] as u64) << self.reg[m2] as u64) as f64; 0 }
    fn instr_shr(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = ((self.reg[m1] as u64) >> self.reg[m2] as u64) as f64; 0 }
    fn instr_add(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = self.reg[m1] + self.reg[m2]; 0 }
    fn instr_sub(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = self.reg[m1] - self.reg[m2]; 0 }
    fn instr_mul(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = self.reg[m1] * self.reg[m2]; 0 }
    fn instr_div(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = self.reg[m1] / self.reg[m2]; 0 }
    fn instr_mod(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out] = self.reg[m1] % self.reg[m2]; 0 }

    fn instr_incjl (&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out]+=1.; if self.reg[out] < self.reg[m2] { self.reg[m1] as isize as usize } else {0} }
    fn instr_decjge(&mut self, out: usize, m1: usize, m2: usize) -> usize { self.reg[out]-=1.; if self.reg[out] >= self.reg[m2] { self.reg[m1] as isize as usize } else {0} }
    fn instr_je    (&mut self, out: usize, m1: usize, m2: usize) -> usize { if self.reg[out] == self.reg[m2] { self.reg[m1] as isize as usize } else { 0 } }
    fn instr_jne   (&mut self, out: usize, m1: usize, m2: usize) -> usize { if self.reg[out] != self.reg[m2] { self.reg[m1] as isize as usize } else { 0 } }
    fn instr_call(&mut self, _out: usize, m1: usize, m2: usize) -> usize { self.callstack.push(self.reg[m2] as usize); self.reg[m1] as usize }
    fn instr_ret(&mut self, _out: usize, _m1: usize, _m2: usize) -> usize { self.callstack.pop().unwrap() }

    pub fn new() -> InterpretCalltable {
	let invalid = usize::max_value()/2;
	InterpretCalltable {
		    callstack: vec![invalid],
		    reg: vec![],
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

    pub fn interpret_calltable(&mut self, vliw_struct: &Vec<VliwStruct>, reg: Vec<f64>) -> Vec<f64> {
	let invalid = usize::max_value()/2;
	self.callstack = vec![invalid];
	self.reg = reg;
	let mut progct: usize=0;

	while progct < invalid {
	    progct += self.instructions[vliw_struct[progct].0 & 0x0f](self, vliw_struct[progct].1, vliw_struct[progct].2, vliw_struct[progct].3);
	    progct+=1;
	}
	return self.reg.clone();
    }
}
