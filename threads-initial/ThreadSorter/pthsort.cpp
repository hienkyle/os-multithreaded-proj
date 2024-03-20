// Includes
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


// Function prototypes

void* mergeSort(void *pos);

std::ifstream openFile(const std::string fileName);
void readArr(std::ifstream& file);


// Global variables

int eleCount;
int *arr; // the original array
int *tmp; // a temporary buffer


// Parameter structure
typedef struct {
  int start;
  int end;
} range;


int main(int argc, char *argv[]) {
  // argument count does not match
  if(argc != 2){
    std::cerr << "usage: pthsort <input>" << std::endl;
    exit(1);
  }

  // open the file
  std::string fileName(argv[1]);
  std::ifstream file = openFile(fileName);

  // read the array in
  readArr(file);

  // if there is only one number, print it out and exit
  if(eleCount == 1){
    std::cout << arr[0] << std::endl;
    exit(0);
  }

  // otherwise, start merge-sort
  // set up the parameter
  range pos;
  pos.start = 0;
  pos.end = eleCount - 1;

  // merge-sort
  mergeSort(&pos);

  // print out the sorted array
  for(int i = 0; i < eleCount; i++){
    std::cout << arr[i] << std::endl;
  }

  exit(0);
}


// A function to merge-sort using multithreads
void* mergeSort(void *pos){
  // extract data from range
  range *positions = (range *) pos;
  int start = positions -> start;
  int end = positions -> end;
  int mid = (start + end) / 2;

  // return condition of merge-sort
  if(start == end){
    pthread_exit(NULL);
  }

  // create two ranges to pass to threads
  range lower;
  range upper;

  // set up the lower range
  lower.start = start;
  lower.end = mid;

  // set up the upper range
  upper.start = mid + 1;
  upper.end = end;

  // set up threads
  pthread_t lower_thread, upper_thread;

  // create threads
  pthread_create(&lower_thread, NULL, mergeSort, &lower);
  pthread_create(&upper_thread, NULL, mergeSort, &upper);

  // wait for the threads to finish
  pthread_join(lower_thread, NULL);
  pthread_join(upper_thread, NULL);

  // merge into the original array
  int i = start;
  int j = mid + 1;
  int k = start;
  while(i <= mid && j <= end){
    if(arr[i] < arr[j]){
      tmp[k] = arr[i];
      k++;
      i++;
    }else{
      tmp[k] = arr[j];
      k++;
      j++;
    }
  }

  while(i <= mid){
    tmp[k] = arr[i];
    k++;
    i++;
  }

  while(j <= end){
    tmp[k] = arr[j];
    k++;
    j++;
  }

  for(i = start; i <= end; i++){
    arr[i] = tmp[i];
  }

  return 0;
}

// A function to open the specified file and handle errors
std::ifstream openFile(const std::string fileName){
  // open the file in read mode
  std::ifstream file(fileName);

  if(!file.is_open()){ // if the file does not exist
    std::cerr << "file " << fileName << " does not exist" << std::endl;
    exit(1);
  }

  // return the file
  return file;
}

// A function to read the array from a file
void readArr(std::ifstream& file){
  // read the first number into eleCount
  file >> eleCount;
  
  // initialize arrays
  arr = new int[eleCount];
  tmp = new int[eleCount];

  // read the subsequent integers into the array
  for(int i = 0; i < eleCount; i++){
    file >> arr[i];
  }
}