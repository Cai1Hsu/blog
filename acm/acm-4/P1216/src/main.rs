use std::io::stdin;

fn read_vector() -> Vec<i32> {
    let mut str: String = String::new();
    stdin().read_line(&mut str).unwrap();

    str.split(' ')
        .into_iter()
        .map(|s| s.trim().parse::<i32>().unwrap())
        .collect()
}

fn main() {
    let n = read_vector()[0];

    let mut triangle: Vec<Vec<i32>> = Vec<Vec<i32>>::new();

    for _ in 0..n {
        triangle.push(read_vector());
    }


}
