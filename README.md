# Part 1 (50%)
_____________________________________________________________________________________________________________
You are tasked with creating a sequential ring of 3 nodes.
The initial node (rank 0) should use values 122 and 321 and pass the difference of the two hashes
to the next node.

Like so:
<br />HashInput1 = worldsWorstHash1(122) - worldsWorstHash2(321)
<br />HashInput2 = worldsWorstHash1(HashInput1) - worldsWorstHash2(HashInput1)
<br />HashInput3 = worldsWorstHash1(HashInput2) - worldsWorstHash2(HashInput2)
<br />Print the final two hash values worldsWorstHash1(HashInput3) and worldsWorstHash2(HashInput3) to the console.
<br />Note: Any time a message is sent, output the message to the console.

Expected output:
<br />Rank 0 sending: 134
<br />Rank 1 sending: -100
<br />Rank 2 sending: -330
<br />Result: hash1= -210, hash2= 142
_____________________________________________________________________________________________________________
# Part 2 (50%)
_____________________________________________________________________________________________________________
You are to create an MPI program containing coordinator and participant nodes that will
calculate the mean and then calculate the standard deviation of a set of numbers.

The program should contain a printArray() method that will print out an array to console in a
single line. It should accept two parameters: a pointer to the array and the size of the array.
Include a createArray() function that takes in an integer n and returns a random array of size n.
Include a sum() method that takes in a reference to an array and an array size, and returns the
sum of all the values in that array.

Include a sumDiferences() method that takes in a reference to an array, an array size, and the
overall mean of the dataset. It should produce a sum of the square of differences between each
value in the dataset and the mean and return this as the result.

The coordinator and participant methods should do the following:
* Generate an array of random numbers (coordinator only). For predictable results seed the random number generator with 1 and limit the maximum value to 50.
* Determine the size of each partition (coordinator only). Broadcast this to all nodes.
* Scatter the partitions to each node.
* Calculate the mean for this node. Use a reduce operation to gather the overall average.
* Compute the overall average (coordinator only).
* Broadcast the overall average to all nodes and then compute the sum of diferences
* Reduce the overall sum of diferences
* Calculate the standard deviation and print out the dataset, mean and standard deviation (coordinator only).

Modify your code to work with any world size and accept a dataset size from the command line.
You may assume that the dataset size will be evenly divisible by the world size.

Report: Perform a comparison evaluating the performance of your program using four nodes
against a single node on datasets of different sizes. Try to find a crossover point where the four
node version is faster than the single node version. Produce a graph containing this crossover
point. Provide a short one page commentary on what this graph states about your algorithm
