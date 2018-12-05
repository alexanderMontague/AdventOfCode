#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct fabric
{
  int xCoord1;
  int yCoord1;
  int xCoord2;
  int yCoord2;
  int area;
} Fabric;

Fabric formatPairing(char *coordinates);

int main()
{
  // Setup Vars
  FILE *fileStream = NULL;
  int lineCount = 0;
  char inputString[100];
  char **inputArray = NULL;

  // File Reading
  fileStream = fopen("./inputs/Dec3-2018.txt", "r");
  while (fgets(inputString, 100, fileStream) != NULL)
  {
    lineCount++;
    inputArray = realloc(inputArray, sizeof(char *) * lineCount);
    inputArray[lineCount - 1] = malloc(sizeof(char) * strlen(inputString) + 1);
    inputString[strlen(inputString) - 1] = '\0';
    strcpy(inputArray[lineCount - 1], inputString);
  }

  // Usage Vars
  int totalSquareInches = 0;
  char *rawCoords = NULL;
  char *rawArea = NULL;

  for (int i = 0; i < lineCount; i++)
  {
    Fabric outerFabric = formatPairing(inputArray[i]);
    for (int j = i; j < lineCount; j++)
    {
      Fabric innerFabric = formatPairing(inputArray[j]);

      if (innerFabric.xCoord1 >= outerFabric.xCoord1 &&
          innerFabric.yCoord1 >= outerFabric.yCoord1 &&
          innerFabric.xCoord2 <= outerFabric.xCoord2 &&
          innerFabric.yCoord2 <= outerFabric.yCoord2)
      {
        // if inner fabric
        totalSquareInches += (outerFabric.area - innerFabric.area);
      }
    }
  }

  printf("Total Square Inches: %d\n", totalSquareInches);

  return 0;
}

Fabric formatPairing(char *rawString)
{
  int startXCoord = 0;
  int startYCoord = 0;
  int addXCoord = 0;
  int addYCoord = 0;
  int endXCoord = 0;
  int endYCoord = 0;

  sscanf(rawString, "#%*d @ %d,%d: %dx%d", &startXCoord, &startYCoord, &addXCoord, &addYCoord);
  endXCoord = startXCoord + addXCoord - 1;
  endYCoord = startYCoord + addYCoord - 1;

  Fabric newFabric;
  newFabric.xCoord1 = startXCoord;
  newFabric.yCoord1 = startYCoord;
  newFabric.xCoord2 = endXCoord;
  newFabric.yCoord2 = endYCoord;
  newFabric.area = addXCoord * addYCoord;

  //printf("Start Coords: (%d, %d)  |  End Coords: (%d, %d)  |  Area: %d\n", newFabric.xCoord1, newFabric.yCoord1, newFabric.xCoord2, newFabric.yCoord2, newFabric.area);
  return newFabric;
}