#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Affiche la grille avec les numéros de colonnes (1, 2, 3...)
void afficherGrilleSimple(char **grille, int rows, int cols) {
    printf("\nCols  : ");
    for (int j = 0; j < cols; j++) {
        printf("[%d]", j + 1);
    }
    printf("\n        ");
    for (int j = 0; j < cols; j++) {
        printf("---");
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("        ");
        for (int j = 0; j < cols; j++) {
            printf(" %c ", grille[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Chiffrement par colonnes simple (clé = nombre de colonnes)
void chiffrerSimple(const char *message, int cols, char *resultat) {
    int len_msg = strlen(message);
    int rows = (len_msg + cols - 1) / cols; // Arrondi supérieur

    // Allocation de la grille
    char **grille = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        grille[i] = malloc(cols * sizeof(char));
    }

    // 1. Remplissage ligne par ligne avec remplacement des espaces par '#'
    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (k < len_msg) {
                grille[i][j] = (message[k] == ' ') ? '#' : message[k];
                k++;
            } else {
                grille[i][j] = '#'; // Complète les cases vides
            }
        }
    }

    printf("\n--- MATRICE DE CHIFFREMENT (SIMPLE) ---");
    afficherGrilleSimple(grille, rows, cols);

    // 2. Lecture colonne par colonne dans l'ordre normal (0 à cols - 1)
    int idx = 0;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            resultat[idx++] = grille[i][j];
        }
    }
    resultat[idx] = '\0';

    for (int i = 0; i < rows; i++) free(grille[i]);
    free(grille);
}

// Déchiffrement par colonnes simple
void dechiffrerSimple(const char *chiffre, int cols, char *resultat) {
    int len_chiffre = strlen(chiffre);

    if (len_chiffre % cols != 0) {
        strcpy(resultat, "ERREUR : La longueur du message n'est pas un multiple du nombre de colonnes.");
        return;
    }

    int rows = len_chiffre / cols;

    char **grille = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        grille[i] = malloc(cols * sizeof(char));
    }

    // 1. Remplissage colonne par colonne de gauche à droite
    int idx = 0;
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            grille[i][j] = chiffre[idx++];
        }
    }

    printf("\n--- MATRICE DE DECHIFFREMENT (SIMPLE) ---");
    afficherGrilleSimple(grille, rows, cols);

    // 2. Lecture ligne par ligne et restauration des espaces
    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            resultat[k++] = (grille[i][j] == '#') ? ' ' : grille[i][j];
        }
    }
    resultat[k] = '\0';

    // Supprime les espaces de remplissage à la fin
    while (k > 0 && resultat[k - 1] == ' ') {
        resultat[--k] = '\0';
    }

    for (int i = 0; i < rows; i++) free(grille[i]);
    free(grille);
}

int main() {
    int choix, cols;
    char texte[1024];
    char resultat[1024];

    printf("=== TRANSPOSITION PAR COLONNES SIMPLE ===\n");
    printf("1. Chiffrer un message\n");
    printf("2. Dechiffrer un message\n");
    printf("Votre choix (1 ou 2) : ");
    scanf("%d", &choix);
    getchar(); // Nettoie le tampon clavier

    if (choix != 1 && choix != 2) {
        printf("Choix invalide.\n");
        return 1;
    }

    printf("Entrez le texte : ");
    fgets(texte, sizeof(texte), stdin);
    texte[strcspn(texte, "\n")] = '\0';

    printf("Entrez le nombre de colonnes (cle numerique > 0) : ");
    scanf("%d", &cols);

    if (cols <= 0) {
        printf("Erreur : le nombre de colonnes doit etre superieur a 0.\n");
        return 1;
    }

    if (choix == 1) {
        chiffrerSimple(texte, cols, resultat);
        printf("Resultat chiffre   : %s\n", resultat);
    } else {
        dechiffrerSimple(texte, cols, resultat);
        printf("Resultat dechiffre : %s\n", resultat);
    }

    return 0;
}