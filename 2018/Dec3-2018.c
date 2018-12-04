#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
  // Vars
  FILE *fileStream = NULL;
  int lineCount = 0;
  char inputString[100];
  char **inputArray = NULL;

  fileStream = fopen("./inputs/Dec3-2018.txt", "r");

  while (fgets(inputString, 100, fileStream) != NULL)
  {
    lineCount++;
    inputArray = realloc(inputArray, sizeof(char *) * lineCount);
    inputArray[lineCount - 1] = malloc(sizeof(char) * strlen(inputString) + 1);
    inputString[strlen(inputString) - 1] = '\0';
    strcpy(inputArray[lineCount - 1], inputString);
  }

  printf("%s\n", inputArray[256]);

  return 0;
}