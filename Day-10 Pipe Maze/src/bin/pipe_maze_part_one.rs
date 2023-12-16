use itertools::Itertools;

#[derive(Debug, Clone, Copy)]
enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
}

fn main() -> std::io::Result<()> {
    let input = include_str!("../../input.txt");
    let maze: Vec<Vec<char>> = input.lines().map(|x| x.chars().collect()).collect();

    let start = get_start(&maze).unwrap();

    // Start has to be manually set!
    let mut curr = (start.0 - 1, start.1);
    let mut dir = Direction::SOUTH;

    let mut steps = 1;

    let mut pipe = Vec::new();
    pipe.push(start);
    let mut left = Vec::new();
    let mut right = Vec::new();
    while maze[curr.0][curr.1] != 'S' {
        pipe.push(curr);

        let uppsize = (curr.0 as i32, curr.1 as i32);
        match (dir, maze[curr.0][curr.1]) {
            (Direction::NORTH, '|') => {
                left.push((uppsize.0, uppsize.1 + 1));
                right.push((uppsize.0, uppsize.1 - 1));
            },
            (Direction::SOUTH, '|') => {
                right.push((uppsize.0, uppsize.1 + 1));
                left.push((uppsize.0, uppsize.1 - 1));
            },
            (Direction::EAST, '-') => {
                right.push((uppsize.0 - 1, uppsize.1));
                left.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::WEST, '-') => {
                left.push((uppsize.0 - 1, uppsize.1));
                right.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::NORTH, 'L') => {
                right.push((uppsize.0, uppsize.1 - 1));
                right.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::EAST, 'L') => {
                left.push((uppsize.0, uppsize.1 - 1));
                left.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::NORTH, 'J') => {
                left.push((uppsize.0, uppsize.1 + 1));
                left.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::WEST, 'J') => {
                right.push((uppsize.0, uppsize.1 + 1));
                right.push((uppsize.0 + 1, uppsize.1));
            },
            (Direction::SOUTH, '7') => {
                right.push((uppsize.0, uppsize.1 + 1));
                right.push((uppsize.0 - 1, uppsize.1));
            },
            (Direction::WEST, '7') => {
                left.push((uppsize.0, uppsize.1 + 1));
                left.push((uppsize.0 - 1, uppsize.1));
            },
            (Direction::SOUTH, 'F') => {
                left.push((uppsize.0, uppsize.1 - 1));
                left.push((uppsize.0 - 1, uppsize.1));
            },
            (Direction::EAST, 'F') => {
                right.push((uppsize.0, uppsize.1 - 1));
                right.push((uppsize.0 - 1, uppsize.1));
            },
            _ => panic!("{:?}, {:?}, {:?}", curr, maze[curr.0][curr.1], dir),
        };

        dir = match (dir, maze[curr.0][curr.1]) {
            (Direction::NORTH, '|') => Direction::SOUTH,
            (Direction::SOUTH, '|') => Direction::NORTH,
            (Direction::EAST, '-') => Direction::WEST,
            (Direction::WEST, '-') => Direction::EAST,
            (Direction::NORTH, 'L') => Direction::EAST,
            (Direction::EAST, 'L') => Direction::NORTH,
            (Direction::NORTH, 'J') => Direction::WEST,
            (Direction::WEST, 'J') => Direction::NORTH,
            (Direction::SOUTH, '7') => Direction::WEST,
            (Direction::WEST, '7') => Direction::SOUTH,
            (Direction::SOUTH, 'F') => Direction::EAST,
            (Direction::EAST, 'F') => Direction::SOUTH,
            _ => panic!("{:?}, {:?}, {:?}", curr, maze[curr.0][curr.1], dir),
        };

        curr = match dir {
            Direction::NORTH => (curr.0 - 1, curr.1),
            Direction::SOUTH => (curr.0 + 1, curr.1),
            Direction::EAST => (curr.0, curr.1 + 1),
            Direction::WEST => (curr.0, curr.1 - 1),
        };

        dir = match dir {
            Direction::NORTH => Direction::SOUTH,
            Direction::SOUTH => Direction::NORTH,
            Direction::EAST => Direction::WEST,
            Direction::WEST => Direction::EAST,
        };
        steps += 1;
    }

    println!("Solution : {}", steps / 2);

    Ok(())
}

fn get_start(matrix: &Vec<Vec<char>>) -> Option<(usize, usize)> {
    let start = matrix
        .iter()
        .enumerate()
        .find_or_first(|x| x.1.contains(&'S'))
        .map(|x| {
            (
                x.0,
                x.1.iter()
                    .enumerate()
                    .find_or_first(|y| y.1 == &'S')
                    .unwrap()
                    .0,
            )
        });

    start
}
