use std::{collections::HashMap, str::Lines};

fn solve(input:  &mut Lines<'_>) -> usize {
    // instructions cyclic iterator
    let mut instructions = input
        .next()
        .unwrap()
        .chars()
        .collect::<Vec<char>>()
        .into_iter()
        .cycle()
        .enumerate();

    // throw away the empty line
    _ = input.next();

    // design the network
    let network = input.fold(
        HashMap::<String, (String, String)>::new(),
        |mut acc, line| {
            // key -> "AAA"  || value -> "(BBB, "CCC")"
            let (key, value) = line.split_once(" = ").unwrap();

            // get the nodes from value
            let (left, right) = value.split_once(",").unwrap();

            acc.insert(
                key.trim().to_string(),
                (
                    left.trim().replace("(", "").to_string(),
                    right.trim().replace(")", "").to_string(),
                ),
            );

            acc
        },
    );

    let mut current = "AAA";

    let count = {
        while current != "ZZZ" {
            current = match instructions.next().unwrap().1 {
                'L' => &network.get(current).unwrap().0,
                _ => &network.get(current).unwrap().1,
            };
        }

        instructions.next().unwrap().0
    };

    count
}

fn main() {
    let input = include_str!("../../part_one.sample.txt").to_string();
    let input = include_str!("../../part_one.input.txt").to_string();
    let mut input = input.lines();

    println!("Solution: {:?}", solve(&mut input));
}
