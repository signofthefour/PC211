#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#define MAX  10000

#define MAX_THREAD  4

double matA[MAX][MAX];
double matB[MAX][MAX];
double matC[MAX][MAX];
int step_i = 0;
 
void* multi(void* arg)
{
    int i = step_i++; //i denotes row number of resultant matC
   
    for (int j = 0; j < MAX; j++)
      for (int k = 0; k < MAX; k++)
        matC[i][j] += matA[i][k] * matB[k][j];
}

int main(){
  
  printf("Carrying out matrix-matrix multiplication\n");
  int n = (int)MAX;
  // Cilk Plus array notation
  for (int i = 0 ; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      matA[i][j]=(double)i/(double)n;
      matB[i][j]=(double)j/(double)n;
      matC[i][j]=0;
    }
  }


  // C = A x B
  clock_t start, stop;
  start = clock();

   pthread_t threads[MAX_THREAD];
 
    // Creating four threads, each evaluating its own part
    for (int i = 0; i < MAX_THREAD; i++) {
        int* p;
        pthread_create(&threads[i], NULL, multi, (void*)(p));
    }
 
    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);   

  double time = double(clock() - start);
  
  printf("Checking the results...\n");
  double norm = 0.0;
  for ( int i = 0 ; i < MAX ; i++)
    for ( int j = 0 ; j < MAX ; j++)
      norm += (matC[i][j]-(double)(i*j)/(double)n)*(matC[i][j]-(double)(i*j)/(double)n);
  
  if (norm > 1e-10)
    printf("Something is wrong... Norm is equal to %f\n", norm);
  else
    printf("Yup, we're good!\n");

  printf("Computing time: %f\n", time);  
}
