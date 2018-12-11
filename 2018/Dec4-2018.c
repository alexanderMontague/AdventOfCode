#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.c"

char *printGuardData(void *guardData);
char *printSleepData(void *sleepData);
int compareDate(const void *dataOne, const void *dataTwo);

int main()
{
  // Setup Vars
  FILE *fileStream = NULL;
  int lineCount = 0;
  char inputString[100];
  char **inputArray = NULL;

  List sortedList = initializeList(&printGuardData, NULL, &compareDate);
  List sleepInfoList = initializeList(&printSleepData, NULL, NULL);

  // File Reading
  fileStream = fopen("./inputs/Dec4-2018.txt", "r");
  //fileStream = fopen("./inputs/DecTEST.txt", "r");
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

  //toString(sortedList);

  GuardData *guardInfo = NULL;
  GuardData *tempNextNode = NULL;
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
        GuardData *firstAction = NULL;
        GuardData *secondAction = NULL;
        do
        {
          firstAction = nextElement(&guardItr);
          secondAction = nextElement(&guardItr);

          // Calculate minutes slept
          int minsAsleep = secondAction->minute - firstAction->minute;
          sleepGuard->minutesSlept += minsAsleep;

          // Add counters for each minute slept
          for (int j = secondAction->minute - 1; j >= firstAction->minute; j--)
          {
            sleepGuard->minuteSleptArray[j]++;
          }

          tempNextNode = getNextNodeData(&guardItr);
        } while (tempNextNode != NULL && tempNextNode->identifier[0] != '#');

        foundInList = true;
        break;
      }
    }

    if (!foundInList) // if went through whole list and didnt find anything
    {
      sleepGuard = malloc(sizeof(SleepData));
      sleepGuard->identifier = tempGuardID;
      for (int i = 0; i < 60; i++)
      { // initialize array to 0;
        sleepGuard->minuteSleptArray[i] = 0;
      }
      GuardData *firstAction = NULL;
      GuardData *secondAction = NULL;
      do
      {
        firstAction = nextElement(&guardItr);
        secondAction = nextElement(&guardItr);

        // Calculate minutes slept
        int minsAsleep = secondAction->minute - firstAction->minute;
        sleepGuard->minutesSlept += minsAsleep;

        // Add counters for each minute slept
        for (int j = secondAction->minute - 1; j >= firstAction->minute; j--)
        {
          sleepGuard->minuteSleptArray[j]++;
        }

        tempNextNode = getNextNodeData(&guardItr);
      } while (tempNextNode != NULL && tempNextNode->identifier[0] != '#');

      insertBack(&sleepInfoList, sleepGuard);
    }
  }

  //toString(sleepInfoList);

  ListIterator finalItr = createIterator(sleepInfoList);
  SleepData *sleepObject = NULL;
  int mostMinsSleptID = 0;
  int totalMinsSlept = 0;
  while ((sleepObject = nextElement(&finalItr)) != NULL)
  {
    if (sleepObject->minutesSlept > totalMinsSlept)
    {
      mostMinsSleptID = sleepObject->identifier;
      totalMinsSlept = sleepObject->minutesSlept;
    }
  }

  printf("Most Mins Slept ID: %d  |  Total Mins Slept: %d\n", mostMinsSleptID, totalMinsSlept);

  ListIterator finalItr2 = createIterator(sleepInfoList);
  SleepData *sleepObject2 = NULL;
  int highestSleptMinute = 0;
  int highestSleptMinuteIndex = 0;
  while ((sleepObject2 = nextElement(&finalItr2)) != NULL)
  {
    if (sleepObject2->identifier == mostMinsSleptID)
    {
      for (int i = 0; i < 60; i++)
      {
        if (sleepObject2->minuteSleptArray[i] > highestSleptMinute)
        {
          highestSleptMinuteIndex = i;
          //highestSleptMinute = sleepObject2->minuteSleptArray[i];
        }
      }
    }
  }

  printf("Minute Most Slept: %d\n", highestSleptMinuteIndex);

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
      if (guardOne->hour > guardTwo->hour)
      { // 23 > 00 - 11:00 vs 12:00
        return -1;
      }
      else if (guardOne->hour < guardTwo->hour)
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

char *printSleepData(void *sleepData)
{
  SleepData *printSleep = (SleepData *)sleepData;
  printf("Identifier: %d  |  Mins Slept: %d\n", printSleep->identifier, printSleep->minutesSlept);
  for (int i = 0; i < 60; i++)
  {
    printf("Minute: %d | Times Slept: %d\n", i, printSleep->minuteSleptArray[i]);
  }
  printf("\n\n");

  return NULL;
}