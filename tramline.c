#include "bool.h"

struct tramline_26 {
    int first;
    int size;
    int free;
    struct list_station *buffer;
};

struct list_station {
    char *name;
    int next;
    struct timetable *timetable_ptr;
    struct list_linked_stop *previous_stop;
    struct list_linked_stop *next_stop;
};

struct list_linked_stop {
    int distance;
    int stop_index;
    struct list_linked_stop *next_ptr;
};

struct timetable {
    int *times;
    int size;
    int head;
    int tail;
};

Boolean entire_string_compare(const char *ptr1, const char *ptr2, int strLengthInput, int strLengthStation) {
    Boolean equals = TRUE;
    int t = 0;
    if (strLengthInput != strLengthStation) {
        equals = FALSE;
        return equals;
    }
    while (t < strLengthInput + 1) {
        if (ptr1[t] != ptr2[t]) {
            equals = FALSE;
            break;
        }
        t++;
    }
    return equals;
}

void get_station(char *station, int length, char W[7][81], int *index, int start_index, char direction) {
    Boolean correct_station = FALSE, wrong_direction;
    int w, strLengthInput, strLengthStation;
    int startOrDestination;
    (*station == '\0') ? (startOrDestination = 0) : (startOrDestination = 1);
    do {
        wrong_direction = FALSE;
        if (startOrDestination == 0)
            printf("\n\nWhere do you take the tram? ");
        else
            printf("\n\nWhere are you traveling to? ");
        scanf("%[^\n]%*c", station);
        strLengthInput = strlen(station);
        for (w = 0; w < length; w++) {
            strLengthStation = strlen(W[w]);
            if (entire_string_compare(W[w], station, strLengthInput, strLengthStation) == TRUE) {
                if (startOrDestination == 1 && direction == 'I')
                    w = length - 1 - w;
                if (startOrDestination == 1 &&
                    (start_index > w)) {
                    wrong_direction = TRUE;
                    printf("\nYou are traveling in the wrong direction. Try again.");
                    break;
                }
                *index = w;
                correct_station = TRUE;
                break;
            }
        }
        if (!correct_station && !wrong_direction)
            printf("\nThe station you entered is incorrect. Try again.");
    } while (!correct_station);
}

void get_direction(char *direction, char D[2][10], int max_directions, int *index, int length) {
    Boolean correct_direction = FALSE;
    int w, strLengthInput, strLengthDirection;
    char selected_direction[10];
    do {
        printf("\n\nWhat is your direction? [inbound=Amsterdam Centraal/outbound=Steigereiland] ");
        scanf("%[^\n]%*c", selected_direction);
        strLengthInput = strlen(selected_direction);
        for (w = 0; w < max_directions; w++) {
            strLengthDirection = strlen(D[w]);
            if (entire_string_compare(D[w], selected_direction, strLengthInput, strLengthDirection) == TRUE) {
                if (w == 0 && *index == 0) {
                    printf("\nYou can only travel outbound from Amsterdam Centraal.");
                    continue;
                }
                if (w == 1 && *index == length - 1) {
                    printf("\nYou can only travel inbound from Steigereiland.");
                    continue;
                }
                (w == 0) ? (*direction = 'I') : (*direction = 'O');
                if (*direction == 'I')
                    *index = length - 1 - (*index);
                correct_direction = TRUE;
                break;
            }
        }
        if (!correct_direction)
            printf("\nThe direction is incorrect or not acceptable. Try again.");
    } while (!correct_direction);
}

void get_deadline(long *deadline, char direction) {
    Boolean in_range = FALSE;
    char input[10], *pointer;
    long MAX_VALUE, MIN_VALUE;
    do {
        if (direction == 'I') {
            printf("\n\nYou are an early bird heading to your workplace.");
            printf("\nWhat time do you start working? [pick an integer between 730 and 900] ");
        } else {
            printf("\n\nYou are an active person coming back from the gym.");
            printf("\nWhat time do you have dinner? [pick an integer between 2000 and 2130] ");
        }
        fgets(input, 10, stdin);
        *deadline = strtol(input, &pointer, 10);
        if (direction == 'I') {
            MIN_VALUE = 730;
            MAX_VALUE = 900;
        } else {
            MIN_VALUE = 2000;
            MAX_VALUE = 2130;
        }
        in_range = MIN_VALUE <= *deadline && *deadline <= MAX_VALUE;
        if (in_range == FALSE) {
            printf("\nDeadline out of range. Try again.");
        }
    } while (in_range == FALSE);
}

