typedef struct Edge {
    int target_id;          // ID de la station de destination
    float weight;           // Temps de trajet (optionnel selon le fichier)
    struct Edge *next;      // Lien vers l'arête suivante (liste chaînée)
} Edge;

typedef struct Station {
    int id;                 // Identifiant unique [0..N-1]
    char *name;             // Nom de la station (allocation dynamique)
    Edge *adj_list;         // Pointeur vers la liste des voisins
    int degree;             // Pour faciliter le tri ultérieur
} Station;

typedef struct Graph {
    Station *stations;      // Tableau dynamique de stations
    int nb_stations;        // Nombre total de stations
} Graph;