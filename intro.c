#include "bool.h"

struct singly_liked_list {
    int value;
    struct singly_liked_list * next_ptr;
};

struct record {
    int fkey;
    int next;
};

struct indexes_array_list {
    int first;
    int free;
    int size;
    struct record * buffer;
};

Boolean sum_array(int * A, int N, int limit, int M, int ** V, int * size) {
    int i, j, k, sum;

    *size = (int) N/M;

    *V = (int *)malloc(sizeof(int)*(*size));
    if(*V == NULL)
        return FALSE;
    for(i=0, j=0; i<N; i=i+M) {
        if(i+M <= N) {
            for(k=i, sum=0; k<i+M; k++) {
                sum += A[k];
            }
            if(sum <= limit) {
                (*V)[j++] = sum;
            } else {
                (*V)[j++] = 0;
            }
        } else {
            break;
        }
    }
    printf("\nThe sum is:\n");
    for(k = 0; k < *size; k++) {
        printf("%d", (*V)[k]);
    }
    return TRUE;
}

Boolean array_eval(int * A, int index, int cut, Boolean ** Evaluation) {
    int i, j, k, size;
    Boolean found;

    size =  index - cut;

    *Evaluation = (Boolean *)malloc(sizeof(Boolean)*(size));
    if(*Evaluation == NULL)
        return FALSE;

    for(i=cut, k=0; i<index; i++) {
        for(j=0; j<cut; j++) {
            found = FALSE;
            if(A[i] == A[j]) {
                (*Evaluation)[k++] = TRUE;
                found = TRUE;
                break;
            }
        }
        if(found == FALSE) {
            (*Evaluation)[k++] = FALSE;
        }
    }
    printf("\nThis is the result:\n");
    for(k = 0; k < size; k++) {
        printf("%d", (*Evaluation)[k]);
    }
    return TRUE;
}

/** Singly linked lists **/

void initSLL(struct singly_liked_list ** listPointer) {
    *listPointer = NULL;
}

void pre_insertSLL(struct singly_liked_list ** ptrptr, int v) {
    struct singly_liked_list * tmp_ptr;

    tmp_ptr = *ptrptr;
    *ptrptr = (struct singly_liked_list *)malloc(sizeof(struct singly_liked_list));
    (*ptrptr)->value = v;
    (*ptrptr)->next_ptr = tmp_ptr;
}

void suf_insertSLL(struct singly_liked_list ** ptrptr, int v) {
    while(*ptrptr != NULL)
        ptrptr = &((*ptrptr)->next_ptr);
    pre_insertSLL(ptrptr, v);
}

void ord_insertSLL(struct singly_liked_list ** ptrptr, int v) {
    while(*ptrptr != NULL && (*ptrptr)->value<v)
        ptrptr = &((*ptrptr)->next_ptr);
    pre_insertSLL(ptrptr, v);
}

Boolean searchSLL(struct singly_liked_list * ptr, int v) {
    Boolean found = FALSE;
    while(ptr != NULL) {
        if(ptr->value == v) {
            found = TRUE;
            //*ptrptr = ptr;
            break;
        }
        ptr = ptr->next_ptr;
    }
    return found;
}

void insert_elements(int * array, int length, struct singly_liked_list ** ptrptr) {
    int i;
    for(i=0; i<length; i++) {
        ord_insertSLL(ptrptr, array[i]);
    }
}

void visitSLL(struct singly_liked_list * ptr) {
    int i = 0;
    while(ptr != NULL) {
        printf("\nValue[%d]: %d", i, ptr->value);
        ptr = ptr->next_ptr;
        i++;
    }
    printf("\n");
}

void intersection_set(struct singly_liked_list * ptr1, struct singly_liked_list * ptr2, struct singly_liked_list ** ptrptr) {

    initSLL(ptrptr);
    while(ptr1 != NULL) {
        if(searchSLL(ptr2, ptr1->value) == TRUE)
            ord_insertSLL(ptrptr, ptr1->value);
        ptr1 = ptr1->next_ptr;
    }
}

void union_set(struct singly_liked_list * ptr1, struct singly_liked_list * ptr2, struct singly_liked_list ** ptrptr) {

    initSLL(ptrptr);
    while(ptr1 != NULL) {
        ord_insertSLL(ptrptr, ptr1->value);
        ptr1 = ptr1->next_ptr;
    }

    while(ptr2 != NULL) {
        int value = ptr2->value;
        if(searchSLL(*ptrptr, ptr2->value) == FALSE)
            ord_insertSLL(ptrptr, ptr2->value);
        ptr2 = ptr2->next_ptr;
    }
}

void XoR_set(struct singly_liked_list * ptr1, struct singly_liked_list * ptr2, struct singly_liked_list ** ptrptr) {

    initSLL(ptrptr);

    struct list * tmp_ptr = ptr1;
    while(ptr1 != NULL) {
        if(searchSLL(ptr2, ptr1->value) == FALSE)
            ord_insertSLL(ptrptr, ptr1->value);
        ptr1 = ptr1->next_ptr;
    }

    while(ptr2 != NULL) {
        if(searchSLL(tmp_ptr, ptr2->value) == FALSE)
            ord_insertSLL(ptrptr, ptr2->value);
        ptr2 = ptr2->next_ptr;
    }
}

