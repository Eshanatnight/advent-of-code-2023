default: one.input two.input

one.sample: target/debug/one
	cargo run --bin=part_one < one.sample.txt

one.input: target/debug/one
	cargo run --bin=part_one < one.input.txt

target/debug/one: Cargo.toml src/bin/part_one.rs
	cargo build

two.sample: target/debug/two
	cargo run --bin=part_two < two.sample.txt

two.input: target/debug/two
	cargo run --bin=part_two < two.input.txt

target/debug/two: Cargo.toml src/bin/part_two.rs
	cargo build
