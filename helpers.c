/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

#define LIMIT 65536

/**
 * Returns true if value is in array of n values, else false.
 * Implemented binary search
 */
bool search(int value, int values[], int n)
{
    // Check if array size is positive, return 0 if not
    if(n < 0)
    {
        return false;
    }
    // Set initial boundaries
    int low = 0;
    int high = n-1;
    int mid = (n-1)/2;
    
    while(true)
    {
        // If value has been found, return 1
        if(values[mid] == value)
        {
            return true;
        }
        else
        {
            // If value not have been found
            // And the sorted location has been checked
            // return 0
            if((high - low) == 0)
            {
                return false;
            }
        }
        // If value is above mid search, update low limit
        if(value > values[mid])
        {
            low = mid+1;
            mid = (high + low)/2;
        }
        // If value is below mid search, update high limit
        else
        {
            high = mid-1;
            mid = (high + low)/2;
        }
    }
    // Returning 0 as default
    return false;
}

/**
 * Sorts array of n values.
 * Implemented counting sort
 */
void sort(int values[], int n)
{
    long countArray[LIMIT + 1];
    
    // Checking integer values in values[]
    // Increments the count array slots for each value
    for(int i = 0; i < n; i++)
    {
        countArray[values[i]]++;
    }
    
    long countMem = 0;
    // Iterates over count array
    for(long i = 0; i < LIMIT + 1; i++)
    {
        // If a slot is found that is not zero
        if(countArray[i] != 0)
        {
            // Store the value in values[] 
            // j number of times found in the count array
            // countMem updates the storage address in values[]
            for(long j = 0; j < countArray[i]; j++)
            {
                values[countMem] = i;
                countMem++;
                // Seg fault fix
                // Int precision seems to result in a value limit of 64062
                // with many 64062s generated for all higher values.
                // Fix avoids memory to be overwritten.
                if(countMem > n-1)
                {
                    break;
                }
            }
        }
    }
    return;
}
