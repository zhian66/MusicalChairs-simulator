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

# Futher Work

Cause we

