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
void *rowCheck(void *pos);
void *colCheck(void *pos);
void *subGridCheck(void *pos);
bool isSudokuValid();

std::ifstream openFile(const std::string fileName);
void readSudoku(std::ifstream& file);

// Global variables
int const SUDOKU_SIZE = 9;
int const NUMBER_OF_THREADS = 27; // 9 subgrids + 9 rows + 9 columns
int validations[NUMBER_OF_THREADS] = {0}; // 0 - 8 for subgrids, 9 - 17 for rows, 18 - 26 for columns
// int sudoku[9][9] = { // a predefined valid sudoku from the instruction
//   {6, 2, 4, 5, 3, 9, 1, 8, 7},
//   {5, 1, 9, 7, 2, 8, 6, 3, 4},
//   {8, 3, 7, 6, 1, 4, 2, 9, 5},
//   {1, 4, 3, 8, 6, 5, 7, 2, 9}, 
//   {9, 5, 8, 2, 4, 7, 3, 6, 1}, 
//   {7, 6, 2, 3, 9, 1, 4, 5, 8}, 
//   {3, 7, 1, 9, 5, 6, 8, 4, 2}, 
//   {4, 9, 6, 1, 8, 2, 5, 7, 3}, 
//   {2, 8, 5, 4, 7, 3, 9, 1, 6}
// };

// int sudoku[9][9] = { // a predefined invalid sudoku from the test files
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9},
//   {1, 2, 3, 4, 5, 6, 7, 8, 9}
// };

int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {0}; // a blank sudoku

// Parameter structure
typedef struct {
  int row;
  int col;
} position;



int main(int argc, char *argv[]) {
  // if more than 2 arguments, error
  if(argc != 2){
    std::cerr << "usage: pthsv <input>" << std::endl;
    exit(1);
  }

  // open the file
  std::string fileName(argv[1]);
  std::ifstream file = openFile(fileName);

  // read the sudoku from the file
  readSudoku(file);
  file.close();

  // check if the sudoku is valid
  bool valid = isSudokuValid();

  // output the appropriate result
  if(valid){
    std::cout << "TRUE" << std::endl;
  }else{
    std::cout << "FALSE" << std::endl;
  }

  // exit
  exit(0);
}

// A function to check a row of the sudoku
void *rowCheck(void *pos){
  // extract data from pos
  position *positions = (position *) pos;
  int row = positions -> row;

  // tracking array
  int appeared[SUDOKU_SIZE] = {0};

  // go to every value in the row to check
  for(int i = 0; i < SUDOKU_SIZE; i++){
    int entry = sudoku[row][i];
    if(entry < 1 || entry > SUDOKU_SIZE || appeared[entry - 1] != 0){ // if the value has appeared
      pthread_exit(NULL); // exit immediately
    }else{ // if the value has not appeared
      appeared[entry - 1] = 1; // set the corresponding entry to be 1
    }
  }

  // if thread has yet exited, the row is valid
  validations[9 + row] = 1; // set the corresponding entry in the global validations
  pthread_exit(NULL);
}

// A function to check a column of the sudoku
void *colCheck(void *pos){
  // extract data from pos
  position *positions = (position *) pos;
  int col = positions -> col;

  // tracking array
  int appeared[SUDOKU_SIZE] = {0};

  // go to every value in the column to check
  for(int i = 0; i < SUDOKU_SIZE; i++){
    int entry = sudoku[i][col];
    if(entry < 1 || entry > SUDOKU_SIZE || appeared[entry - 1] != 0){ // if the value has appeared
      pthread_exit(NULL); // exit immediately
    }else{ // if the value has yet appeared
      appeared[entry - 1] = 1; // set the corresponding entry to be 1
    }
  }

  // if thread has yet exited, the column is valid
  validations[18 + col] = 1; // set the corresponding entry in the global validations
  pthread_exit(NULL);
}

// A function to check a subgrid of the sudoku
void *subGridCheck(void *pos){
  // extract data from pos
  position *positions = (position *) pos;
  int row = positions -> row;
  int col = positions -> col;

  // tracking array
  int appeared[SUDOKU_SIZE] = {0};

  // go to every value in the subgrid to check
  for(int i = row; i < row + 3; i++){
    for(int j = col; j < col + 3; j++){
      int entry = sudoku[i][j];
      if(entry < 1 || entry > SUDOKU_SIZE || appeared[entry - 1] != 0){
        pthread_exit(NULL);
      }else{
        appeared[entry - 1] = 1;
      }
    }
  }

  // if thread has yet exited, the subgrid is valid
  validations[row + col/3] = 1; // set the corresponding entry in the global validations
  pthread_exit(NULL);
}

// A function to check the whole sudko
bool isSudokuValid(){
  // thread array and index setup
  pthread_t threads[NUMBER_OF_THREADS];
  int threadIndex = 0;

  for(int row = 0; row < SUDOKU_SIZE; row++){
    for(int col = 0; col < SUDOKU_SIZE; col++){
      if(row % 3 == 0 && col % 3 == 0){ // subgrids check
        position *gridPos = (position *) malloc(sizeof(position));
        gridPos -> row = row;
        gridPos -> col = col;
        pthread_create(&threads[threadIndex++], NULL, subGridCheck, gridPos);
      }

      if(row == 0){ // columns check
        position *colPos = (position *) malloc(sizeof(position));
        colPos -> row = row;
        colPos -> col = col;
        pthread_create(&threads[threadIndex++], NULL, colCheck, colPos);
      }

      if(col == 0){ // rows check
        position *rowPos = (position *) malloc(sizeof(position));
        rowPos -> row = row;
        rowPos -> col = col;
        pthread_create(&threads[threadIndex++], NULL, rowCheck, rowPos);
      }
    }
  }

  // wait for all threads to exit
  for(int i = 0; i < NUMBER_OF_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  // check the validations array
  for(int i = 0; i < NUMBER_OF_THREADS; i++){
    if(validations[i] == 0){ // if violates any conditions
      return false;
    }
  }
  
  // if everything is fine
  return true;
}

// A function to open the specified file and handle error
std::ifstream openFile(const std::string fileName){
  // open the file in read mode
  std::ifstream file(fileName);

  // if the file cannot be open
  if(!file.is_open()){
    std::cerr << "file " << fileName << " does not exist" << std::endl;
    exit(1);
  }

  // return the file
  return file;
}

// A function to read in a sudoku from a file
void readSudoku(std::ifstream& file){
  std::string line;
  int row = 0;

  while(std::getline(file, line)){
    std::istringstream stream(line);
    int col = 0;

    int entry = 0;
    while(stream >> entry){
      sudoku[row][col] = entry;
      col++;
    }

    row++;
  }
}