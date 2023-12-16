use std::collections::HashMap;

#[derive(Debug)]
struct Game {
    races: HashMap<usize, HashMap<usize, usize>>,
}

impl Game {
    fn parse(input: &str) -> Self {
        let mut game = Game {
            races: HashMap::new(),
        };

        let game_split = input.split("\n").collect::<Vec<_>>();

        let times = vec![game_split[0]
            .replace("Time: ", "")
            .replace(" ", "")
            .parse::<usize>()
            .unwrap()];

        let distances = vec![game_split[1]
            .replace("Distance: ", "")
            .replace(" ", "")
            .parse::<usize>()
            .unwrap()];

        for (index, race_length) in times.iter().enumerate() {
            let mut times: HashMap<usize, usize> = HashMap::new();
            let distance_record = distances[index];

            for hold_length in 1..race_length - 1 {
                let distance = (race_length - hold_length) * hold_length;

                if distance > distance_record {
                    times.insert(hold_length, distance);
                }
            }

            game.races.insert(*race_length, times);
        }
        game
    }
}

fn main() {
    // let input = include_str!("../../sample.txt");
    let input = include_str!("../../input.txt");

    let game = Game::parse(input);

    let res: usize = game.races.values().map(|v| v.len()).product();

    println!("Result: {}", res)
}
