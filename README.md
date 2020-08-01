**Lem-in**

Lem-in is an algorithm project in Hive Helsinki where the goal is to move a number of ants through a maze in as little turns as possible. The rules are as follows:

- One room can contain a maximum of one ant, with the exception of start and end rooms
- Each ant can move only once per turn

We used Edmonds-Karp maximum flow algorithm and breath-first search (and a little bit of random depth-first search) to solve this problem.

This project was made with jmetelin: https://github.com/janettem

**Usage And Input**

```
./lem-in [-et] < map_file_name

e: More detailed error messages (school assignment requires them to be non-descriptive by default for some reason)
t: Prints the number of turns after the solution lines

```

An example of a valid map file:

```
# Number of ants
3
# command to determine the start room (does not have to be before the first room of the list)
##start
start 4 0
# room definitions by format [name] [x-coord] [y-coord]
a 4 2
b 4 4
c 2 2
d 0 4
e 8 2
f 8 4
g 9 0
h 9 1
i 9 2
j 9 3
k 9 4
l 9 5
m 9 6
# command to determine the end room
##end
end 4 6
# link definitions by format [room_name_1]-[room_name_2], order doesn't matter
start-a
a-b
b-end
start-c
c-d
d-b
a-e
e-f
f-end
start-g
g-h
h-i
i-j
j-k
k-l
l-m
m-end
```

**Output:**

The program will print input before solution.

Each turn is printed on its own line. Each ant movement is formatted as [ant]-[room name it moved to], where ant is marked by a letter "L" and the ant's unique number.

```
L1-a
L1-b L2-a
L1-end L2-b L3-a
L2-end L3-b
L3-end
```

**Bonus programs**

We also created two extra programs:
- gen: A simple map generator (usage: ./gen <ant_nb> <room_nb> <path_nb>
- lem-in-checker: A program that checks the validity of lem-in output (usage: ./lem-in < map_file | ./lem-in-checker)
