k -> number of maximum possible cars on a bridge in same direction.

file -> "a". This stores times when a car wants to go. i.e.scheduled time.

1.2.3.4 implies that the car goes to city1 to city2 on clock value 1,
then from city2 to city1 on clock value 2 and so on.

To run, just change k defined in headers.
LENGTH is the max possible length of string in the file(1.2.3.4 etc). Change if needed, default is 50.

Then to run:

mpicc -o scheduling scheduling.c
mpirun -np 4 scheduling

4 is number of total number of cars including both sides. Please keep it even.
