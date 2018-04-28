use interpreter_calltable::VliwStruct;

pub struct InterpretCalltable {
    instructions: Vec<fn(&mut InterpretCalltable, rid: &VliwStruct) -> usize>,
    reg: Vec<f64>,
    callstack: Vec<usize>,
}

impl InterpretCalltable {
    fn instr_and(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = (self.reg[rid.2] as u64 & self.reg[rid.3] as u64) as f64; 0 }
    fn instr_or (&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = (self.reg[rid.2] as u64 | self.reg[rid.3] as u64) as f64; 0 }
    fn instr_xor(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = (self.reg[rid.2] as u64 ^ self.reg[rid.3] as u64) as f64; 0 }
    fn instr_shl(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = ((self.reg[rid.2] as u64) << self.reg[rid.3] as u64) as f64; 0 }
    fn instr_shr(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = ((self.reg[rid.2] as u64) >> self.reg[rid.3] as u64) as f64; 0 }
    fn instr_add(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = self.reg[rid.2] + self.reg[rid.3]; 0 }
    fn instr_sub(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = self.reg[rid.2] - self.reg[rid.3]; 0 }
    fn instr_mul(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = self.reg[rid.2] * self.reg[rid.3]; 0 }
    fn instr_div(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = self.reg[rid.2] / self.reg[rid.3]; 0 }
    fn instr_mod(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1] = self.reg[rid.2] % self.reg[rid.3]; 0 }

    fn instr_incjl (&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1]+=1.; if self.reg[rid.1] < self.reg[rid.3] { self.reg[rid.2] as isize as usize } else {0} }
    fn instr_decjge(&mut self, rid: &VliwStruct) -> usize { self.reg[rid.1]-=1.; if self.reg[rid.1] >= self.reg[rid.3] { self.reg[rid.2] as isize as usize } else {0} }
    fn instr_je    (&mut self, rid: &VliwStruct) -> usize { if self.reg[rid.1] == self.reg[rid.3] { self.reg[rid.2] as isize as usize } else { 0 } }
    fn instr_jne   (&mut self, rid: &VliwStruct) -> usize { if self.reg[rid.1] != self.reg[rid.3] { self.reg[rid.2] as isize as usize } else { 0 } }
    fn instr_call(&mut self, rid: &VliwStruct) -> usize { self.callstack.push(self.reg[rid.3] as usize); self.reg[rid.2] as usize }
    fn instr_ret(&mut self, _rid: &VliwStruct) -> usize { self.callstack.pop().unwrap() }

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
	    progct += self.instructions[vliw_struct[progct].0 & 0x0f](self, &vliw_struct[progct]);
	    progct+=1;
	}
	return self.reg.clone();
    }
}
