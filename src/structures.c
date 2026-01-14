
#include "structures.h"

//Défintions des constructeurs de strucutures

Node* init_Node(int v, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erreur d'allocation pour le Node");
        exit(EXIT_FAILURE);
    }
    newNode->dest = v;
    newNode->weight = weight;
    newNode->next = NULL; // Très important pour éviter les pointeurs fous
    return newNode;
}


void setup_station(Station *s, int id, const char *name) {
    s->id = id;
    s->degree = 0;
    s->adj_list = NULL;
    
    // On duplique le nom en mémoire dynamique pour qu'il survive 
    // en dehors de la fonction de lecture du fichier.
    if (name != NULL) {
        s->name = strdup(name); 
    } else {
        s->name = NULL;
    }
}

Graph* init_Graph(int nb){
    Graph* new_Graph = malloc(sizeof(Graph)) ;
    new_Graph->nb_stations = nb ;
    new_Graph->stations = malloc(nb*sizeof(Station)) ;
    
    return new_Graph;
}


//Affiche les informations d’une station
int display_station(){
    if (G == NULL) {
        printf("ERREUR CRITIQUE : Le graphe n'est pas chargé en mémoire globale.\n");
        return -1;
    }
    if(id==-1){
        printf("ERREUR : ID invalide\n") ;
        return -1;
    }
    for(int i = 0 ; i < nb ; i++ ){
        if(G->stations[i].id==id){
            printf(">>>>>>>>>>>>>> INFORMATION STATION <<<<<<<<<<<<<<\n");
            printf("- ID      :   %d\n", G->stations[i].id) ;
            printf("- NOM     :   %s\n", G->stations[i].name) ;
            printf("- DEGRÉ   :   %d\n", G->stations[i].degree) ;
        }
    }
    return 1;
}

//Détermine l’id par le nom
int get_id_by_name() {
    // On initialise l'id à -1 au cas où on ne trouve rien
    id = -1;

    for (int i = 0; i < nb; i++) {
        // Sécurité : on vérifie que le nom de la station n'est pas NULL
        if (G->stations[i].name != NULL) {
            // strcmp renvoie 0 si les chaînes sont strictement identiques
            if (strcmp(G->stations[i].name, name) == 0) {
                id = i;
                return id; // On a trouvé, on sort de la boucle
            }
        }
    }

    // Si on arrive ici, c'est que la boucle est finie sans succès
    printf("\nERREUR : La station '%s' n'existe pas dans la base.\n", name);
    return -1 ;
}



// Fonctions utilitaires pour libérer les blocs de mémoires alloués

void free_adj_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}


void free_graph(Graph *g) {
    if (g == NULL) return;

    // Pour chaque station, on nettoie ses composants internes
    for (int i = 0; i < g->nb_stations; i++) {
        free(g->stations[i].name);      // Libère la chaîne de caractères
        free_adj_list(g->stations[i].adj_list); // Libère la liste de voisins
    }

    free(g->stations); // Libère le tableau de structures Station
    free(g);           // Libère la structure Graph elle-même
}



void dijkstra(Graph *g, int start_id) {
    int n = g->nb_stations;
    
    // Tableaux de travail
    int *dist = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));
    int *visite = malloc(n * sizeof(int));

    // 1. Initialisation
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;  // Infini
        parent[i] = -1;     // Aucun prédécesseur
        visite[i] = 0;      // Non visité
    }

    dist[start_id] = 0; // La distance vers le départ est nulle

    

    for (int count = 0; count < n - 1; count++) {
        // 2. Trouver la station non visitée avec la distance minimale
        int min = INT_MAX;
        int u = -1;

        for (int v = 0; v < n; v++) {
            if (!visite[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        // Si aucune station n'est atteignable, on s'arrête
        if (u == -1 || dist[u] == INT_MAX) break;

        // 3. Marquer la station u comme traitée
        visite[u] = 1;

        // 4. Relâcher les arêtes (Relaxation)
        Node *neighbor = g->stations[u].adj_list;
        while (neighbor != NULL) {
            int v = neighbor->dest;
            int weight = neighbor->weight;

            // Vérification anti-overflow avant l'addition
            if (!visite[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            neighbor = neighbor->next;
        }
    }

    // --- Exemple d'affichage des résultats ---
    printf("Résultats depuis la station %s (ID: %d) :\n", g->stations[start_id].name, start_id);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INT_MAX) {
            printf("Station %d : Inaccessible\n", i);
        } else {
            printf("Station %d (%s) : %d min\n", i, g->stations[i].name, dist[i]);
        }
    }

    // Libération des tableaux temporaires
    free(dist);
    free(parent);
    free(visite);
}



// Fonction de comparaison pour qsort (ordre croissant)
int comparerCroissant(const void *a, const void *b) {
    Tuple *tupleA = (Tuple *)a;
    Tuple *tupleB = (Tuple *)b;
    
    // Si A > B, renvoie un nombre positif -> A sera placé après B
    return (tupleA->degree - tupleB->degree);
}

Tuple* obtenir_stations_triees(Graph *g) {
    int n = g->nb_stations;
    
    // 1. Allouer le tableau de Tuples
    Tuple *tableau = malloc(n * sizeof(Tuple));
    if (tableau == NULL) return NULL;

    // 2. Parcourir le graphe pour remplir le tableau
    for (int i = 0; i < n; i++) {
        tableau[i].id = g->stations[i].id;
        
        // Si tu n'as pas stocké le degré, on le compte ici
        int d = 0;
        Node *curr = g->stations[i].adj_list;
        while (curr != NULL) {
            d++;
            curr = curr->next;
        }
        tableau[i].degree = d;
    }

    // 3. Trier le tableau par degré croissant
    qsort(tableau, n, sizeof(Tuple), comparerCroissant);

    return tableau;
}