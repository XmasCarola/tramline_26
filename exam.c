#include "bool.h"

/* Ex. 3 */

struct seq_list {
    int head;
    int tail;
    int size;
    float * buffer;
};

void init_seq_list(struct seq_list * ptr, int size) {
    ptr->size = size+1;
    ptr->head = 0;
    ptr->tail = 0;
    ptr->buffer = (float *)malloc(size*(sizeof(float)));
}

int suf_insert_seq_list(struct seq_list * ptr, float value) {
    if ((ptr->tail+1)%ptr->size != ptr->head) {
        ptr->buffer[ptr->tail] = value;
        ptr->tail = (ptr->tail+1)%ptr->size;
    }
}

void create_seq_lists(struct seq_list * ptr1, struct seq_list * ptr2, float A[], float B[], int A1, int B2) {
    int count;

    for(count=0; count<A1; count++) {
        suf_insert_seq_list(ptr1, A[count]);
    }
    for(count=0; count<B2; count++) {
        suf_insert_seq_list(ptr2, B[count]);
    }
}

Boolean isequal(float a, float b) {
    float diff, max;
    diff = fabs(a - b);
    a = fabs(a);
    b = fabs(b);
    max = (a > b) ? a : b;
    if(diff <= epsilon*max)
        return TRUE;
    else
        return FALSE;
}

void visit_seq_list(struct seq_list * ptr) {
    int position;
    for(position = ptr->head; position<ptr->tail; position=(position+1)%ptr->size)
        printf("\nSeq[%d]: %.1f", position, ptr->buffer[position]);
    printf("\n");
}

Boolean inverse_check(struct seq_list * L1, struct seq_list * L2) {
    int length1, length2, pos1, pos2;
    Boolean equal;

    equal = FALSE;
    length1 = (L1->tail+L1->size-L1->head)%L1->size;
    length2 = (L2->tail+L2->size-L2->head)%L2->size;

    if(length1==length2 && length1!=0) {
        equal = TRUE;
        pos1 = L1->head;
        pos2 = (L2->tail-1+L1->size)%L1->size;
        while(pos1<L1->tail && equal==TRUE) {
            if(isequal(L1->buffer[pos1], L2->buffer[pos2])) {
                pos1 = (pos1+1)%L1->size;
                pos2 = (pos2-1+L2->size)%L2->size;
            } else {
                equal = FALSE;
            }
        }
    }
    return equal;
}

/* Ex. 4 */

struct ind_list {
    int first;
    int free;
    int size;
    struct element * buffer;
};

struct element {
    int value;
    int next;
};

void init_ind_list(struct ind_list * ptr, int size) {
    int count;

    ptr->buffer = (struct element *)malloc(size*(sizeof(struct element)));
    ptr->size = size;
    ptr->first = ptr->size;
    ptr->free = 0;
    for(count=1; count<size+1; count++)
        ptr->buffer[count-1].next = count;
}

void visit_ind_list(struct ind_list * ptr) {
    int position;
    position = ptr->first;

    while(position != ptr->size) {
        printf("[%d]: %d | ", position, ptr->buffer[position].value);
        position = ptr->buffer[position].next;
    }
    printf("\n");
}

Boolean pre_insert_ind_list(struct ind_list * ptr, int value) {

    if(ptr->free != ptr->size) {
        int moved;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;

        ptr->buffer[moved].value = value;
        ptr->buffer[moved].next = ptr->first;
        ptr->first = moved;

        return TRUE;
    }
    return FALSE;
}

void create_ind_list(struct ind_list * ptr, int C[], int M) {
    int count;

    for(count=M-1; count>=0; count--) {
        pre_insert_ind_list(ptr, C[count]);
    }
}

Boolean swap_equals (struct ind_list * ptr, int n1, int n2) {
    int * position, * swap1, * swap2, tmp;
    Boolean found1, found2, swap;

    position = &(ptr->first);
    found1 = FALSE;
    found2 = FALSE;
    swap = FALSE;

    while(*position!=ptr->size && (!found1 || !found2)) {
        if(*position == n1) {
            swap1 = position;
            found1 = TRUE;
        }
        if(*position == n2) {
            swap2 = position;
            found2 = TRUE;
        }
        position = &(ptr->buffer[*position].next);
    }
    if(found1 == TRUE && found2 == TRUE) {
        tmp = *swap1;
        *swap1 = *swap2;
        *swap2 = tmp;

        tmp = ptr->buffer[*swap1].next;
        ptr->buffer[*swap1].next = ptr->buffer[*swap2].next;
        ptr->buffer[*swap2].next = tmp;
        swap = TRUE;
    }
    return swap;
}

