#include "bool.h"

struct sequential_list {
    int head;
    int tail;
    int size;
    int * buffer;
};

struct float_sequential_list {
    int head;
    int tail;
    int size;
    float * buffer;
};

struct record {
    float number;
    int next;
};

struct indexes_array_list {
    int first;
    int free;
    int size;
    struct record * buffer;
};

struct int_record {
    int number;
    int next;
};

struct int_indexes_array_list {
    int first;
    int free;
    int size;
    struct int_record * buffer;
};

struct pointer_list {
    int value;
    struct pointer_list * next_ptr;
};

/** 2022-01-11 **/

/* Ex. 1 */

void init_sequential_list(struct sequential_list * ptr, int size) {
    ptr->size = size+1;
    ptr->head = 0;
    ptr->tail = 0;
    ptr->buffer = (int *)malloc(size*(sizeof(int)));
}

int suf_insertSL(struct sequential_list * ptr, int value) {
    if ((ptr->tail+1)%ptr->size != ptr->head) {
        ptr->buffer[ptr->tail] = value;
        ptr->tail = (ptr->tail+1)%ptr->size;
    }
}

void pre_insertSL(struct sequential_list * ptr, int value) {
    if ((ptr->tail+1)%ptr->size != ptr->head) {
        ptr->buffer[(ptr->head-1+ptr->size)%ptr->size] = value;
        ptr->head = (ptr->head-1+ptr->size)%ptr->size;
    }
}

Boolean remove_from_list(struct sequential_list * ptr, int position) {
    int count, stop, insert;
    insert = position;
    stop = (ptr->tail+ptr->size == ptr->size) ? (ptr->size-1) : (ptr->tail-1);
    if((ptr->tail-1+ptr->size)%ptr->size != ptr->head) {
        for (count=position; count<stop; count++) {
            ptr->buffer[insert] = ptr->buffer[(insert + 1) % ptr->size];
            insert = (insert+1) % ptr->size;
        }
        ptr->tail = (ptr->tail-1+ptr->size)%ptr->size;
        return TRUE;
    }
    return FALSE;
}

int remove_indexes(struct sequential_list * ptr, int L, int V[], int N, int A[10]) {
    int count_removed=0, i, j, k, before, M;

    init_sequential_list(ptr, L);
    for(i=0; i<L; i++) {
        suf_insertSL(ptr, A[i]);
    }

    M=(ptr->tail-ptr->head<=0) ? (ptr->tail-ptr->head+ptr->size) : (ptr->tail-ptr->head);
    for(j=0; j<N; j++) {
        before = 0;
        if(V[j]>=0 && V[j]<M) {
            for(k=0; k<j; k++) {
                if(V[k]>=0 && V[k]<V[j])
                    before++;
            }
            if(remove_from_list(ptr, (before == 0) ? (V[j]) : (V[j]-before)))
                count_removed ++;
        }
    }
    return count_removed;
}

/* Ex. 2 */

void float_initIASL(struct indexes_array_list * ptr, int size) {

    int count;

    ptr->buffer = (struct record *)malloc(size*(sizeof(struct record)));
    ptr->size = size;
    ptr->first = ptr->size;
    ptr->free = 0;
    for(count=1; count<size+1; count++) {
        ptr->buffer[count - 1].next = count;
    }
}

void float_visitIASL(struct indexes_array_list * ptr) {

    int position, i=0;

    position = ptr->first;
    while(position != ptr->size) {
        printf("\nFloat[%d]: %.1f", i, ptr->buffer[position].number);
        printf(" | Index: %d", ptr->buffer[position].next);
        position = ptr->buffer[position].next;
        i++;
    }
    printf("\n");
}

Boolean float_pre_insertIASL(struct indexes_array_list * ptr, float value) {

    if(ptr->free != ptr->size) {
        int moved;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;

        ptr->buffer[moved].number = value;
        ptr->buffer[moved].next = ptr->first;
        ptr->first = moved;

        return TRUE;
    }
    return FALSE;
}

Boolean is_equal(float a, float b, float error) {
    float diff, largest;
    diff = fabs(a - b);
    a = fabs(a);
    b = fabs(b);
    largest = (a > b) ? a : b;
    if(diff <= error*largest)
        return TRUE;
    else
        return FALSE;
}

