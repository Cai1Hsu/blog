use std::io::stdin;
use std::cmp::max;

fn read_vector() -> Vec<i32> {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();

    str.split_whitespace()
        .map(|s| s.trim().parse::<i32>().unwrap_or(0))
        .collect()
}

fn main() {
    let n = read_vector()[0] as usize;

    let mut triangle: Vec<Vec<i32>> = Vec::new();

    for _ in 0..n {
        triangle.push(read_vector());
    }

    for i in (0..n-1).rev() {
        for j in 0..=i {
            triangle[i][j] = max(
                *triangle.get(i + 1).and_then(|row| row.get(j)).unwrap_or(&0),
                *triangle.get(i + 1).and_then(|row| row.get(j + 1)).unwrap_or(&0),
            );
        }
    }

    println!("{}", triangle[0][0]);
}
