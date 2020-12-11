/**
Student Name: Yi How Tan
Student Number: 3011712
**/
#include <iostream>
#include <mpi.h>
/** messages for communicating tasks **/
int COMPUTE_HASH_DIFFERENCE = 1;
/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

int worldsWorstworldsWorstHash1(int ip) {
	for (int l = 1; l < 100; l++) {
		ip = ip * l % 254;
	}
	return ip;
}

int worldsWorstworldsWorstHash2(int ip) {
	for (int l = 1; l < 50; l++) {
		ip = ((ip * l) + 2) % 254;
	}
	return ip;
}
void coordinator(void) {
	std::cout << "Student Name  : Yi How Tan" << std::endl;
	std::cout << "student Number: 3011712" << std::endl;
	std::cout << std::endl;
	int message;
	// tell the next node that we have to start computing an hash defference
	MPI_Send(&COMPUTE_HASH_DIFFERENCE, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	// we will expect the last node to send us a message to compute hash difference. get it
	// and ignore it
	MPI_Recv(&message, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	// compute the diffence of the tho hashs
	// initaite 122 and 321
	int hashInput = worldsWorstworldsWorstHash1(122) - worldsWorstworldsWorstHash2(321);
	// pass the hashInput to node 1 
	MPI_Send(&hashInput, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
	std::cout << "Rank " << world_rank << " sending: " << hashInput << std::endl;
	//receive the hash Input from last node
	MPI_Recv(&hashInput, 1, MPI_FLOAT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	// compute the hash1 and hash2 by using hashInput that recieved from last node
	// print out the result
	int hash1 = worldsWorstworldsWorstHash1(hashInput);
	int hash2 = worldsWorstworldsWorstHash2(hashInput);
	std::cout << "Result: hash1 = " << hash1 << ", hash2 = " << hash2 << std::endl;
}
void computeHastDifference(void) {
	int message;
	// wait till we get the compute message after this tell the next node to start
	MPI_Recv(&message, 1, MPI_INT, (world_rank + world_size - 1) % world_size, 0,
		MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(&COMPUTE_HASH_DIFFERENCE, 1, MPI_INT, (world_rank + 1) % world_size, 0,
		MPI_COMM_WORLD);

	int hashInput;
	// first, we recieve the hashInput from previous node
	// secound, we compute the hashInput 
	// we pass it on to the next node
	MPI_Recv(&hashInput, 1, MPI_FLOAT, (world_rank + world_size - 1) % world_size,
		0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	hashInput = worldsWorstworldsWorstHash1(hashInput) - worldsWorstworldsWorstHash2(hashInput);
	std::cout << "Rank " << world_rank << " sending: " << hashInput << std::endl;
	MPI_Send(&hashInput, 1, MPI_FLOAT, (world_rank + 1) % world_size, 0,
		MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
	// see if we can initialise the mpi library this is always the first thing that we
	// must do at the start of an MPI program
	MPI_Init(NULL, NULL);
	// one of the important tasks that we have to establish is how many processes are in
	// this MPI instance. this will determine who and what we have to communicate with
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// another important job is finding out which rank we are. we can use this rank
	// number to assign seperate jobs to different mpi units
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	// compute the difference of 2 hashs. whoever is
	// node zero should be given the job of coordinator
	if (world_rank == 0)
		coordinator();
	else
		computeHastDifference();
	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	// standard C/C++ thing to do at the end of main
	return 0;
}