Boolean move_element_to_tail(struct indexes_array_list * ptr, float target, int size, float B[10], float error) {
    int * position, i, * tail, toTail;
    Boolean found = FALSE;

    float_initIASL(ptr, size);

    for(i=size-1; i>=0; i--) {
        float_pre_insertIASL(ptr, B[i]);
    }

    tail = &ptr->first;
    while(*tail != ptr->size) {
        tail = &(ptr->buffer[*tail].next);
    }

    position = &ptr->first;
    i = 0;
    while(*position != ptr->size && i<size) {
        if(is_equal(target, ptr->buffer[*position].number, error)) {
            found = TRUE;
            toTail = *position;
            *position = ptr->buffer[*position].next; //index moved one position ahead

            *tail = toTail;
            tail = &ptr->buffer[toTail].next;
            ptr->buffer[toTail].next = ptr->size;
        } else {
            position = &(ptr->buffer[*position].next);
        }
        i++;
    }
    return found;
}

/** 2021-01-12 **/

/* Ex. 1 */

void pre_insertPL(struct pointer_list ** ptrptr, int v) {
    struct pointer_list * tmp_ptr;

    tmp_ptr = *ptrptr;
    *ptrptr = (struct pointer_list *)malloc(sizeof(struct pointer_list));
    (*ptrptr)->value = v;
    (*ptrptr)->next_ptr = tmp_ptr;
}

void visitPL(struct pointer_list * ptr) {
    int i = 0;
    while(ptr != NULL) {
        printf("\nValue[%d]: %d", i, ptr->value);
        ptr = ptr->next_ptr;
        i++;
    }
    printf("\n");
}

Boolean alternate_insert(struct pointer_list ** ptrptr, int A[10], int V_insert[], int M) {
    int i,j;
    struct pointer_list ** jump_insert;
    Boolean added = FALSE;

    *ptrptr = NULL; //init_PL

    for(i=9; i>=0; i--) {
        pre_insertPL(ptrptr, A[i]);
    }

    jump_insert = ptrptr;
    for(j=0; j<M; j++) {
        pre_insertPL(jump_insert, V_insert[j]);
        added = TRUE;
        jump_insert = &((*jump_insert)->next_ptr);
        if(*jump_insert != NULL) {
            jump_insert = &(*jump_insert)->next_ptr;
        }
    }
    visitPL(*ptrptr);
    return added;
}

/* Ex. 2 */

Boolean remove_after_match(struct indexes_array_list * ptr, float target, float error) {
    int * position, removed;
    Boolean found = FALSE;

    position = &ptr->first;
    while(*position != ptr->size && !found) {
        if (is_equal(target, ptr->buffer[*position].number, error)) {
            found = TRUE;
            removed = ptr->buffer[*position].next;
            ptr->buffer[*position].next = ptr->buffer[removed].next;
            ptr->buffer[removed].next = ptr->free;
            ptr->free = removed;
        }
        position = &(ptr->buffer[*position].next);
    }
    return found;
}

/* Ex. 3 */

Boolean remove_elements(struct pointer_list ** ptrptr, const int C[], int size) {
    int k;
    struct pointer_list ** tmp_ptrx2, * tmp_ptr;
    Boolean removed = FALSE;

    for(k=0; k<size; k++) {
        tmp_ptrx2 = ptrptr;
        while(*tmp_ptrx2 != NULL) {
            if((*tmp_ptrx2)->value == C[k]) {
                removed = TRUE;
                tmp_ptr = *tmp_ptrx2;
                *tmp_ptrx2 = (*tmp_ptrx2)->next_ptr;
                free(tmp_ptr);
                break;
            }
            tmp_ptrx2 = &((*tmp_ptrx2)->next_ptr);
        }
    }
    visitPL(*ptrptr);
    return removed;
}

/* Ex. 4 */

void float_visit_SL(struct float_sequential_list * ptr) {
    int position;
    for(position = ptr->head; position<ptr->tail; position=(position+1)%ptr->size)
        printf("\nSeq[%d]: %.1f", position, ptr->buffer[position]);
    printf("\n");
}

