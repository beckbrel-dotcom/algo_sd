#ifndef STRUCTURES_H
#define STRUCTURES_H


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct Node {
    int dest;          // ID de la station de destination
    int weight;           // Temps de trajet (optionnel selon le fichier)
    struct Node *next;      // Liens vers ses voisins (liste chaînée)
} Node;

typedef struct Station {
    int id;                 // Identifiant unique [0..N-1]
    char *name;             // Nom de la station (allocation dynamique)
    int degree;             // Pour faciliter le tri ultérieur
    struct Node *adj_list;         // Pointeur vers la liste des voisins

} Station;

typedef struct Tuple { //Structure (id_station, degree)
    int id ;
    int degree ;
} Tuple;

typedef struct Graph {
    int nb_stations; // Nombre total de stations
    Station *stations;      // Tableau dynamique de stations
} Graph;


//Constructeurs de structures
Node* init_Node(int v, int weight) ;
void setup_station(Station *s, int id, const char *name) ;
Graph* init_Graph(int nb) ;



//Fonction de libération d'allocation de liste d'adjacence
void free_adj_list(Node *head) ;


//Fonction de libération d'allocation de graphe
void free_graph(Graph *g) ;

//Algorithme de Dijkstra
void dijkstra(Graph *g, int start_id) ;



// Fonction de comparaison pour qsort (ordre croissant)
int comparerCroissant(const void *a, const void *b) ;


Tuple* obtenir_stations_triees(Graph *g) ;

#endif