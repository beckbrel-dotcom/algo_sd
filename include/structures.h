/*
@Auteurs : 
    - OBIE OBOLO Becker
    - NOUMBOGO IGOUWE Hermès

@Description :
    Document header 
    1. Pour la création des structures manipulées 
    dans l’ensemble du projet :
        -Node : structure de liste chaînée  
        -Station : structure qui stocke les informations sur les stations
        -Tuple : structure exigée par le cahier des charges facilitant le tri
        des stattions suivant le degré

    2. Pour l’implémentation des constructeurs de structure et de quelques
    fonctions utilitaires à leurs manipulation dans src/structures.c :


@Implémenation :
    11 janvier 2026
*/


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



//Variables Globales 
extern Graph* G;
extern int id ;
extern int nb ;
extern char* name ;


//Constructeurs de structures
void setup_station(Station *s, int id, const char *name) ;
Graph* init_Graph(int nb) ;


// Vérifie si une arête existe déjà de src_idx vers dest_idx (Contrainte : doublons)
int edge_exists(Graph* g, int src_idx, int dest_idx)  ;

// Trouve l'index dans le tableau à partir de l'ID du fichier
int find_idx_by_id(Graph* g, int id_recherche)  ;

//Affiche les informations d’une station
int display_station() ;

int display_by_id(int num, int rg) ;

int display_neighbors();


//Détermine l’id par le nom
int get_id_by_name() ;


//Fonction de libération d’allocation de liste d'adjacence
void free_adj_list(Node *head) ;


//Fonction de libération d'allocation de graphe
void free_graph(Graph *g) ;

//Algorithme de Dijkstra
void dijkstra(int start_node, int end_node) ;



#endif