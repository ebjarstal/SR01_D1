#include <stdio.h>


typedef struct Position Position;
struct Position {
    int x;
    int y;
};


typedef struct Restaurant Restaurant; 
struct Restaurant {
    char *nom_restaurant;
    char *adresse_restaurant;
    Position *position;
    char *specialite;
};


int main() {
    return 0;
}