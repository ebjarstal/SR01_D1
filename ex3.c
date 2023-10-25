#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LIGNE 1000
#define MAX_TAB 100
#define MAX_CHEMIN 500


// structure position qui permet de stocker les coordonnées d'un restaurant
typedef struct Position Position;
struct Position {
    double x;
    double y;
};


// structure restaurant qui stocke les informations d'un restaurant
typedef struct Restaurant Restaurant; 
struct Restaurant {
    char *nom_restaurant;
    char *adresse_restaurant;
    Position position;
    char *specialite;
};


// fonction qui permet d'allouer dynamiquement de la mémoire à une structure Restaurant et qui renvoie l'adresse de la structure créée
Restaurant *creer_restaurant(char *nom, char *adresse, Position position, char *specialite);

// fonction qui lit le fichier désigné par 'chemin_global' et qui stocke les restaurants du fichier dans une structure de type Restaurant
void supprimerChar(char *str, char c);
int lire_ligne(char *ligne, Restaurant *restaurant);
int lire_restaurant (char *chemin, Restaurant *restaurants);

void inserer_restaurant(Restaurant restaurant, char *chemin);

double calculer_distance(double x1, double x2, double y1, double y2);
void cherche_restaurant(double x, double y, double rayon_recherche, Restaurant *restaurants, Restaurant *results);

void trier_restaurants_selon_distance(double x, double y, Restaurant *restaurants[]);
int verifier_doublon(Restaurant results[], Restaurant restaurant, int index);
void cherche_par_specialite(double x, double y, char *specialite[], Restaurant *results);

void afficher_menu(Restaurant *restaurants, int nb_restaurants);
int traiter_choix(Restaurant *restaurants, int nb_restaurants);
int traiter_lire_fichier(Restaurant *restaurants, int nb_restaurants);
void traiter_inserer_restaurant();
void traiter_chercher_restaurant(Restaurant *restaurants, int nb_restaurants);
void traiter_chercher_par_specialite(Restaurant *restaurants, int nb_restaurants);

char chemin_global[MAX_CHEMIN];

int main() {
//    Restaurant *restaurants = malloc(sizeof(Restaurant) * MAX_TAB);
//    if (restaurants == NULL) {
//        printf("Erreur allocation mémoire pour restaurants\n");
//        return 1;
//    }
//
//    int nb_restaurants = lire_restaurant(chemin_global, restaurants);
//    printf("%d restaurants dans le fichier.\n", nb_restaurants);
//
//    Restaurant results[nb_restaurants];  // on peut ajuster la taille comme on le souhaite
//    cherche_restaurant(12, 65, 200, restaurants, results);
//
//    char *specialites[MAX_TAB] = {"Cuisine gastronomique"};
//    cherche_par_specialite(12, 65, specialites, results);
//
//    // libère dynamiquement la mémoire allouée au tableau restaurants
//    for (int i = 0; i < nb_restaurants; i++) {
//        free(restaurants[i].nom_restaurant);
//        free(restaurants[i].adresse_restaurant);
//        free(restaurants[i].specialite);
//    }
//    free(restaurants);
//    printf("Mémoire libérée.\n");

    Restaurant *restaurants = malloc(sizeof(Restaurant) * MAX_TAB);
    if (restaurants == NULL) {
        printf("Erreur allocation mémoire pour restaurants\n");
        return 1;
    }

    int nb_restaurants = 0;
    int choix = 1;

    afficher_menu(restaurants, nb_restaurants);
    choix = traiter_choix(restaurants, nb_restaurants);

    while (choix) {
        afficher_menu(restaurants, nb_restaurants);
        choix = traiter_choix(restaurants, nb_restaurants);
    }

    return 0;
}


Restaurant *creer_restaurant(char *nom, char *adresse, Position position, char *specialite) {
    Restaurant *restaurant;
    restaurant = malloc(sizeof(Restaurant));
    if (!restaurant) {
        printf("Erreur allocation mémoire Restaurant\n");
    }

    restaurant->nom_restaurant = nom;
    restaurant->adresse_restaurant = adresse;
    restaurant->position.x = position.x;
    restaurant->position.y = position.y;
    restaurant->specialite = specialite;

    return restaurant;
}

