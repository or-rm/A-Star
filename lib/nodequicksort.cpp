#include <nodestructures.h>
#include <nodequicksort.h>

void swap(connection *a, connection *b)
{
    unsigned long source = a->source;
	unsigned long destination=a->destination;

    a->source = b->source;
    a->destination = b->destination;


	b->source = source;
	b->destination = destination;
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */

unsigned long partition (connection* arr, unsigned long low, unsigned long high)
{
    unsigned long pivot = (arr+high)->source;    // pivot
    unsigned long i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if ((arr+j)->source <= pivot)
        {
            i++;    // increment index of smaller element
            swap((arr+i), (arr+j));
        }
    }

    swap(arr+i + 1, arr+high);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */

void quickSort(connection *arr, unsigned long low, unsigned long high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        unsigned long pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


