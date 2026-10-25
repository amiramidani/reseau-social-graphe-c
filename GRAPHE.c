#include <stdio.h>
#include <stdlib.h>
#include "GRAPHE.h"
#include "ELTUSER.h"

graphePondere creerGraphe(int V) {
    graphePondere g = (graphePondere)malloc(sizeof(*g));
    if (!g) return NULL;

    g->V = V;
    g->utilisateurs = (ELEMENT*)malloc(V * sizeof(ELEMENT));
    g->adjList = (Node*)malloc(V * sizeof(Node));

    for (int i = 0; i < V; i++) {
        g->utilisateurs[i] = NULL;
        g->adjList[i] = NULL;
    }

    return g;
}


void detruireGraphe(graphePondere g) {
    if (!g) return;

    for (int i = 0; i < g->V; i++) {
        elementDetruire(g->utilisateurs[i]);

        Node curr = g->adjList[i];
        while (curr) {
            Node tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }

    free(g->utilisateurs);
    free(g->adjList);
    free(g);
}


int ajouterUtilisateur(graphePondere g, ELEMENT user) {
    if (!g || !user || user->id < 0)
        return 0;

    // Redimensionner si id > V - 1
    if (user->id >= g->V) {
        int newSize = user->id + 1;

        ELEMENT* newUsers = realloc(g->utilisateurs, newSize * sizeof(ELEMENT));
        Node* newAdj = realloc(g->adjList, newSize * sizeof(Node));

        if (!newUsers || !newAdj) {
            printf("Erreur de reallocation memoire.\n");
            return 0;
        }

        // Initialiser les nouvelles cases
        for (int i = g->V; i < newSize; i++) {
            newUsers[i] = NULL;
            newAdj[i] = NULL;
        }

        g->utilisateurs = newUsers;
        g->adjList = newAdj;
        g->V = newSize; // Mise à jour de la taille du graphe
    }

    if (g->utilisateurs[user->id] != NULL)
        return 0; // utilisateur déjà existant

    ELEMENT copie = elementCreer();
    copie->id = user->id;
    copie->age = user->age;
    strcpy(copie->nom, user->nom);

    g->utilisateurs[user->id] = copie;
    return 1;
}


int ajouterRelation(graphePondere g, int src, int dest, int poids) {
    if (!g || src < 0 || dest < 0 || src >= g->V || dest >= g->V)
        return 0;

    if (!g->utilisateurs[src] || !g->utilisateurs[dest])
        return 0;

    // Vérifier si la relation existe déjà (dans src -> dest)
    Node current = g->adjList[src];
    while (current) {
        if (current->user->id == dest) {
            current->weight = poids; // mise à jour du poids
            return 1;
        }
        current = current->next;
    }

    // Ajouter la nouvelle relation dans src -> dest
    Node newNode = (Node)malloc(sizeof(structNode));
    newNode->user = g->utilisateurs[dest];
    newNode->weight = poids;
    newNode->next = g->adjList[src];
    g->adjList[src] = newNode;

    // Si relation bidirectionnelle : ajouter dest -> src
    Node newNode2 = (Node)malloc(sizeof(structNode));
    newNode2->user = g->utilisateurs[src];
    newNode2->weight = poids;
    newNode2->next = g->adjList[dest];
    g->adjList[dest] = newNode2;

    return 1;
}


int supprimerRelation(graphePondere g, int src, int dest) {
    if (!g || src < 0 || dest < 0 || src >= g->V || dest >= g->V)
        return 0;

    // Supprimer dans src -> dest
    Node *p = &g->adjList[src];
    while (*p) {
        if ((*p)->user->id == dest) {
            Node tmp = *p;
            *p = (*p)->next;
            free(tmp);
            break;
        }
        p = &(*p)->next;
    }

    // Supprimer dans dest -> src (relation bidirectionnelle)
    p = &g->adjList[dest];
    while (*p) {
        if ((*p)->user->id == src) {
            Node tmp = *p;
            *p = (*p)->next;
            free(tmp);
            break;
        }
        p = &(*p)->next;
    }

    return 1;
}


int supprimerUtilisateur(graphePondere g, int idUser) {
    if (!g || idUser < 0 || idUser >= g->V || !g->utilisateurs[idUser])
        return 0;

    // Supprimer toutes les relations vers cet utilisateur
    for (int i = 0; i < g->V; i++) {
        if (i != idUser) {
            supprimerRelation(g, i, idUser);
        }
    }

    // Supprimer ses propres relations
    Node curr = g->adjList[idUser];
    while (curr) {
        Node tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    g->adjList[idUser] = NULL;

    // Supprimer l'utilisateur
    elementDetruire(g->utilisateurs[idUser]);
    g->utilisateurs[idUser] = NULL;

    return 1;
}


void afficherGraphe(graphePondere g) {
    if (!g) return;

    printf("=== Graphe du Réseau Social ===\n");
    for (int i = 0; i < g->V; i++) {
        if (g->utilisateurs[i]) {
            printf("%s (%d)", g->utilisateurs[i]->nom, g->utilisateurs[i]->id);

            Node curr = g->adjList[i];
            while (curr) {
                printf(" --%d--> %s (%d)", curr->weight, curr->user->nom, curr->user->id);
                curr = curr->next;
            }
            printf("\n");
        }
    }
}

graphePondere chargerGraphe(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Erreur : fichier %s introuvable.\n", filename);
        return NULL;
    }

    // Étape 1 : Compter le nombre d’utilisateurs pour connaître V
    int maxId = -1;
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == 'U') {
            int id, age;
            char nom[50];
            if (sscanf(ligne, "U %d %s %d", &id, nom, &age) == 3) {
                if (id > maxId) maxId = id;
            }
        }
    }

    // Création du graphe avec V = maxId + 1
    graphePondere g = creerGraphe(maxId + 1);
    rewind(f); // retour au début du fichier

    // Étape 2 : Lecture réelle du fichier
    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == 'U') {
            int id, age;
            char nom[50];
            if (sscanf(ligne, "U %d %s %d", &id, nom, &age) == 3) {
                ELEMENT e = elementCreer();
                e->id = id;
                strcpy(e->nom, nom);
                e->age = age;
                ajouterUtilisateur(g, e);
                elementDetruire(e); // détruire la copie locale
            }
        } else if (ligne[0] == 'R') {
            int src, dest, poids;
            if (sscanf(ligne, "R %d %d %d", &src, &dest, &poids) == 3) {
                ajouterRelation(g, src, dest, poids);
            }
        }
    }

    fclose(f);
    return g;
}