Boolean remove_after_target(struct float_sequential_list * ptr, const float B[10], int size, float target, float error) {
    int l, position, count;
    Boolean match = FALSE;

    ptr->size = size+1;
    ptr->head = 0;
    ptr->tail = 0;
    ptr->buffer = (float *)malloc(size*(sizeof(float)));

    for(l=0; l<size; l++) {
        if((ptr->tail+1)%ptr->size != ptr->head) {
            ptr->buffer[ptr->tail] = B[l];
            ptr->tail = (ptr->tail+1)%ptr->size;
        }
    }

    position = ptr->head;
    while((position+1)%ptr->size != ptr->tail && match == FALSE) {
        if(is_equal(ptr->buffer[position], target, error)) {
            match = TRUE;
            position = (position+1)%ptr->size;
            for(count = position; count<(ptr->tail-1)%ptr->size; count++)
                ptr->buffer[count] = ptr->buffer[(count+1)%ptr->size];
            ptr->tail = (ptr->tail-1+ptr->size)%ptr->size;
        } else
            position = (position + 1)%ptr->size;
    }
    float_visit_SL(ptr);
    return match;
}

/** 2020-01-10 && 2019-01-08 **/

/* Ex. 1 */

Boolean remove_doubles(struct indexes_array_list * ptr, float target, float error) {
    Boolean bis = FALSE;
    int * position;

    position = &ptr->first;
    while(*position != ptr->size) {
        if(is_equal(ptr->buffer[*position].number, target, error)) {
            bis = TRUE;
            *position = ptr->buffer[*position].next;
        } else
            position = &(ptr->buffer[*position].next);
    }
    float_visitIASL(ptr);
    return bis;
}

/* Ex. 2 & 3 */

void visit_SL(struct sequential_list * ptr) {
    int position;
    for(position=ptr->head; position!=ptr->tail; position=(position+1)%ptr->size) {
        printf("\n%d", ptr->buffer[position]);
    }
    printf("\n");
}

void swap(int * buffer, int l, int r) {
    int tmp;
    tmp = buffer[l];
    buffer[l] = buffer[r];
    buffer[r] = tmp;
}

void init_intSL(struct sequential_list * ptr, int L, int A[10]) {
    int i;

    init_sequential_list(ptr, L);
    for(i=0; i<L; i++) {
        suf_insertSL(ptr, A[i]);
    }
}

int partition(struct sequential_list * ptr, int start, int end) {
    int l, r, pivot;

    //pivot = ptr->buffer[ptr->head];
    pivot = ptr->buffer[start];
    l = start;
    r = end;
//    printf("\nPivot: %d\n", pivot);
    while(l != r) {
        do { r = (r-1+ptr->size)%ptr->size; } while (r != l && ptr->buffer[r] > pivot);
        if(l != r) {
            do { l = (l+1+ptr->size)%ptr->size; } while (r != l && ptr->buffer[l] <= pivot);
//            printf("\nSWAP %d", ++count);
//            printf("\nUpper bound: %d\nLower bound: %d", ptr->buffer[l], ptr->buffer[r]);
            swap(ptr->buffer, l, r);
        }
    }
//    printf("\nSWAP %d", ++count);
//    printf("\nLower bound: %d\nPivot: %d\n", ptr->buffer[l], ptr->buffer[start]);
    //swap(ptr->buffer, l, ptr->head);
    swap(ptr->buffer, l, start);
    return l;
}

void quicksort(struct sequential_list * ptr, int start, int end) {
    int q;
    if(start != end) {
        q = partition(ptr, start, end);
        quicksort(ptr, start, q);
        quicksort(ptr, (q+1)%ptr->size, end);
    }
}

/** 2019-01-08 && 2018-01-10 **/

/* Ex. 1 */

void createSeqList(struct sequential_list * ptr, int L, int * D) {
    int k;

    init_sequential_list(ptr, L);
    for(k=0; k<L; k++) {
        suf_insertSL(ptr, D[k]);
    }
}

void merge(int * V, int N, int N1) {
    int l, r, count;
    int * tmp;

    tmp = (int *)malloc(N1*sizeof(int));
    for(count = 0; count<N1; count++) {
        tmp[count] = V[count];
    }

    l = 0;
    r = 0;
    while(l<N1 && r<N-N1) {
        if(tmp[l]<V[N1+r]) {
            V[l+r] = tmp[l];
            l++;
        } else {
            V[l+r] = V[N1+r];
            r++;
        }
    }
    while(l<N1) {
        V[l+r] = tmp[l];
        l++;
    }
    free(tmp);
}

