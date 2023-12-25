use itertools::Itertools;

#[derive(Debug, Clone, Copy, PartialEq)]
struct Coord {
    x: isize,
    y: isize,
    z: isize,
}

impl Coord {
    fn new(x: isize, y: isize, z: isize) -> Self {
        Coord { x, y, z }
    }
    fn cross(&self, other: &Coord) -> Coord {
        Coord {
            x: self.y * other.z - self.z * other.y,
            y: self.z * other.x - self.x * other.z,
            z: self.x * other.y - self.y * other.x,
        }
    }
    fn abs(&self) -> f64 {
        let x = self.x as f64;
        let y = self.y as f64;
        let z = self.z as f64;
        (x * x + y * y + x * x).sqrt()
    }
}

#[derive(Debug, Clone)]
struct Hailstone {
    pos: Coord,
    vel: Coord,
}
impl Hailstone {
    fn new(pos: Vec<isize>, vel: Vec<isize>) -> Self {
        Hailstone {
            pos: Coord {
                x: pos[0],
                y: pos[1],
                z: pos[2],
            },
            vel: Coord {
                x: vel[0],
                y: vel[1],
                z: vel[2],
            },
        }
    }
}

fn get_intersection_2d(h1: &Hailstone, h2: &Hailstone) -> Option<(f64, f64)> {
    let determinant = (h1.vel.x * h2.vel.y - h1.vel.y * h2.vel.x) as f64;

    if determinant == 0.0 {
        return None;
    }
    let time: f64 =
        ((h2.pos.x - h1.pos.x) * h2.vel.y - (h2.pos.y - h1.pos.y) * h2.vel.x) as f64 / determinant;
    let time2: f64 =
        ((h1.pos.x - h2.pos.x) * h1.vel.y - (h1.pos.y - h2.pos.y) * h1.vel.x) as f64 / determinant;

    if time < 0.0 {
        return None;
    }
    if time * time2 > 0.0 {
        return None;
    }
    let x = h1.pos.x as f64 + h1.vel.x as f64 * time;
    let y = h1.pos.y as f64 + h1.vel.y as f64 * time;

    Some((x, y))
}

fn solve(input: &str, area: (f64, f64)) -> usize {
    let mut hailstones = Vec::new();
    for line in input.lines() {
        let (pos, vel) = line.split_once(" @ ").unwrap();
        let pos: Vec<isize> = pos.split(',').map(|p| p.trim().parse().unwrap()).collect();
        let vel: Vec<isize> = vel.split(',').map(|p| p.trim().parse().unwrap()).collect();
        hailstones.push(Hailstone::new(pos, vel))
    }
    let (lower, upper) = area;
    let mut collisions = 0;
    for combinations in hailstones.iter().combinations(2) {
        if let Some(intersection) = get_intersection_2d(combinations[0], combinations[1]) {
            let (x, y) = intersection;

            if (x > lower && x < upper) && (y > lower && y < upper) {
                collisions += 1;
            }
        }
    }
    return collisions;
}

fn main() {
    let input = include_str!("../../sample.txt");
    let sample_area = (7.0, 27.0);

    assert_eq!(solve(input, sample_area), 2);

    let input = include_str!("../../input.txt");
    let input_area = (200000000000000.0, 400000000000000.0);

    let result = solve(input, input_area);

    println!("Solution: {}", result);
}
