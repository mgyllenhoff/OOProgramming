#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <vector>
using namespace std;

const int NUM_ROWS = 10;
const int NUM_COLS = 10;

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

// Determines the seconds elapsed between two times
double elapsedSec(const clock_t& startTime, const clock_t& endTime) {
    return double(endTime - startTime) / CLOCKS_PER_SEC;
} // end elapsedSec

// Prints a matrix to see it's components
void printMatrix(double matrix[NUM_ROWS][NUM_COLS]){
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";
} // end printMatrix

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

    cout << "MATRIX A:" << endl;
    printMatrix(A);

    cout << "MATRIX B:" << endl;
    printMatrix(B);

    double unthreadedTotalSum = 0;

    // Add matrices normally
    clock_t startTime1 = clock();
    matrixAdd(A, B, C1, 0, NUM_ROWS - 1, unthreadedTotalSum);
    clock_t endTime1 = clock();

    cout << "OUTPUT MATRIX:" << endl;
    printMatrix(C1);
    cout << "Adding matrices normally took " << elapsedSec(startTime1, endTime1) << "seconds" << endl;

    // Add matrices using threading
    int numThreads = 4; // Manually choose # threads (safe for small matrices)
    vector<thread> threads; // Holds all the thread objects
    vector<double> threadSums(numThreads, 0); // To store partial sum of matrix additions for each thread

    int rowsPerThread = NUM_ROWS / numThreads; // Divides work among threads

    clock_t startTime2 = clock();

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

    clock_t endTime2 = clock();

    // Combine sums
    double threadedTotalSum = 0;
    for (double s : threadSums) {
        threadedTotalSum += s;
    }

    cout << "THREADED OUTPUT MATRIX:" << endl;
    printMatrix(C2);

    cout << "Adding matrices using threading took " << elapsedSec(startTime2, endTime2) << " seconds" << endl;

    cout << "Unthreaded sum = " << unthreadedTotalSum << "\n";
    cout << "Threaded sum = " << threadedTotalSum << "\n";

} // end main