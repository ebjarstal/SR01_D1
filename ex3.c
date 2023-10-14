#include <stdio.h>
#include <stdlib.h>
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
int lire_restaurant (char *chemin, Restaurant restaurants []);


int main() {
    Position pos;
    pos.x = 1.4;
    pos.y = 4.3;
    Restaurant *restaurant = creer_restaurant("crous", "5 rue port à bateaux", pos, "Steak frites");
    int t = lire_restaurant("/Users/ericb/Desktop/UTC/TC04/SR01/SR01_D1/restau.txt", restaurant);
    printf("%d\n", t);

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


int lire_restaurant (char *chemin, Restaurant restaurants []) {
    int compteur = 0;
    FILE *fichier = fopen(chemin, "r");
    char ligne[MAX_LIGNE];
    fgets(ligne, MAX_LIGNE, fichier);
    printf("%s", ligne);

    return compteur;
}