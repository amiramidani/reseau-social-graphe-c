#ifndef ELTUSER_H_INCLUDED
#define ELTUSER_H_INCLUDED

typedef struct {
    int id;
    char nom[50];
    int age;
} UtilisateurStruct, *ELEMENT;

ELEMENT elementCreer();
void elementLire(ELEMENT *e);
void elementAfficher(ELEMENT e);
void elementDetruire(ELEMENT e);

#endif // ELTUSER_H_INCLUDED
