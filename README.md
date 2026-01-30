# Philosophers

A multithreaded solution to the classic Dining Philosophers synchronization problem. The project demonstrates concurrent programming principles, deadlock avoidance, and thread synchronization in C.

## About

Multiple philosophers sit at a round table with a bowl of spaghetti. Each philosopher alternates between eating, thinking, and sleeping. They need two forks to eat, but there's only one fork between each pair of philosophers. The challenge is to prevent deadlock and starvation.


## Implementation

- Each philosopher is a separate thread
- Each fork is protected by a mutex
- Timestamp-based monitoring detects deaths within 10ms
- Proper synchronization prevents data races

## Requirements

- C compiler (gcc/clang)

## Installation

```bash
git clone https://github.com/KozhInna/42_philosophers philo
cd philo
make
```

## Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_must_eat]
```

### Arguments

- `number_of_philosophers` - Number of philosophers
- `time_to_die` - Time in ms before a philosopher dies without eating
- `time_to_eat` - Time in ms a philosopher spends eating
- `time_to_sleep` - Time in ms a philosopher spends sleeping
- `number_of_times_each_must_eat` - (Optional) Simulation stops when all philosophers have eaten this many times

### Examples

```bash
./philo 5 800 200 200        # 5 philosophers, no one should die
./philo 4 410 200 200        # Edge case timing test
./philo 5 800 200 200 7      # Stop after 7 meals each
```

## Testing

**Data race detection:**
```bash
valgrind --tool=helgrind ./philo 5 800 200 200
```