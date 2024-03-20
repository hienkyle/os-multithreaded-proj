// Includes
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <pthread.h>
#include <thread>
using namespace std;


// Function prototypes

void *estimator(void *arg);


// Global variables

const int NUM_OF_THREADS = 10;
long hitCircle = 0;
long tossesPerThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(){
    // get the number of tosses from stdin
    // long tosses;
    std::cout << "Enter the number of tosses: " << std::flush;
    std::cin >> tossesPerThread;

    // calculate the number of tosses per thread
    // tossesPerThread = (long) std::ceil(tosses / NUM_OF_THREADS);

    // create threads to do estimation
    pthread_t threads[NUM_OF_THREADS];
    for(int i = 0; i < NUM_OF_THREADS; i++){
        pthread_create(&threads[i], NULL, estimator, NULL);
    }

    // join all threads
    for(int i= 0; i < NUM_OF_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    // destroy the mutex lock
    pthread_mutex_destroy(&mutex);
    
    // calculate the value of pi
    double pi = 4.0 * (double) hitCircle / ((double) tossesPerThread * NUM_OF_THREADS);

    // print out the results
    std::cout << std::left << std::setprecision(16);
    std::cout << std::setw(28) << "Final Estimation of Pi:" << std::setw(20) << pi << std::endl;
    std::cout << std::setw(28) << "Machine Value of Pi:" << std::setw(20) << M_PI << std::endl;
}

// A function using random number generator to define the number of hits
void *estimator(void *arg){
    long inCircle = 0;
    unsigned int rand_state = rand();

    for(long i = 0; i < tossesPerThread; i++){
        double x = rand_r(&rand_state) / ((double) RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double) RAND_MAX + 1) * 2.0 - 1.0;

        double distance_squared = x * x + y * y;

        if(distance_squared <= 1){
            inCircle++;
        }
    }

    pthread_mutex_lock(&mutex);
    hitCircle += inCircle;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
