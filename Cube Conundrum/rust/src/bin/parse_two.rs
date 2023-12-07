

use std::{collections::BTreeMap, fmt::Result};

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
    #[allow(dead_code)]
    id: &'a str,
    rounds: Vec<Vec<Cube<'a>>>,
}

impl<'a> Game<'a> {
    #[allow(dead_code)]
    fn valid_for_cube_set(
        &self,
        map: &BTreeMap<&str, u32>,
    ) -> Option<u32> {
        self.rounds
            .iter()
            .all(|round| {
                round.iter().all(|shown_cube| {
                    shown_cube.amount
                        <= *map
                            .get(shown_cube.color)
                            .expect("a valid cube")
                })
            })
            .then_some(
                self.id.parse::<u32>().expect(
                    "game id should a parsable u32",
                ),
            )
    }
    fn minimum_cube_set(&self) -> u32 {
        let map: BTreeMap<&str, u32> = BTreeMap::new();
        self.rounds
            .iter()
            .fold(map, |mut acc, round| {
                for cube in round.iter() {
                    acc.entry(cube.color)
                        .and_modify(|v| {
                            *v = (*v).max(cube.amount);
                        })
                        .or_insert(cube.amount);
                }
                acc
            })
            .values()
            .product()
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
) ->  std::result::Result<String, Box<dyn std::error::Error>>
{
    let games = parse_games(input).expect("should parse");

    Ok(games
        .1
        .iter()
        .map(|game| game.minimum_cube_set())
        .sum::<u32>()
        .to_string())
}

fn main() {
    let input = std::fs::read_to_string("input.txt").expect("can't read file");
    println!("{}", process(&input).expect("should work"));
}