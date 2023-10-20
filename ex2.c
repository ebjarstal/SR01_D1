#include <stdio.h>
#define MAXN 6
#define NB_LIGNES (MAXN+2)
#define NB_COLONNES 8

// Fonction pour trouver la note maximale :
double trouverNoteMaximale(const double POINTS[], int N) {
    double max = POINTS[0];
    for (int i = 1; i < N; i++) {
        if (POINTS[i] > max) {
            max = POINTS[i];
        }
    }
    return max;
}

// Fonction pour trouver la note minimale :
double trouverNoteMinimale(const double POINTS[], int N) {
    double min = POINTS[0];
    for (int i = 1; i < N; i++) {
        if (POINTS[i] < min) {
            min = POINTS[i];
        }
    }
    return min;
}

// Fonction pour calculer la moyenne des notes :
double calculerMoyenne(const double POINTS[], int N) {
    double somme = 0.0;
    for (int i = 0; i < N; i++) {
        somme += POINTS[i];
    }
    return somme / N;
}

void classerNotes(const double POINTS[], int N, int NOTES[7]) {
    // On initialise le tableau NOTES à zéro :
    for (int i = 0; i < 7; i++) {
        NOTES[i] = 0;
    }

    // On classe les notes dans les intervalles appropriés :
    for (int i = 0; i < N; i++) {
        if (POINTS[i] >= 60) {
            NOTES[6]++;
        } else if (POINTS[i] >= 50) {
            NOTES[5]++;
        } else if (POINTS[i] >= 40) {
            NOTES[4]++;
        } else if (POINTS[i] >= 30) {
            NOTES[3]++;
        } else if (POINTS[i] >= 20) {
            NOTES[2]++;
        } else if (POINTS[i] >= 10) {
            NOTES[1]++;
        } else {
            NOTES[0]++;
        }
    }
}

