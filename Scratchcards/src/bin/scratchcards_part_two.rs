fn process(input: &str) -> i32 {
    let lines: Vec<&str> = input.split('\n').into_iter().collect();

    let mut card_count: Vec<i32> = vec![1; lines.len()];

    for (num, line) in lines.into_iter().enumerate() {

        let mut str_itr = line.split(":").last().unwrap().split("|").into_iter();
        let key_str = str_itr.next().unwrap();
        let val_str = str_itr.next().unwrap();

        let mut keys = Vec::new();
        let mut vals = Vec::new();

        for k in key_str.split_whitespace() {
            keys.push(k.parse::<i32>().unwrap());
        }

        for v in val_str.split_whitespace() {
            vals.push(v.parse::<i32>().unwrap());
        }

        let count = vals.into_iter().filter(|x| keys.contains(x)).count();

        if count != 0 {
            for i in num + 1.. num + count + 1 {
                card_count[i] += card_count[num];
            }
        }
    }

    return card_count.into_iter().fold(0, |acc, x| acc + x);
}

fn main() {
    let input = include_str!("../../part_two.sample.txt");
    println!("Sample: {}", process(input));

    let input = include_str!("../../part_two.input.txt");
    println!("Input: {}", process(input));
}
