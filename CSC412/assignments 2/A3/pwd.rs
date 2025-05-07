use std::env;
use std::process;

fn main () {
    match env::current_dir() {
        Ok(path) => println!("{}", path.display()),
        Err(_) => {
            eprintln!("pwd: error: No such file or directory");
            process::exit(1);
        }
    }
}