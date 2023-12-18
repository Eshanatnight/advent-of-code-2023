default: one.input two.input

one.input: target/debug/one
	cargo run --bin=lavaduct_lagoon_part_one

target/debug/one: Cargo.toml src/bin/lavaduct_lagoon_part_one.rs
	cargo build

two.input: target/debug/two
	cargo run --bin=lavaduct_lagoon_part_two

target/debug/two: Cargo.toml src/bin/lavaduct_lagoon_part_two.rs
	cargo build