/** Indexes-array linked lists **/

void initIALS(struct indexes_array_list * ptr, int size) {

    int count;

    ptr->buffer = (struct record *)malloc(size*(sizeof(struct record)));
    ptr->size = size;
    ptr->first = ptr->size;
    ptr->free = 0;
    for(count=1; count<size+1; count++)
        ptr->buffer[count-1].next = count;
}

void visitIASL(struct indexes_array_list * ptr) {

    int position, i=0;

    position = ptr->first;
    while(position != ptr->size) {
        printf("\nKey[%d]: %d", i, ptr->buffer[position].fkey);
        position = ptr->buffer[position].next;
        i++;
    }
    printf("\n");
}

Boolean pre_insertIASL(struct indexes_array_list * ptr, int value) {

    if(ptr->free != ptr->size) {
        int moved;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;

        ptr->buffer[moved].fkey = value;
        ptr->buffer[moved].next = ptr->first;
        ptr->first = moved;

        return TRUE;
    }
    return FALSE;
}

Boolean suf_insertIASL(struct indexes_array_list * ptr, int v) {

    if(ptr->free != ptr->size) {
        int moved, * position_ptr;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;
        position_ptr = &ptr->first;

        while (*position_ptr != ptr->size) {
            position_ptr = &(ptr->buffer[*position_ptr].next);
        }
        *position_ptr = moved;
        ptr->buffer[moved].fkey = v;
        ptr->buffer[moved].next = ptr->size;

        return TRUE;
    }
    return FALSE;
}

/** Es. 2.7.19 **/

void fromIASLtoSLL(struct indexes_array_list * W, struct singly_liked_list ** doublePointer, int key) {

    initSLL(doublePointer);
    int count=0;

    int position;
    position = W->first;
    while(position != W->size) {
        if(W->buffer[position].fkey == key)
            suf_insertSLL(doublePointer, count);
        position = W->buffer[position].next;
        count++;
    }
}

/** ------------------------ **/

int intro() {

    int A[] = {4, 2, 1, 2, 4, 5, 3};
    int limit = 6, N = 7, M = 2, size, *ptr;
    int index = 7, cut = 3;
    Boolean *ptrEvaluation;

    struct singly_liked_list *listPointer1;
    int L[] = {31, 19, 56, 44};
    struct singly_liked_list *listPointer2;
    int O[] = {23, 75, 56, 13, 44, 99};

    struct singly_liked_list *intersectionPointer;
    struct singly_liked_list *unionPointer;
    struct singly_liked_list *XoRPointer;

    struct indexes_array_list List1;
    struct indexes_array_list *IAListPointer1 = &List1;
    int sizeIASL = 10;
    int key = 7;
    struct singly_liked_list *fromIASLtoSLLPointer;

    /* ------------- */

    sum_array(&A, N, limit, M, &ptr, &size);
    printf("\n");
    array_eval(&A, index, cut, &ptrEvaluation);
    printf("\n");

    /** Singly linked lists **/
    initSLL(&listPointer1);
    pre_insertSLL(&listPointer1, 11);
    suf_insertSLL(&listPointer1, 23);
    insert_elements(&L, (sizeof L / sizeof L[0]), &listPointer1);
    printf("\n>>> List 1 <<<\n");
    visitSLL(&(*listPointer1));

    initSLL(&listPointer2);
    insert_elements(&O, (sizeof O / sizeof O[0]), &listPointer2);
    printf("\n>>> List 2 <<<\n");
    visitSLL(&(*listPointer2));

    intersection_set(&(*listPointer1), &(*listPointer2), &intersectionPointer);
    printf("\n>>> Intersection <<<\n");
    visitSLL(&(*intersectionPointer));

    union_set(&(*listPointer1), &(*listPointer2), &unionPointer);
    printf("\n>>> Union <<<\n");
    visitSLL(&(*unionPointer));

    XoR_set(&(*listPointer1), &(*listPointer2), &XoRPointer);
    printf("\n>>> Exclusive_Or <<<\n");
    visitSLL(&(*XoRPointer));

    /** Indexes-array linked lists **/
    initIALS(IAListPointer1, sizeIASL);
    pre_insertIASL(IAListPointer1, 7);
    pre_insertIASL(IAListPointer1, 11);
    pre_insertIASL(IAListPointer1, 7);
    pre_insertIASL(IAListPointer1, 9);
    suf_insertIASL(IAListPointer1, 13);
    visitIASL(IAListPointer1);
    fromIASLtoSLL(IAListPointer1, &fromIASLtoSLLPointer, key);
    printf("\n>>> From indexes_array_list to singly_linked_list <<<\n");
    visitSLL(&(*fromIASLtoSLLPointer));

}