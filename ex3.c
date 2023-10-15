#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIGNE 1000


// structure position qui permet de stocker les coordonnées d'un restaurant
typedef struct Position Position;
struct Position {
    float x;
    float y;
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
int lire_restaurant (char *chemin, Restaurant restaurants []);

void inserer_restaurant(Restaurant restaurant, char *chemin);


int main() {
    Restaurant *restaurants;
    int nb_restaurants = lire_restaurant("/Users/ericb/Desktop/UTC/TC04/SR01/SR01_D1/restau.txt", restaurants);
    printf("%d restaurants\n", nb_restaurants);
    inserer_restaurant(restaurants[3], "/Users/ericb/Desktop/UTC/TC04/SR01/SR01_D1/restau.txt");
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

    sscanf(spos, "(x=%f, y=%f)", &pos.x, &pos.y);
    supprimerChar(specialite, '{');
    supprimerChar(specialite, '}');

    restaurant->nom_restaurant = strdup(nom);
    restaurant->adresse_restaurant = strdup(adresse);
    restaurant->position = pos;
    restaurant->specialite = strdup(specialite);

    return 1;
}


int lire_restaurant (char *chemin, Restaurant restaurants []) {
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
    printf("Fichier lu\n");
    return compteur;
}


void inserer_restaurant(Restaurant restaurant, char *chemin) {
    char ligne[MAX_LIGNE];
    snprintf(ligne, MAX_LIGNE, "\n%s;%s;(x=%f, y=%f);{%s};", restaurant.nom_restaurant, restaurant.adresse_restaurant, restaurant.position.x, restaurant.position.y, restaurant.specialite);

    FILE *fichier = fopen(chemin, "a");
    fprintf(fichier, "%s", ligne);
    fclose(fichier);
    printf("Fichier modifié\n");
}
