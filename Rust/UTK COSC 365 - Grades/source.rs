use std::io;

fn main() {
    let mut input = String::new();
    
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");
        
    let num_of_grades: i32 = input.trim().parse().expect("Not a valid number");
    
    let mut grades: Vec<f64> = Vec::new();
    let mut sum: f64 = 0.0;

    for i in 1..=num_of_grades {
        
        input.clear();
        
        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");
            
        let grade: f64 = input.trim().parse().expect("Not a valid grade");
        grades.push(grade);
    }
    
    for i in grades {
        sum += i;
    }
    
    let avg = sum / (num_of_grades as f64);
    
   if avg >= 92.0 {
        println!("{:.2} A", avg);
    } else if avg >= 88.0 {
        println!("{:.2} A-", avg);
    } else if avg >= 85.0 {
        println!("{:.2} B+", avg);
    } else if avg >= 82.0 {
        println!("{:.2} B", avg);
    } else if avg >= 78.0 {
        println!("{:.2} B-", avg);
    } else if avg >= 75.0 {
        println!("{:.2} C+", avg);
    } else if avg >= 72.0 {
        println!("{:.2} C", avg);
    } else if avg >= 68.0 {
        println!("{:.2} C-", avg);
    } else if avg >= 65.0 {
        println!("{:.2} D+", avg);
    } else if avg >= 62.0 {
        println!("{:.2} D", avg);
    } else if avg >= 58.0 {
        println!("{:.2} D-", avg);
    } else {
        println!("{:.2} F", avg);
    }
}
