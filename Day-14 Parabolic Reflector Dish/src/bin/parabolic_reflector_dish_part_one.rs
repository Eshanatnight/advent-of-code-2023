struct Grid {
    grid: Vec<Vec<Option<bool>>>,
}

fn tilt_north(grid: &[Vec<Option<bool>>]) -> Vec<Vec<Option<bool>>> {
    let height = grid.len();
    let width = grid[0].len();
    let mut new_grid = vec![vec![None; width]; height];

    for x in 0..width {
        let mut previous_rock = 0;

        for y in 0..height {
            match grid[y][x] {
                Some(true) => {
                    new_grid[previous_rock][x] = Some(true);
                    previous_rock += 1;
                },
                Some(false) => {
                    new_grid[y][x] = Some(false);
                    previous_rock = y + 1;
                },
                None => {},
            }
        }
    }

    new_grid
}

fn get_load(grid: &[Vec<Option<bool>>]) -> usize {
    let height = grid.len();
    grid.iter()
        .enumerate()
        .map(|(y, row)| {
            row.iter()
                .filter_map(|r| r.filter(|&rolls| rolls).and(Some(height - y)))
                .sum::<usize>()
        })
        .sum()
}

impl Grid {
    fn from_str(input: &str) -> Self {
        Self {
            grid: input
                .lines()
                .map(|l| {
                    l.bytes()
                        .map(|b| match b {
                            b'.' => None,
                            _ => Some(b == b'O'),
                        })
                        .collect()
                })
                .collect(),
        }
    }
}

fn solve(input: &str) -> usize {
    let grid = Grid::from_str(input);
    get_load(&tilt_north(&grid.grid))
}

fn main() {
    // let input = include_str!("../../sample.txt");
    let input = include_str!("../../input.txt");
    let result = solve(input);

    println!("Result: {:?}", result);
}
