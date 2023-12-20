use std::collections::{HashMap, VecDeque};

/*
    Yanked from aochelpers
*/
use num::Integer;

pub fn gcd<T: Integer + Copy >(first: T, second: T) -> T {
    let mut max = first;
    let mut min = second;
    if min > max {
        (min, max) = (max,min)
    }

    loop {
        let res = max % min;
        if res == num::zero() {
            return min;
        }
        max = min;
        min = res;
    }
}

fn lcm<T: Integer + Copy>(first: T , second: T) -> T {
    first * second / gcd(first, second)
}


#[derive(Debug,Clone, Copy,PartialEq)]
enum ModuleType {
    FlipFlop,
    Conjunction,
    Broadcast
}

#[derive(Debug,Clone,Copy, PartialEq)]
enum ModuleState {
    On,
    Off,
}

#[derive(Debug,Clone, Copy,PartialEq, Eq)]
enum Pulse {
    Low, 
    High, 
    None
}

#[derive(Debug,Clone,PartialEq)]
struct CommunicationModule {
    kind: ModuleType,
    state: ModuleState,
    next_modules: Vec<String>,
    memory: HashMap<String,Pulse>

}

#[derive(Debug,Clone,PartialEq, Eq)]
struct PulsePacket{
    sender: String,
    receiver: String,
    pulse: Pulse
}

#[derive(Debug,Clone,PartialEq, Eq)]
struct TerminationCondition{
    sender: String,
    pulse: Pulse
}

impl CommunicationModule {
    fn receive(&mut self, pulse: &Pulse, sender: &str) -> Pulse{
        match self.kind {
            ModuleType::FlipFlop => {
                if *pulse == Pulse::Low  && self.state == ModuleState::Off {
                    self.state = ModuleState::On;
                    Pulse::High
                } else if *pulse == Pulse::Low && self.state == ModuleState::On {
                    self.state = ModuleState::Off;
                    Pulse::Low
                } else {
                    Pulse::None
                }
            },
            ModuleType::Conjunction => {
                self.memory.insert(sender.to_string(), *pulse);
                if self.memory.values().all(|x| *x == Pulse::High) {
                    Pulse::Low
                } else {
                    Pulse::High
                }
            }
            ModuleType::Broadcast => *pulse,
        }
    }
}


fn parse(data:&str) -> HashMap<String, CommunicationModule> {
    let mut modules = HashMap::new();
    for line in data.lines() {
        let mut sections = line.split(" -> ");
        let name = sections.next().unwrap();
        if let Some(label) = name.strip_prefix('%') {
            modules.insert(label.to_string(), CommunicationModule {
                kind: ModuleType::FlipFlop,
                state: ModuleState::Off,
                next_modules: sections.next().unwrap().split(", ").map(|x| x.to_string()).collect::<Vec<_>>(),
                memory: HashMap::new()
            });
        } else if let Some(label) = name.strip_prefix('&') {
            modules.insert(label.to_string(), CommunicationModule {
                kind: ModuleType::Conjunction,
                state: ModuleState::Off,
                next_modules: sections.next().unwrap().split(", ").map(|x| x.to_string()).collect::<Vec<_>>(),
                memory: HashMap::new()
            });
        } else {
            modules.insert(name.to_string(), CommunicationModule {
                kind: ModuleType::Broadcast,
                state: ModuleState::Off,
                next_modules: sections.next().unwrap().split(", ").map(|x| x.to_string()).collect::<Vec<_>>(),
                memory: HashMap::new()
            });
        }
    }
    for (module, details) in modules.clone().iter() {
        for next_module_name in details.next_modules.iter(){
            if !modules.contains_key(next_module_name) {
                continue;
            }
            let next_module  = modules.get_mut(next_module_name).unwrap();
            if next_module.kind == ModuleType::Conjunction {
                next_module.memory.insert(module.clone(), Pulse::Low);
            }
        }
    }
    modules
}

fn press_button(network: &mut HashMap<String,CommunicationModule>, termination_condition: Option<&TerminationCondition>) -> (usize, usize, bool){
    let mut high_pulses = 0;
    let mut low_pulses = 0;
    let mut unprocessed = VecDeque::new();
    unprocessed.push_back(PulsePacket{sender: "button". to_string(), receiver: "broadcaster".to_string(), pulse: Pulse::Low});
    while let Some(message) = unprocessed.pop_front() {
        match message.pulse {
            Pulse::Low => {low_pulses += 1},
            Pulse::High => {high_pulses +=1},
            Pulse::None => {continue;}
        };
        if let Some(tc) = termination_condition{
            if message.sender == tc.sender && message.pulse == tc.pulse {
                return (0,0,true)
            }
        }
        if !network.contains_key(&message.receiver) { // Messages going to "rx" or "output"
            continue;
        }
        let receiver = network.get_mut(&message.receiver).unwrap();
        let result = receiver.receive(&message.pulse, &message.sender);

        for module in receiver.next_modules.iter() {
            let packet = PulsePacket{
                sender: message.receiver.clone(),
                receiver: module.to_owned(),
                pulse: result
            };
            unprocessed.push_back(packet);
        }
    }
    (low_pulses, high_pulses, false)
}

fn subcondition(network: &mut HashMap<String,CommunicationModule>, termination_condition: &TerminationCondition) -> usize {
    let mut counter = 0;
    loop {
        let (_,_, satisfied ) = press_button(network, Some(termination_condition));
        counter+=1;
        if satisfied {
            return counter;
        }
    }
}

fn solve(input: &str) -> usize {
    let network = parse(input);

    // Assumption: Only one module can feed to rx, and it is a Conjunction Module
    for (module, details) in network.iter() {
        for next_module_name in details.next_modules.iter(){
            if next_module_name == "rx" {
                let mut cycles = Vec::new();
                let previous_stage: &CommunicationModule = network.get(module).unwrap();
                for feeder_stage in previous_stage.memory.keys() {
                    // If I really cared about performance I'd reset an existing instance of the network
                    // instead of parsing the input again. I don't care about performance.
                    let mut network: HashMap<String, CommunicationModule> = parse(input); 
                    let cycle_time = subcondition(&mut network,
                         &TerminationCondition{pulse: Pulse::High, sender: feeder_stage.to_string()});
                    cycles.push(cycle_time);
                }
                // Time for rx to be triggered is LCM of each of the cycles for rx's input's inputs 
                return cycles[1..].iter().fold(cycles[0], |c: usize,v| lcm(c, *v));
            }
        }
    }

    panic!("This should never happen");
}

fn main() {
    let input = include_str!("../../input.txt");

    let result = solve(input);
    println!("Solution: {:?}", result);
}
