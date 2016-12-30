#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define ROOT 0

void ReceiveMessages(int PROCESS_NUM) {
    MPI_Status status;
    char **messages = (char **) malloc((PROCESS_NUM - 1) * sizeof(char *));

    for (int i = 1; i < PROCESS_NUM; ++i) {

        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int message_length;
        MPI_Get_count(&status, MPI_CHAR, &message_length);

        char *message = (char *) malloc(message_length * sizeof(char));
        MPI_Recv(message, message_length, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        messages[i - 1] = message;
    }

    for (int i = 0; i < PROCESS_NUM - 1; i++) {
        printf(messages[i]);
        free(messages[i]);
    }
    free(messages);
}

void SendMessage(int PROCESS_RANK) {
    char message[30];
    int len = sprintf(message, "Hello from MPI process #%d\n", PROCESS_RANK);
    MPI_Send(message, len + 1, MPI_CHAR, ROOT, PROCESS_RANK, MPI_COMM_WORLD);
}

int main(int argc, char **argv) {

    int PROCESS_NUM;
    int PROCESS_RANK;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &PROCESS_NUM);
    MPI_Comm_rank(MPI_COMM_WORLD, &PROCESS_RANK);


    if (PROCESS_RANK == ROOT) {
        ReceiveMessages(PROCESS_NUM);
    } else {
        SendMessage(PROCESS_RANK);
    }

    MPI_Finalize();
    return 0;
}