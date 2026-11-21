#include <stdio.h>
#include <string.h>

void chiffrementCesar(char texte[], int cle) {
    int i;

    // Ramener la clé entre 0 et 25
    cle = cle % 26;

    for (i = 0; texte[i] != '\0'; i++) {

        // Chiffrement des lettres majuscules
        if (texte[i] >= 'A' && texte[i] <= 'Z') {
            texte[i] = ((texte[i] - 'A' + cle) % 26) + 'A';
        }

        // Chiffrement des lettres minuscules
        else if (texte[i] >= 'a' && texte[i] <= 'z') {
            texte[i] = ((texte[i] - 'a' + cle) % 26) + 'a';
        }

        // Les espaces, chiffres et signes restent inchangés
    }
}

int main() {
    char texte[500];
    int cle;

    printf("====================================\n");
    printf("       CHIFFREMENT DE CESAR\n");
    printf("====================================\n");

    printf("Entrez le texte a chiffrer : ");
    fgets(texte, sizeof(texte), stdin);

    // Supprimer le retour à la ligne
    texte[strcspn(texte, "\n")] = '\0';

    printf("Entrez la cle de chiffrement : ");
    scanf("%d", &cle);

    chiffrementCesar(texte, cle);

    printf("\n------------------------------------\n");
    printf("Texte chiffre : %s\n", texte);
    printf("------------------------------------\n");

    return 0;
}