struct Grid {
    data: Box<[u8]>,
    offset: usize,
}

impl Grid {
    fn from_str(input: &str) -> Self {
        let mut lines = input.lines().peekable();
        let line_len = lines.peek().map_or(0, |line| line.len());

        Self {
            data: lines.flat_map(str::as_bytes).copied().collect::<Box<_>>(),
            offset: line_len,
        }
    }

    fn next_pos(&self, p: usize, dir: u8) -> Option<usize> {
        // 0 = up, 1 = right, 2 = down, 3 = left
        match dir {
            0 => p.checked_sub(self.offset),
            1 => ((p + 1) % self.offset != 0).then_some(p + 1),
            2 => (p < self.data.len() - self.offset).then_some(p + self.offset),
            3 => (p % self.offset != 0).then_some(p - 1),
            _ => None,
        }
    }

    #[inline]
    fn step(&self, q: &mut Vec<(usize, u8)>, p: usize, dir: u8) {
        if let Some(np) = self.next_pos(p, dir) {
            q.push((np, dir));
        }
    }

    fn energize(&self, q: &mut Vec<(usize, u8)>) -> usize {
        let mut visit = vec![0u8; self.data.len()];
        while let Some((p, dir)) = q.pop() {
            if visit[p] & (1 << dir) != 0 {
                continue;
            }
            visit[p] |= 1 << dir;
            match self.data[p] {
                b'.' => self.step(q, p, dir),
                b'/' => self.step(q, p, dir ^ 1),
                b'\\' => self.step(q, p, dir ^ 3),
                b'|' => {
                    if dir & 1 == 0 {
                        self.step(q, p, dir);
                    } else {
                        self.step(q, p, 0);
                        self.step(q, p, 2);
                    }
                },
                b'-' => {
                    if dir & 1 == 1 {
                        self.step(q, p, dir);
                    } else {
                        self.step(q, p, 1);
                        self.step(q, p, 3);
                    }
                },
                _ => unreachable!(),
            }
        }
        visit.into_iter().filter(|&x| x != 0).count()
    }
}

fn solve(input: &str) -> usize {
    let grid = Grid::from_str(input);
    grid.energize(&mut vec![(0, 1)])
}

fn main() {
    let _input = include_str!("../../sample.txt");
    let input = include_str!("../../input.txt");
    println!("Solution: {}", solve(input));
}
