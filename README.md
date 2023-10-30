# philosophers
Dining philosophers problem with twist of threads and mutexes

## Brief
This project aims to provide a solution for a multi-threaded system AKA dining philosophers problem.
Assuming we have a table with N philos and forks, we should try to keep the agents alive as long as they could not eat anymore (e.g. the forks are occupied).

![Dining philosophers ](./visual.drawio.svg)

## Methodology
The forks are locks (mutexes) and each philo has a seperate thread; I have used `pthread` module to work with threads and mutexes.
To make philosophers avoid dying, I have used the following policy on which fork is picked up:

$$
fork_{right}(i, n) = (i + (i \mod 2)) \mod n
$$

Where:
- `i` is the index of the target philosopher
- `n` is the number of the forks

<small>*Note the left fork is `right fork - 1`*</small>

This ensures that the harmonic aspect of picking the forks is applied.

## Interface
Following arguments are passed to `./philo` respectively:
- `number_of_philosophers`: The number of philosophers and also the number
of forks.
- `time_to_die` (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
- `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
- `number_of_times_each_philosopher_must_eat` (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.

<small>*Arguments' description copied from 42 Network Subject Version 11*</small>

### Example
```
make && ./philo 3 100 60 100
```
```
0 1 is thinking
0 1 has taken a fork
0 1 has taken a fork
0 2 is thinking
0 2 has taken a fork
0 2 has taken a fork
0 3 is thinking
0 2 is eating
0 1 is eating
60 1 is sleeping
60 2 is sleeping
60 3 has taken a fork
60 3 has taken a fork
60 3 is eating
101 1 died
```
