default: one two

one: target/debug/one
	cargo run --bin=hot_springs_part_one

target/debug/one: Cargo.toml src/bin/hot_springs_part_one.rs
	cargo build

two: target/debug/two
	cargo run --bin=hot_springs_part_two

target/debug/two: Cargo.toml src/bin/hot_springs_part_two.rs
	cargo build
