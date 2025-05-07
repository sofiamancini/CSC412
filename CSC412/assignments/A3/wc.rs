use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};

fn process_in<R: BufRead>(reader: R, filename: Option<&str>, 
    total_line_count: &mut usize, total_word_count: &mut usize, total_byte_count: &mut usize) {

    let mut line_count = 0;
    let mut word_count = 0;
    let mut byte_count = 0;

    for line in reader.lines() {
        let line = line.unwrap_or_else(|_| String::new());
        line_count += 1;
        *total_line_count += 1;

        byte_count += line.len() + 1; // +1 for newline character
        *total_byte_count += line.len() + 1;

        let words = line.split_whitespace().count();
        word_count += words;
        *total_word_count += words;
    }

    if let Some(name) = filename {
        println!("{} {} {} {}", line_count, word_count, byte_count, name);
    } else {
        println!("{} {} {} (stdin)", line_count, word_count, byte_count);
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut total_line_count = 0;
    let mut total_word_count = 0;
    let mut total_byte_count = 0;

    if args.len() == 1 {
        let stdin = io::stdin();
        let reader = stdin.lock();
        process_in(reader, None, &mut total_line_count, &mut total_word_count, &mut total_byte_count);
    } else {
        let mut file_count = 0;
        for filename in &args[1..] {
            match File::open(filename) {
                Ok(file) => {
                    let reader = BufReader::new(file);
                    process_in(reader, Some(filename), &mut total_line_count, &mut total_word_count, &mut total_byte_count);
                    file_count += 1;
                }
                Err(_) => {
                    eprintln!("{}: {}: No such file or directory", args[0], filename);
                }
            }
        }
        if file_count > 1 {
            println!("{} {} {} total", total_line_count, total_word_count, total_byte_count);
        }
    }
}