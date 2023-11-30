#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#define MAX_SIZE 256

typedef struct {
    char oznacenie; //A
    int cislovanie; //111
    char druh; //a
}IDmer;

typedef struct {
    char sign1; // + / -
    float latitude; //float
    char sign2; // + / -
    float longitude; //float

}Typmer;

typedef struct RD_data {
    IDmer ID;
    Typmer TYP;

    char modul[2]; //2 uppercase

    double value; //float

    int time; //int

    int date; //4+2+2
    struct RD_data *next;
    struct RD_data *previous;
} RD;

void free_linked_list (RD **head) { //free the linked list
    RD *current = *head;
    RD *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; //set head to NULL due to wanted reset
}

void n(RD **head) {
    // Open the file and read data into the linked list using head pointer
    FILE *file = fopen("C:\\Users\\jakub\\Desktop\\pr02\\dataloger_V2.txt", "r");
    if (file == NULL) {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    RD *current = NULL; //pointer to the current node
    int count = 0;

    while (!feof(file)) { //reading data until the file is empty
        RD *newRD = (RD *)malloc(sizeof(RD));

        //populating red data into structure with fscanf
        if(fscanf(file, " %c%d%c\n %c%f %c%f\n %c%c\n %lf\n %d\n %d",
                  &newRD->ID.oznacenie, &newRD->ID.cislovanie, &newRD->ID.druh,
                  &newRD->TYP.sign1, &newRD->TYP.latitude, &newRD->TYP.sign2,
                  &newRD->TYP.longitude,  &newRD->modul[0], &newRD->modul[1],
                  &newRD->value, &newRD->time, &newRD->date) == 12)
        {
            newRD->next = NULL; //initialazing new nodes pointers
            newRD->previous = NULL;

            if (*head == NULL) { //check if empty
                *head = newRD;
                current = newRD;
            } else { // add new node to the end of the list
                current->next = newRD;
                newRD->previous = current;
                current = newRD;
            }

            count++;
        } else {
            free(newRD);
        }
    }
    fclose(file);

    if (count > 0) {
        printf("Nacitalo sa %d zaznamov\n", count);
    }
}

void v(RD *head) {
    RD *current = head;
    int recordNumber = 1;

    while (current != NULL) { //printing loaded values from structure
        printf("%d:\n", recordNumber);
        printf("ID: %c%03d%c  %c%c\t %.2lf\n", current->ID.oznacenie, current->ID.cislovanie, current->ID.druh,
               current->modul[0], current->modul[1], current->value);
        printf("Poz: %c%.4f\t %c%.4f\n", current->TYP.sign1, current->TYP.latitude,current->TYP.sign2,
               current->TYP.longitude);
        printf("DaC: %d\t %d\n", current->date, current->time);
        printf("\n");

        current = current->next;
        recordNumber++;
    }
}

void p(RD **head) {
    int position; //initializing new position
    printf("Zadaj poziciu, kde bude novy zaznam:\n");
    if (scanf("%d", &position) != 1 || position < 1 || position > 999) {
        printf("Zle zadana hodnota pre poziciu.\n");
        return;
    }

    RD *newRD = (RD *)malloc(sizeof(RD));

    if (newRD == NULL) { //checking if failed
        printf("Zlyhalo alokovanie\n");
        return;
    }

    printf("Zadaj hodnoty pre novy zaznam:\n");

    printf("ID: ");
    if (scanf(" %c%d%c", &newRD->ID.oznacenie, &newRD->ID.cislovanie, &newRD->ID.druh) != 3) {
        printf("Zle zadane hodnoty pre ID.\n");
        free(newRD);
        return;
    }

    // Secure input for position
    printf("Pozicia modulu (lat., long.): ");
    if (scanf(" %c%f%c%f", &newRD->TYP.sign1, &newRD->TYP.latitude, &newRD->TYP.sign2,
              &newRD->TYP.longitude) != 4) {
        printf("Zle zadane hodnoty pre poziciu modulu.\n");
        free(newRD);
        return;
    }

    // Secure input for mer. velicniy
    printf("Typ. mer. velicniy: ");
    if (scanf(" %c%c", &newRD->modul[0], &newRD->modul[1]) != 2) {
        printf("Zle zadane hodnoty pre typ. mer. velicniy.\n");
        free(newRD);
        return;
    }

    // Secure input for hodnota
    printf("Hodnota: ");
    if (scanf("%lf", &newRD->value) != 1) {
        printf("Zle zadana hodnota pre hodnotu.\n");
        free(newRD);
        return;
    }

    // Secure input for cas merania
    printf("Cas merania: ");
    if (scanf("%d", &newRD->time) != 1) {
        printf("Zle zadana hodnota pre cas merania.\n");
        free(newRD);
        return;
    }

    // Secure input for datum merania
    printf("Datum merania: ");
    if (scanf("%d", &newRD->date) != 1) {
        printf("Zle zadana hodnota pre datum merania.\n");
        free(newRD);
        return;
    }

    newRD->next = NULL; //reset of prev and next for the new node
    newRD->previous = NULL;

    if (*head == NULL || position == 1) { //checking if the list is empty or the pos is number 1
        newRD->next = *head;
        if (*head != NULL) {
            (*head)->previous = newRD;
        }
        *head = newRD;
    } else { // if the list exists and the pos is greater than 1
        RD *current = *head;
        int count = 1;

        // Traverse the list to the position before the desired insertion point
        while (current->next != NULL && count < position - 1) {
            current = current->next;
            count++;
        }

        // Insert the new record at the specified position
        newRD->next = current->next; // Link the new record to the rest of the list
        newRD->previous = current; // Link the new record to the previous node

        if (current->next != NULL) { // Updating the previous pointer of the next node, if it exists
            current->next->previous = newRD;
        }
        current->next = newRD; // Update the next pointer of the current node to point to the new record
    }
}

void z(RD **head) {
    char searchOznacenie;
    int searchCislovanie;
    char searchDruh;

    printf("Zadaj ID mer. modulu pre vymazanie zaznamov (oznacenie cislovanie druh): \n");
    if (scanf(" %c%d%c", &searchOznacenie, &searchCislovanie, &searchDruh) != 3) {
        printf("Chyba pri nacitavani ID.\n");
        return;
    }

    RD *current = *head;
    int count = 0;

    while (current != NULL) {
        char currentOznacenie = current->ID.oznacenie;
        int currentCislovanie = current->ID.cislovanie;
        char currentDruh = current->ID.druh;

        if (searchOznacenie == currentOznacenie &&
            searchCislovanie == currentCislovanie &&
            searchDruh == currentDruh) {
            //if match is found, the record is deleted

            if (current->previous != NULL) { //updating pointers to neighbours nodes
                current->previous->next = current-> next;
            } else {
                *head = current->next;
            }

            if (current->next != NULL) { //checking the end of the list and reconnecting pointers
                current->next->previous = current->previous;
            }

            free(current);

            printf("Zaznam/zaznamy pre ID: %c%d%c bol vymazany\n", searchOznacenie, searchCislovanie, searchDruh);
            count++;
        }

        current = current->next; //moving to the next node
    }

    if (count == 0) {
        printf("Zaznam/zaznamy pre ID: %c%d%c nebol vymazany\n", searchOznacenie, searchCislovanie, searchDruh);
    }
}

void swap(RD *a, RD *b) { //creating temp storage for data and then swaping them
    char tmpOznacenie = a->ID.oznacenie;
    int tmpCislovanie = a->ID.cislovanie;
    char tmpDruh = a->ID.druh;

    char tempSign1 = a->TYP.sign1;
    float tempLatitude = a->TYP.latitude;
    char tempSign2 = a->TYP.sign2;
    float tempLongitude = a->TYP.longitude;

    char tempModul1 = a->modul[0];
    char tempModul2 = a->modul[1];

    double tempValue = a->value;

    int tmpTime = a->time;
    int tmpDate = a->date;

    a->ID.oznacenie = b->ID.oznacenie;
    a->ID.cislovanie = b->ID.cislovanie;
    a->ID.druh = b->ID.druh;

    a->TYP.sign1 = b ->TYP.sign1;
    a->TYP.latitude = b->TYP.latitude;
    a->TYP.sign2 = b ->TYP.sign2;
    a->TYP.longitude = b->TYP.longitude;

    a->modul[0] = b->modul[0];
    a->modul[1] = b->modul[1];

    a->value = b->value;

    a->time = b->time;
    a->date = b->date;

    b->ID.oznacenie = tmpOznacenie;
    b->ID.cislovanie = tmpCislovanie;
    b->ID.druh = tmpDruh;

    b->TYP.sign1 = tempSign1;
    b->TYP.latitude = tempLatitude;
    b->TYP.sign2 = tempSign2;
    b->TYP.longitude = tempLongitude;

    b->modul[0] = tempModul1;
    b->modul[1] = tempModul2;

    b->time = tmpTime;
    b->date = tmpDate;
}

int u(RD **head) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("Zoznam je uz usporiadany alebo obsahuje iba jeden prvok.\n");
        return 0;
    }

    RD *sorted = NULL; //pointer will be used to build the sorted list

    while (*head != NULL) {
        RD *current = *head; //initiates current node at the head of the original list
        *head = (*head)->next; // moving to the next node

        if (sorted == NULL || (current->date < sorted->date) ||
            (current->date == sorted->date && current->time < sorted->time)) {
            //Checks if the sorted list is empty or if the current node should be inserted
            // at the beginning of the sorted list based on date and time

            current->next = sorted;
            sorted = current; // insert the current node at the beginning of the sorted list

        } else { //if the node need to be inserted elsewhere not at the beginning of the sorted list

            RD *temp = sorted; //temp pointer traverses the sorted list
            while (temp->next != NULL && (current->date > temp->next->date ||
                    (current->date == temp->next->date && current->time > temp->next->time))) {
                temp = temp->next; //Moves temp to the next node in the sorted list
            }
            //Traverses the sorted list to find the correct position for the current node based on date and time.

            // Insert the current node into the sorted position
            current->next = temp->next;
            temp->next = current;
            //Inserts the current node into the sorted list at the correct position.
        }
    }

    *head = sorted; // Update the head to point to the sorted list
    return 1; // 1 indicates successful rearrangement
}

