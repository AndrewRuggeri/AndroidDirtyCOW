/**
 * @author  Andrew Ruggeri
 * @breif This is an example dirty COW program which can be compile and run for Linux as well as built
 *        into a Android app.
 * @details header for dirty COW functions
 * @date 2017-12-28
 */
#ifndef DIRTYCOW_H
#define DIRTYCOW_H

#include <sys/mman.h>

int dirtyCOWrun(const char* filepath, const char* replaceText, off_t offset);

#endif