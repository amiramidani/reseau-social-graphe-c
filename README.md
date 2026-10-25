# Réseau social : graphe pondéré en C

Projet d'algorithmique et structures de données (1re année Licence Informatique de Gestion).
Chaque utilisateur est un sommet, chaque relation est une arête pondérée par la force de la connexion.

## Fonctionnalités
- Chargement et sauvegarde du graphe depuis un fichier
- Ajout et suppression d'utilisateurs et de relations
- Affichage du graphe (liste d'adjacence)
- Parcours en profondeur (DFS) et en largeur (BFS)
- Chemin le plus fort entre deux utilisateurs (Dijkstra)
- Détection de communautés

## Structure du projet
- `main.c` : menu principal
- `GRAPHE.c` / `GRAPHE.h` : structure du graphe et algorithmes
- `ELTUSER.c` / `ELTUSER.h` : structure d'un utilisateur
- `graph1.txt` : exemple de graphe
- `Rapport du projet ASD.pdf` : rapport complet

## Lancer le projet
Compilation : `gcc main.c GRAPHE.c ELTUSER.c -o reseau`
Exécution : `./reseau`
(Le menu utilise `system("cls")`, donc il est prévu pour Windows.)

## Outils
Langage C, Code::Blocks, gcc

## Auteures
Amira Midani et Eya Ibn Abdelhedi