/* Ex. 5 */

struct pointer_list {
    int value;
    struct pointer_list * next_ptr;
};

void init_pointer_list(struct pointer_list ** listPointer) {
    *listPointer = NULL;
}

void pre_insert_pointer_list(struct pointer_list ** ptrptr, int v) {
    struct pointer_list * tmp_ptr;

    tmp_ptr = *ptrptr;
    *ptrptr = (struct pointer_list *)malloc(sizeof(struct pointer_list));
    (*ptrptr)->value = v;
    (*ptrptr)->next_ptr = tmp_ptr;
}

void create_pointer_list(struct pointer_list ** ptrptr, int D[], int O) {
    int count;
    for(count=O-1; count>=0; count--) {
        pre_insert_pointer_list(ptrptr, D[count]);
    }
}

void visit_pointer_list(struct pointer_list * ptr) {
    while(ptr != NULL) {
        printf("%d | ", ptr->value);
        ptr = ptr->next_ptr;
    }
    printf("\n");
}

void subtract_and_check(const int V[], int N, struct pointer_list ** L, struct pointer_list ** R) {
    struct pointer_list ** ptrptr, * tmp_ptr, ** ptr_R, ** next_ptr;
    int iter;

    ptrptr = L;
    next_ptr = R;
    iter = 0;

    while(*ptrptr != NULL) {
        (*ptrptr)->value -= V[iter];
        if((*ptrptr)->value < 0) {
            ptr_R = next_ptr;
            tmp_ptr = *ptr_R;
            *ptr_R = *ptrptr;
            (*ptrptr) = (*ptrptr)->next_ptr;
            (*ptr_R)->next_ptr = tmp_ptr;
            next_ptr = &((*ptr_R)->next_ptr);
        } else {
            ptrptr = &((*ptrptr)->next_ptr);
        }
        iter = (iter+1)%N;
    }
}

/* ----------------------- */

int exam() {

    /* Ex. 3 */
    float A[] = {4.1,7.2,9.3,2.5};
    float B[] = {2.5,9.3,7.2,4.1};
    struct seq_list L1;
    struct seq_list L2;
    int size = 8, A1 = 4, B2 = 4;
    Boolean seq_lists_checked;

    /* Ex. 4 */
    int n1 = 0, n2 = 9, M = 10;
    int C[] = {1,2,3,4,5,6,7,8,9,10};
    struct ind_list IASL;

    /* Ex. 5 */
    int D[] = {5,8,7,2,9,7,1,6,7,4,9};
    int V[] = {5,2,3};
    int O = 11, N = 3;
    struct pointer_list * L;
    struct pointer_list * R;

    /* ----------------------- */

    printf("\n> Ex. 3 <\n");
    init_seq_list(&L1, size);
    init_seq_list(&L2, size);
    create_seq_lists(&L1, &L2, A, B, A1, B2);
    seq_lists_checked = inverse_check(&L1, &L2);
    seq_lists_checked ? printf("\nBINGO! The lists are reverted.\n") : printf("\nThe lists do not correspond.\n");

    printf("\n> Ex. 4 <\n");
    init_ind_list(&IASL, M);
    create_ind_list(&IASL, C, M);
    printf("\nInitial list: ");
    visit_ind_list(&IASL);
    swap_equals(&IASL, n1, n2);
    printf("\nFinal list: ");
    visit_ind_list(&IASL);

    printf("\n> Ex. 5 <\n");
    init_pointer_list(&L);
    init_pointer_list(&R);
    create_pointer_list(&L, D, O);
    printf("\nInitial list L: ");
    visit_pointer_list(L);
    subtract_and_check(V, N, &L, &R);
    printf("\nFinal list L: ");
    visit_pointer_list(L);
    printf("\nFinal list R: ");
    visit_pointer_list(R);

    return 0;
}