void mergesort(int * ptr, int N) {
    if(N>1) {
        mergesort(ptr, N/2);
        mergesort(&ptr[N/2], N-N/2);
        merge(ptr, N, N/2);
    }
}

/* Ex. 2 */

void float_merge(float * V, int N, int N1) {
    int l, r, count;
    float * tmp;

    tmp = (float *)malloc(N1*sizeof(float));
    for(count = 0; count<N1; count++) {
        tmp[count] = V[count];
    }

    l = 0;
    r = 0;
    while(l<N1 && r<N-N1) {
        if(tmp[l]<V[N1+r]) {
            V[l+r] = tmp[l];
            l++;
        } else {
            V[l+r] = V[N1+r];
            r++;
        }
    }
    while(l<N1) {
        V[l+r] = tmp[l];
        l++;
    }
    free(tmp);
}

void float_mergesort(float * ptr, int N) {
    if(N>1) {
        float_mergesort(ptr, N/2);
        float_mergesort(&ptr[N/2], N-N/2);
        float_merge(ptr, N, N/2);
    }
}

Boolean float_ord_insert_SL(struct float_sequential_list * ptr, float target, float error) {
    int position, count;

    if(ptr->head != (ptr->tail+1)%ptr->size) {
        position = ptr->head;
        while(position != ptr->tail && ptr->buffer[position]<target && !is_equal(ptr->buffer[position], target, error)) {
            position = (position+1)%ptr->size;
        }
        for(count=(ptr->tail+1)%ptr->size; count!=position; count=(count-1+ptr->size)%ptr->size) {
            ptr->buffer[count] = ptr->buffer[(count-1+ptr->size)%ptr->size];
        }
        ptr->buffer[position] = target;
        ptr->tail = (ptr->tail+1)%ptr->size;
        return TRUE;
    }
    return FALSE;
}

/* Ex. 3 */

void swap_floatIASL(struct record * W, int * position) {
    int tmp1, tmp2;

    tmp1 = W[W[*position].next].next;
    tmp2 = W[*position].next;
    W[W[*position].next].next = *position;
    W[*position].next = tmp1;
    *position = tmp2;

}

void bubblesort(struct record * W, int N, int * first, float error) {
    int iter = 0, count, * position, tmp1, tmp2;
    Boolean swap_found = TRUE;

    do {
        position = first;
        for(count = 0, swap_found = FALSE; count<N-1-iter; count++) {
            if(W[*position].number>W[W[*position].next].number && !is_equal(W[*position].number, W[W[*position].next].number, error)) {
                if(*position != *first)
                    swap_floatIASL(W, position);
                else {
                    tmp1 = W[W[*position].next].next;
                    tmp2 = W[*position].next;
                    W[W[*position].next].next = *position;
                    W[*position].next = tmp1;
                    *first = tmp2;
                }
                swap_found = TRUE;
            }
            position = &(W[*position].next);
        }
        iter++;
    } while (swap_found == TRUE);
}

/* Ex. 4 */

Boolean float_ord_insertIASL(struct indexes_array_list * ptr, float target, float error) {
    int moved, * position;

    if(ptr->free != ptr->size) {
        moved = ptr->free;
        ptr->free = ptr->buffer[moved].next;
        ptr->buffer[moved].number = target;
        position = &(ptr->first);
        while(*position != ptr->size && ptr->buffer[*position].number<target && !is_equal(ptr->buffer[*position].number, target, error) ) {
            position = &(ptr->buffer[*position].next);
        }
        ptr->buffer[moved].next = *position;
        *position = moved;
        return TRUE;
    }
    return FALSE;
}

/* Ex. 5 */

void init_float_sequential_list(struct float_sequential_list * ptr, const float E[10], int size) {
    int l;

    ptr->size = size+1;
    ptr->head = 0;
    ptr->tail = 0;
    ptr->buffer = (float *)malloc(size*(sizeof(float)));

    for(l=0; l<size; l++) {
        if((ptr->tail+1)%ptr->size != ptr->head) {
            ptr->buffer[ptr->tail] = E[l];
            ptr->tail = (ptr->tail+1)%ptr->size;
        }
    }
}

