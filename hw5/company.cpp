#include <mpi.h>
#include <iostream>
using namespace std;

struct message
{
	char name[32];
	int n;
};

bool findInArray(int* a, int n, int x) {
	for (int i = 0; i < n; i++) {
		if (a[i] == x) return true;
	}
	return false;
}

int main(int argc, char *argv[]) {
	MPI_Status status;
	int psize, prank;
 	int count = 0;
 	/*MPI_Datatype messageType;
    MPI_Datatype type[3] = { MPI_CHAR, MPI_INT};
    int blocklen[2] = {32, 1};
    MPI_Aint disp[3];
	message m;*/
	char name[32];

	MPI_Init(&argc, &argv);

	/*
    disp[0] = &message.name - &message;
    disp[1] = &message.n - &message;

    MPI_Type_create_struct(2, blocklen, disp, type, &messageType);
    MPI_Type_commit(&messageType);*/


	MPI_Comm_size(MPI_COMM_WORLD, &psize);

	MPI_Comm_rank(MPI_COMM_WORLD, &prank);

	srand(prank);

	int* ranks = new int[psize];
    char* names = new char[32 * psize];

	if(prank == 0) {
		sprintf(name, "name%d\n", prank);
		strcpy(names + 0 * 32, name);
		ranks[0] = prank;
		count = 1;
		
		int j = rand() % psize;
		while (j == prank) j = rand() % psize;

		//message m(name, j);
		
		int tag = 0;
		cout << prank << " sents to " << j << endl;
		MPI_Ssend(&count, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
		MPI_Ssend(ranks, psize, MPI_INT, j, tag, MPI_COMM_WORLD);
		MPI_Ssend(names, 32 * psize, MPI_CHAR, j, tag, MPI_COMM_WORLD);
		//MPI_SSend(&m, count, messageType, j, tag, MPI_COMM_WORLD);
	}
	else {
		//message m;
		MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int sender = status.MPI_SOURCE;
		MPI_Recv(ranks, psize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(names, 32 * psize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		ranks[count] = prank;
		cout << ranks[count] << endl;
		sprintf(name, "random%d%d_name%d\n",rand()%10, rand()%10, prank);
		strcpy(names + count * 32, name);
		count++;

		if (count < psize) {
			int j = rand() % psize;
			while (findInArray(ranks, count, j)) j = rand() % psize;

			int tag = 0;
			MPI_Ssend(&count, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
			MPI_Ssend(ranks, count, MPI_INT, j, tag, MPI_COMM_WORLD);
			MPI_Ssend(names, 32 * count, MPI_CHAR, j, tag, MPI_COMM_WORLD);
		}
		else {
			cout << "All names collected:\n";
			for (int i = 0; i < psize; i++) {
				cout << ranks[i] << " process has name: " << string(names + 32*i, 32) << endl;
			}
		}
	}
	MPI_Finalize();
	delete[] names;
	delete[] ranks;
	return 0;
}