#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTUSER.h"

ELEMENT elementCreer() {
    ELEMENT e = (ELEMENT)malloc(sizeof(UtilisateurStruct));
    if (e) {
        e->id = 0;
        strcpy(e->nom, "");
        e->age = 0;
    }
    return e;
}

void elementLire(ELEMENT *e) {
    *e = elementCreer();
    printf("ID : "); scanf("%d", &(*e)->id);
    getchar();
    printf("Nom : "); fgets((*e)->nom, 50, stdin);
    (*e)->nom[strcspn((*e)->nom, "\n")] = '\0';
    printf("Age : "); scanf("%d", &(*e)->age);
}

void elementAfficher(ELEMENT e) {
    printf("ID: %d | Nom: %s | Âge: %d\n", e->id, e->nom, e->age);
}

void elementDetruire(ELEMENT e) {
    free(e);
}
