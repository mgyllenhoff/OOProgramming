#include <iostream>
#include <iomanip>
#include <ctime>
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

    double sum1 = 0;

    // Add matrices normally
    clock_t startTime1 = clock();
    matrixAdd(A, B, C1, 0, NUM_ROWS - 1, sum1);
    clock_t endTime1 = clock();

    cout << "OUTPUT MATRIX:" << endl;
    printMatrix(C1);
    cout << "Adding matrices normally took " << elapsedSec(startTime1, endTime1) << "seconds" << endl;

    // Add matrices using threading
    

} // end main