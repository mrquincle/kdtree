/* This function takes last element as pivot, places the pivot element at its correct position in sorted
 * array, and places all smaller (smaller than pivot) to left of pivot and all greater elements to right
 * of pivot.
 */
#include <algorithm>
#include <utility>

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
 * We sort two containers using the information from the first container. This means, if indices are put in the
 * second container, they will point to the original values in ascending order.
 */
template<typename RandomAccessIterator1, typename RandomAccessIterator2>
void quicksort(RandomAccessIterator1 first1, RandomAccessIterator1 last1, 
        RandomAccessIterator2 first2, RandomAccessIterator2 last2)
{
    quicksort_recursive(first1, last1, first2, last2, 0, (last1 - first1) - 1);
}

