// == Direct ==

pub fn interpret_direct(out: &mut f64, m2: f64) {
    *out += m2;
}

pub fn interpret_direct2(out: f64, m2: f64) -> f64 {
    out + m2
}

// == Switch ==

pub enum Vliw {
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

pub fn interpret_switch(vliw: Vec<Vliw>, mut reg: Vec<f64>) -> Vec<f64> {
    let invalid = usize::max_value()/2;
    let mut callstack: Vec<usize> = vec![invalid];
    let mut progct: usize=0;
    while progct < invalid {
	match vliw[progct] { // hint: NOP <== and 0, 0, 0  ( out = reg0 & reg0 )
	    Vliw::And (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 & reg[m2ptr] as u64) as f64,
	    Vliw::Or  (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 | reg[m2ptr] as u64) as f64,
	    Vliw::Xor (outptr, m1ptr, m2ptr)  => reg[outptr] = (reg[m1ptr] as u64 ^ reg[m2ptr] as u64) as f64,
	    Vliw::Shl (outptr, m1ptr, m2ptr)  => reg[outptr] = ((reg[m1ptr] as u64) << reg[m2ptr] as u64) as f64,
	    Vliw::Shr (outptr, m1ptr, m2ptr)  => reg[outptr] = ((reg[m1ptr] as u64) >> reg[m2ptr] as u64) as f64,
	    Vliw::Add (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] + reg[m2ptr],
	    Vliw::Sub (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] - reg[m2ptr],
	    Vliw::Mul (outptr, m1ptr, m2ptr)  => reg[outptr] = reg[m1ptr] * reg[m2ptr],

	    Vliw::Div   (outptr, m1ptr, m2ptr)=> reg[outptr] = reg[m1ptr] / reg[m2ptr],
	    Vliw::Mod   (outptr, m1ptr, m2ptr)=> reg[outptr] = reg[m1ptr] % reg[m2ptr],
	    Vliw::Incjl (outptr, m1ptr, m2ptr)=> {reg[outptr]+=1.; if reg[outptr] <  reg[m2ptr] {progct+=reg[m1ptr] as usize}},
	    Vliw::Decjge(outptr, m1ptr, m2ptr)=> {reg[outptr]-=1.; if reg[outptr] >= reg[m2ptr] {progct+=reg[m1ptr] as usize}},

	    Vliw::Je    (outptr, m1ptr, m2ptr)=> if reg[outptr] == reg[m2ptr] {progct+=reg[m1ptr] as usize},
	    Vliw::Jne   (outptr, m1ptr, m2ptr)=> if reg[outptr] != reg[m2ptr] {progct+=reg[m1ptr] as usize},

	    Vliw::Call  (_outptr, m1ptr, m2ptr)=> {
		callstack.push(m2ptr);
		progct += m1ptr;
	    },
	    Vliw::Ret   (_outptr, _m1ptr, _m2ptr)=> if let Some(ret) = callstack.pop() { progct += ret; } // return (last return --> exit)
	}
	progct+=1;
    }
    reg
}
