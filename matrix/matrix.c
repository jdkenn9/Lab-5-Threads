#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int MAX = 0;
int *matA; 
int *matB; 
int *matSumResult;
int *matDiffResult; 
int *matProductResult;

void fillMatrix(int *matrix) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            *(matrix + (i * MAX) + j) = rand()%10+1;
        }
    }
}

void printMatrix(int *matrix) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", *(matrix + (i * MAX) + j));
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  for (int i = 0; i < MAX; ++i)
    for (int j = 0; j < MAX; ++j) 
      *(matSumResult + (i * MAX) + j) = *(matA + (i * MAX) + j) + *(matB + (i * MAX) + j);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  for (int i = 0; i < MAX; ++i)
    for (int j = 0; j < MAX; ++j) 
      *(matDiffResult + (i * MAX) + j) = *(matA + (i * MAX) + j) - *(matB + (i * MAX) + j);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  for (int i = 0; i < MAX; ++i)
    for (int j = 0; j < MAX; ++j) 
      *(matProductResult + (i * MAX) + j) = *(matA + (i * MAX) + j) * *(matB + (i * MAX) + j);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
  srand(time(0));  // Do Not Remove. Just ignore and continue below.
  // 0. Get the matrix size from the command line and assign it to MAX
  printf("Length of Matrix: ");
  scanf("%d", &MAX);
  // Allocate as much space as needed for input size
  matA = (int*)malloc(MAX * MAX * sizeof(int));
  matB = (int*)malloc(MAX * MAX * sizeof(int));
  matSumResult = (int*)malloc(MAX * MAX * sizeof(int));
  matDiffResult = (int*)malloc(MAX * MAX * sizeof(int)); 
  matProductResult = (int*)malloc(MAX * MAX * sizeof(int));
  // 1. Fill the matrices (matA and matB) with random values.
  fillMatrix(matA);
  fillMatrix(matB);
  // 2. Print the initial matrices.
  printf("Matrix A:\n");
  printMatrix(matA);
  printf("Matrix B:\n");
  printMatrix(matB);
  // 3. Create pthread_t objects for our threads.
  pthread_t thread[MAX];
  // 4. Create a thread for each cell of each matrix operation.
  // You'll need to pass in the coordinates of the cell you want the thread
  // to compute.
  // 
  // One way to do this is to malloc memory for the thread number i, populate the coordinates
  // into that space, and pass that address to the thread. The thread will use that number to calcuate 
  // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
  for (int i = 0; i < MAX; i++) {
    int* p;
    pthread_create(&thread[i], NULL, computeSum, (void*)(p));
    pthread_create(&thread[i], NULL, computeDiff, (void*)(p));
    pthread_create(&thread[i], NULL, computeProduct, (void*)(p));
  }
  // 5. Wait for all threads to finish.
   for (int i = 0; i < MAX; i++)
    pthread_join(thread[i], NULL);
  // 6. Print the results.
  printf("Results:\n");
  printf("Sum:\n");
  printMatrix(matSumResult);
  printf("Difference:\n");
  printMatrix(matDiffResult);
  printf("Product:\n");
  printMatrix(matProductResult);
  return 0;
}