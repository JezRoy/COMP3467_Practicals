// --- Point-to-point communication with MPI ---
//
// The below skeleton of an MPI program initializes MPI, retrieves the rank ID of the process that 
// runs the specific copy of the program as well as the number of processes the program is executed with. 
// Afterwards, it prints a message to the screen.
// 
// a) Compile and run the code on NCC via the provided sbatch script "run_mpi_program.sh".
//      1. Login to NCC: 
//          ssh your-user-name@mira.dur.ac.uk 
//          ssh your-user-name@ncc1.clients.dur.ac.uk 
//      2. Submit the job script to the workload manager slurm:
//          sbatch run_mpi_program.sh
// b) Extend the program such that rank 0 sends the message to rank 1 for printing via MPI_Send and MPI_Recv.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    
    int count;
    int maxNum = 64;

    if (rank == 0) {
        char message[maxNum] = "Hello World!\n\0";
        // Rank no.0
        count = strlen(message);
        MPI_Send(&message, count, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);
        printf("Rank 0 sent a message to rank 1");
    } else if (rank == 1) {
        // Rank no.1
        char message[];
        // Defines some blank meta data to be used in message probing
        MPI_Status status;
        // Probe an incoming message for meta data about the incoming message (inc message size)
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        
        // Get the size of the incoming message from the probe
        MPI_Get_count(&status, MPI_INT, &count);

        // Create a buffer to help receive the right amount of data from the incoming above based on the above.
        int msgLen = (int)malloc(sizeof(int) * count);

        MPI_Recv(&message, msgLen, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("Rank 1 received (from rank 0): %s",message);
        printf("Message length was: %d", msgLen);

        free(msgLen);
    }

    MPI_Finalize();
}