Boolean binary_search_seqlist(struct float_sequential_list * ptr, float target, int length, int first, float error) {
    if(length>0) {
        if (is_equal(ptr->buffer[(first + length / 2) % ptr->size], target, error)) {
            printf("\nTarget found %.1f\n", ptr->buffer[(first + length / 2) % ptr->size]);
            return TRUE;
        } else {
            if (!is_equal(ptr->buffer[(first + length / 2) % ptr->size], target, error) &&
                ptr->buffer[(first + length / 2) % ptr->size] > target)
                return binary_search_seqlist(ptr, target, length / 2, first, error);
            else
                return binary_search_seqlist(ptr, target, length - length / 2 - 1, (first + length / 2 + 1) % ptr->size,
                                             error);
        }
    }
    else {
        printf("\nTarget not found.\n");
        return FALSE;
    }
}

/** 2022-01-27 **/

/* Ex. 6 */

void int_initIALS(struct int_indexes_array_list * ptr, int size) {

    int count;

    ptr->buffer = (struct record *)malloc(size*(sizeof(struct int_record)));
    ptr->size = size;
    ptr->first = ptr->size;
    ptr->free = 0;
    for(count=1; count<size+1; count++) {
        ptr->buffer[count - 1].next = count;
    }
}

void visit_int_IASL(struct int_indexes_array_list * ptr) {
    int position = ptr->first, i = 0;
    while(position != ptr->size) {
        printf("\nInt[%d]: %d | Index: %d", i, ptr->buffer[position].number, ptr->buffer[position].next);
        position = ptr->buffer[position].next;
        i++;
    }
    printf("\n");
}

void remove_median_element(struct int_indexes_array_list * ptr, int * position) {
    int moved;
    moved = *position;
    *position = ptr->buffer[*position].next;
    ptr->buffer[moved].next = ptr->free;
    ptr->free = moved;
}

Boolean remove_median(struct int_indexes_array_list * ptr, int size, const int F[10], float * value) {

    int_initIALS(ptr,size);
    int * position, count=size-1, average_value;

    while(count>=0) {
        if(ptr->free != ptr->size) {
            int moved;
            moved = ptr->free;
            ptr->free = ptr->buffer[ptr->free].next;

            ptr->buffer[moved].number = F[count];
            ptr->buffer[moved].next = ptr->first;
            ptr->first = moved;
        }
        count--;
    }

    average_value = (size-1)/2;
    position = &ptr->first;
    for(count=0; count<average_value; count++) {
        position = &ptr->buffer[*position].next;
    }
    //printf("%d", ptr->buffer[*position].number);
    if(size%2 == 1) {
        *value = (float)ptr->buffer[*position].number;
        printf("\nAverage = %.1f\n", *value);
        remove_median_element(ptr, position);
        return TRUE;
    } else if (size%2 == 0) {
        *value = (float) ptr->buffer[*position].number/2 + (float) ptr->buffer[ptr->buffer[*position].next].number/2;
        printf("\nAverage = %.1f\n", *value);
        remove_median_element(ptr, position);
        remove_median_element(ptr, position);
        return TRUE;
    } else
        return FALSE;
}

/** 2021-04-07 **/

/* Ex. 3 */

void invertSL(struct sequential_list * ptr) {
    int length, i, position, exchange;
    length = (ptr->tail+ptr->size-ptr->head)%ptr->size;
    for(i=0; i<length/2; i++) {
        position = (ptr->head + i) % ptr->size;
        exchange = ptr->buffer[position];
        ptr->buffer[position] = ptr->buffer[(ptr->tail - 1 - i + ptr->size) % ptr->size];
        ptr->buffer[(ptr->tail - 1 - i + ptr->size) % ptr->size] = exchange;
    }
    visit_SL(ptr);
}

/* Ex. 4 */

Boolean checkLength(struct int_indexes_array_list * ptr, int K) {
    int * pos, count = 0, moved, next;
    pos = &ptr->first;
    while(*pos != ptr->size) {
        ++count;
        if(count >= K) {
            if(ptr->buffer[*pos].next == ptr->size) return FALSE;
            else {
                while (ptr->buffer[*pos].next != ptr->size) {
                    if (ptr->free != ptr->size) {
                        moved = ptr->free;
                        ptr->free = ptr->buffer[*pos].next;
                        next = ptr->buffer[ptr->free].next;
                        ptr->buffer[ptr->free].next = ptr->buffer[moved].next;
                        ptr->buffer[moved].number = ptr->buffer[ptr->buffer[*pos].next].number;
                        ptr->buffer[moved].next = ptr->first;
                        ptr->first = moved;
                        ptr->buffer[*pos].next = next;
                    }
                }
                return TRUE;
            }
        }
        pos = &ptr->buffer[*pos].next;
    }
    return FALSE;
}