void init_timetable(struct timetable *ptr, int size) {
    ptr->times = (int *) malloc(size * (sizeof(int)));
    ptr->size = size + 1;
    ptr->head = 0;
    ptr->tail = 0;
}

Boolean suf_insert_time(struct timetable *ptr, int value) {
    if ((ptr->tail + 1) % ptr->size != ptr->head) {
        ptr->times[ptr->tail] = value;
        ptr->tail = (ptr->tail + 1) % ptr->size;
        return TRUE;
    }
    return FALSE;
}

void visit_timetable(struct timetable *ptr) {
    int position;
    for (position = ptr->head; position != ptr->tail; position = (position + 1) % ptr->size) {
        printf("\n%d", ptr->times[position]);
    }
}

void init_list_linked_stop(struct list_linked_stop **listPointer) {
    *listPointer = NULL;
}

void pre_insert_linked_stop(struct list_linked_stop **ptrptr, int distance, int stop_index) {
    struct list_linked_stop *tmp_ptr;

    tmp_ptr = *ptrptr;
    *ptrptr = (struct list_linked_stop *) malloc(sizeof(struct list_linked_stop));
    (*ptrptr)->distance = distance;
    (*ptrptr)->stop_index = stop_index;
    (*ptrptr)->next_ptr = tmp_ptr;
}

void suf_insert_linked_stop(struct list_linked_stop **ptrptr, int distance, int stop_index) {
    while (*ptrptr != NULL)
        ptrptr = &((*ptrptr)->next_ptr);
    pre_insert_linked_stop(ptrptr, distance, stop_index);
}

void visit_linked_stop(struct list_linked_stop *ptr, struct tramline_26 *ref) {
    int i = 0;
    while (ptr != NULL) {
        printf("\n%s at %d meters", ref->buffer[ptr->stop_index].name, ptr->distance);
        ptr = ptr->next_ptr;
        i++;
    }
    printf("\n");
}

void init_tramline_vector(struct tramline_26 *ptr, int size, const char *selected_direction) {
    int i;
    ptr->buffer = (struct list_station *) malloc(size * (sizeof(struct list_station)));
    ptr->size = size;
    ptr->first = size;
    if (*selected_direction == 'I') {
        ptr->free = 0;
        for (i = 0; i < size; i++) {
            ptr->buffer[i].next = i + 1;
        }
    } else {
        ptr->free = size - 1;
        for (i = size - 1; i >= 0; i--) {
            ptr->buffer[i].next = (i + 6) % size;
            //printf("\nIndex_next: %d", (i+size)%(size+1));
        }
    }
}

Boolean pre_insert_station(struct tramline_26 *ptr, char name[], int timetable_size, const int timetable[7], int gap) {

    if (ptr->free != ptr->size) {
        int moved;
        moved = ptr->free;
        ptr->free = ptr->buffer[ptr->free].next;

        ptr->buffer[moved].name = (char *) calloc(strlen(name), sizeof(char));
        strcpy(ptr->buffer[moved].name, name);
        ptr->buffer[moved].next = ptr->first;
        ptr->buffer[moved].timetable_ptr = (struct timetable *) malloc(sizeof(struct timetable));
        init_timetable(ptr->buffer[moved].timetable_ptr, timetable_size);
        int h, time;
        printf("\n");
        printf("\n%s\n", ptr->buffer[moved].name);
        for (h = 0; h < timetable_size; h++) {
            time = timetable[h] + gap;
            float point_time = (float) time / 100;
            int floor_time = (int) point_time * 100;
            int diff = time - floor_time;
            if (diff >= 60) time += 40;
            printf("Time: %d | ", time);
            suf_insert_time(ptr->buffer[moved].timetable_ptr, time);
        }
        ptr->first = moved;

        return TRUE;
    }
    return FALSE;
}

void
create_vector_stations(struct tramline_26 *ptr, char stops[7][81], int size, int timetable_size, const int timetable[7],
                       char selected_direction, const int gaps[], int max_time_gap) {
    int z, gap_sum;
    (selected_direction == 'I') ? (gap_sum = 0) : (gap_sum = max_time_gap);
    for (z = size - 1; z >= 0; z--) {
        pre_insert_station(ptr, stops[z], timetable_size, timetable, gap_sum);
        if (selected_direction == 'I')
            gap_sum += gaps[z - 1];
        else {
            if (z > 0)
                gap_sum -= gaps[size - 1 - z];
        }
    }
}

