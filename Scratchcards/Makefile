default: one two

one: target/debug/one
	cargo run --bin=scratchcards_part_one

target/debug/one: Cargo.toml src/bin/part_one.rs
	cargo build

two: target/debug/two
	cargo run --bin=scratchcards_part_two

target/debug/two: Cargo.toml src/bin/part_two.rs
	cargo build
