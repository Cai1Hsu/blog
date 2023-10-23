use std::io::stdin;

#[derive(Debug)]
struct Rectangle {
    first: Point,
    second: Point
}

#[derive(Debug)]
struct Point {
    x: i32,
    y: i32
}

fn read_i32() -> i32 {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();
    
    str.trim().parse::<i32>().unwrap()
}

fn read_point() -> Point {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();

    let p: Vec<i32> = str.split(' ')
        .into_iter()
        .map(|s| s.trim().parse::<i32>().unwrap())
        .collect();
        
    Point {
        x: p[0],
        y: p[1],
    }
}

fn read_rectangle() -> Rectangle {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();
    
    let vec4: Vec<i32> = str.split(' ')
        .into_iter()
        .map(|s| s.trim().parse::<i32>().unwrap())
        .collect();

    Rectangle { 
        first: Point {
            x: vec4[0], 
            y: vec4[1]

        },
        second: Point {
            x: vec4[0] + vec4[2],
            y: vec4[1] + vec4[3]
        }
    }
}

fn main() {
    let n: i32 = read_i32();

    let mut carpets: Vec<Rectangle> = Vec::new(); 
    for _ in 0..n {
        carpets.push(read_rectangle());
    }

    let target: Point = read_point();

    let mut ans: i32 = -1;

    for (i, rect) in carpets.iter().enumerate() {
        if rect.first.x <= target.x && 
            rect.first.y <= target.y && 
            rect.second.x >= target.x &&
            rect.second.y >= target.y
        {
            ans = i + 1; 
        }
    }

    println!("{}", ans);
}
