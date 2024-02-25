#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;
using namespace chrono;

// Generate a vector with random values
vector<int> generateVector(size_t length) {
    srand(static_cast<unsigned int>(time(0)));
    vector<int> vector;
    for (size_t p = 0; p < length; ++p) {
        vector.push_back(rand());
    }
    return vector;
}

// Perform divide and return the pivot index
int divide(vector<int>& array, int min, int max) {
    int indx = array[max];
    int p = min - 1;

    for (int m = min; m < max; ++m) {
        if (array[m] <= indx) {
            p++;
            swap(array[p], array[m]);
        }
    }
    swap(array[p + 1], array[max]);
    return p + 1;
}

// Implement parallel quicksort algorithm using OpenMP
void parallelQuicksort(vector<int>& array, int min, int max) {
    if (min < max) {
        int indx = divide(array, min, max);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelQuicksort(array, min, indx - 1);
            }

            #pragma omp section
            {
                parallelQuicksort(array, indx + 1, max);
            }
        }
    }
}

int main() {
    // Set the size of the vector
    size_t vectorSize = 15000000;

    // Generate a random vector
    vector<int> array = generateVector(vectorSize);

    // Measure time before sorting
    auto start = high_resolution_clock::now();

    #pragma omp parallel
    #pragma omp single
    // Perform parallel quicksort
    parallelQuicksort(array, 0, array.size() - 1);

    // Measure time after sorting
    auto end = high_resolution_clock::now();

    // Calculate time based on input size
    auto elapse = duration_cast<milliseconds>(end - start);

    cout << "Vector Size: " << array.size() << endl;
    cout << "Time taken by parallel program is " << elapse.count() << " milliseconds" << endl;

    return 0;
}
