default: one.input two.input

one.input: target/debug/one
	cargo run --bin=pulse_propagation_part_one

target/debug/one: Cargo.toml src/bin/pulse_propagation_part_one.rs
	cargo build

two.input: target/debug/two
	cargo run --bin=pulse_propagation_part_two

target/debug/two: Cargo.toml src/bin/pulse_propagation_part_two.rs
	cargo build --release
