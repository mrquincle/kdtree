#include <algorithm>
#include <utility>

/* This function takes last element as pivot, places the pivot element at its correct position in sorted
 * array, and places all smaller (smaller than pivot) to left of pivot and all greater elements to right
 * of pivot.
 */
template<typename RandomAccessIterator1, typename RandomAccessIterator2>
int partition(RandomAccessIterator1 first1, RandomAccessIterator1 last1, 
        RandomAccessIterator2 first2, RandomAccessIterator2 last2, int low, int high)
{
    __attribute__((unused)) typedef typename std::iterator_traits<RandomAccessIterator1>::value_type ValueType;

    ValueType pivot = *(first1 + high);
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++) 
    {
        if (*(first1 + j) <= pivot)
        {
            i++;   
            std::swap(*(first1 + i), *(first1 + j));
            std::swap(*(first2 + i), *(first2 + j));
        }
    }
    std::swap(*(first1 + i + 1), *(first1 + high));
    std::swap(*(first2 + i + 1), *(first2 + high));
    return (i + 1);
}

template<typename RandomAccessIterator1, typename RandomAccessIterator2>
void quicksort_recursive(RandomAccessIterator1 first1, RandomAccessIterator1 last1, 
        RandomAccessIterator2 first2, RandomAccessIterator2 last2, int low, int high)
{
    if (low < high)
    {
        // pivot is partitioning index, values[p] is now at right place 
        int pivot = partition(first1, last1, first2, last2, low, high);

        // Separately sort elements before partition and after partition
        quicksort_recursive(first1, last1, first2, last2, low, pivot - 1);
        quicksort_recursive(first1, last1, first2, last2, pivot + 1, high);
    }
}

/**
 * We sort a container and update a second container as well using the same sorting operation. The values in the 
 * second container are sorted according to the information in the first container. If the second container contains
 * indices from 0 to N-1, the sorted values can be obtained by original_values[indices[i]]. The smallest value will
 * be original_values[indices[0]]. Note, that the caller has to make a copy of the values. The values are sorted 
 * in-place.
 */
template<typename RandomAccessIterator1, typename RandomAccessIterator2>
void quicksort(RandomAccessIterator1 first1, RandomAccessIterator1 last1, 
        RandomAccessIterator2 first2, RandomAccessIterator2 last2)
{
    quicksort_recursive(first1, last1, first2, last2, 0, (last1 - first1) - 1);
}

template<typename RandomAccessIterator1, typename RandomAccessIterator2>
void inverse(RandomAccessIterator1 first1, RandomAccessIterator1 last1, 
        RandomAccessIterator2 first2, RandomAccessIterator2 last2)
{
    // should be integers
    __attribute__((unused)) typedef typename std::iterator_traits<RandomAccessIterator1>::value_type ValueType;

    for (int i = 0; i < last1 - first1; ++i) {
        ValueType index = *(first1 + i);
        *first2 + index = i;
    }
}
