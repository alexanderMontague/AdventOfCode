#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
  // Vars
  FILE *fileStream = NULL;
  char inputString[100];
  int doubleCharCount = 0;
  int tripleCharCount = 0;
  // fileStream = fopen("./inputs/Dec2-2018.txt", "r");
  fileStream = fopen("./inputs/Dec2-2018.txt", "r");

  while (fgets(inputString, 100, fileStream) != NULL)
  {
    inputString[strlen(inputString) - 1] = '\0';
    printf("%s\n", inputString);

    int *letterArray = malloc(250 * sizeof(int));

    for (int i = 0; i < strlen(inputString); i++)
    {
      letterArray[(int)inputString[i]]++;
      printf("Letter: %c  |  Freq: %d\n", inputString[i], letterArray[(int)inputString[i]]);
    }

    bool foundDouble = false;
    bool foundTriple = false;
    for (int j = 0; j < 250; j++)
    {
      if (letterArray[j] == 2 && !foundDouble)
      {
        doubleCharCount++;
        foundDouble = true;
      }
      else if (letterArray[j] == 3 && !foundTriple)
      {
        tripleCharCount++;
        foundDouble = false;
      }
    }
  }

  printf("Doubles: %d  |  Triples: %d\n", doubleCharCount, tripleCharCount);

  return 0;
}