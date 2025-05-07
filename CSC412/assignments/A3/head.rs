
use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};

const MAX_LINES: usize = 10;

fn process_in<R: BufRead>(reader: R, filename: Option<&str>) {
    if let Some(name) = filename {
        println!("==> {} <==", name);
    }

    for (i, line) in reader.lines().enumerate() {
        if i >= MAX_LINES {
            break;
        }
        match line {
            Ok(line) => println!("{}", line),
            Err(_) => {
                eprintln!("Error: could not read from file");
                std::process::exit(1);
            }
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 1 {
        let stdin = io::stdin();
        let reader = stdin.lock();
        process_in(reader, None);
    } else {
        let mut first = true;
        for filename in &args[1..] {
            match File::open(filename) {
                Ok(file) => {
                    let reader = BufReader::new(file);
                    if args.len() > 2 {
                        if !first {
                            println!();
                        }
                        first = false;
                    }
                    process_in(reader, Some(filename));
                }
                Err(_) => {
                    eprintln!("head: cannot open '{}' for reading: No such file or directory", filename);
                }
            }
        }
    }
}