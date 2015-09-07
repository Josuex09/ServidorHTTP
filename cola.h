//
// Created by josue on 9/2/15.
//

#ifndef SERVIDORHTTP_COLA_H
#define SERVIDORHTTP_COLA_H


struct nodo {
    int fd;
    struct nodo *ptr;
}*front,*rear,*temp,*front1;

int count = 0;

/* Create an empty queue */
void create() {
    front = rear = NULL;
}

/* Returns queue size */
int queuesize() {
    return count;
}

/* Enqueing the queue */
void enq(int data) {
    if (rear == NULL)
    {
        rear = (struct nodo *)malloc(1*sizeof(struct nodo));
        rear->ptr = NULL;
        rear->fd = data;
        front = rear;
    }
    else
    {
        temp=(struct nodo *)malloc(1*sizeof(struct nodo));
        rear->ptr = temp;
        temp->fd = data;
        temp->ptr = NULL;

        rear = temp;
    }
    count++;
}

/* Displaying the queue elements */
void display() {
    front1 = front;

    if ((front1 == NULL) && (rear == NULL))
    {
        printf("Queue is empty");
        return;
    }
    while (front1 != rear)
    {
        printf("%d ", front1->fd);
        front1 = front1->ptr;
    }
    if (front1 == rear)
        printf("%d", front1->fd);
}

/* Dequeing the queue */
int deq() {
    front1 = front;
    int value;

    if (front1 == NULL) {
        printf("\n Error: Trying to display elements from empty queue");
        return -1;
    }
    else
    if (front1->ptr != NULL) {
        front1 = front1->ptr;
        //printf("\n Dequed value : %d", front->fd);
        value = front->fd;
        free(front);
        front = front1;
    }
    else {
        //printf("\n Dequed value : %d", front->fd);
        value = front->fd;
        free(front);
        front = NULL;
        rear = NULL;
    }
    count--;
    return value;
}

/* Returns the front element of queue */
int frontelement() {
    if ((front != NULL) && (rear != NULL))
        return(front->fd);
    else
        return 0;
}

/* Display if queue is empty or not */
int empty() {
    if ((front == NULL) && (rear == NULL))
        return 1;
    else
        return 0;
}
#endif //SERVIDORHTTP_COLA_H
