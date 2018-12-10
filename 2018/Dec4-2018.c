#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.c"

typedef struct
{
  int month;
  int day;
  int hour;
  int minute;
  char identifier[50];
} GuardData;

typedef struct
{
  int identifier;
  int minutesSlept;
  int minuteSleptArray[60];
} SleepData;

char *printGuardData(void *guardData);
int compareDate(const void *dataOne, const void *dataTwo);

int main()
{
  // Setup Vars
  FILE *fileStream = NULL;
  int lineCount = 0;
  char inputString[100];
  char **inputArray = NULL;

  List sortedList = initializeList(&printGuardData, NULL, &compareDate);
  List sleepInfoList = initializeList(NULL, NULL, NULL);

  // File Reading
  //fileStream = fopen("./inputs/Dec4-2018.txt", "r");
  fileStream = fopen("./inputs/DecTEST.txt", "r");
  while (fgets(inputString, 100, fileStream) != NULL)
  {
    lineCount++;
    inputArray = realloc(inputArray, sizeof(char *) * lineCount);
    inputArray[lineCount - 1] = malloc(sizeof(char) * strlen(inputString) + 1);
    inputString[strlen(inputString) - 1] = '\0';

    int month, day, hour, minute;
    char guardID[50];
    GuardData *newGuard = NULL;
    newGuard = malloc(sizeof(GuardData));

    sscanf(inputString, "[%*d-%d-%d %d:%d] %*s %s", &month, &day, &hour, &minute, guardID);
    newGuard->month = month;
    newGuard->day = day;
    newGuard->hour = hour;
    newGuard->minute = minute;
    strcpy(newGuard->identifier, guardID);

    insertSorted(&sortedList, newGuard);
  }

  toString(sortedList);

  GuardData *guardInfo = NULL;
  GuardData *nextGuardInfo = NULL;
  ListIterator guardItr = createIterator(sortedList);
  int tempMinsSlept = 0;
  int tempGuardID = 0;

  while ((guardInfo = nextElement(&guardItr)) != NULL)
  {

    if (guardInfo->identifier[0] == '#') // if guard identifier
    {
      sscanf(guardInfo->identifier, "#%d", &tempGuardID);
    }

    bool foundInList = false;
    SleepData *sleepGuard = NULL; // the new guard data being added
    ListIterator sleepItr = createIterator(sleepInfoList);
    while ((sleepGuard = nextElement(&sleepItr)) != NULL)
    {
      if (sleepGuard->identifier == tempGuardID) // if guardID is already in list
      {
        // do stuff

        foundInList = true;
        break;
      }
    }

    if (!foundInList)
    {
      sleepGuard = malloc(sizeof(SleepData));
      sleepGuard->identifier = tempGuardID;
      GuardData *firstAction;
      GuardData *secondAction;
      while ((firstAction = nextElement(&guardItr)) != NULL || (firstAction = nextElement(&guardItr))->identifier[0] != '#')
      {
        secondAction = nextElement(&guardItr);
        printf("%s %s\n", firstAction->identifier, secondAction->identifier);
      }

      //insertBack(&sleepInfoList, sleepGuard);
    }

    // else
    // {
    //   nextGuardInfo = nextElement(&guardItr);
    //   tempMinsSlept = guardInfo->
    // }
  }

  //toString(sortedList);
  return 0;
}

int compareDate(const void *dataOne, const void *dataTwo)
{ // -1 one < two  :  1 one > two  :  0 one == two
  GuardData *guardOne = (GuardData *)dataOne;
  GuardData *guardTwo = (GuardData *)dataTwo;

  if (guardOne->month < guardTwo->month)
  {
    return -1;
  }
  else if (guardOne->month > guardTwo->month)
  {
    return 1;
  }
  else if (guardOne->month == guardTwo->month)
  {
    if (guardOne->day < guardTwo->day)
    {
      return -1;
    }
    else if (guardOne->day > guardTwo->day)
    {
      return 1;
    }
    else if (guardOne->day == guardTwo->day)
    {
      if (guardOne->hour < guardTwo->hour)
      { // 23 < 00 - 11:00 vs 12:00
        return -1;
      }
      else if (guardOne->hour > guardTwo->hour)
      {
        return 1;
      }
      else if (guardOne->hour == guardTwo->hour)
      {
        if (guardOne->minute < guardTwo->minute)
        {
          return -1;
        }
        else if (guardOne->minute > guardTwo->minute)
        {
          return 1;
        }
        else
        {
          printf("Well...\n");
        }
      }
    }
  }
  return 0;
}

char *printGuardData(void *guardData)
{
  GuardData *printGuard = (GuardData *)guardData;
  printf("Month:%d | Day:%d | Hour:%d | Min:%d | Identifier: %s\n", printGuard->month, printGuard->day, printGuard->hour, printGuard->minute, printGuard->identifier);

  return NULL;
}