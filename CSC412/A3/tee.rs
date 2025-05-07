
use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufWriter, Write};

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() > 2 {
        eprintln!("Usage: {} <file1> <file2> ...", args[0]);
        std::process::exit(1);
    }

    let mut files = Vec::new();

    // Open the file if provided
    for file_name in &args[1..] {
        match File::create(file_name) {
            Ok(file) => files.push(BufWriter::new(file)),
            Err(_) => {
                eprintln!("Error: could not open file {}", file_name);
                std::process::exit(1);
            }
        }
    }
    let stdin = io::stdin();
    let handle = stdin.lock();

    for line in handle.lines() {
        match line {
            Ok(line) => {
                println!("{}", line);
                for file in &mut files {
                    writeln!(file, "{}", line).unwrap_or_else(|_| {
                        eprintln!("Error: could not write to file");
                        std::process::exit(1);
                    });
                }
            }
            Err(_) => {
                eprintln!("Error: could not read from stdin");
                std::process::exit(1);
            }
        }
    }
}