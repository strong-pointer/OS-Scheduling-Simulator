#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

struct task {
  int
  task_id,           // alphabetic tid is be obtained by 'A'+(task_counter++)
    arrival_time,    // from input file
    service_time,    // from input file
    remaining_time,  // task_id|rst should go down 1 after it's in CPU column
    completion_time, // current time++
    response_time,   // completion_time - arrival_time
    wait_time;       // response_time - service_time
  struct task *next;
};

// Checks if any of the tasks are still needing to run, returns false if not.
bool remainingTasks(struct task* head) {
  for (struct task* curr = head; curr != NULL; curr = curr->next) {
    if (curr->remaining_time != 0) return true;
  }
  return false;
}

/* Used for Debugging Purposes */
void printList(struct task* head) {
  while (head != NULL) {
    printf("%c ", (char)head->task_id);
    head = head->next;
  }
  printf("\n");
}

// Sorts the Tasks by their task_id
void sortByTaskID(struct task *head) {
  struct task *index = NULL, *curr = head;
  // Temporary copying variables
  int ttask_id, tarrival_time, tremaining_time, tcompletion_time, tresponse_time, twait_time, tservice_time;

  while (curr != NULL) {
    index = curr->next;
    while (index != NULL) {
      if (curr->task_id > index->task_id) {
        // Swap the data
        ttask_id = curr->task_id;
        curr->task_id = index->task_id;
        index->task_id = ttask_id;

        tarrival_time = curr->arrival_time;
        curr->arrival_time = index->arrival_time;
        index->arrival_time= tarrival_time;

        tremaining_time = curr->remaining_time;
        curr->remaining_time = index->remaining_time;
        index->remaining_time = tremaining_time;

        tcompletion_time= curr->completion_time;
        curr->completion_time = index->completion_time;
        index->completion_time = tcompletion_time;

        tresponse_time= curr->response_time;
        curr->response_time = index->response_time;
        index->response_time = tresponse_time;

        twait_time = curr->wait_time;
        curr->wait_time = index->wait_time;
        index->wait_time = twait_time;

        tservice_time = curr->service_time;
        curr->service_time = index->service_time;
        index->service_time = tservice_time;
      }
      index = index->next;
    }
    curr = curr->next;
  }
}

// Sorts the Tasks by their service_time
void sortByServiceTime(struct task *head) {
  struct task *index = NULL, *curr = head;
  // Temporary copying variables
  int ttask_id, tarrival_time, tremaining_time, tcompletion_time, tresponse_time, twait_time, tservice_time;

  while (curr != NULL) {
    index = curr->next;
    while (index != NULL) {
      if (curr->service_time > index->service_time || (curr->service_time == index->service_time && curr->wait_time > index->wait_time)) {
        // Swap the data
        ttask_id = curr->task_id;
        curr->task_id = index->task_id;
        index->task_id = ttask_id;

        tarrival_time = curr->arrival_time;
        curr->arrival_time = index->arrival_time;
        index->arrival_time= tarrival_time;

        tremaining_time = curr->remaining_time;
        curr->remaining_time = index->remaining_time;
        index->remaining_time = tremaining_time;

        tcompletion_time= curr->completion_time;
        curr->completion_time = index->completion_time;
        index->completion_time = tcompletion_time;

        tresponse_time= curr->response_time;
        curr->response_time = index->response_time;
        index->response_time = tresponse_time;

        twait_time = curr->wait_time;
        curr->wait_time = index->wait_time;
        index->wait_time = twait_time;

        tservice_time = curr->service_time;
        curr->service_time = index->service_time;
        index->service_time = tservice_time;
      }
      index = index->next;
    }
    curr = curr->next;
  }
}

// Deallocate a linked list when passed in a head node
void deAllocateList(struct task *head) {
  struct task *trav = head, *nextNode = trav->next;
  free(trav);

  while (nextNode != NULL) {
    trav = nextNode;
    nextNode = trav->next;
    free(trav);
  }
}

