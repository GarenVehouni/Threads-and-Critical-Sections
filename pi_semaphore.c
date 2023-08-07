#include <semaphore.h> //header file for semaphore functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h> //allows us to do pthread programming
#include "timer.h"
//when compiling the code, after the whole gcc part, include -lm -lpthread
//
////global variables
sem_t sem; //semaphore declaration
int thread_count;
double n = 6400000.0; //fixed n instead of user input
double a = 0.0;
double b = 1.0;
double pi = 0.0; //initializing pi to zero. Each thread will contribute to its count later 
//timer variables
double start, finish, elapsed;

//timer variables for speedup calculation
double startOneThread, finishOneThread, elapsedOneThread;

//timer variables for tabulating
double startTimeTable, finishTimeTable, elapsedTimeTable;

double givenFunction(double x) {
 return 4.0 / (1.0 + x * x);
	}
void* simpson(void* arg) { //takes in number of threads enterred by user as argument
	int thread_id = (int)arg;
	double leadingCo;
        double c = b - a;
        double d = 3.0 * n;
        leadingCo = c / d;
//below calculates how many iterations each thread will make
double incrementValue = (b - a) / n;
//his dx is my incrementValue
//////each thread has its own a, b and n values
double local_N = n/(double)thread_count;
double local_low = a + (local_N * thread_id * incrementValue);
double local_high = local_low + (local_N * incrementValue);
        double sum = 0.0; //initializing sum var
        int i;
        for (i = 0; i < local_N; i+= 2) { //calculates each time the parenthesis calculates 4f(x)
                double x = local_low + i * incrementValue;
                 sum += givenFunction(x) * 4.0; //calculates 4f(x) 
         }
        for (i = 1; i < local_N - 1; i+= 2) {
                double y = local_low + i * incrementValue;
                sum += givenFunction(y) * 2.0; //calculates 2f(x)
         }
        double firstTerm = givenFunction(a);
        double lastTerm = givenFunction(b);
        double local_pi = leadingCo * (firstTerm + sum + lastTerm);

	sem_wait(&sem); //lock the following section for one thread
	pi += local_pi; //the value pi is calculated by adding all of the individual local pi values each thread calculated together
	sem_post(&sem); //unlock the section for other threads

}


int main(int arg, char* argv[]) {
//add a checker to see if user enterred a valid number

if (arg != 2) {
fprintf(stderr, "Incorrect Usage. Run as ./my_code6 <number>\n");
exit(EXIT_FAILURE);
}

thread_count = strtol(argv[1], NULL, 10);

if (thread_count <= 0) {
fprintf(stderr, "The entered argument is not a valid number.\n");
exit(EXIT_FAILURE);
}



pthread_t* thread_handles = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
sem_init(&sem, 0, 1); //semaphore initialization

int i;

GET_TIME(start); //timer starts

for (i = 0; i < thread_count; i++) { //creates number of threads depending on user input 
        pthread_create(&thread_handles[i], NULL, simpson, (void*)i);
                }
for (i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
                }


GET_TIME(finish); //timer ends
        elapsed = finish - start; //calculates elapsed time
        printf("The code to be timed took %e seconds\n", elapsed);

sem_destroy(&sem); // destroy semaphore

free(thread_handles); // free thread handle
printf("Estimated value of pi: %lf\n", pi); //prints the value of pi calculated by threads



//below calculates speedup and efficiency
////first lets get the time of running with 1 thread

GET_TIME(startOneThread); //might need to change start variable
simpson(1); //pass in 1 thread
GET_TIME(finishOneThread);
elapsedOneThread = finishOneThread - startOneThread;
double speedup = elapsedOneThread / elapsed; //calc speedup
printf("Speedup is: %lf\n", speedup);

//below calculates efficiency

double efficiency = speedup / thread_count; //calc efficiency
printf("Efficiency is: %lf\n", efficiency);

// below tabulates the results

printf("Performance evaluation using semaphore method: ");
printf("\n%-20s %-20s %-20s %-20s\n", "Number of threads", "Runtime", "efficiency", "Speed-up");

//loop for tabulating results

int j;
        for(j = 0; j <= 4; j++) {
                int tableThreadCount = pow(2, j); //thread counts 1, 2, 4, 8, 16
                GET_TIME(startTimeTable); //start time
                simpson((void*)tableThreadCount); //i ^ 2 is passed into simpson
                GET_TIME(finishTimeTable); //end time            
                double elapsedTimeTable = finishTimeTable - startTimeTable;
                double speedupTable = elapsedTimeTable / elapsed;
                double tableEfficiency = speedupTable / tableThreadCount;
                printf("\n%-20d %-20lf %-20lf %-20lf\n", tableThreadCount, elapsedTimeTable, tableEfficiency, speedupTable);
}


return 0;


}









