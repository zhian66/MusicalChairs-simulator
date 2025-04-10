# Introduction

This project simulates the "musical chairs" problem: N persons compete for M chairs in multiple rounds. Each person must win at least W times within R consecutive rounds to succeed.

The goal is to simulate justice and fairness in a realistic setting. For example:
- Chairs may be refreshed in each round (like concert tickets).
- Participants run at different speeds.
- People might not wait for all others to start the next round.
- N and M can be very large.

# How to run

To compile the simulator:
```
g++ -std=c++14 -o simulator main.cpp
```
To run the simulator:
```
// Usage: N M R W
./simulator 100 20 500 100  
```

# Our Goal

Supposed there are 100 people, 20 chairs in multiple rounds. 
Each person must win at least 100 times within 500 consecutive rounds to succeed.

We expected that every person wins 100 times in average.

# Input

Parameters:

| Parameter | Description                   |Value|
|----------|-------------------------------|---|
| N        | Number of participants         |100|
| M        | Number of chairs per round     |20|
| R        | Number of rounds               |500|
| W        | Minimum wins required          |100|

# Output

In the Begining of running the simulator, the program will display the ID and speed of each participant.
Each participant’s speed is randomly generated and simulates their performance in the competition.

Sorted by speed.
```
ID: 86, Speed: 0.997215
ID: 19, Speed: 0.981123
ID: 73, Speed: 0.968161
...
ID: 52, Speed: 0.0226218
ID: 99, Speed: 0.0204464
ID: 77, Speed: 0.00104919
```

After simulating, 

```
Participant 86 wins: 108
Participant 19 wins: 104
Participant 73 wins: 102
...
Participant 70 wins: 98
Participant 82 wins: 98
Participant 33 wins: 93
Participant 12 wins: 109
Participant 52 wins: 104
Participant 99 wins: 107
Participant 77 wins: 107

Win Count Standard Deviation: 4.49889
Mean Win Count: 100
Max Win Count: 112
Min Win Count: 89
Max-Min Win Gap: 23
```

We get mean win count 100!

<img width="483" alt="image" src="https://github.com/user-attachments/assets/6c741626-f09f-45c6-ad82-c9c625c978d2" />

# Methodology

Initialization:
```cpp=
// === Initialization ===
Input N, M, R, W  // N = number of participants, M = number of chairs
                  // R = number of rounds, W = minimum wins per participant

If R * M < N * W:
    Print "Impossible to complete"
    Exit program

// Initialize participant list
For i = 0 to N-1:
    Participant[i].id = i
    Participant[i].winCount = 0
    Participant[i].speed = random value in [0.0, 1.0]  // higher = slower

// Initialize chair locks
For i = 0 to M-1:
    Create chairLocks[i] as mutex

// Print participant speeds
Sort participants by descending speed
For each participant:
    Print ID and Speed
```

Simulation: 
```
// === Simulation ===

For round = 1 to R:
    // Reset all chairs (unlock forcibly)
    For each chair in chairLocks:
        chair.unlock()

    // Mixed selection strategy based on win count
    For i = 0 to N-1:
        weight[i] = 1 / (1 + winCount[i])^k  

    selected = empty set
    While selected.size < M:
        Randomly sample one index using weighted probability
        If index not in selected:
            Add index to selected

    // Start threads for each selected participant
    For each index in selected:
        Start thread run: compete(participant[index], chairLocks)

    // Wait for all threads to finish
    Join all threads
```

# Futher Work

Cause we