// argv[1] == choice | 2 == infile | 3 == outfile
int main(int argc, char* argv[]) {
  int task_counter = 0;
  char *choice, *infile, *outfile;
  struct task* head = NULL;
  
  if (argc != 4) {
    printf("ERROR: Incorrect number of arguments detected.\nPlease re-enter arguments in this format: <scheduler> <infile> <outfile>\n");
    // Close the program
    return 1;
  }
  else {
    choice = argv[1];
    infile = argv[2];
    outfile = argv[3];
  }

  // Open the required in and outfile(s)
  FILE *fpIn = fopen(infile, "r");
  FILE *fpOut = fopen(outfile, "w");
  // The temp. read-in integers to put into LL struct
  int arrTime, serTime;
  struct task *trav, *newNode;
  head = (struct task*)malloc(sizeof(struct task));

  // Beginning of reading in for Linked List*****
  fscanf(fpIn, "%d %d", &arrTime, &serTime);
  head->task_id = 'A'+task_counter;
  task_counter++;
  head->arrival_time = arrTime;
  head->service_time = serTime;
  head->remaining_time = serTime;
  head->next = NULL;
    
  trav = head;
  while (fscanf(fpIn, "%d %d", &arrTime, &serTime) != EOF) {
    newNode = (struct task*)malloc(sizeof(struct task));
    newNode->task_id = 'A'+task_counter;
    task_counter++;
    newNode->arrival_time = arrTime;
    newNode->service_time = serTime;
    newNode->remaining_time = serTime;
    newNode->next = NULL;

    trav->next = newNode;
    trav = trav->next;
  }
  fclose(fpIn);
  // End of reading in data for Linked List*****

  // First-In First-Out Selection
  if (strcmp(choice, "-fifo") == 0) {
    int time = 0;
    struct task* curr = head;

    // Final Print Out Headers
    fprintf(fpOut, "FIFO scheduling results\ntime  cpu ready queue (tid/rst)\n");
    fprintf(fpOut, "-------------------------------\n");
    // Loop until reaching the end of the Linked List
    while (curr != NULL) {
      // Sees if this task is during the time
      if (time >= curr->arrival_time) {
        fprintf(fpOut, "%2d    %c%d    ", time, (char)curr->task_id, curr->remaining_time);
        curr->remaining_time--;
        // Checks if it is possible for a queue
        if (curr->next != NULL) {
          // If the next task has an arrival time at or past current time
          if (time >= curr->next->arrival_time) {
            // Loop to check for future tasks to put in ready queue
            for (struct task* nextNode = curr->next; nextNode != NULL; nextNode = nextNode->next) {
              if (time >= nextNode->arrival_time) {
                if (nextNode->task_id != curr->next->task_id) {
                  fprintf(fpOut, ", ");
                }
                fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
              }
            }
            fprintf(fpOut, "\n");
          }
          // No other task in CPU
          else {
            fprintf(fpOut, "--\n");
          }
        }
        // If there are no 'waiting' tasks
        else {
          fprintf(fpOut, "--\n");
        }
        // Checks for more time left in this task
        if (curr->remaining_time <= 0) {
          curr->completion_time = time+1;
          curr->response_time = curr->completion_time - curr->arrival_time;
          curr->wait_time = curr->response_time - curr->service_time;
          // Iterate over to the next node once it has no more time left
          curr = curr->next;
        }
      }
      // No tasks to print for this time segment
      else {
        fprintf(fpOut, "%2d          --\n", time);
      }
      time++;
    }
    // Print table in order of ascending ID's (No sorting needed)
    fprintf(fpOut, "\n      arrival  service completion response wait\n");
    fprintf(fpOut, "tid   time     time    time       time     time\n");
    fprintf(fpOut, "-----------------------------------------------\n");
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, " %c%7d%8d%10d%9d%9d\n", (char)curr->task_id, curr->arrival_time, curr->service_time, curr->completion_time, curr->response_time, curr->wait_time);
    }
    // Print table in order of ascending service time
    fprintf(fpOut, "\nservice wait\n");
    fprintf(fpOut, " time   time\n");
    fprintf(fpOut, "------- ----\n");

    // Sort the Linked List by ascending Service Time
    sortByServiceTime(head);
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, "%3d%7d\n", curr->service_time, curr->wait_time);
    }
    // Close the outfile
    fclose(fpOut);
  }
  // Shortest Job First Selection
  else if (strcmp(choice, "-sjf") == 0) {
    int time = 0;
    struct task* curr = head;
    // Temporary copying variables
    int ttask_id, tarrival_time, tremaining_time, tcompletion_time, tresponse_time, twait_time, tservice_time;

    // Final Print Out Headers
    fprintf(fpOut, "SJF(preemptive) scheduling results\ntime  cpu ready queue (tid/rst)\n");
    fprintf(fpOut, "-------------------------------\n");
    // Loop until reaching the end of the Linked List
    while (curr != NULL) {
      // Sort the tasks and swap them by time remaining
      if (curr != NULL && curr->next != NULL) {
        struct task *nextNode = curr, *index = NULL;
        while (nextNode != NULL) {
          index = nextNode->next;
          while (index != NULL) {
            if (nextNode->remaining_time > index->remaining_time && time >= index->arrival_time) {
              // Swap the data
              ttask_id = nextNode->task_id;
              nextNode->task_id = index->task_id;
              index->task_id = ttask_id;

              tarrival_time = nextNode->arrival_time;
              nextNode->arrival_time = index->arrival_time;
              index->arrival_time = tarrival_time;

              tremaining_time = nextNode->remaining_time;
              nextNode->remaining_time = index->remaining_time;
              index->remaining_time = tremaining_time;

              tcompletion_time= nextNode->completion_time;
              nextNode->completion_time = index->completion_time;
              index->completion_time = tcompletion_time;

              tresponse_time= nextNode->response_time;
              nextNode->response_time = index->response_time;
              index->response_time = tresponse_time;

              twait_time = nextNode->wait_time;
              nextNode->wait_time = index->wait_time;
              index->wait_time = twait_time;

              tservice_time = nextNode->service_time;
              nextNode->service_time = index->service_time;
              index->service_time = tservice_time;
            }
            index = index->next;
          }
          nextNode = nextNode->next;
        }
      }

      // Sees if this task is during the time
      if (time >= curr->arrival_time) {
        fprintf(fpOut, "%2d    %c%d    ", time, (char)curr->task_id, curr->remaining_time);
        curr->remaining_time--;
        // Checks if it is possible for a queue
        if (curr->next != NULL) {
          // If the next task has an arrival time at or past current time
          if (time >= curr->next->arrival_time) {
            // Loop to check for future tasks to put in ready queue
            for (struct task* nextNode = curr->next; nextNode != NULL; nextNode = nextNode->next) {
              if (time >= nextNode->arrival_time) {
                if (nextNode->task_id != curr->next->task_id) {
                  fprintf(fpOut, ", ");
                }
                fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
              }
            }
            fprintf(fpOut, "\n");
          }
          // No other task in CPU
          else {
            fprintf(fpOut, "--\n");
          }
        }
        // If there are no 'waiting' tasks
        else {
          fprintf(fpOut, "--\n");
        }
        // Checks for more time left in this task
        if (curr->remaining_time <= 0) {
          curr->completion_time = time+1;
          curr->response_time = curr->completion_time - curr->arrival_time;
          curr->wait_time = curr->response_time - curr->service_time;
          // Iterate over to the next node once it has no more time left
          curr = curr->next;
        }
      }
      // No tasks to print for this time segment
      else {
        fprintf(fpOut, "%2d          --\n", time);
      }
      time++;      
    }
    // Print table in order of ascending ID's
    fprintf(fpOut, "\n      arrival  service completion response wait\n");
    fprintf(fpOut, "tid   time     time    time       time     time\n");
    fprintf(fpOut, "-----------------------------------------------\n");

    // Sort the Linked List by ascending Task ID
    sortByTaskID(head);
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, " %c%7d%8d%10d%9d%9d\n", (char)curr->task_id, curr->arrival_time, curr->service_time, curr->completion_time, curr->response_time, curr->wait_time);
    }
    // Print table in order of ascending service time
    fprintf(fpOut, "\nservice wait\n");
    fprintf(fpOut, " time   time\n");
    fprintf(fpOut, "------- ----\n");

    // Sort the Linked List by ascending Service Time
    sortByServiceTime(head);
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, "%3d%7d\n", curr->service_time, curr->wait_time);
    }
    // Close the outfile
    fclose(fpOut);
  }
  // Round Robin Selection
  else if (strcmp(choice, "-rr") == 0) {
    int time = 0, count = 0;
    struct task *curr = head, *newHead = head;
    // Temporary copying variables
    int ttask_id, tarrival_time, tremaining_time, tcompletion_time, tresponse_time, twait_time, tservice_time;
    bool noTasks = true;

    // Final Print Out Headers
    fprintf(fpOut, "RR scheduling results (time slice is 1)\ntime  cpu ready queue (tid/rst)\n");
    fprintf(fpOut, "-------------------------------\n");

    // Loop until there arent any remaining tasks
    while (remainingTasks(head)) {
      noTasks = true;
      // Sees if this task is during the time
      for (curr = newHead; curr != NULL; curr = curr->next) {
        if (time >= curr->arrival_time && curr->remaining_time > 0) {
          newHead = curr->next;
          if (newHead == NULL) newHead = head;
          noTasks = false;
          fprintf(fpOut, "%2d    %c%d    ", time, (char)curr->task_id, curr->remaining_time);
          curr->remaining_time--;
          // Find later nodes that may be waiting
          count = 0;
          for (struct task* nextNode = newHead; nextNode != NULL; nextNode = nextNode->next) {
            if (time >= nextNode->arrival_time && nextNode->task_id != curr->task_id && nextNode->remaining_time > 0) {
              if (count >= 1) {
                fprintf(fpOut, ", ");
              }
              count++;
              fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
            }
          }
          for (struct task* nextNode = head; nextNode != newHead; nextNode = nextNode->next) {
            if (time >= nextNode->arrival_time && nextNode->task_id != curr->task_id && nextNode->remaining_time > 0) {
              if (count >= 1) {
                fprintf(fpOut, ", ");
              }
              count++;
              fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
            }
          }
          if (count == 0) fprintf(fpOut, "--");
          fprintf(fpOut, "\n");
            
          // Checks for more time left in this task
          if (curr->remaining_time <= 0) {
            curr->completion_time = time+1;
            curr->response_time = curr->completion_time - curr->arrival_time;
            curr->wait_time = curr->response_time - curr->service_time;
          }
          break;
        }
      }

      for (curr = head; curr != newHead && noTasks == true; curr = curr->next) {
        if (time >= curr->arrival_time && curr->remaining_time > 0) {
          newHead = curr->next;
          if (newHead == NULL) newHead = head;
          noTasks = false;
          fprintf(fpOut, "%2d    %c%d    ", time, (char)curr->task_id, curr->remaining_time);
          curr->remaining_time--;
          // Find later nodes that may be waiting
          count = 0;
          for (struct task* nextNode = newHead; nextNode != NULL; nextNode = nextNode->next) {
            if (time >= nextNode->arrival_time && nextNode->task_id != curr->task_id && nextNode->remaining_time > 0) {
              if (count >= 1) {
                fprintf(fpOut, ", ");
              }
              count++;
              fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
            }
          }
          for (struct task* nextNode = head; nextNode != newHead; nextNode = nextNode->next) {
            if (time >= nextNode->arrival_time && nextNode->task_id != curr->task_id && nextNode->remaining_time > 0) {
              if (count >= 1) {
                fprintf(fpOut, ", ");
              }
              count++;
              fprintf(fpOut, "%c%d", (char)nextNode->task_id, nextNode->remaining_time);
            }
          }
          
          if (count == 0) fprintf(fpOut, "--");
          fprintf(fpOut, "\n");
          // Checks for more time left in this task
          if (curr->remaining_time <= 0) {
            curr->completion_time = time+1;
            curr->response_time = curr->completion_time - curr->arrival_time;
            curr->wait_time = curr->response_time - curr->service_time;
          }
          break;
        }
      }

      if (noTasks) {
       fprintf(fpOut, "%2d          --\n", time);
       newHead = head;
      }
      time++;
    }
    
    // Print table in order of ascending ID's
    fprintf(fpOut, "\n      arrival  service completion response wait\n");
    fprintf(fpOut, "tid   time     time    time       time     time\n");
    fprintf(fpOut, "-----------------------------------------------\n");

    // Sort the Linked List by ascending Task ID
    sortByTaskID(head);
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, " %c%7d%8d%10d%9d%9d\n", (char)curr->task_id, curr->arrival_time, curr->service_time, curr->completion_time, curr->response_time, curr->wait_time);
    }
    // Print table in order of ascending service time
    fprintf(fpOut, "\nservice wait\n");
    fprintf(fpOut, " time   time\n");
    fprintf(fpOut, "------- ----\n");

    // Sort the Linked List by ascending Service Time
    sortByServiceTime(head);
    for (curr = head; curr != NULL; curr = curr->next) {
      fprintf(fpOut, "%3d%7d\n", curr->service_time, curr->wait_time);
    }
    // Close the outfile
    fclose(fpOut);
  }
  // Not a proper scheduling selection
  else {
    printf("Not a proper selection for scheduling!\nExiting the Program!\n");
    // Close the outfile & free the allocated linked list memory
    fclose(fpOut);
    deAllocateList(head);
    return 1;
  }

  // Free the linked list from memory
  deAllocateList(head);
  
  return 0;
}
