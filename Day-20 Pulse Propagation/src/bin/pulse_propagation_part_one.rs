use std::collections::{HashMap, VecDeque};

#[derive(Debug, Clone, Copy, PartialEq)]
enum ModuleType {
    FlipFlop,
    Conjunction,
    Broadcast,
}

#[derive(Debug, Clone, Copy, PartialEq)]
enum ModuleState {
    On,
    Off,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Pulse {
    Low,
    High,
    None,
}

#[derive(Debug, Clone, PartialEq)]
struct CommunicationModule {
    kind: ModuleType,
    state: ModuleState,
    next_modules: Vec<String>,
    memory: HashMap<String, Pulse>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct PulsePacket {
    sender: String,
    receiver: String,
    pulse: Pulse,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct TerminationCondition {
    sender: String,
    pulse: Pulse,
}

impl CommunicationModule {
    fn receive(&mut self, pulse: &Pulse, sender: &str) -> Pulse {
        match self.kind {
            ModuleType::FlipFlop => {
                if *pulse == Pulse::Low && self.state == ModuleState::Off {
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
            },
            ModuleType::Broadcast => *pulse,
        }
    }
}

fn parse(data: &str) -> HashMap<String, CommunicationModule> {
    let mut modules = HashMap::new();
    for line in data.lines() {
        let mut sections = line.split(" -> ");
        let name = sections.next().unwrap();
        if let Some(label) = name.strip_prefix('%') {
            modules.insert(
                label.to_string(),
                CommunicationModule {
                    kind: ModuleType::FlipFlop,
                    state: ModuleState::Off,
                    next_modules: sections
                        .next()
                        .unwrap()
                        .split(", ")
                        .map(|x| x.to_string())
                        .collect::<Vec<_>>(),
                    memory: HashMap::new(),
                },
            );
        } else if let Some(label) = name.strip_prefix('&') {
            modules.insert(
                label.to_string(),
                CommunicationModule {
                    kind: ModuleType::Conjunction,
                    state: ModuleState::Off,
                    next_modules: sections
                        .next()
                        .unwrap()
                        .split(", ")
                        .map(|x| x.to_string())
                        .collect::<Vec<_>>(),
                    memory: HashMap::new(),
                },
            );
        } else {
            modules.insert(
                name.to_string(),
                CommunicationModule {
                    kind: ModuleType::Broadcast,
                    state: ModuleState::Off,
                    next_modules: sections
                        .next()
                        .unwrap()
                        .split(", ")
                        .map(|x| x.to_string())
                        .collect::<Vec<_>>(),
                    memory: HashMap::new(),
                },
            );
        }
    }
    for (module, details) in modules.clone().iter() {
        for next_module_name in details.next_modules.iter() {
            if !modules.contains_key(next_module_name) {
                continue;
            }
            let next_module = modules.get_mut(next_module_name).unwrap();
            if next_module.kind == ModuleType::Conjunction {
                next_module.memory.insert(module.clone(), Pulse::Low);
            }
        }
    }
    modules
}

fn press_button(
    network: &mut HashMap<String, CommunicationModule>,
    termination_condition: Option<&TerminationCondition>,
) -> (usize, usize, bool) {
    let mut high_pulses = 0;
    let mut low_pulses = 0;
    let mut unprocessed = VecDeque::new();
    unprocessed.push_back(PulsePacket {
        sender: "button".to_string(),
        receiver: "broadcaster".to_string(),
        pulse: Pulse::Low,
    });
    while let Some(message) = unprocessed.pop_front() {
        match message.pulse {
            Pulse::Low => low_pulses += 1,
            Pulse::High => high_pulses += 1,
            Pulse::None => {
                continue;
            },
        };
        if let Some(tc) = termination_condition {
            if message.sender == tc.sender && message.pulse == tc.pulse {
                return (0, 0, true);
            }
        }
        if !network.contains_key(&message.receiver) {
            // Messages going to "rx" or "output"
            continue;
        }
        let receiver = network.get_mut(&message.receiver).unwrap();
        let result = receiver.receive(&message.pulse, &message.sender);

        for module in receiver.next_modules.iter() {
            let packet = PulsePacket {
                sender: message.receiver.clone(),
                receiver: module.to_owned(),
                pulse: result,
            };
            unprocessed.push_back(packet);
        }
    }
    (low_pulses, high_pulses, false)
}

fn solve(input: &str) -> usize {
    let mut network: HashMap<String, CommunicationModule> = parse(input);
    let mut low: usize = 0;
    let mut high = 0;
    for _ in 0..1000 {
        let (l, h, _) = press_button(&mut network, None);
        low += l;
        high += h;
    }

    low * high
}

fn main() {
    let input = include_str!("../../sample.txt");
    let result = solve(input);

    assert_eq!(result, 32000000);

    let input = include_str!("../../input.txt");
    let result = solve(input);

    assert_eq!(result, 898731036);
    println!("Solution: {:?}", result);
}
