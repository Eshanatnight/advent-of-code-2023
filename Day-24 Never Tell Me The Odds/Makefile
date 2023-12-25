default: one two

one: target/debug/one
	cargo run --bin=never_tell_me_the_odds_part_one

target/debug/one: Cargo.toml src/bin/never_tell_me_the_odds_part_one.rs
	cargo build

two: target/debug/two
	cargo run --bin=never_tell_me_the_odds_part_two

target/debug/two: Cargo.toml src/bin/never_tell_me_the_odds_part_two.rs
	cargo build
