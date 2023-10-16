#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LIGNE 1000
#define MAX_TAB 100


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
int verifier_doublon(Restaurant restaurants[], Restaurant restaurant);
void cherche_par_specialite(double x, double y, char *specialite[], Restaurant *results);

char *chemin_global = "/Users/ericb/Desktop/UTC/TC04/SR01/SR01_D1/restau.txt";

int main() {
    Restaurant *restaurants = malloc(sizeof(Restaurant) * MAX_TAB);
    if (restaurants == NULL) {
        printf("Erreur allocation mémoire pour restaurants\n");
        return 1;
    }

    int nb_restaurants = lire_restaurant(chemin_global, restaurants);
    printf("%d restaurants dans le fichier.\n", nb_restaurants);

    Restaurant results[nb_restaurants];  // on peut ajuster la taille comme on le souhaite
    cherche_restaurant(18.5, 7.5, 200, restaurants, results);

    char *specialites[MAX_TAB] = {"Cuisine gastronomique"};
    cherche_par_specialite(18.5, 7.5, specialites, results);

    // libère dynamiquement la mémoire allouée au tableau restaurants
    for (int i = 0; i < nb_restaurants; i++) {
        free(restaurants[i].nom_restaurant);
        free(restaurants[i].adresse_restaurant);
        free(restaurants[i].specialite);
    }
    free(restaurants);
    printf("Mémoire libérée.\n");

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
        free(restaurant);
    }
    fclose(fichier);
    printf("Fichier lu. Liste des restaurants créée.\n");
    return compteur;
}


void inserer_restaurant(Restaurant restaurant, char *chemin) {
    char ligne[MAX_LIGNE];
    snprintf(ligne, MAX_LIGNE, "\n%s;%s;(x=%f, y=%f);{%s};", restaurant.nom_restaurant, restaurant.adresse_restaurant, restaurant.position.x, restaurant.position.y, restaurant.specialite);

    FILE *fichier = fopen(chemin, "a");
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


int verifier_doublon(Restaurant *restaurants, Restaurant restaurant) {
    for (int i = 0; &restaurants[i] != NULL; i++) {
        if (strcmp(restaurants[i].adresse_restaurant, restaurant.adresse_restaurant) == 0) {
            return 1;
        }
    }
    return 0;  // renvoie 0 si aucun doublon trouvé
}


void cherche_par_specialite(double x, double y, char *specialite[], Restaurant results[]) {
    Restaurant *restaurants_potentiels = malloc(sizeof(Restaurant)*MAX_TAB);

    int i, k = 0;
    for (i = 0; i < MAX_TAB; i++) {
        if (results[i].nom_restaurant == NULL) {
            break;
        }
        for (int j = 0; j < MAX_TAB && specialite[j]; j++) {
//            if (strcmp(results[i].specialite, specialite[j]) == 0) {
//                if (verifier_doublon(restaurants_potentiels, results[i]) == 0) {
//                    restaurants_potentiels[k] = results[i];
//                    k++;
//                }
//            }
        }
//        printf("%s\n", results[i].nom_restaurant);
    }

    printf("%d restaurants correspondant aux critères.\n", k);
    free(restaurants_potentiels);
}
