use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::collections::VecDeque;

const MAX_LINES: usize = 10;

fn process_in_last10<R: BufRead>(reader: R, filename: Option<&str>, print_filename: bool) {
    let mut last_10_lines = VecDeque::new();
    for line in reader.lines() {
        match line {
            Ok(line) => {
                if last_10_lines.len() > MAX_LINES {
                    last_10_lines.pop_front();
                }
                last_10_lines.push_back(line);
            }
            Err(_) => {
                eprintln!("tail: error: could not read from file");
                return;
            }
        }
    }
    if print_filename {
        println!("==> {} <==", filename.unwrap());
    }  

    for line in last_10_lines {
        println!("{}", line);
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 1 {
        let stdin = io::stdin();
        let reader = stdin.lock();
        process_in_last10(reader, None, false);
    } else {
        let muli_files = args.len() > 2;
        let mut first = true;

        for filename in &args[1..] {
            match File::open(filename) {
                Ok(file) => {
                    let reader = BufReader::new(file);
                    if muli_files {
                        if !first {
                            println!();
                        }
                        first = false;
                    }
                    process_in_last10(reader, Some(filename), muli_files);
                }
                Err(_) => {
                    eprintln!("tail: error: cannot open '{}' for reading: No such file or directory", filename);
                }
            }
        }
    }
}