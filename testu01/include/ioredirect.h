#ifndef IOREDIRECT_H
#define IOREDIRECT_H


//  see https://stackoverflow.com/a/17821809/73007
void SwapIOB(FILE *A, FILE *B) {
    FILE temp;
    // make a copy of IOB A (usually this is "stdout")
    memcpy(&temp, A, sizeof(FILE));
    // copy IOB B to A's location, now any output
    // sent to A is redirected thru B's IOB.
    memcpy(A, B, sizeof(FILE));
    // copy A into B, the swap is complete
    memcpy(B, &temp, sizeof(FILE));
}

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>


// prints the content of filename and return 1 if target is a substring of any line
int printAndSeekSubstring(const char * filename, const char * target) {
  ssize_t read;
  char * line = NULL;
  size_t len = 0;
  FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s \n", filename);
        return 0;
      }
      int found = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
              printf("%s", line);
  if (strstr(line, target) != NULL) {
    found = 1;
    break;
  }
    }

    fclose(fp);
    if (line)
        free(line);
  return found;

}

#endif
