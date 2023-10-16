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

// fonction qui lit le fichier désigné par 'chemin' et qui stocke les restaurants du fichier dans une structure de type Restaurant
void supprimerChar(char *str, char c);
int lire_ligne(char *ligne, Restaurant *restaurant);
int lire_restaurant (char *chemin, Restaurant *restaurants);

void inserer_restaurant(Restaurant restaurant, char *chemin);

double calculer_distance(double x1, double x2, double y1, double y2);
void cherche_restaurant(double x, double y, double rayon_recherche, Restaurant *restaurants, Restaurant *results);

void trier_restaurants_selon_distance(double x, double y, Restaurant *restaurants);
int verifier_doublon(Restaurant *restaurants, Restaurant *restaurant);
void cherche_par_specialite(double x, double y, char *specialite[], Restaurant *results);

char *chemin = "/Users/ericb/Desktop/UTC/TC04/SR01/SR01_D1/restau.txt";

int main() {
    Restaurant *restaurants = malloc(sizeof(Restaurant) * MAX_TAB);
    if (restaurants == NULL) {
        printf("Erreur allocation mémoire pour restaurants\n");
        return 1;
    }

    int nb_restaurants = lire_restaurant(chemin, restaurants);
    printf("%d restaurants dans le fichier.\n", nb_restaurants);

    Restaurant results[MAX_TAB];  // on peut ajuster la taille comme on le souhaite
    cherche_restaurant(18.5, 7.5, 10, restaurants, results);

    char *specialites[] = {"Cuisine gastronomique"};
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
    printf("%d restaurants trouvés à une distance inférieure à %.2lf km de votre position.\n", k, rayon_recherche);
}


void trier_restaurants_selon_distance(double x, double y, Restaurant *restaurants) {
    if (!restaurants) {
        printf("Liste des restaurants vide.\n");
        return;
    }

    double distance_courant, distance_min;
    for (int i = 0; &restaurants[i] != NULL; i++) {
        distance_min = calculer_distance(x, restaurants[i].position.x, y, restaurants[i].position.y);
        for (int j = i+1; &restaurants[j] != NULL; j++) {
            distance_courant = calculer_distance(x, restaurants[j].position.x, y, restaurants[j].position.y);
            if (distance_courant < distance_min) {
                Restaurant *restaurant_intermediaire = &restaurants[i];
                restaurants[i] = restaurants[j];
                restaurants[j] = *restaurant_intermediaire;
            }
        }
    }
    printf("Liste des restaurants triée selon la distance de l'utilisateur.\n");
}


int verifier_doublon(Restaurant *restaurants, Restaurant *restaurant) {
    for (int i = 0; &restaurants[i] != NULL; i++) {
        if (strcmp(restaurants[i].adresse_restaurant, restaurant->adresse_restaurant) == 0) {
            return 1;
        }
    }
    return 0;  // renvoie 0 si aucun doublon trouvé
}


void cherche_par_specialite(double x, double y, char *specialite[], Restaurant *results) {
    if (!specialite[0]) {
        printf("Liste des spécialités vide.\n");
        return;
    }

    Restaurant *restaurants_potentiels = malloc(sizeof(Restaurant)*MAX_TAB);
    if (!restaurants_potentiels) {
        printf("Erreur allocation mémoire restaurants potentiels.\n");
        return;
    }
    int nb_restaurants = lire_restaurant(chemin, restaurants_potentiels);

    int k = 0;
    for (int i = 0; &restaurants_potentiels[i] != NULL && i < MAX_TAB; i++) {
        for (int j = 0; specialite[j] != NULL && j < MAX_TAB; j++) {
            Restaurant *restaurant_courant = creer_restaurant(restaurants_potentiels[i].nom_restaurant, restaurants_potentiels[i].adresse_restaurant, restaurants_potentiels[i].position, restaurants_potentiels[i].specialite);
            if (strcmp(restaurant_courant->specialite, specialite[j]) == 0 && !verifier_doublon(results, restaurant_courant)) {
                results[k] = *restaurant_courant;
                k++;
            }
            free(restaurant_courant);
        }
    }

    trier_restaurants_selon_distance(x, y, results);
    printf("%d restaurants correspondent aux critères.\n", k);
    free(restaurants_potentiels);
}
