#include "tri.h"

StatsTri tri_selection(Tuple* tab, int n) {
    StatsTri s = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            s.nb_comparaisons++;
            if (tab[j].degree > tab[min_idx].degree) { // Tri décroissant
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Tuple temp = tab[i];
            tab[i] = tab[min_idx];
            tab[min_idx] = temp;
            s.nb_permutations++;
        }
    }
    return s;
}


StatsTri tri_insertion(Tuple* tab, int n) {
    StatsTri s = {0, 0};
    for (int i = 1; i < n; i++) {
        Tuple cle = tab[i];
        int j = i - 1;

        // On cherche la place de 'cle'
        while (j >= 0) {
            s.nb_comparaisons++;
            if (tab[j].degree < cle.degree) { // < pour tri décroissant
                tab[j + 1] = tab[j]; // Décalage
                s.nb_permutations++;
                j--;
            } else {
                break;
            }
        }
        tab[j + 1] = cle;
    }
    return s;
}

int partitionner(Tuple* tab, int bas, int haut, StatsTri* s) {
    Tuple pivot = tab[haut];
    int i = (bas - 1);

    for (int j = bas; j < haut; j++) {
        s->nb_comparaisons++;
        if (tab[j].degree > pivot.degree) { // > pour décroissant
            i++;
            // Échange
            Tuple temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
            s->nb_permutations++;
        }
    }
    // Place le pivot au milieu
    Tuple temp = tab[i + 1];
    tab[i + 1] = tab[haut];
    tab[haut] = temp;
    s->nb_permutations++;
    
    return (i + 1);
}

// Fonction récursive principale
void quicksort_rec(Tuple* tab, int bas, int haut, StatsTri* s) {
    if (bas < haut) {
        int pi = partitionner(tab, bas, haut, s);
        quicksort_rec(tab, bas, pi - 1, s);
        quicksort_rec(tab, pi + 1, haut, s);
    }
}

// Fonction "enveloppe" pour l'appel depuis le menu
StatsTri tri_rapide_rec(Tuple* tab, int n) {
    StatsTri s = {0, 0};
    quicksort_rec(tab, 0, n - 1, &s);
    return s;
}


int partitionner_simple(Tuple* tab, int bas, int haut, StatsTri* s) {
    int pivot = tab[haut].degree; // On prend la valeur du dernier élément
    int i = bas; // Indice du premier élément plus petit que le pivot

    for (int j = bas; j < haut; j++) {
        s->nb_comparaisons++;
        if (tab[j].degree > pivot) { // > pour tri décroissant
            // On échange tab[i] et tab[j]
            Tuple temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
            s->nb_permutations++;
            i++;
        }
    }
    // On place le pivot à sa place définitive (en i)
    Tuple temp = tab[i];
    tab[i] = tab[haut];
    tab[haut] = temp;
    s->nb_permutations++;

    return i; // On retourne la position du pivot
}


StatsTri tri_rapide_iter(Tuple* tab, int n) {
    StatsTri s = {0, 0};
    
    // Création d'une pile pour stocker les indices (bas, haut)
    // Taille max nécessaire : n
    int* pile = malloc(n * sizeof(int));
    int sommet = -1;

    // On place les indices de départ sur la pile
    pile[++sommet] = 0;
    pile[++sommet] = n - 1;

    // Tant que la pile n'est pas vide
    while (sommet >= 0) {
        // On dépile haut et bas
        int haut = pile[sommet--];
        int bas = pile[sommet--];

        // On partitionne (on utilise la même fonction partitionner_simple)
        int p = partitionner_simple(tab, bas, haut, &s);

        // Si il y a des éléments à gauche du pivot, on empile les indices
        if (p - 1 > bas) {
            pile[++sommet] = bas;
            pile[++sommet] = p - 1;
        }

        // Si il y a des éléments à droite du pivot, on empile les indices
        if (p + 1 < haut) {
            pile[++sommet] = p + 1;
            pile[++sommet] = haut;
        }
    }

    free(pile);
    return s;
}