void supprimerChar(char *str, char c) {
    int i, j;
    int len = strlen(str);
    for (i = j = 0; i < len; i++) {
        if (str[i] != c) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int lire_ligne(char *ligne, Restaurant *restaurant) {

    char nom[MAX_LIGNE];
    char adresse[MAX_LIGNE];
    char spos[MAX_LIGNE];
    Position pos;
    char specialite[MAX_LIGNE];

    sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];", nom, adresse, spos, specialite);

    sscanf(spos, "(x=%lf, y=%lf)", &pos.x, &pos.y);
    supprimerChar(specialite, '{');
    supprimerChar(specialite, '}');

    restaurant->nom_restaurant = strdup(nom);
    restaurant->adresse_restaurant = strdup(adresse);
    restaurant->position = pos;
    restaurant->specialite = strdup(specialite);

    return 1;
}


int lire_restaurant (char *chemin, Restaurant *restaurants) {
    int compteur = 0;
    FILE *fichier = fopen(chemin, "r");
    char ligne[MAX_LIGNE];

    while (fgets(ligne, MAX_LIGNE, fichier) != NULL) {
        Restaurant *restaurant;
        restaurant = malloc(sizeof(Restaurant));
        if (ligne[0] != '\n') {
            compteur += lire_ligne(ligne, restaurant);
            restaurants[compteur-1] = *restaurant;
        }
        // free(restaurant);
    }
    fclose(fichier);
    printf("Fichier lu. Liste des restaurants créée.\n");
    return compteur;
}


void inserer_restaurant(Restaurant restaurant, char *chemin) {
    char ligne[MAX_LIGNE];
    snprintf(ligne, MAX_LIGNE, "\n%s;%s;(x=%f, y=%f);{%s};", restaurant.nom_restaurant, restaurant.adresse_restaurant, restaurant.position.x, restaurant.position.y, restaurant.specialite);

    FILE *fichier = fopen(chemin, "a");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fprintf(fichier, "%s", ligne);
    fclose(fichier);
    printf("Restaurant inséré.\n");
}


double calculer_distance(double x1, double x2, double y1, double y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


void cherche_restaurant(double x, double y, double rayon_recherche, Restaurant *restaurants, Restaurant *results) {
    int k = 0;
    for (int i = 0; i < MAX_TAB; i++) {
        if (restaurants[i].nom_restaurant == NULL) {
            break;  // si on a atteint la fin du tableau de restaurants
        }
        if (calculer_distance(x, restaurants[i].position.x, y, restaurants[i].position.y) <= rayon_recherche) {
            results[k] = restaurants[i];
            k++;
        }
    }
    printf("%d restaurants trouvés à une distance inférieure à %.2lf unités de votre position.\n", k, rayon_recherche);
}


void trier_restaurants_selon_distance(double x, double y, Restaurant *restaurants[]) {

    printf("Liste des restaurants triée selon la distance de l'utilisateur.\n");
}


void cherche_par_specialite(double x, double y, char *specialite[], Restaurant *results) {
    int k = 0;
    for (int i = 0; i < MAX_TAB; i++) {
        if (results[i].nom_restaurant == NULL) {
            break;
        }
        int match = 0; // vérifie si la spécialité correspond à au moins une des spécialités recherchées

        for (int j = 0; specialite[j]; j++) {
            if (strcmp(results[i].specialite, specialite[j]) == 0) {
                match = 1;
                break; // sort de la boucle interne si une correspondance est trouvée
            }
        }

        // si une correspondance est trouvée et qu'il n'y a pas de doublon -> ajoute le restaurant aux résultats
        if (match && !verifier_doublon(results, results[i], i)) {
            results[k] = results[i];
            k++;
        }
    }

    printf("%d restaurants correspondant aux critères.\n", k);
}

int verifier_doublon(Restaurant *results, Restaurant restaurant, int index) {
    for (int i = 0; i < index; i++) {
        if (strcmp(results[i].adresse_restaurant, restaurant.adresse_restaurant) == 0) {
            return 1;
        }
    }
    return 0; // Renvoie 0 si aucun doublon trouvé
}



void afficher_menu(Restaurant *restaurants, int nb_restaurants) {
    printf("––– MENU UTILISATEUR –––\n");
    printf("(1) Lire fichier texte\n");
    printf("(2) Insérer un restaurant à la fin du fichier texte\n");
    printf("(3) Chercher un restaurant en fonction de votre position\n");
    printf("(4) Chercher un restaurant en fonction de sa spécialité et de votre position\n");
    printf("(0) Quitter le programme\n");
    printf("\nVotre choix: ");
}


int traiter_choix(Restaurant *restaurants, int nb_restaurants) {
    int choix;
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            traiter_lire_fichier(restaurants, nb_restaurants);
            break;
        case 2:
            traiter_inserer_restaurant();
            break;
        case 3:
            traiter_chercher_restaurant(restaurants, nb_restaurants);
            break;
        case 4:
            traiter_chercher_par_specialite(restaurants, nb_restaurants);
            break;
        case 0:
            exit(0);
        default:
            main();
            break;
    }

    return choix;
}


int traiter_lire_fichier(Restaurant *restaurants, int nb_restaurants) {
    printf("\n");
    printf("Entrer le chemin d'accès absolu du fichier texte:\n");
    scanf("%s", chemin_global);
    printf("\n");

    nb_restaurants = lire_restaurant(chemin_global, restaurants);
    printf("%d restaurants dans le fichier.\n", nb_restaurants);

    return 0;
}


void traiter_inserer_restaurant() {
    char nom[MAX_TAB];
    char adresse[MAX_TAB];
    double x;
    double y;
    char specialite[MAX_TAB];

    printf("Entrez le nom du restaurant: ");
    scanf(" %[^\n]s", nom);

    printf("Entrez l'adresse du restaurant: ");
    scanf(" %[^\n]s", adresse);

    printf("Entrez la position x du restaurant: ");
    scanf("%lf", &x);

    printf("Entrez la position y du restaurant: ");
    scanf("%lf", &y);

    printf("Entrez la spécialité du restaurant: ");
    scanf(" %[^\n]s", specialite);

    Position pos;
    pos.x = x;
    pos.y = y;

    Restaurant *restaurant = creer_restaurant(nom, adresse, pos, specialite);

    inserer_restaurant(*restaurant, chemin_global);

    free(restaurant);
}

void traiter_chercher_restaurant(Restaurant *restaurants, int nb_restaurants) {
    double x, y, rayon_recherche;

    printf("Entrez votre position (coordonnée x) : ");
    scanf("%lf", &x);

    printf("Entrez votre position (coordonnée y) : ");
    scanf("%lf", &y);

    printf("Entrez le rayon de recherche : ");
    scanf("%lf", &rayon_recherche);

    Restaurant results[MAX_TAB];
    cherche_restaurant(x, y, rayon_recherche, restaurants, results);

    printf("––– Restaurants trouvés –––\n");

    for (int i = 0; i < MAX_TAB; i++) {
        if (results[i].nom_restaurant == NULL) {
            break;
        }
        printf("Nom : %s\n", results[i].nom_restaurant);
        printf("Adresse : %s\n", results[i].adresse_restaurant);
        printf("Position (x=%lf, y=%lf)\n", results[i].position.x, results[i].position.y);
        printf("Spécialité : %s\n", results[i].specialite);
        printf("\n");
    }
}

void traiter_chercher_par_specialite(Restaurant *restaurants, int nb_restaurants) {
    double x, y;
    char specialite[MAX_TAB];

    printf("Entrez votre position (coordonnée x) : ");
    scanf("%lf", &x);

    printf("Entrez votre position (coordonnée y) : ");
    scanf("%lf", &y);

    printf("Entrez la spécialité recherchée : ");
    scanf(" %[^\n]s", specialite);

    Restaurant results[MAX_TAB];
    cherche_par_specialite(x, y, (char **) specialite, results);

    printf("––– Restaurants trouvés –––\n");

    for (int i = 0; i < MAX_TAB; i++) {
        if (results[i].nom_restaurant == NULL) {
            break;
        }
        printf("Nom : %s\n", results[i].nom_restaurant);
        printf("Adresse : %s\n", results[i].adresse_restaurant);
        printf("Position (x=%lf, y=%lf)\n", results[i].position.x, results[i].position.y);
        printf("Spécialité : %s\n", results[i].specialite);
        printf("\n");
    }
}

