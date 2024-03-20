# Pthread Programming
## Operating Systems
## Due March 21 by 23:59

In this project, you will complete three pthread programs.  The first program is a sudoku validator.  The second program is a sorting program that uses threads.  The third program is a Monte Carlo Pi estimator.  By completing these programs, you will gain confidence in writing pthread programs necessary for the next project.

# Logistics
- This project is worth 100 points.
- This is an individual assignment.  I will be checking your source code using MOSS against other programs on GitHub, CoPilot, ChatGPT, etc. as well as other students.  **[Click here for the MOSS site.](https://theory.stanford.edu/~aiken/moss/)**
- Each of your programs must be written in C/C++.  The use of the standard template library is permitted.
- Each program subdirectory has a `Makefile` supplied.  Additional targets have been added to clean your source code subdirectory and another target to test your program.
- Your source code file is `pthsv.cpp`, `pthsort.cpp`, and `pthpi.cpp`.
- The name of your executable must be `pthsv`, `pthsort`, and `pthpi`.
- Your program must be well documented. (10 points)
- Your program must be well structured.  Programs that are all in `main()` or have excessively long functions will result in loss of points.
- 
- Your program will be tested on `lovelace.cs.tcu.edu` (Ubuntu Server 22.04 LTS).
- Keep your program up-to-date using git.
- The test cases included with this project are the minimum test cases for submission.  Note: I may test your program with other test cases.
- Create your own `Makefile` for the Monte Carlo problem.


# Sudoku Validator (30 points)
A Sudoku puzzle uses a 9 × 9 grid in which each column and row, as well as each of the nine 3 × 3 subgrids, must contain all of the digits 1..9. The figure below presents an example of a valid Sudoku puzzle. This project consists of designing a multithreaded application that determines whether the solution to a Sudoku puzzle is valid.
```text
6 2 4 5 3 9 1 8 7
5 1 9 7 2 8 6 3 4
8 3 7 6 1 4 2 9 5
1 4 3 8 6 5 7 2 9 
9 5 8 2 4 7 3 6 1 
7 6 2 3 9 1 4 5 8 
3 7 1 9 5 6 8 4 2 
4 9 6 1 8 2 5 7 3 
2 8 5 4 7 3 9 1 6
```

There are several different ways of multithreading this application. One suggested strategy is to create threads that check the following criteria:

+ A thread to check that each column contains the digits 1 through 9
+ A thread to check that each row contains the digits 1 through 9
+ Nine threads to check that each of the 3 × 3 subgrids contains the digits 1 through 9

This would result in a total of eleven separate threads for validating a Sudoku puzzle. However, you are welcome to create even more threads for this project. For example, rather than creating one thread that checks all nine columns, you could create nine separate threads and have each of them check one column.

## Passing parameters to each thread

The parent thread will create the worker threads, passing each worker the location that it must check in the Sudoku grid. This step will require passing several parameters to each thread. The easiest approach is to create a data structure using a struct. For example, a structure to pass the row and column where a thread must begin validating would appear as follows:

```c
/* structure for passing data to threads */
typedef struct
{
    int row;
    int column;
} parameters;
``` 

Pthreads programs will create worker threads using a strategy similar to that shown below:

```c
parameters *data = (parameters *) malloc(sizeof(parameters));
data->row = 1;
data->column = 1;
/* Now create the thread passing it data as a parameter */
``` 

The data pointer will be passed to the `pthread_create()` function, which in turn will pass it as a parameter to the function that is to run as a separate thread.

## Returning results to the parent thread

Each worker thread is assigned the task of determining the validity of a particular region of the Sudoku puzzle. Once a worker has performed this check, it must pass its results back to the parent. One good way to handle this is to create an array of Boolean values that is visible to each thread. The $i^{th}$ index in this array corresponds to the ith worker thread. If a worker sets its corresponding value to 1, it is indicating that its region of the Sudoku puzzle is valid. A value of 0 indicates otherwise. When all worker threads have completed, the parent thread checks each entry in the result array to determine if the Sudoku puzzle is valid.

## Input and Output
The commandline will contain the name of the single file to validate.  Your program must open the file, read in the sudoku board (9 rows of 9 columns of single integers) and store them in an array.  The numbers in the file will be from the digits 1..9 with one space separating them.

Output will be either `TRUE` or `FALSE` followed by a newline to standard output.  The exit code must be 0.

If the number of command line arguments is not correct, then output the message to standard error `usage: pthsv <input>`, followed by a newline. Set the exit code to 1, and exit your program.  If the file does not exist, then output the message to standard error `file <filename> does not exist`, followed by a newline.  Set the exit code to 1, and exit your program.


# Multithreaded Sorting Application (30 points)

Write a multithreaded sorting program that works as follows: A list of integers is divided into two smaller lists of equal size. Two separate threads (which we will term sorting threads) sort each sublist using a sorting algorithm of your choice. The two sublists are then merged by a third thread — a merging thread — which merges the two sublists into a single sorted list.

Because global data are shared across all threads, perhaps the easiest way to set up the data is to create a global array. Each sorting thread will work on one half of this array. A second global array of the same size as the unsorted integer array will also be established. The merging thread will then merge the two sublists into this second array. 

This programming project will require passing parameters to each of the sorting threads. In particular, it will be necessary to identify the starting index from which each thread is to begin sorting. 

The parent thread will output the sorted array once all sorting threads have exited.

## Input and Output
The commandline will contain the name of a single file to sort.  The input file will have the following structure.  The first line will be a single integer $n$ followed by $n$ integers.  Here is an example:
```text
5
42
41
40
5
6
```

Output will be the sorted array of integers with one integer per line printed to standard output.  Here is an example:
```text
5 
6 
40 
41 
42 
43
```

If the number of command line arguments is not correct, then output the message to standard error `usage: pthsort <input>`, followed by a newline. Set the exit code to 1, and exit your program.  If the file does not exist, then output the message to standard error `file <filename> does not exist`, followed by a newline.  Set the exit code to 1, and exit your program.

# Monte Carlo Pi Estimator (30 points)

Suppose we toss darts randomly at a square dartboard, whose bullseye is at the origin, and whose sides are 2 feet in length.  Suppose also that tthere is a circle inscribed in the square dartboard.  The radius of the circle is 1 foot, and its area is $\pi$ square feet.  If the points that are hit by the darts are uniformly distributed (and we always hit the square), then the number of darts that hit inside the circle should approximately satisfy the equation $\frac{number in circle}{total number of tosses} = \frac{\pi}{4}$ since the ratio of the area of the circle to the area of the square is $\frac{\pi}{4}$.

We can use this formula to estimate the value of $\pi$ with a random number generator:

```cpp
number_in_circle = 0;
for(toss=0; toss<number_of_tosses; toss++){
    x = random double between -1 and 1;
    y = random double between -1 and 1;
    distance_squared = x*x + y*y;
    if(distance_squared <=1)
        number_in_circle++;
}
pi_estimate = 4 * number_in_circle / (double) number_of_tosses;
```

This is called a "Monte Carlo" method, since it uses randomness (the dart tosses).

Write a Pthreads program that uses a Monte Carlo method to estimate $\pi$.  You may want to use `long  int` for the number of hits in the circle and the number of tosses, since both may have to very large to get a reasonable estimate of $\pi$.

## Input and Output
There are no test cases for this program like those used in previous problems.

Input will be from standard input.  Prompt the user to enter the number of tosses.

Output will be to standard output.  Echo print the number of tosses and the estimate of $\pi$.  Also output the "machine" value of $\pi$.

```text
user@lovelace> ./pthpi
Enter the number of tosses: 100
Final Estimation of Pi:     3.0916
Machine Value of Pi:        3.141592653589793
```