/** 2022-02-23 **/

/* Ex. 3 */

Boolean seq_list_inclusion(struct sequential_list * ptr1, struct sequential_list * ptr2, int P, int G[]) {
    int max_length, base, offset, N1, N2;
    Boolean found, mismatch;
    init_sequential_list(ptr2, P);
    int index;
    for(index=0; index<P; index++) {
        suf_insertSL(ptr2, G[index]);
    }


    N1 = (ptr1->tail+ptr1->size-ptr1->head)%ptr1->size;
    N2 = (ptr2->tail+ptr2->size-ptr2->head)%ptr2->size;
    max_length = N1 - N2;
    found = FALSE;
    base = ptr1->head;

    while(found == FALSE && base<max_length){
        mismatch = FALSE;
        offset = 0;
        while(mismatch == FALSE && offset<N2) {
            if(ptr1->buffer[(base+offset)%ptr1->size] != ptr2->buffer[(ptr2->head+offset)%ptr2->size]) mismatch = TRUE;
            else offset++;
        }
        if(mismatch == FALSE) found = TRUE;
        else base = (base+1)%ptr1->size;
    }
    return found;
}

/* Ex. 4 */

Boolean swap_equals_bis(struct indexes_array_list * ptr, float a, float b) {
    int * position, * pos_a, * pos_b, moved;
    Boolean found_a, found_b;

    position = &ptr->first;
    found_a = FALSE;
    found_b = FALSE;

    while(*position != ptr->size && (found_a == FALSE || found_b == FALSE)) {
        if(is_equal(ptr->buffer[*position].number, a, epsilon)) {
            pos_a = position;
            found_a = TRUE;
        }
        if(is_equal(ptr->buffer[*position].number, b, epsilon)) {
            pos_b = position;
            found_b = TRUE;
        }
        position = &ptr->buffer[*position].next;
    }
    if(found_a && found_b) {
        moved = *pos_a;
        *pos_a = *pos_b;
        *pos_b = moved;

        moved = ptr->buffer[*pos_a].next;
        ptr->buffer[*pos_a].next = ptr->buffer[*pos_b].next;
        ptr->buffer[*pos_b].next = moved;
        return TRUE;
    }
    return FALSE;
}

/* ----------------------- */

