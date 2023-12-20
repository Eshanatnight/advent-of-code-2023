use std::num::NonZeroU64;

fn mapper(l: &str) -> (u8, u64) {
    let (_, b) = l.split_once('#').unwrap();
    let hex = u32::from_str_radix(&b[..6], 16).unwrap();
    ((hex & 3) as u8, (hex >> 4) as u64)
}

const fn pt_move(pt: (i64, i64), dir: u8, dist: u64) -> (i64, i64) {
    match dir {
        0 => (pt.0.wrapping_add_unsigned(dist), pt.1),
        1 => (pt.0, pt.1.wrapping_add_unsigned(dist)),
        2 => (pt.0.wrapping_sub_unsigned(dist), pt.1),
        3 => (pt.0, pt.1.wrapping_sub_unsigned(dist)),
        _ => unreachable!(),
    }
}

fn solve(input: &str) -> Option<NonZeroU64> {
    let mut spos = (0, 0);
    let (mut perim, mut sum) = (0, 0);
    for (dir, num) in input.lines().map(mapper) {
        let npos = pt_move(spos, dir, num);
        sum += (spos.1 + npos.1) * (spos.0 - npos.0);
        perim += num;
        spos = npos;
    }
    NonZeroU64::new(perim.wrapping_add_signed(sum) / 2 + 1)
}

fn main() {
    // let input = include_str!("../../sample.txt");
    let input = include_str!("../../input.txt");

    println!("Solution: {:?}", solve(input).unwrap());
}
