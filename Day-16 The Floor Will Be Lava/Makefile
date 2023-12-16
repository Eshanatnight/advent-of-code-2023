default: one.input two.input

one.input: target/debug/one
	cargo run --bin=the_floor_will_be_lava_part_one

target/debug/one: Cargo.toml src/bin/the_floor_will_be_lava_part_one.rs
	cargo build

two.input: target/debug/two
	cargo run --bin=the_floor_will_be_lava_part_two

target/debug/two: Cargo.toml src/bin/the_floor_will_be_lava_part_two.rs
	cargo build
