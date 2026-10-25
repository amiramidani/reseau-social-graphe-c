#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED
#include "ELTUSER.h"

// NOEUD DE LA LISTE D'ADJACENCE
typedef struct structNode {
    ELEMENT user;
    int weight;
    struct structNode* next;
} structNode, *Node;

//graphe
typedef struct {
    int V;
    ELEMENT* utilisateurs;
    Node* adjList;
} *graphePondere;
//les primitives
graphePondere creerGraphe(int V);
void detruireGraphe(graphePondere g);
int ajouterUtilisateur(graphePondere g, ELEMENT user);
int ajouterRelation(graphePondere g, int src, int dest, int poids);
int supprimerRelation(graphePondere g, int src, int dest);
int supprimerUtilisateur(graphePondere g, int idUser);
void afficherGraphe(graphePondere g);
graphePondere chargerGraphe(const char* filename);
void sauvegarderGraphe(graphePondere g, const char* filename);

//les algorithmes
void dfs(graphePondere g, int idUser);
void bfs(graphePondere g, int idUser);
void dijkstra(graphePondere g, int src_id, int dest_id);
void trouver_communautes(graphePondere g);


#endif
