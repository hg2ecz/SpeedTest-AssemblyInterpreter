mod interpreter_direct;
mod interpreter_switch_enum;
mod interpreter_switch_vec;
mod interpreter_calltable;
mod interpreter_calltable_obj;
mod interpreter_calltable_obj2;
mod interpreter_calltable_obj3;

use interpreter_switch_enum::VliwEnum;
use interpreter_calltable::VliwStruct;

use std::time::Instant;

const REPEAT: i32 = 1_000_000;
const DATANUM: i32 = 100;

fn main() {
    let start_time = Instant::now();
    let mut out: f64 = 0.;
    for _i in 0..REPEAT*DATANUM {
	interpreter_direct::interpret_direct(&mut out, 1.);
    }
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Direct:  {} ms (out: {})", milliseconds, out);

    let start_time = Instant::now();
    let mut out: f64 = 0.;
    for _i in 0..REPEAT*DATANUM {
	out = interpreter_direct::interpret_direct2(out, 1.);
    }
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Direct2: {} ms (out: {})", milliseconds, out);


    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let mut vliw_enum: Vec<VliwEnum> = vec![];
    for _i in 0..DATANUM-2 {
	vliw_enum.push(VliwEnum::Add(0, 0, 1));
    }
    // 0: outreg,    1: const 1,    2: const 2
    vliw_enum.push(VliwEnum::Add(0, 0, 2)); // 1+2 ... result comp. (sub; jnz)
    // 3: loopct,    4: reljmp,     5: repeatnum
    vliw_enum.push(VliwEnum::Incjl(3, 4, 5)); // inclj loopct reljump repeatnum
    vliw_enum.push(VliwEnum::Ret(3, 4, 5)); // +1 ret (exit)


    let start_time = Instant::now();
    let res = interpreter_switch_enum::interpret_switch(&vliw_enum, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Switch interpreter enum: {} ms (out: {})", milliseconds, res[0]);


    let mut vliw_struct: Vec<VliwStruct> = vec![];
    for _i in 0..DATANUM-2 {
	vliw_struct.push(VliwStruct(5, 0, 0, 1)); // add
    }
    // 0: outreg,    1: const 1,    2: const 2
    vliw_struct.push(VliwStruct(5, 0, 0, 2)); // 1+2 ... result comp. (sub; jnz)
    // 3: loopct,    4: reljmp,     5: repeatnum
    vliw_struct.push(VliwStruct(10, 3, 4, 5)); // inclj loopct reljump repeatnum
    vliw_struct.push(VliwStruct(15, 3, 4, 5)); // +1 ret (exit)


    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let start_time = Instant::now();
    let res = interpreter_switch_vec::interpret_switch(&vliw_struct, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Switch interpreter vec: {} ms (out: {})", milliseconds, res[0]);


    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let start_time = Instant::now();
    let res = interpreter_calltable::interpret_calltable(&vliw_struct, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Calltable interpreter: {} ms (out: {})", milliseconds, res[0]);


    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let mut interpret_obj = interpreter_calltable_obj::InterpretCalltable::new();
    let start_time = Instant::now();
    let res = interpret_obj.interpret_calltable(&vliw_struct, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Calltable_obj interpreter: {} ms (out: {})", milliseconds, res[0]);

    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let mut interpret_obj = interpreter_calltable_obj2::InterpretCalltable::new();
    let start_time = Instant::now();
    let res = interpret_obj.interpret_calltable(&vliw_struct, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Calltable_obj2 interpreter: {} ms (out: {})", milliseconds, res[0]);


    // out, const1, cons2, loopct, reljmp, repeatnum
    let reg: Vec<f64> = vec![0., 1., 2., 0., -DATANUM as f64, REPEAT as f64];
    let mut interpret_obj = interpreter_calltable_obj3::InterpretCalltable::new();
    let start_time = Instant::now();
    let res = interpret_obj.interpret_calltable(&vliw_struct, reg);
    let elapsed_time = start_time.elapsed();
    let milliseconds = (elapsed_time.as_secs() as f64 * 1000.0) + (elapsed_time.subsec_nanos() as f64 / 1_000_000.0);
    println!("Calltable_obj3 interpreter: {} ms (out: {})", milliseconds, res[0]);
}
