#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#define MAX_SIZE 256

typedef struct RD_data {
    char oznacenie; //A
    int cislovanie; //111
    char druh; //a

    char sign1; // + / -
    float latitude; //float
    char sign2; // + / -
    float longitude; //float

    char modul[2]; //2 uppercase

    double value; //float

    int time; //int

    int date; //4+2+2
    struct RD_data *next;
    struct RD_data *previous;
} RD;

void n(RD **head) {
    // Open the file and read data into the linked list using head pointer
    FILE *file = fopen("C:\\Users\\jakub\\Desktop\\pr02\\dataloger_V2.txt", "r");
    if (file == NULL) {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    RD *current = NULL;
    int count = 0;

    while (!feof(file)) {
        RD *newRD = (RD *)malloc(sizeof(RD));

        if(fscanf(file, " %c%d%c\n %c%f %c%f\n %c%c\n %lf\n %d\n %d",
                  &newRD->oznacenie, &newRD->cislovanie, &newRD->druh,
                  &newRD->sign1, &newRD->latitude, &newRD->sign2,
                  &newRD->longitude,  &newRD->modul[0], &newRD->modul[1],
                  &newRD->value, &newRD->time, &newRD->date) == 12)
        {
            newRD->next = NULL;
            newRD->previous = NULL;

            if (*head == NULL) {
                *head = newRD;
                current = newRD;
            } else {
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

    while (current != NULL) {
        printf("%d:\n", recordNumber);
        printf("ID: %c%03d%c  %c%c\t %.2lf\n", current->oznacenie, current->cislovanie, current->druh,
               current->modul[0], current->modul[1], current->value);
        printf("Poz: %c%.4f\t %c%.4f\n", current->sign1, current->latitude,current->sign2,
               current->longitude);
        printf("DaC: %d\t %d\n", current->date, current->time);
        printf("\n");

        current = current->next;
        recordNumber++;
    }
}

void p(RD **head) {
    int position;
    printf("Zadak poziciu, kde bude novy zaznam:\n");
    scanf("%d",&position);

    if (position <= 0) {
        printf("Zle zadana hodnota\n");
        return;
    }

    RD *newRD = (RD *)malloc(sizeof(RD));

    if (newRD == NULL) {
        printf("Zlyhalo alokovanie\n");
        return;
    }

    printf("Zadaj hodnoty pre novy zaznam:\n");

    printf("ID: ");
    scanf(" %c%d%c", &newRD->oznacenie, &newRD->cislovanie, &newRD->druh);

    printf("Pozicia modulu (lat., long.): ");
    scanf(" %c%f%c%f", &newRD->sign1, &newRD->latitude, &newRD->sign2, &newRD->longitude);

    printf("Typ. mer. velicniy: ");
    scanf(" %c%c", &newRD->modul[0], &newRD->modul[1]);

    printf("Hodnota: ");
    scanf("%lf", &newRD->value);

    printf("Cas merania: ");
    scanf("%d", &newRD->time);

    printf("Datum merania: ");
    scanf("%d", &newRD->date);

    newRD->next = NULL; //reset of prev and next
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

int compareID(RD *RD, char oznacenie, int cislovanie, char druh) {
    return (RD->oznacenie == oznacenie &&
            RD->cislovanie == cislovanie &&
            RD->druh == druh);
}

void z(RD **head) {
    char searchOznacenie;
    int searchCislovanie;
    char searchDruh;

    printf("Zadaj ID mer. modulu pre vymazanie zaznamov (oznacenie cislovanie druh): ");
    if (scanf(" %c%d%c", &searchOznacenie, &searchCislovanie, &searchDruh) != 3) {
        printf("Chyba pri nacitavani ID.\n");
        return;
    }

    RD *current = *head;
    int count = 0;

    while (current != NULL) {
        char currentOznacenie = current->oznacenie;
        int currentCislovanie = current->cislovanie;
        char currentDruh = current->druh;

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
    char tmpOznacenie = a->oznacenie;
    int tmpCislovanie = a->cislovanie;
    char tmpDruh = a->druh;

    char tempSign1 = a->sign1;
    float tempLatitude = a->latitude;
    char tempSign2 = a->sign2;
    float tempLongitude = a->longitude;

    char tempModul1 = a->modul[0];
    char tempModul2 = a->modul[1];

    double tempValue = a->value;

    int tmpTime = a->time;
    int tmpDate = a->date;

    a->oznacenie = b->oznacenie;
    a->cislovanie = b->cislovanie;
    a->druh = b->druh;

    a->sign1 = b ->sign1;
    a->latitude = b->latitude;
    a->sign2 = b ->sign2;
    a->longitude = b->longitude;

    a->modul[0] = b->modul[0];
    a->modul[1] = b->modul[1];

    a->value = b->value;

    a->time = b->time;
    a->date = b->date;

    b->oznacenie = tmpOznacenie;
    b->cislovanie = tmpCislovanie;
    b->druh = tmpDruh;

    b->sign1 = tempSign1;
    b->latitude = tempLatitude;
    b->sign2 = tempSign2;
    b->longitude = tempLongitude;

    b->modul[0] = tempModul1;
    b->modul[1] = tempModul2;

    b->time = tmpTime;
    b->date = tmpDate;
}

void u(RD **head) {
    if (*head == NULL || (*head)->next == NULL) {
        printf("Zoznam je uz usporiadany alebo obsahuje iba jeden prvok.\n");
        return;
    }

    int swapped = 1;
    RD *ptr;
    RD *ptr_com = NULL;

    while (swapped) {
        swapped = 0; //making sure if something does not work loop will be functional only once
        ptr = *head;

        while (ptr->next != ptr_com) {
            if (ptr->date > ptr->next->date ||
                (ptr->date == ptr->next->date && ptr->time > ptr->next->time)) {
                swap(ptr, ptr->next);
                swapped = 1;
            }

            ptr = ptr->next;
        }
        ptr_com = ptr;
    }

    printf("Spajany zoznam bol usporiadany.\n");
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
    printf("Zaznamy na poziciach %d a %d boli prehodene.\n", c1, c2);
}

int main() {
    RD *head = NULL;

    char sign;

    printf("Zadaj na klavesnici: v/V, n/N, p/P, z/Z, u/U, r/R, k/K\n");
    while (1) {
        scanf(" %c", &sign);

        switch (sign) {
            case 'n':
            case 'N':
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
                u(&head);
                break;

            case 'r':
            case 'R':
                r(&head);
                break;

            case 'k':
            case 'K':
            {
                RD *current = head;
                RD *next;

                while (current != NULL) {
                    next = current->next;
                    free(current);
                    current = next;
                }
            }
                return 0;

            default:
                printf("Neexistuje dana moznost.\n");
                break;
        }
    }
}
