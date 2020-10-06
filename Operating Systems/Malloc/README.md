# Malloc

## Description
This program implements the behavior of the library functions malloc, calloc, realloc, and free. Malloc, calloc, and realloc, return (void *) to the user upon call, while free has no return value. Free instead removes data from the doublely linked list applied to the heap. Malloc expects the amount of memory requested as a parameter. Malloc initializes a memory space for the user. Calloc expects a type and the number of these types that need to fit in the allocated memory space. This essentially acts like a malloc call, but the memory space provided is preset to NULL. Realloc takes in an existing memory space and the new size for this memory space. Realloc adjusts the size of an input memory space.

