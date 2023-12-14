use std::env;

pub fn get_env_var(key: &str) -> String {
    match env::var(key) {
        Ok(val) => val,
        Err(e) => panic!("couldn't interpret {}: {}", key, e),
    }
}

pub fn get_cli_arg() -> Vec<String> {
    let args: Vec<String> = env::args().collect();
    args
}