void r(RD **head) {
    int c1, c2;

    printf("Zadaj poziciu na prehodenie zaznamov (c1 a c2): \n");
    if (scanf("%d %d", &c1, &c2) != 2 || c1 <= 0 || c2 <= 0) {
        printf("Chyba pri nacitavani pozicii.\n");
        return;
    }

    // Check if the list is empty or contains only one element
    if (*head == NULL || (*head)->next == NULL) {
        printf("Zoznam je prazdny alebo obsahuje iba jeden prvok.\n");
        return;
    }

    int count = 1;
    RD *ptr1 = *head;
    RD *ptr2 = *head;

    // Finding the nodes at positions c1
    while (ptr1 != NULL && count < c1) {
        ptr1 = ptr1->next;
        count++;
    }

    count = 1;

    while (ptr2 != NULL && count < c2) { // doing the same at c2
        ptr2 = ptr2->next;
        count++;
    }

    if (ptr1 == NULL || ptr2 == NULL) { // Checking if positions c1 and c2 are valid
        printf("Neplatne pozicie zaznamov.\n");
        return;
    }

    swap(ptr1, ptr2); // Swaping the records at positions c1 and c2
//    printf("Zaznamy na poziciach %d a %d boli prehodene.\n", c1, c2);
}

int main() {
    // pointer that indicates the starting point of a linked list,
    // serving as the reference to the first node in the sequence of connected elements
    RD *head = NULL;

    char sign;

    printf("Zadaj na klavesnici: v/V, n/N, p/P, z/Z, u/U, r/R, k/K\n");
    while (1) {
        scanf(" %c", &sign);

        switch (sign) {
            case 'n':
            case 'N':
                free_linked_list(&head);
                n(&head);
                break;
            case 'v':
            case 'V':
                v(head);
                break;
            case 'p':
            case 'P':
                p(&head);
                break;

            case 'z':
            case 'Z':
                z(&head);
                break;

            case 'u':
            case 'U':
                if (u(&head)) {
                    printf("Spajany zoznam bol uspesne usporadany.\n");
                } else {
                    printf("Chyba usporiadania.\n");
                }
                break;

            case 'r':
            case 'R':
                r(&head);
                break;

            case 'k':
            case 'K':
                free_linked_list(&head);
                return 0;

            default:
                printf("Neexistuje dana moznost.\n");
                break;
        }
    }
}
