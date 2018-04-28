use interpreter_calltable::VliwStruct;

pub fn interpret_switch(vliw_struct: &Vec<VliwStruct>, mut reg: Vec<f64>) -> Vec<f64> {
    let invalid = usize::max_value()/2;
    let mut callstack: Vec<usize> = vec![invalid];
    let mut progct: usize=0;
    while progct < invalid {
	let outptr = vliw_struct[progct].1;
	let m1ptr = vliw_struct[progct].2;
	let m2ptr = vliw_struct[progct].3;
	match vliw_struct[progct].0 { // hint: NOP <== and 0, 0, 0  ( out = reg0 & reg0 )
	    0 => reg[outptr] = (reg[m1ptr] as u64 & reg[m2ptr] as u64) as f64,
	    1 => reg[outptr] = (reg[m1ptr] as u64 | reg[m2ptr] as u64) as f64,
	    2 => reg[outptr] = (reg[m1ptr] as u64 ^ reg[m2ptr] as u64) as f64,
	    3 => reg[outptr] = ((reg[m1ptr] as u64) << reg[m2ptr] as u64) as f64,
	    4 => reg[outptr] = ((reg[m1ptr] as u64) >> reg[m2ptr] as u64) as f64,
	    5 => reg[outptr] = reg[m1ptr] + reg[m2ptr],
	    6 => reg[outptr] = reg[m1ptr] - reg[m2ptr],
	    7 => reg[outptr] = reg[m1ptr] * reg[m2ptr],

	    8 => reg[outptr] = reg[m1ptr] / reg[m2ptr],
	    9 => reg[outptr] = reg[m1ptr] % reg[m2ptr],
	    10 => {reg[outptr]+=1.; if reg[outptr] <  reg[m2ptr] {progct+=reg[m1ptr] as isize as usize}},
	    11 => {reg[outptr]-=1.; if reg[outptr] >= reg[m2ptr] {progct+=reg[m1ptr] as isize as usize}},

	    12 => if reg[outptr] == reg[m2ptr] {progct+=reg[m1ptr] as isize as usize},
	    13 => if reg[outptr] != reg[m2ptr] {progct+=reg[m1ptr] as isize as usize},

	    14 => {
		callstack.push(m2ptr);
		progct += m1ptr;
	    },
	    15 => progct += callstack.pop().unwrap(), // return (last return --> exit)
	    _  => println!("Unknown token"),
	}
	progct+=1;
    }
    reg
}