int main() {

    int N;

    printf("Entrez le nombre d'étudiants : ");
    scanf("%d", &N);

    double POINTS[N];

    for (int i = 0; i < N; i++) {
        printf("Entrez la note de l'étudiant %d : ", i + 1);
        scanf("%lf", &POINTS[i]);
    }

    printf("Les notes des étudiants sont :\n");
    for (int i = 0; i < N; i++) {
        printf("Etudiant %d : %.2lf\n", i + 1, POINTS[i]);
    }

    double noteMax = trouverNoteMaximale(POINTS, N);
    double noteMin = trouverNoteMinimale(POINTS, N);
    double moyenne = calculerMoyenne(POINTS, N);

    printf("La note maximale du devoir de SR01 est : %.2lf\n", noteMax);
    printf("La note minimale du devoir de SR01 est : %.2lf\n", noteMin);
    printf("La moyenne des notes du devoir SR01 est : %.2lf\n", moyenne);

    int NOTES[7]; // Tableau pour classer les notes

    classerNotes(POINTS, N, NOTES);

    printf("Répartition des notes :\n");
    for (int i = 6; i >= 0; i--) {
        printf("NOTES[%d] : %d\n", i, NOTES[i]);
    }
    //Graphique en nuage de points:
    printf("Graphique de répartition des notes en nuage de points :\n");

    for (int i = 0; i < NB_LIGNES; i++) {
        if (NB_LIGNES-(i+1) > 1) {
            printf("%d > ", NB_LIGNES-(i+2));
            printf("\t");
        }
        else {
            printf("\t");
        }
        for (int j = 0; j < NB_COLONNES; j++) {
            // Affichage de l'axe des abscisses.
            //Vérification des cas où on est sur la première colonne, la dernière colonne ou une colonne quelconque.
            //Cela permet un formatage plus propre.
            //On vérifie de plus si la donnée correspondante à la colonne est nulle pour l'afficher directement sur l'abscisse.
            if (NB_LIGNES-(i+1) == 1) {
                if (j==0 && NOTES[j] == 0){
                    printf("----o---+");
                }
                else if (j==0 && NOTES[j] != 0){
                    printf("--------+");
                }
                else if (j<7 && j*10 == 60 && NOTES[j] == 0) {
                    printf("---o--+");
                }
                else if (j<7 && j*10 == 60 && NOTES[j] != 0) {
                    printf("------+");
                }
                else if (j<7 && j*10<60 && NOTES[j] == 0){
                    printf("---o---+");
                }
                else if (j<7 && j*10<60 && NOTES[j] != 0){
                    printf("-------+");
                }
            }
            // Affichage de la graduation de l'axe des abscisses.
            else if (NB_LIGNES-(i+1) == 0) {
                //Comme sur l'axe des abscisses:
                //Vérification des cas où on est sur la première colonne, la dernière colonne ou une colonne quelconque.
                //On adapte le formatage en conséquence.
                if (j*10 < 10 && ((j+1)*10)-1 < 10){
                    printf("| %d - %d ", j * 10, ((j + 1) * 10) - 1);
                }
                else if ((j*10) < 60){
                    printf("| %d-%d ", j * 10, ((j + 1) * 10) - 1);
                }
                else if (j*10 == 60){
                    printf("|  60  |");
                }
            }
            else {
                if (j < 7 && NOTES[j] == NB_LIGNES-(i+2)) {
                    if (j*10 < 10 && ((j+1)*10)-1 < 10) {
                        printf("    o    ");
                    }
                    else {
                        printf("   o    ");
                    }
                }
                else {
                    if (j*10 < 10 && ((j+1)*10)-1 < 10) {
                        printf("         ");
                    }
                    else {
                        printf("        ");
                    }
                }
            }
        }
        printf("\n");
    }

    // Graphique en barres:
    printf("Graphique de répartition des notes en barres :\n");

    for (int i = 0; i < NB_LIGNES; i++) {
        if (NB_LIGNES-(i+1) > 1) {
            printf("%d > ", NB_LIGNES-(i+2));
            printf("\t");
        }
        else {
            printf("\t");
        }
        for (int j = 0; j < NB_COLONNES; j++) {
            // Affichage de l'axe des abscisses.
            //Vérification des cas où on est sur la première colonne, la dernière colonne ou une colonne quelconque.
            //Cela permet un formatage plus propre.
            //On vérifie de plus si la donnée correspondante à la colonne est nulle pour l'afficher directement sur l'abscisse.
            if (NB_LIGNES-(i+1) == 1) {
                if (j==0){
                    printf("--------+");
                }
                else if (j*10 == 60) {
                    printf("------+");
                }
                else if (j*10<60){
                    printf("-------+");
                }
            }
            // Affichage de la graduation de l'axe des abscisses.
            else if (NB_LIGNES-(i+1) == 0) {
                //Comme sur l'axe des abscisses:
                //Vérification des cas où on est sur la première colonne, la dernière colonne ou une colonne quelconque.
                //On adapte le formatage en conséquence.
                if (j*10 < 10 && ((j+1)*10)-1 < 10){
                    printf("| %d - %d ", j * 10, ((j + 1) * 10) - 1);
                }
                else if ((j*10) < 60){
                    printf("| %d-%d ", j * 10, ((j + 1) * 10) - 1);
                }
                else if (j*10 == 60){
                    printf("|  60  |");
                }
            }
            else {
                //La différence entre le graphique en nuage de points et le graphique en barre
                //réside dans la condition qui suit:
                if (j < 7 && NOTES[j] >= NB_LIGNES-(i+2)) {
                    //On affiche à chaque ligne en dessous du point de donnée.
                    //On met des conditions sur le formatage pour aligner le tout.
                    if (j*10 < 10 && ((j+1)*10)-1 < 10) {
                        printf(" ####### ");
                    }
                    else if (j*10 == 60) {
                        printf(" ##### ");
                    }
                    else {
                        printf(" ###### ");
                    }
                }
                else {
                    if (j*10 < 10 && ((j+1)*10)-1 < 10) {
                        printf("         ");
                    }
                    else {
                        printf("        ");
                    }
                }
            }
        }
        printf("\n");
    }
    return 0;
}