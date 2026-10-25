#include <stdio.h>
#include <stdlib.h>
#include "GRAPHE.h"
#include "ELTUSER.h"

void afficherMenu() {
    printf("====== MENU RESEAU SOCIAL ======\n");
    printf("1. =====Charger un graphe depuis un fichier=====\n");
    printf("2. =====Ajouter un utilisateur=====\n");
    printf("3. ======Ajouter une relation======\n");
    printf("4. ======Supprimer une relation======\n");
    printf("5. =====Supprimer un utilisateur======\n");
    printf("6. =====Afficher le graphe=====\n");
    printf("7. =====Sauvegarder le graphe dans un fichier=====\n");
    printf("8. =====DFS - Explorer les amis directs et indirects======\n");
    printf("9. ====BFS - Explorer les niveaux d’amis=====\n");
    printf("10.===== Dijkstra - Chemin le plus fort entre deux utilisateurs======\n");
    printf("11. =====Détecter les communautés=====\n");
    printf("0. =====Quitter=====\n");
    printf("Votre choix : ");
}

void pause() {
    printf("\nAppuyez sur Entrée pour continuer...");
    getchar();
    getchar(); // pause propre
}

int main() {
    graphePondere g = NULL;
    int choix, id1, id2, poids;
    char nomFichier[100];

    do {
        system("cls"); // Effacer l'écran à chaque tour de boucle
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // consomme \n

        switch (choix) {
            case 1:
                printf("Nom du fichier à charger : ");
                scanf("%s", nomFichier);
                g = chargerGraphe(nomFichier); // utiliser le nom saisi
                afficherGraphe(g);
                pause();
                break;

            case 2: {
                ELEMENT e = elementCreer();
                elementLire(&e);
                if (ajouterUtilisateur(g, e)) {
                    printf("Utilisateur ajouté.\n");
                    afficherGraphe(g);
                } else {
                    printf("Erreur d’ajout.\n");
                }
                pause();
                break;
            }

            case 3:
                printf("ID source : "); scanf("%d", &id1);
                printf("ID destination : "); scanf("%d", &id2);
                printf("Poids de la relation : "); scanf("%d", &poids);
                if (ajouterRelation(g, id1, id2, poids)) {
                    printf("Relation ajoutée.\n");
                    afficherGraphe(g);
                } else {
                    printf("Erreur d’ajout.\n");
                }
                pause();
                break;

            case 4:
                printf("ID source : "); scanf("%d", &id1);
                printf("ID destination : "); scanf("%d", &id2);
                if (supprimerRelation(g, id1, id2)) {
                    printf("Relation supprimée.\n");
                    afficherGraphe(g);
                } else {
                    printf("Erreur de suppression.\n");
                }
                pause();
                break;

            case 5:
                printf("ID de l'utilisateur à supprimer : ");
                scanf("%d", &id1);
                if (supprimerUtilisateur(g, id1)) {
                    printf("Utilisateur supprimé.\n");
                    afficherGraphe(g);
                } else {
                    printf("Erreur de suppression.\n");
                }
                pause();
                break;

            case 6:
                afficherGraphe(g);
                pause();
                break;

            case 7:
                printf("Nom du fichier de sauvegarde : ");
                scanf("%s", nomFichier);
                sauvegarderGraphe(g, nomFichier);
                pause();
                break;

            case 8: {
                printf("ID utilisateur pour DFS : ");
                scanf("%d", &id1);
                int* visite = (int*)calloc(g->V, sizeof(int));
                dfs(g, id1);
                free(visite);
                pause();
                break;
            }

            case 9:
                printf("ID utilisateur pour BFS : ");
                scanf("%d", &id1);
                bfs(g, id1);
                pause();
                break;

            case 10:
                printf("ID source : "); scanf("%d", &id1);
                printf("ID destination : "); scanf("%d", &id2);
                dijkstra(g, id1, id2);
                pause();
                break;

            case 11:
                trouver_communautes(g);
                pause();
                break;

            case 0:
                printf("Fermeture du programme.\n");
                break;

            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while (choix != 0);

    detruireGraphe(g);
    return 0;
}
