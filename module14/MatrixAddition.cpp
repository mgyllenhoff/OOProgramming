#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

const int NUM_ROWS = 9000;
const int NUM_COLS = 9000;

// Inline
inline void addRow(const double* left, const double* right, double* result, int numCols, double &sum) {
    for (int col = 0; col < numCols; col++) {
        double value = left[col] + right[col];
        result[col] = value;
        sum += value;
    }
}

// Add two matrices returning the sum of all elements of the result
inline void matrixAdd(double leftMatrix[NUM_ROWS][NUM_COLS],
                      double rightMatrix[NUM_ROWS][NUM_COLS],
                      double resultMatrix[NUM_ROWS][NUM_COLS],
                      int startRow,
                      int endRow,
                      double &sum)
{
    sum = 0;

    for (int row = startRow; row <= endRow; row++) {
        addRow(leftMatrix[row], rightMatrix[row], resultMatrix[row], NUM_COLS, sum);
    }
} // end matrixAdd

// Main function for running programs
int main(void) {
    static double matrix1[NUM_ROWS][NUM_COLS];
    static double matrix2[NUM_ROWS][NUM_COLS];
    static double unthreadedResult[NUM_ROWS][NUM_COLS]; // unthreaded result
    static double threadedResult[NUM_ROWS][NUM_COLS]; // threaded result

    // Initialize matrices
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            matrix1[i][j] = i + j;
            matrix2[i][j] = i - j;
        }
    }

    // UNTHREADED MATRIX ADDITION
    double unthreadedTotalSum = 0;
    
    auto startTime1 = chrono::high_resolution_clock::now();

    matrixAdd(matrix1, matrix2, unthreadedResult, 0, NUM_ROWS - 1, unthreadedTotalSum);
    auto endTime1 = chrono::high_resolution_clock::now();

    auto time1 = chrono::duration<double, milli>(endTime1 - startTime1).count();

    // THREADED MATRIX ADDITION
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 4;
    }

    // Use fixed-size arrays 
    std::thread threads[numThreads];
    double threadSums[numThreads];

    // Initialize sums
    for (int i = 0; i < numThreads; i++) {
        threadSums[i] = 0.0;
    }

    int rowsPerThread = NUM_ROWS / numThreads;

    auto startTime2 = chrono::high_resolution_clock::now();

    // Launch threads
    for (int t = 0; t < numThreads; t++) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? NUM_ROWS - 1 : startRow + rowsPerThread - 1;

        threads[t] = std::thread(matrixAdd, matrix1, matrix2, threadedResult, startRow, endRow, std::ref(threadSums[t]));
    }

    // Join threads
    for (int t = 0; t < numThreads; t++) {
        threads[t].join();
    }

    // Combine sums
    double threadedTotalSum = 0;
    for (int i = 0; i < numThreads; i++) {
        threadedTotalSum += threadSums[i];
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