#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

const int NUM_ROWS = 9000;
const int NUM_COLS = 9000;

// Add two matrices returning the sum of all elements of the result
void matrixAdd(double leftMatrix[NUM_ROWS][NUM_COLS],
               double rightMatrix[NUM_ROWS][NUM_COLS],
               double resultMatrix[NUM_ROWS][NUM_COLS],
               int startRow,
               int endRow,
               double &sum)
{
    sum = 0;
    for (int row = startRow; row <= endRow; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            int value = leftMatrix[row][col] + rightMatrix[row][col];
            resultMatrix[row][col] = value;
            sum += value;
        }
    }
} // end matrixAdd

// Main function for running programs
int main(void) {
    static double A[NUM_ROWS][NUM_COLS];
    static double B[NUM_ROWS][NUM_COLS];
    static double C1[NUM_ROWS][NUM_COLS]; // unthreaded result
    static double C2[NUM_ROWS][NUM_COLS]; // threaded result

    // Initialize matrices
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
        }
    }

    // UNTHREADED MATRIX ADDITION
    double unthreadedTotalSum = 0;
    
    auto startTime1 = chrono::high_resolution_clock::now();

    matrixAdd(A, B, C1, 0, NUM_ROWS - 1, unthreadedTotalSum);
    auto endTime1 = chrono::high_resolution_clock::now();

    auto time1 = chrono::duration<double, milli>(endTime1 - startTime1).count();

    // THREADED MATRIX ADDITION
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 4;
    }

    vector<thread> threads; // Holds all the thread objects
    vector<double> threadSums(numThreads, 0); // To store partial sum of matrix additions for each thread

    int rowsPerThread = NUM_ROWS / numThreads; // Divides work among threads

    auto startTime2 = chrono::high_resolution_clock::now();

    // Perform threading
    for (int t = 0; t < numThreads; t++) {
        // Determine the rows each thread will execute on
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? NUM_ROWS - 1 : startRow + rowsPerThread - 1;

        threads.emplace_back(matrixAdd, A, B, C2, startRow, endRow, ref(threadSums[t]));
    }

    // Wait for all threads to finish
    for (auto &th : threads) {
        th.join();
    }

    // Combine sums
    double threadedTotalSum = 0;
    for (double s : threadSums) {
        threadedTotalSum += s;
    }

    auto endTime2 = chrono::high_resolution_clock::now();
    auto time2 = chrono::duration<double, milli>(endTime2 - startTime2).count();

    cout << "Matrix size: " << NUM_ROWS << " x " << NUM_COLS << endl;
    cout << "Threads used: " << numThreads << endl;
    cout << "Unthreaded sum = " << unthreadedTotalSum << endl;
    cout << "Threaded total sum = " << threadedTotalSum << endl;
    cout << "Unthreaded time: " << time1 << " ms" << endl;
    cout << "Threaded time: " << time2 << " ms" << endl;

} // end main