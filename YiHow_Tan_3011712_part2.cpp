/**
Student Name: Yi How Tan
Student Number: 3011712
**/
#include <iostream>
#include <mpi.h>
#include <cmath>
/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

// a method to print an array 
void printArray(int* array, int array_size) {
	std::cout << "Array" << std::endl;
	std::cout << "=====" << std::endl;
	std::cout << array[0];
	for (int i = 1; i < array_size; i++) {
		std::cout << ", " << array[i];
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

// a method to create a number array and return it
int* createArray(int n) {
	// seeding random number to 1 and set maximum number to 50 
	srand(1);
	int* num_array = new int[n];
	for (int i = 0; i < n; i++) {
		num_array[i] = rand() % 51;
	}
	return num_array;
}

// a method to received the an array and compute sum
int sum(int* num_array, int array_size) {
	int sum = 0;
	for (int i = 0; i < array_size; i++) {
		sum += num_array[i];
	}
	return sum;
}

// a method to  produce a sum of the square of differences between each
// value in the datasetand the meanand return this as the result.
float sumDifferences(int* num_array, int array_size, float mean) {
	float sum_differences = 0;
	float difference;
	for (int i = 0; i < array_size; i++) {
		difference = num_array[i] - mean;
		sum_differences += difference * difference;
	}
	return sum_differences;
}

void coordinator(void) {
	std::cout << "Student Name  : Yi How Tan" << std::endl;
	std::cout << "Student Number: 3011712" << std::endl;
	std::cout << std::endl;
	// ask user to enter an valid number for array size
	int array_size;
	std::cout << "Your world size is " << world_size << std::endl;
	do {
		std::cout << "Please enter a number for array size - ";
		std::cin >> array_size;

		if (array_size % world_size == 0)
			break;
		std::cout << "Please enter a number can be devided by world size" << std::endl;
	} while (true);
	std::cout << std::endl;

	// initiate the variables
	int* num_array = createArray(array_size);
	int partition_size = array_size / world_size;
	int* partition = new int[partition_size];

	// broardcast the partition size to all nodes
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// scatter the partition to each node
	MPI_Scatter(num_array, partition_size, MPI_INT, partition, partition_size, MPI_INT, 0, MPI_COMM_WORLD);

	// calculate mean
	float mean = 0.0;
	float sum_mean = 0.0;
	float overall_average;
	mean = sum(partition, partition_size) / (float)partition_size;
	// sum of the mean that cumputed by all nodes
	MPI_Reduce(&mean, &sum_mean, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	// copute the mean for whole array
	overall_average = sum_mean / world_size;
	// broardcast the mean to all nodes
	MPI_Bcast(&overall_average, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	// compute the sum differences of partittion
	float sum_differences = sumDifferences(partition, partition_size, overall_average);
	float total_sum_differences;
	// compute the sum differences of whole array
	MPI_Reduce(&sum_differences, &total_sum_differences, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	// compute the standard deviation
	float standard_deviation = sqrt(total_sum_differences / array_size);
	// printe out the result
	printArray(num_array, array_size);
	std::cout << "mean               - " << overall_average << std::endl;
	std::cout << "standard deviation - " << standard_deviation << std::endl;

	delete num_array;
	delete partition;
}



void paticipant(void) {
	int partition_size = 0;
	// receive partition size from coordinator by using broadcast
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// initiate the variables
	int* num_array = new int[partition_size * world_size];
	int* partition = new int[partition_size];
	// receive the partition from corrdinator by using scatter
	MPI_Scatter(num_array, partition_size, MPI_INT, partition, partition_size, MPI_INT, 0, MPI_COMM_WORLD);
	// calculate mean
	float mean = 0.0;
	float sum_mean = 0.0;
	mean = sum(partition, partition_size) / (float)partition_size;
	// sum of the mean that cumputed by all nodes and send to coordinator
	MPI_Reduce(&mean, &sum_mean, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	float overall_average = 0.0;
	// receive overall mean from coordinator by using broadcast
	MPI_Bcast(&overall_average, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	// compute the sum differences of partittion
	float sum_differences = sumDifferences(partition, partition_size, overall_average);
	// compute the sum differences of whole array and send to coordinator
	float total_sum_differences;
	MPI_Reduce(&sum_differences, &total_sum_differences, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	delete num_array;
	delete partition;
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

	if (world_rank == 0)
		coordinator();
	else
		paticipant();
	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	// standard C/C++ thing to do at the end of main
	return 0;
}