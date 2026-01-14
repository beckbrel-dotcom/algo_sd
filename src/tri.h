#ifndef TRI_H
#define TRI_H

#include "structures.h" //pour la définition de Tuple


typedef struct {
    long nb_comparaisons;
    long nb_permutations;
} StatsTri;

// Prototypes des fonctions de tri
StatsTri tri_selection(Tuple* tab, int n);
StatsTri tri_insertion(Tuple* tab, int n);
StatsTri tri_rapide_rec(Tuple* tab, int n);
StatsTri tri_rapide_iter(Tuple* tab, int n) ;

#endif