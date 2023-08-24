//
//  main.c
//  CS3305_Assignment_4
//
//  Created by Michael Mendes on 2022-11-04.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

struct LinkedList{
    char* process;
    int arrTime;
    int burTime;
    int timeElapsed;
    int turnAround;
    int wait;
    int arrived;
    struct LinkedList *next;
 };

typedef struct LinkedList* node;

node createNode(char* processNum, int arrivalTime, int burstTime, int elapsed){
    node newNode;
    newNode = (node)malloc(sizeof(struct LinkedList));
    newNode->next = NULL;
    newNode->process = processNum;
    newNode->arrTime = arrivalTime;
    newNode->burTime = burstTime;
    newNode->timeElapsed = elapsed;
    return newNode;
}

node addNode(node head, char* processNum, int arrivalTime, int burstTime, int elapsed){
    node newNode,temp;
    newNode = createNode(processNum, arrivalTime, burstTime, elapsed);
    if(head == NULL)
    {
        head = newNode;
    }
    else
    {
        temp = head;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    return head;
}



int main(int argc, const char * argv[]) {
    FILE* file;
    char testCase[MAX];
    int caseNum = 0;
    node head;
    node rQueue;
    char* token1;
    char* token2;
    char* processNum;
    int arrTime, burTime;
    char* qt;
    int numProcesses, processCount, time, turnAround, totalTA, waitTime, totalWait;
    node tempNode;
    
    file = fopen("rr_input.txt", "r");
    
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }
    
    while (fgets(testCase, MAX, file))
    {
        head = NULL;
        caseNum++;
        printf("Test case #%d: %s", caseNum, testCase);
        
        numProcesses = 0;
        token1 = strtok(testCase, " ");
        token2 = strtok(NULL, " ");
        
        while (token2 != NULL)
        {
            numProcesses++;
            processNum = token1;
            arrTime = atoi(token2);
            burTime = atoi(strtok(NULL, " "));
            head = addNode(head, processNum, arrTime, burTime, 0);
            
            token1 = strtok(NULL, " ");
            qt = token1;
            token2 = strtok(NULL, " ");
        }
        printf("Number of Processes: %d, Quantum: %s", numProcesses, qt);
        printf("Process Scheduling Started:\n");
        time = 0;
        totalTA = 0;
        totalWait = 0;
        processCount = numProcesses;
        rQueue = NULL;
        while (processCount > 0)
        {
            printf("CPU Time %d: ", time);
            while (head!= NULL && head->arrTime == time)
            {
                printf("[%s Arrived] ", head->process);
                rQueue = addNode(rQueue, head->process, head->arrTime, head->burTime, head->timeElapsed);
                head = head->next;
            }
            
            if (rQueue != NULL)
            {
                rQueue->timeElapsed += 1;
                printf("%s [%d/%d]\n", rQueue->process, rQueue->timeElapsed, rQueue->burTime);
                
                if (rQueue->timeElapsed == rQueue->burTime)
                {
                    turnAround = time + 1 - rQueue->arrTime;
                    waitTime = turnAround - rQueue->burTime ;
                    totalTA += turnAround;
                    totalWait += waitTime;
                    printf("Process %s completed with Turn Around Time: %d, Waiting Time: %d\n", rQueue->process, turnAround, waitTime);
                    processCount--;
                    rQueue = rQueue->next;
                }
                else if (rQueue->timeElapsed % atoi(qt) == 0)
                {
                    tempNode = rQueue;
                    rQueue = rQueue->next;
                    rQueue = addNode(rQueue, tempNode->process, tempNode->arrTime, tempNode->burTime, tempNode->timeElapsed);
                }
            }
            else
            {
                printf("None\n");
            }
            time++;
        }
        printf("Process scheduling completed with Avg Turn Around Time: %.2f, Avg Waiting Time: %.2f\n\n", (double)totalTA/numProcesses, (double)totalWait/numProcesses);
    }
    fclose(file);
    
    return 0;
}
