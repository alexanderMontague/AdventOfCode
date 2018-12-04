#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
  // Vars
  FILE *fileStream = NULL;
  char stringArray[300][50];
  char inputString[100];
  int lineCount = 0;
  int doubleCharCount = 0;
  int tripleCharCount = 0;
  fileStream = fopen("./inputs/Dec2-2018.txt", "r");

  while (fgets(inputString, 100, fileStream) != NULL)
  {
    inputString[strlen(inputString) - 1] = '\0';
    lineCount++;
    strcpy(stringArray[lineCount - 1], inputString);

    int *letterArray = malloc(250 * sizeof(int));

    for (int i = 0; i < strlen(inputString); i++)
    {
      letterArray[(int)inputString[i]]++;
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

  char diffWord[50];
  char formattedWord[50];
  int newDiffCharIndex = 0;

  for (int x = 0; x < lineCount; x++)
  {
    for (int y = x; y < lineCount; y++)
    {
      int diffChars = 0;
      int diffWordOneIndex = 0;
      int diffWordTwoIndex = 0;
      int diffCharIndex = 0;

      for (int z = 0; z < strlen(stringArray[x]); z++)
      { // O(n^3) hurts me
        if (stringArray[x][z] != stringArray[y][z])
        {
          diffChars++;
          diffCharIndex = z;
          diffWordOneIndex = x;
          diffWordTwoIndex = y;
        }
      }
      if (diffChars == 1)
      {
        printf("Diff Words: %s | %s\n", stringArray[diffWordOneIndex], stringArray[diffWordTwoIndex]);
        strcpy(diffWord, stringArray[diffWordOneIndex]);
        newDiffCharIndex = diffCharIndex;
      }
    }
  }

  int count = 0;
  for (int a = 0; a < strlen(diffWord); a++)
  {
    if (a != newDiffCharIndex)
    {
      formattedWord[count] = diffWord[a];
      count++;
    }
  }

  printf("Doubles: %d  |  Triples: %d\n", doubleCharCount, tripleCharCount);
  printf("New Word: %s\n", formattedWord);

  return 0;
}