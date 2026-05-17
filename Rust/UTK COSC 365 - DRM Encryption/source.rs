use std::io;

fn divide(s: &str) -> (String, String) {
    let mid = s.len() / 2;

    let left = s[..mid].to_string();
    let right = s[mid..].to_string();

    (left, right)
}

fn rotate(s: &str) -> String {
    // Step 1: compute shift
    let shift: u8 = s.chars()
        .map(|c| c as u8 - b'A')
        .sum();

    // Step 2: rotate each char
    s.chars()
        .map(|c| {
            let val = c as u8 - b'A';
            let new_val = (val + shift) % 26;
            (new_val + b'A') as char
        })
        .collect()
}

fn merge(left: &str, right: &str) -> String {
    left.chars()
        .zip(right.chars())
        .map(|(l, r)| {
            let l_val = l as u8 - b'A';
            let r_val = r as u8 - b'A';

            let new_val = (l_val + r_val) % 26;
            (new_val + b'A') as char
        })
        .collect()
}

fn main() {
    
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();

    let input = input.trim().to_uppercase();
    if input.len() % 2 != 0{
        println!("Error: The number of characters is not even!");
        return;
    }

    let (first_half, second_half) = divide(&input);
    let first_rot = rotate(&first_half);
    let second_rot = rotate(&second_half);
    let result = merge(&first_rot, &second_rot);

    println!("{}", result);
}