#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdbool.h>

bool resource_available[8];

void get_resources(int, int *);
void release_resources(int, int *);

#endif
