#include <stdio.h>
#include <string.h>
#include "utils.h"

void print_number(double number) {
    char buffer[128];
    sprintf(buffer, "%f", number);
    int i = strlen(buffer) - 1;
    while (i >= 1 && buffer[i] == '0') i--;
    if (buffer[i] == '.') i--;
    buffer[i+1] = '\0';
    printf("%s", buffer);
}