void generate_route(struct tramline_26 *ptr, int index, int size, const int *D) {
    int p, n, prev_total_distance = 0, next_total_distance = 0;

    init_list_linked_stop(&(ptr->buffer[index].next_stop));
    init_list_linked_stop(&(ptr->buffer[index].previous_stop));
    if (index != size - 1) {
        for (n = index + 1; n < size; n++) {
            next_total_distance += D[n - 1];
            suf_insert_linked_stop(&(ptr->buffer[index].next_stop), next_total_distance, n);
        }
        printf("\nNext stops:\n");
        visit_linked_stop(ptr->buffer[index].next_stop, ptr);
    }
    if (index != 0) {
        for (p = index - 1; p >= 0; p--) {
            prev_total_distance += D[p];
            suf_insert_linked_stop(&(ptr->buffer[index].previous_stop), prev_total_distance, p);
        }
        printf("\nPrevious stops:\n");
        visit_linked_stop(ptr->buffer[index].previous_stop, ptr);
    }
}

int get_last_time(struct list_station start, struct list_station end, long deadline) {
    int last_time, position;
    Boolean time_found;
    struct timetable *tmt_ptr = end.timetable_ptr;

    time_found = FALSE;
    position = tmt_ptr->head;
    last_time = -1;

    if (tmt_ptr->times[position] < deadline) {
        while (!time_found && position != tmt_ptr->tail) {
            if (tmt_ptr->times[position] > deadline) {
                last_time = start.timetable_ptr->times[position - 1];
                time_found = TRUE;
                break;
            } else if (tmt_ptr->times[position] == deadline) {
                last_time = start.timetable_ptr->times[position];
                time_found = TRUE;
                break;
            }
            position = (position + 1) % tmt_ptr->size;
        }
    }
    return last_time;
}

int tramline() {

    char stops[7][81] = {"Amsterdam Centraal", "Muziekgeb. Bimhuis", "Kattenburgerstraat", "Rietlandpark",
                         "Zuiderzeeweg", "Bob Haarmslaan", "Steigereiland"};
    int distances[] = {950, 550, 1000, 2000, 750, 950};
    int time_gaps[] = {3, 2, 2, 3, 1, 3};
    char allowed_directions[2][10] = {"inbound", "outbound"};
    int outbound_timetable[7] = {2006, 2021, 2040, 2100, 2120, 2140, 2200};
    int inbound_timetable[7] = {742, 757, 812, 827, 842, 857, 912};
    struct tramline_26 tw3ntySix;
    struct tramline_26 *tramline_ptr = &tw3ntySix;
    int S = 7, k, start_index, end_index, max_time_gap = 0, last_time;
    long deadline;
    char selected_stop[81], destination[81], selected_direction;
    *selected_stop = '\0';

    printf("\n--Tramline--\n");

    printf("\nTramline 26 stops are: | ");
    for (k = 0; k < S; k++) {
        printf("%s | ", stops[k]);
        if (k > 0)
            max_time_gap += time_gaps[k - 1];
    }
    get_station(selected_stop, S, stops, &start_index, 0, selected_direction);
    printf("\nYour station is: %s", selected_stop);
    get_direction(&selected_direction, allowed_directions, (sizeof(allowed_directions) / sizeof(allowed_directions[0])),
                  &start_index, S);
    init_tramline_vector(tramline_ptr, S, &selected_direction);
    printf("\n\n#########################");
    printf("\n        TIMETABLE        \n");
    printf("#########################");
    if (selected_direction == 'I')
        create_vector_stations(tramline_ptr, stops, S, (sizeof(inbound_timetable) / sizeof(inbound_timetable[0])),
                               inbound_timetable, selected_direction, time_gaps, max_time_gap);
    else
        create_vector_stations(tramline_ptr, stops, S, (sizeof(outbound_timetable) / sizeof(outbound_timetable[0])),
                               outbound_timetable, selected_direction, time_gaps, max_time_gap);
    printf("\n\n");
    for (k = 0; k < S; k++) {
        printf("\nStop[%d]: %s", k, tramline_ptr->buffer[k].name);
    }
    printf("\n");
    generate_route(tramline_ptr, start_index, S, distances);
    //visit_timetable(tramline_ptr->buffer[start_index].timetable_ptr);
    get_station(destination, S, stops, &end_index, start_index, selected_direction);
    get_deadline(&deadline, selected_direction);
    last_time = get_last_time(tramline_ptr->buffer[start_index], tramline_ptr->buffer[end_index], deadline);
    if (last_time == -1)
        printf("\nThere are no trams available.\n");
    else
        printf("\nThe last available ride to reach %s before %ld leaves at %d from %s.\n", destination, deadline,
               last_time, selected_stop);
    //fputs(&selected_stop, stdout);

    return 0;
}