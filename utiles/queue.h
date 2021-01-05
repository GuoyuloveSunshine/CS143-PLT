#ifndef QUEUE
#define QUEUE
#include <stdlib.h>
#include <memory.h>

#define TRUE 1
#define FALSE 0

typedef struct QueueNode {
    void *pData;
    int iNumBytes;
    struct QueueNode *pPrev;
    struct QueueNode *pNext;
} QueueNode;

typedef struct CQueue {
    int iLength;
    struct QueueNode *pHead;
    struct QueueNode *pTail;
} CQueue;


CQueue *cqueue_create() {
    CQueue *queue;
    queue = (CQueue *) malloc(sizeof(CQueue));
    if (NULL == queue) return NULL;
    queue->pHead = NULL;
    queue->pTail = NULL;

    queue->iLength = 0;
    return queue;
}

int cqueue_init(CQueue * queue) {
    queue->pHead = NULL;
    queue->pTail = NULL;

    queue->iLength = 0;
    return TRUE;
}

int cqueue_is_empty(CQueue *queue) {
    if (queue->pHead == NULL && queue->pTail == NULL) return TRUE;
    else return FALSE;
}

int cqueue_push_back(CQueue *queue, void *pData, int iNumBytes) {
    // enqueue pData at the rear end

    // allocate memory
    QueueNode *pNode;
    pNode = (QueueNode *) malloc(sizeof(QueueNode));
    if (pNode == NULL) return FALSE;
    pNode->pData = malloc(iNumBytes);
    if (pNode->pData == NULL) return FALSE;
    memcpy(pNode->pData, pData, iNumBytes);
    pNode->iNumBytes = iNumBytes;

    // join pNode in the queue 
    pNode->pNext = pNode;
    if (queue->pTail != NULL) {
        pNode->pPrev = queue->pTail;
    } else {
        pNode->pPrev = pNode;
    }

    // modify the queue
    if (queue->pTail == NULL){
        queue->pTail = pNode;
    } else {
        queue->pTail->pNext = pNode;
    }
    queue->pTail = pNode;
    if (queue->pHead == NULL) {
        queue->pHead = pNode;
    }

    queue->iLength += 1;
    return TRUE;
}

int cqueue_pop_front(CQueue *queue, void *pData) {
    // deque the data(ptr) at the front

    if (cqueue_is_empty(queue)) {
        return FALSE;
    }

    if (pData != NULL) {
        memcpy(pData, queue->pHead->pData, queue->pHead->iNumBytes);
    }

    QueueNode * pNodeTmp = queue->pHead;
    //modify the queue
    if (queue->pHead != queue->pTail) {
        queue->pHead = queue->pHead->pNext;
        queue->pHead->pPrev = queue->pHead;
    } else {
        queue->pHead = queue->pTail = NULL;
    }

    // free the node
    free(pNodeTmp->pData);
    free(pNodeTmp);

    queue->iLength -= 1;
    return TRUE;
}


void cqueue_destroy(CQueue *queue) {
    // applies to a dynamically created queue
    while (queue->pHead != NULL) {
        cqueue_pop_front(queue, NULL);
    }
    free(queue);
}

void cqueue_clear(CQueue *queue) {
    // applies to a dynamically created queue or defined queue
    while (queue->pHead != NULL) {
        cqueue_pop_front(queue, NULL);
    }
    queue->iLength = 0;
}

QueueNode *cqueue_next(QueueNode * pNode){
    QueueNode * pRet = pNode->pNext;
    if (pRet == pNode) {
        // end of the queue
        return NULL;
    } else {
        return pRet;
    }
}

QueueNode *cqueue_prev(QueueNode * pNode){
    QueueNode * pRet = pNode->pPrev;
    if (pRet == pNode) {
        //end of the queue
        return NULL;
    } else {
        return pRet;
    }
}

#endif