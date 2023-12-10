use std::{collections::HashMap, str::Lines};

use num::integer::lcm;

fn solve_for(node: &str, network: &HashMap<String, (String, String)>, instructions: &[char]) -> usize {
    let mut instruction_iter = instructions.iter().cycle().enumerate();

    let mut current = node;
    while current.chars().nth(2) != Some('Z') {
        current = match instruction_iter.next().unwrap().1 {
            'L' => &network.get(current).unwrap().0,
            _ => &network.get(current).unwrap().1,
        };
        
    }

    instruction_iter.next().unwrap().0
}

fn solve(input: &mut Lines<'_>) -> usize {
    // instructions cyclic iterator
    let instructions = input.next().unwrap().chars().collect::<Vec<char>>();

    // throw away the empty line
    _ = input.next();

    // design the network
    let network = input.fold(
        HashMap::<String, (String, String)>::new(),
        |mut acc, line| {
            // key -> "AAA"  || value -> "(BBB, "CCC")"
            let (key, value) = line.split_once("=").unwrap();

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

    let count = network
        .clone()
        .into_iter()
        .map(|v| v.0)
        .filter(|node| node.clone().split_off(2) == "A")
        .map(|node| solve_for(&node, &network, &instructions))
        .fold(1, lcm);
    
    count
}

fn main() {
    // let input = include_str!("../../part_two.sample.txt").to_string();
    let input = include_str!("../../part_two.input.txt").to_string();
    let mut input = input.lines();

    println!("Solution: {:?}", solve(&mut input));
}
