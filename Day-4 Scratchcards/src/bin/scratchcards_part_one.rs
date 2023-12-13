fn process(input: &str) -> i32 {
    let mut out = 0;
    let lines = input.split("\n").into_iter().collect::<Vec<&str>>();

    for line in lines {
        let mut score: i32 = 0;
        let mut str_itr = line.split(":").last().unwrap().split("|").into_iter();
        let key_str = str_itr.next().unwrap();
        let val_str = str_itr.next().unwrap();

        let mut keys = Vec::new();
        let mut vals = Vec::new();

        // this can be done with the iterators directly,
        for k in key_str.split_whitespace() {
            keys.push(k.parse::<i32>().unwrap());
        }

        for v in val_str.split_whitespace() {
            vals.push(v.parse::<i32>().unwrap());
        }

        let mut count = vals.into_iter().filter(|x| keys.contains(x)).count();

        if count > 0 {
            score = 1;
            count -= 1;
        }
        if count > 0 {
            score = 2;
            count -= 1;
        }
        if count > 0 {
            score *= score.pow(count as u32);
        }
        out += score;
    }

    return out;
}

fn main() {
    let input = include_str!("../../part_one.sample.txt");
    println!("Sample: {}", process(input));

    let input = include_str!("../../part_one.input.txt");
    println!("Input: {}", process(input));
}