int solution() {
    /* 2022-01-11 */
    struct sequential_list sequentialList;
    int L=10, N=5, count_removed, k;
    int A[10] = {22, 48, 93, 71, 62, 33, 67, 91, 59, 7};
    int V[5]={2, 0, 8, 13, 1};

    float B[10] = {2.2, 1.7, 7.1, 1.7, 6.2, 3.3, 1.7, 1.9, 2.3, 7.};
    float target =  1.7;
    struct indexes_array_list floatArray;

    /* 2021-01-12 */
    int V_insert[] = {56, 75, 43};
    int C[] = {67, 56, 91};
    int M = 3;
    struct pointer_list * PL_ptr;
    struct float_sequential_list floatSequentialList;

    /* 2019-01-08 */
    int D[10] = {35, 86, 23, 78, 55, 94, 16, 8, 47, 29};
    struct sequential_list secondSeqList;
    float insert1 = 5.4, insert2 = 4.7;

    /* 2022-01-27 */
    float E[10] = {1.7, 1.9, 2.2, 3.3, 3.9,4.2, 5.6, 6.4, 7., 7.5 };
    struct float_sequential_list secondFloatSequentialList;
    float toBeFound = 6.4, average;
    struct int_indexes_array_list intIASL;
    int F[10] = {5,12,23,29,34,47,55,61,77,83 };

    /* 2021-04-07 */
    int K = 4;

    /* 2022-02-23 */
    int G[4] = {67, 62, 59, 48};
    struct sequential_list shortSeqList;
    int P = 4;
    float num1 = 1.9, num2 = 7.1;

    /* ----------------------- */

    printf("\n/* 2022-01-11 */\n");
    printf("\n> Ex. 1 <\n");
    count_removed = remove_indexes(&sequentialList, L, V, N, A);
    printf("\nRemoved elements: %d", count_removed);
    printf("\nFinal list:");
    for(k=0; k<L-count_removed; k++) {
        printf(" %d", sequentialList.buffer[k]);
    }
    printf("\n");
    printf("\n> Ex. 2 <\n");
    move_element_to_tail(&floatArray, target, L, B, epsilon);
    float_visitIASL(&floatArray);

    printf("\n/* 2021-01-12 */\n");
    printf("\n> Ex. 1 <\n");
    alternate_insert(&PL_ptr, A, V_insert, M);
    printf("\n> Ex. 2 <\n");
    remove_after_match(&floatArray, target, epsilon);
    float_visitIASL(&floatArray);
    printf("\n> Ex. 3 <\n");
    remove_elements(&PL_ptr, C, M);
    printf("\n> Ex. 4 <\n");
    remove_after_target(&floatSequentialList, B, L, target, epsilon);

    printf("\n/* 2020-01-10 && 2019-01-08 */\n");
    printf("\n> Ex. 1 <\n");
    remove_doubles(&floatArray, target, epsilon);
    printf("\n> Ex. 2 & 3: Partition e Quicksort <\n");
    init_intSL(&sequentialList, L, A);
    //partition(&sequentialList, L, A);
    quicksort(&sequentialList, sequentialList.head, sequentialList.tail);
    visit_SL(&sequentialList);

    printf("\n/* 2019-01-08 && 2018-01-10 */\n");
    printf("\n> Ex. 1: Mergesort <\n");
    createSeqList(&secondSeqList, L, D);
    mergesort(secondSeqList.buffer, L);
    visit_SL(&secondSeqList);
    printf("\n> Ex. 2 <\n");
    float_mergesort(floatSequentialList.buffer, 9);
    float_visit_SL(&floatSequentialList);
    float_ord_insert_SL(&floatSequentialList, insert1, epsilon);
    float_visit_SL(&floatSequentialList);
    printf("\n> Ex. 3: Bubblesort <\n");
    bubblesort(floatArray.buffer, 7, &floatArray.first, epsilon);
    float_visitIASL(&floatArray);
    printf("\n> Ex. 4 <\n");
    float_ord_insertIASL(&floatArray, insert2, epsilon);
    float_visitIASL(&floatArray);

    printf("\n/* 2022-01-27 */\n");
    printf("\n> Ex. 5: Binary search <\n");
    init_float_sequential_list(&secondFloatSequentialList, E, L);
    binary_search_seqlist(&secondFloatSequentialList, toBeFound, L, secondFloatSequentialList.head, epsilon);
    printf("\n> Ex. 6 <\n");
    remove_median(&intIASL, L, F, &average);
    visit_int_IASL(&intIASL);

    printf("\n/* 2021-04-07 */\n");
    printf("\n> Ex. 3: Revert sequential list <\n");
    invertSL(&sequentialList);
    printf("\n> Ex. 4 <\n");
    checkLength(&intIASL, K);
    visit_int_IASL(&intIASL);

    printf("\n/* 2022-02-23 */\n");
    printf("\n> Ex. 3 <\n");
    Boolean included = seq_list_inclusion(&sequentialList, &shortSeqList, P, G);
    (included) ? printf("\nThe second list is included\n") : printf("\nThe second list is not included\n");

    printf("\n/* 2022-02-23 */\n");
    printf("\n> Ex. 4 <\n");
    swap_equals_bis(&floatArray, num1, num2);
    float_visitIASL(&floatArray);



//    init_sequential_list(&sequentialList, L);
//    int index;
//    for(index=0; index<11; index++) {
//        suf_insertSL(&sequentialList, A[index]);
//    }
//    visit_SL(&sequentialList);
//    printf("\ntail %d", sequentialList.tail);
////    pre_insertSL(&sequentialList, 34);
////    pre_insertSL(&sequentialList, 76);
////    pre_insertSL(&sequentialList, 62);
//    printf("\nhead %d", sequentialList.head);
//    printf("\nlength %d", (sequentialList.tail+sequentialList.size-sequentialList.head)%sequentialList.size);
//    visit_SL(&sequentialList);
//    quicksort(&sequentialList, sequentialList.head, sequentialList.tail);
//    visit_SL(&sequentialList);
//    invertSL(&sequentialList);

}

