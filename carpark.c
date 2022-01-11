#include "bool.h"

struct list_dept {
    int phone_number;
    int next;
    struct list_plate * ptr;
};

struct list_plate {
    char * plate_no;
    struct list_plate * next_ptr;
};

struct parking_vector {
    int first;
    int size;
    int free;
    struct list_dept * buffer;
};

void init_list_plate(struct list_plate ** listPointer) {
    *listPointer = NULL;
}

void pre_insert_list_plate(struct list_plate ** ptrptr, char plate_no[7]) {
    struct list * tmp_ptr;

    tmp_ptr = *ptrptr;
    *ptrptr = (struct list *)malloc(sizeof(struct list_plate));
    (*ptrptr)->plate_no = (char*)calloc(strlen(plate_no),sizeof(char));
    strcpy((*ptrptr)->plate_no, plate_no);
    (*ptrptr)->next_ptr = tmp_ptr;
}

void visit_plate(struct list_plate * ptr) {
    int i = 0;
    while(ptr != NULL) {
        printf("\nValue[%d]: %s", i, ptr->plate_no);
        ptr = ptr->next_ptr;
        i++;
    }
    printf("\n");
}

void init_parking_vector(struct parking_vector * ptr, int size) {
    int i;
    ptr->buffer = (struct list_dept *)malloc(size*(sizeof(struct list_dept)));
    ptr->size = size;
    ptr->first = size;
    ptr->free = 0;
    for(i=1; i<size+1; i++) {
        ptr->buffer[i-1].next = i;
    }
}

Boolean pre_insert_dept(struct parking_vector * ptr, int no) {

    if (ptr->free != ptr->size) {
        int moved;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;

        ptr->buffer[moved].phone_number = no;
        ptr->buffer[moved].next = ptr->first;
        ptr->first = moved;

        return TRUE;
    }
    return FALSE;
}

void create_vector_depts(struct parking_vector * ptr, char L[4][3][20], int size, int max_no_plates, int phone_numbers[]) {
    int i, j;
    for(i=0; i<size; i++) {
        j=0;
        pre_insert_dept(ptr, phone_numbers[size-1-i]);
        init_list_plate(&(ptr->buffer[i].ptr));
        while(j<max_no_plates) {
            if(L[size-1-i][j][0] != '\0')
                pre_insert_list_plate(&(ptr->buffer[i].ptr), L[size-1-i][j]);
            j++;
        }
    }
}

Boolean string_compare(char * ptr1, char * ptr2) {
    Boolean equals = TRUE;
    int t=0;
    while(t<7) {
        if(ptr1[t] != ptr2[t]) {
            equals = FALSE;
            break;
        }
        t++;
    }
    return equals;
}

Boolean search_plate(struct list_plate * ptr, char value[]) {
    Boolean found = FALSE;
    while(ptr != NULL) {
        if(string_compare(ptr->plate_no, value) == TRUE) {
            found = TRUE;
            break;
        }
        ptr = ptr->next_ptr;
    }
    return found;
}

Boolean search_dept(struct parking_vector * ptr, char value[], int * phone_ptr, char ** dept_ptr, char P[4][20]) {
    Boolean found = FALSE;
    int position, i=0;

    position = ptr->first;
    while(position != ptr->size) {
        if(search_plate(ptr->buffer[position].ptr, value) == TRUE) {
            *phone_ptr = ptr->buffer[position].phone_number;
            (*dept_ptr) = (char*)calloc(strlen(P[i]),sizeof(char));
            strcpy(*dept_ptr, P[i]);
            found = TRUE;
            break;
        }
        position = ptr->buffer[position].next;
        i++;
    }
    return found;
}

int carpark() {

    char departments[4][20] = {"Economics", "Engineering","Linguistics", "Physics"};
    int phone_numbers[] = { 579, 365, 148, 920 };
    char plates[4][3][20] = {{"IT000XS", "DE334GH", "DK743YR"}, {"FI267BN", "NL890DQ", ""}, {"PT562NK", "SE881AB", ""}, {"ES264FU", "FR451ZT", "RO913EI"}};
    struct parking_vector Park;
    struct parking_vector * park_ptr = &Park;
    int S = 4, max_no_plates = 3, k, phone_ptr;
    char plate_value[] = "NL890DQ";
    char * department;

    printf("\n--Car park--\n");

    init_parking_vector(park_ptr, S);
    create_vector_depts(park_ptr, plates, S, max_no_plates, phone_numbers);
    for(k=0; k<S; k++) {
        printf("\nPhone: %d", park_ptr->buffer[k].phone_number);
        visit_plate(park_ptr->buffer[k].ptr);
    }
    if(search_dept(park_ptr, plate_value, &phone_ptr, &department, departments) == TRUE) {
        printf("\nPlate admitted\n");
        printf("Department of %s\n", department);
        printf("Please call: %d\n", phone_ptr);
    } else
        printf("\nPlate rejected\n");

    return 0;
}