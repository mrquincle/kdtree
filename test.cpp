#include <quicksort.hpp>

#include <iostream>
#include <vector>

int main() {

    //double seed = 1231;
    double seed = time(NULL);
    srand(seed);

    std::vector<double> test;
    std::vector<double> test_copy;

    int N = 30;

    for (int i = 0; i < N; ++i) {
        double value = (rand() % 1000) / 100.;
        test.push_back(value);
        test_copy.push_back(value);
    }
    
    std::cout << "Test: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test[i] << " ";
    }
    std::cout << std::endl;

    std::vector<int> indices;
    for (int i = 0; i < N; ++i) {
        indices.push_back(i);
    }

    quicksort(test.begin(), test.end(), indices.begin(), indices.end());
    
    std::cout << "Indices: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << indices[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Sorted: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Sorted: " << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << test_copy[indices[i]] << " ";
    }
    std::cout << std::endl;
    

    return EXIT_SUCCESS;
}
