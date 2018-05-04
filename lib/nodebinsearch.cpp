#include <nodestructures.h>
#include <nodebinsearch.h>

unsigned long binarySearch(node *arr, unsigned long l, unsigned long r, unsigned long x){
	unsigned long rtemp=r;
    while (l <= r)
    {
        unsigned long m = l + (r-l)/2;
        // Check if x is present at mid
		//printf("%lu\n", (arr+m)->id);

        if ((arr+m)->id == x)
            return m;
 
        // If x greater, ignore left half
        if ((arr+m)->id < x)
            l = m + 1;
        // If x is smaller, ignore right half
        else
            r = m - 1;
    }
    // if we reach here, then element was
    // not present
    return rtemp+2;
}