void sauvegarderGraphe(graphePondere g, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Erreur : impossible d’ouvrir le fichier %s pour l’écriture.\n", filename);
        return;
    }

    // Écriture des utilisateurs
    for (int i = 0; i < g->V; i++) {
        if (g->utilisateurs[i]) {
            fprintf(f, "U %d %s %d\n", g->utilisateurs[i]->id, g->utilisateurs[i]->nom, g->utilisateurs[i]->age);
        }
    }

    // Écriture des relations
    for (int i = 0; i < g->V; i++) {
        Node curr = g->adjList[i];
        while (curr) {
            int id1 = i;
            int id2 = curr->user->id;

            // Pour éviter les doublons, on n'écrit qu’une seule direction (id1 < id2)
            if (id1 < id2) {
                fprintf(f, "R %d %d %d\n", id1, id2, curr->weight);
            }

            curr = curr->next;
        }
    }

    fclose(f);
    printf("Graphe sauvegardé dans le fichier %s.\n", filename);
}





void dfs(graphePondere g, int idUser) {
    int visite[g->V];
    for (int i = 0; i < g->V; i++) visite[i] = 0;

    printf("DFS à partir de %s (%d) :\n", g->utilisateurs[idUser]->nom, idUser);
    dfsRec(g, idUser, visite);
}
void dfsRec(graphePondere g, int id, int visite[]) {
    visite[id] = 1;
    printf("%s (%d)  ", g->utilisateurs[id]->nom, id);

    Node voisin = g->adjList[id];
    while (voisin != NULL) {
        int v = voisin->user->id;
        if (!visite[v]) {
            dfsRec(g, v, visite);
        }
        voisin = voisin->next;
    }
}


void bfs(graphePondere g, int idUser) {

    // Vérifie que l'id est valide et que l'utilisateur existe
    if (idUser < 0 || idUser >= g->V || g->utilisateurs[idUser] == NULL)
        return;

    int* visite = (int*)calloc(g->V, sizeof(int));       // Tableau pour marquer les utilisateurs déjà visités
    int* file = (int*)malloc(g->V * sizeof(int));         // File (queue) pour stocker les utilisateurs à visiter
    int debut = 0, fin = 0;                               // Indices de début et fin de la file

    visite[idUser] = 1;                                   // Marque le point de départ comme visité
    file[fin++] = idUser;                                 // Ajoute l'utilisateur de départ dans la file

    printf("Utilisateurs accessibles depuis %s (BFS) :\n", g->utilisateurs[idUser]->nom);

    while (debut < fin) {                                 // Tant que la file n'est pas vide
        int courant = file[debut++];                      // Prend l'utilisateur au début de la file
        elementAfficher(g->utilisateurs[courant]);        // Affiche ses informations

        Node voisin = g->adjList[courant];                // Récupère la liste de ses voisins

        while (voisin != NULL) {                          // Parcourt tous ses voisins
            int idVoisin = voisin->user->id;              // Récupère l'id du voisin

            if (!visite[idVoisin]) {                      // Si ce voisin n'a pas encore été visité
                visite[idVoisin] = 1;                     // On le marque comme visité
                file[fin++] = idVoisin;                   // Et on l'ajoute à la file pour le traiter plus tard
            }

            voisin = voisin->next;                        // Passe au voisin suivant
        }
    }

    free(visite);                                         // Libère la mémoire du tableau de visite
    free(file);                                           // Libère la mémoire de la file
}


