use std::io::{self};
use std::fs;

fn b64_value(c: char) -> Option<u8> {
    match c {
        'A'..='Z' => Some(c as u8 - b'A'),
        'a'..='z' => Some(c as u8 - b'a' + 26),
        '0'..='9' => Some(c as u8 - b'0' + 52),
        '+' => Some(62),
        '/' => Some(63),
        '=' => None,
        _ => None,
    }
}

fn main() {
    // Step 1: read filename from stdin
    let mut filename = String::new();
    io::stdin().read_line(&mut filename).unwrap();
    let filename = filename.trim();

    // Step 2: read file contents
    let contents = fs::read_to_string(filename).expect("Failed to read file");

    let chars: Vec<char> = contents.trim().chars().collect();

    // Step 3: decode
    for chunk in chars.chunks(4) {
        let mut buffer: u32 = 0;
        let mut padding = 0;

        for &c in chunk {
            buffer <<= 6;

            if let Some(val) = b64_value(c) {
                buffer |= val as u32;
            } else {
                padding += 1;
            }
        }

        let b1 = ((buffer >> 16) & 0xFF) as u8;
        let b2 = ((buffer >> 8) & 0xFF) as u8;
        let b3 = (buffer & 0xFF) as u8;

        print!("{}", b1 as char);

        if padding < 2 {
            print!("{}", b2 as char);
        }
        if padding < 1 {
            print!("{}", b3 as char);
        }
    }
}