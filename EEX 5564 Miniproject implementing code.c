#include <stdio.h>
#include <stdlib.h>

// Process structure
typedef struct {
    int process_id;
    int priority;
    int burst_time;
    int waiting_time;
} Process;

// Queue structure
typedef struct {
    Process** processes;
    int front, rear, size;
} Queue;

// Function to create a process
Process* createProcess(int process_id, int priority, int burst_time) {
    Process* process = (Process*)malloc(sizeof(Process));
    process->process_id = process_id;
    process->priority = priority;
    process->burst_time = burst_time;
    process->waiting_time = 0;
    return process;
}

// Function to create a queue
Queue* createQueue(int size) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->processes = (Process**)malloc(size * sizeof(Process*));
    queue->front = queue->rear = -1;
    queue->size = size;
    return queue;
}

// Function to check if a queue is empty
int isEmpty(Queue* queue) {
    return queue->front == -1;
}

// Function to enqueue a process
void enqueue(Queue* queue, Process* process) {
    if (queue->rear == queue->size - 1) {
        printf("Queue is full\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = queue->rear = 0;
    } else {
        queue->rear++;
    }
    queue->processes[queue->rear] = process;
}

// Function to dequeue a process
Process* dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    Process* process = queue->processes[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front++;
    }
    return process;
}

// Function to age the processes in a queue
void ageProcesses(Queue* queue) {
    for (int i = queue->front; i <= queue->rear; i++) {
        queue->processes[i]->waiting_time++;
    }
}

// Function to simulate MLQ CPU scheduling
void MLQScheduler(Queue** queues, int num_queues, int* time_quantum) {
    int currentTime = 0;
    while (1) {
        for (int i = 0; i < num_queues; i++) {
            if (!isEmpty(queues[i])) {
                Process* currentProcess = dequeue(queues[i]);
                printf("Executing process %d from Queue %d at time %d\n", currentProcess->process_id, i, currentTime);
                currentProcess->burst_time -= time_quantum[i];
                ageProcesses(queues[i]);
                currentTime += time_quantum[i];

                // Check if the process is completed
                if (currentProcess->burst_time <= 0) {
                    free(currentProcess);
                } else {
                    enqueue(queues[i], currentProcess);
                }
            }
        }
    }
}

int main() {
    // Example: Creating 3 queues with different time quantum values
    int num_queues = 3;
    int time_quantum[] = {20, 40, 80};

    Queue** queues = (Queue**)malloc(num_queues * sizeof(Queue*));

    for (int i = 0; i < num_queues; i++) {
        queues[i] = createQueue(10);  // Assuming a maximum of 10 processes in each queue
    }

    // Example: Enqueue processes with different priorities
    enqueue(queues[0], createProcess(1, 0, 100));
    enqueue(queues[1], createProcess(2, 1, 120));
    enqueue(queues[2], createProcess(3, 2, 80));

    // Run the MLQ scheduling simulation
    MLQScheduler(queues, num_queues, time_quantum);

    // Free allocated memory
    for (int i = 0; i < num_queues; i++) {
        free(queues[i]->processes);
        free(queues[i]);
    }
    free(queues);

    return 0;
}
