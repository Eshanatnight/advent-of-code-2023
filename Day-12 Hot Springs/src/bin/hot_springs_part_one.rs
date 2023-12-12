use std::borrow::Cow;

type Row<'a> = (Cow<'a, [u8]>, Box<[u8]>);

fn parse_groups(input: &str) -> impl Iterator<Item = Row<'_>> + '_ {
    input.lines().map(move |l| {
        let (s, n) = l.split_once(' ').unwrap();
        let n = n.split(',').map(|x| x.parse().unwrap()).collect::<Box<_>>();
        let sb = s.as_bytes();
        (sb.into(), n)
    })
}

fn possible(l: &[u8], n: &[u8]) -> usize {
    let l = if l.last() == Some(&b'.') {
        &l[..l.len() - 1]
    } else {
        l
    };
    let mut vl = Vec::with_capacity(l.len() + 1);
    vl.push(b'.');
    vl.extend_from_slice(l);
    let sz = vl.len() + 1;

    let (mut oldstate, mut newstate) = (vec![0; sz], vec![0; sz]);
    oldstate[0] = 1;

    for i in 1..vl.len() {
        if vl[i] != b'#' {
            oldstate[i] = 1;
        } else {
            break;
        }
    }

    for &cnt in n {
        let cnt = cnt as usize;
        let mut grp = 0;
        for (i, &c) in vl.iter().enumerate() {
            if c == b'.' {
                grp = 0;
            } else {
                grp += 1;
            }
            if c != b'#' {
                newstate[i + 1] += newstate[i];
            }
            if grp >= cnt && vl[i - cnt] != b'#' {
                newstate[i + 1] += oldstate[i - cnt];
            }
        }
        oldstate.iter_mut().for_each(|x| *x = 0);
        (oldstate, newstate) = (newstate, oldstate);
    }

    oldstate[sz - 1]
}

fn solve(input: &str) -> usize {
    parse_groups(input).map(|(l, n)| possible(&l, &n)).sum()
}

fn main() {
    // let mut input = include_str!("../../sample.txt");
    let input = include_str!("../../input.txt");
    println!("{:?}", solve(input));
}
