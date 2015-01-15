Simple to use directory lister.
============================

# What is this "dynamicArray.h"?
Get it [here](https://github.com/tuggan/dynamic-array)

## Example
``` c
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include "dynamicArray.h"
#include "dirRead.h"


int main() {

    dynArray list = dynA_init();
  
 
    if(list != NULL) {
        char *paths = getenv("PATH");

        puts(paths);

        const char delim = ':';

        char *loc = strtok(paths, &delim);

        do {
            printf("%s\n", loc);
            directoryList(list, loc);
        } while((loc = strtok(NULL, &delim)) != NULL);
    
        sortDirList(list);
        printList(list);
    }
  
    return 0;
}


```
