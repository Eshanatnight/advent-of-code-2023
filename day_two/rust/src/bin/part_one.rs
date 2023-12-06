use std::{collections::BTreeMap, ops::Not, error::Error};

use nom::{
    bytes::complete::tag,
    character::complete::{
        self, alpha1, digit1, line_ending,
    },
    multi::separated_list1,
    sequence::{preceded, separated_pair},
    IResult,
};


#[derive(Debug)]
struct Cube<'a> {
    color: &'a str,
    amount: u32,
}

#[derive(Debug)]
struct Game<'a> {
    id: &'a str,
    rounds: Vec<Vec<Cube<'a>>>,
}

impl<'a> Game<'a> {
    fn valid_for_cube_set(
        &self,
        map: &BTreeMap<&str, u32>,
    ) -> Option<u32> {
        self.rounds
            .iter()
            .any(|round| {
                round.iter().any(|shown_cube| {
                    shown_cube.amount
                        > *map
                            .get(shown_cube.color)
                            .expect("a valid cube")
                })
            })
            .not()
            .then_some(
                self.id.parse::<u32>().expect(
                    "game id should a parsable u32",
                ),
            )
    }
}

// 4 red
fn cube(input: &str) -> IResult<&str, Cube> {
    let (input, (amount, color)) =
        separated_pair(complete::u32, tag(" "), alpha1)(
            input,
        )?;
    Ok((input, Cube { color, amount }))
}
// 3 blue, 4 red
fn round(input: &str) -> IResult<&str, Vec<Cube>> {
    let (input, cubes) =
        separated_list1(tag(", "), cube)(input)?;
    Ok((input, cubes))
}
// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
fn game(input: &str) -> IResult<&str, Game> {
    let (input, id) =
        preceded(tag("Game "), digit1)(input)?;
    let (input, rounds) = preceded(
        tag(": "),
        separated_list1(tag("; "), round),
    )(input)?;
    Ok((input, Game { rounds, id }))
}
fn parse_games(input: &str) -> IResult<&str, Vec<Game>> {
    let (input, games) =
        separated_list1(line_ending, game)(input)?;
    Ok((input, games))
}

pub fn process(
    input: &str,
) -> Result<String, Box<dyn Error>> {
    let map = BTreeMap::from([
        ("red", 12),
        ("green", 13),
        ("blue", 14),
    ]);
    let games = parse_games(input).expect("should parse");

    Ok(games
        .1
        .iter()
        .filter_map(|game| game.valid_for_cube_set(&map))
        .sum::<u32>()
        .to_string())
}


fn main() {
    let input = std::fs::read_to_string("day_two/input.txt").expect("should be able to read input file");
    let result = process(&input).expect("should work");
    println!("{}", result);
}