#define INF 1000000

void dijkstra(graphePondere g, int src_id, int dest_id) {
    int* distance = (int*)malloc(g->V * sizeof(int));      // distance[i] : distance min connue de src à i
    int* visite = (int*)calloc(g->V, sizeof(int));         // visite[i] : 1 si le sommet i a été traité
    int* precedent = (int*)malloc(g->V * sizeof(int));     // precedent[i] : sommet précédent sur le chemin vers i

    // Initialisation
    for (int i = 0; i < g->V; i++) {
        distance[i] = INF;        // On suppose que tout est infiniment loin au départ
        precedent[i] = -1;        // Pas encore de chemin trouvé
    }

    distance[src_id] = 0;         // Distance de départ à lui-même = 0

    //  Étape principale : répéter pour tous les sommets
    for (int count = 0; count < g->V - 1; count++) {
        int min = INF;
        int u = -1;

        // Trouver le sommet non visité avec la distance minimale
        for (int v = 0; v < g->V; v++) {
            if (!visite[v] && distance[v] < min) {
                min = distance[v];
                u = v;
            }
        }

        // Si aucun sommet non visité accessible → on continue à itérer, mais on ne fait rien ce tour
        if (u != -1) {
            visite[u] = 1;  // Marquer comme visité

            // Mettre à jour les distances de ses voisins
            Node voisin = g->adjList[u];
            while (voisin != NULL) {
                int v = voisin->user->id;

                if (!visite[v] && distance[u] + voisin->weight < distance[v]) {
                    distance[v] = distance[u] + voisin->weight;
                    precedent[v] = u;  // On enregistre le chemin
                }

                voisin = voisin->next;
            }
        }
    }

    //  Affichage du chemin trouvé
    if (distance[dest_id] == INF) {
        printf("Aucun chemin entre %s et %s\n", g->utilisateurs[src_id]->nom, g->utilisateurs[dest_id]->nom);
    } else {
        printf("Chemin le plus court de %s à %s :\n", g->utilisateurs[src_id]->nom, g->utilisateurs[dest_id]->nom);

        // Reconstruction du chemin (inversé)
        int chemin[100], index = 0;
        int v = dest_id;
        while (v != -1) {
            chemin[index++] = v;
            v = precedent[v];
        }

        //  Affichage dans le bon ordre
        for (int i = index - 1; i >= 0; i--) {
            printf("%s", g->utilisateurs[chemin[i]]->nom);
            if (i > 0) printf(" → ");
        }
        printf("\nPoids total : %d\n", distance[dest_id]);
    }

    // libération mémoire
    free(distance);
    free(visite);
    free(precedent);
}



void trouver_communautes(graphePondere g) {

    int* visite = (int*)calloc(g->V, sizeof(int));      // Marque les utilisateurs déjà visités (0 = non visité)
    int* queue = (int*)malloc(g->V * sizeof(int));      // File pour parcourir chaque communauté avec BFS
    int num = 1;                                        // Numéro de communauté à afficher

    // Parcourir tous les utilisateurs du graphe
    for (int i = 0; i < g->V; i++) {
        // Si l'utilisateur existe et n'a pas encore été visité
        if (g->utilisateurs[i] != NULL && !visite[i]) {
            printf("Communauté %d : ", num++);          // On commence une nouvelle communauté

            int front = 0, rear = 0;                    // Réinitialise les indices de la file
            queue[rear++] = i;                          // Ajoute le premier utilisateur de la communauté
            visite[i] = 1;                               // Le marque comme visité

            // Parcours BFS de la communauté
            while (front < rear) {
                int courant = queue[front++];           // Prend l'utilisateur courant dans la file
                printf("%s ", g->utilisateurs[courant]->nom); // Affiche son nom

                Node voisin = g->adjList[courant];      // Récupère ses voisins

                // Parcourt tous les voisins de l'utilisateur courant
                while (voisin != NULL) {
                    int idVoisin = voisin->user->id;

                    if (!visite[idVoisin]) {            // Si ce voisin n'a pas été visité
                        visite[idVoisin] = 1;           // On le marque
                        queue[rear++] = idVoisin;       // Et on l'ajoute dans la file
                    }

                    voisin = voisin->next;              // Passe au voisin suivant
                }
            }

            printf("\n");                               // Saute une ligne pour passer à la prochaine communauté
        }
    }

    free(visite);                                       // Libère le tableau des visites
    free(queue);                                        // Libère la file
}
