use std::thread::{self};

use primes::is_prime;

const GOAL: usize = 10000;
const THREADS: usize = 32;

fn main() {
    let mut results = [0; GOAL - 1];
    let mut handles = vec![];

    for t in 0..THREADS {
        let handle = thread::spawn(move || 
        
        {
            let mut results_t = [0; (GOAL - 1).div_ceil(THREADS)];

            let mut i = 0;
            for x in ((t as u64 + 2)=..(GOAL as u64)).step_by(THREADS) {
                results_t[i] = highest_cyclength(x);
                if x % 100 == 0 {
                    eprintln!("{x}");
                }
                i += 1; 
            }

            eprintln!("Worker {t} finished");

            results_t
        });

        handles.push(handle);
    }

    for (t, h) in handles.into_iter().enumerate() {
        let results_t = h.join().unwrap();
        for i in 0..((GOAL - 2 - t) / THREADS + 1) {
            results[(i * THREADS) + t] = results_t[i];
        }
    }

    print!("[");
    for i in 0..(GOAL - 1) {
        let m = results[i];
        if i == GOAL - 2 {
            println!("{m}]");
        } else {
            print!("{m}, ");
        }
    }
}

fn highest_cyclength(x: u64) -> u64 {
    if is_prime(x) {
        x - 1
    } else {
        let mut m = 1;
        for i in 2..x {
            let b = cyclength(i, x);
            if b > m {
                m = b;
            }
        }
        m
    }
}

fn qmod(n: u64, x: u64, b: u64) -> u64 {
    let mut r = 1;
    let mut base = n % b;
    let mut exp = x;
    while exp > 0 {
        if exp % 2 == 1 {
            r = (r * base) % b;
        }
        base = (base * base) % b;
        exp /= 2;
    }
    r
}

fn cyclength(n: u64, b: u64) -> u64 {
    let mut seen = [false; GOAL];
    let mut x = 1;
    loop {
        let qmod = qmod(n, x, b) as usize;
        if seen[qmod] {
            break;
        }
        seen[qmod] = true;
        x += 1;
    }
    x - 1
}