
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

// Vérifie si une arête existe déjà entre deux index (Contrainte : doublons)
int edge_exists(Graph* g, int src_idx, int dest_idx) {
    Node* current = g->stations[src_idx].adj_list;
    while (current != NULL) {
        if (current->dest == dest_idx) return 1;
        current = current->next;
    }
    return 0;
}

// Trouve l'index dans le tableau à partir de l'ID du fichier
int trouver_index_par_id(Graph* g, int id_recherche) {
    for (int i = 0; i < g->nb_stations; i++) {
        if (g->stations[i].id == id_recherche) return i;
    }
    return -1;
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

int display_by_id(int num, int rg){ //Affichage par id (num) donné
    if (G == NULL) {
        printf("ERREUR CRITIQUE : Le graphe n'est pas chargé en mémoire globale.\n");
        return -1;
    }
    if(num==-1){
        printf("ERREUR : ID voisine invalide\n") ;
        return -1;
    }
    for(int i = 0 ; i < nb ; i++ ){
        if(G->stations[i].id==num){
            printf("%i -  %-6d %-30s %d\n", rg, G->stations[i].id, G->stations[i].name, G->stations[i].degree);    
        }
    }
    return 1;
}

int display_neighbors() {
    if (G == NULL || id == -1) {
        printf("ERREUR : Graphe non chargé ou ID invalide.\n");
        return -1;
    }

    // On affiche la station sans attendre ici
    display_station(); 
    
    printf("\n>>>>>>>>>>>>>> INFORMATIONS VOISINS <<<<<<<<<<<<<\n");
    printf("     %-6s %-25s %s\n\n", "[ ID ]", "[ NOM ]", "[ DEGRÉ ]");

    Node* voisin = G->stations[id].adj_list;
    int count = 0;

    if (voisin == NULL) {
        printf("Cette station n'a aucune voisine.\n");
    }

    while (voisin != NULL) {
        count++;
        display_by_id(voisin->dest,count); 
        
        voisin = voisin->next;
    }

    printf("-------------------------------------------------------------\n");
    printf("Fin de la liste (%d voisins trouvés).\n", count);
    printf("(Appuyez sur Entrée pour revenir au menu)");
    
    // On vide le tampon et on attend une seule fois à la fin
    getchar(); 
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


void dijkstra(int start_node, int end_node) {
    int n = G->nb_stations;
    int dist[n];
    int prev[n];
    int visited[n];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    dist[start_node] = 0;

    for (int count = 0; count < n - 1; count++) {
        // Trouver la station avec la distance min non visitée
        int min = INT_MAX, u = -1;
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1 || u == end_node) break; // On a fini ou on est bloqué

        visited[u] = 1;

        // Mise à jour des voisins
        Node* curr = G->stations[u].adj_list;
        while (curr != NULL) {
            int v = curr->dest;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + curr->weight < dist[v]) {
                dist[v] = dist[u] + curr->weight;
                prev[v] = u;
            }
            curr = curr->next;
        }
    }

    // --- AFFICHAGE DU RÉSULTAT ---
    if (dist[end_node] == INT_MAX) {
        printf("Aucun chemin trouvé entre ces deux stations.\n");
    } else {
        printf("\nTemps total estimé : %d minutes\n", dist[end_node]);
        printf("Itinéraire : ");
        
        // Reconstitution du chemin via une pile ou récursivité
        int path[n];
        int path_count = 0;
        int curr = end_node;
        while (curr != -1) {
            path[path_count++] = curr;
            curr = prev[curr];
        }
        
        for (int i = path_count - 1; i >= 0; i--) {
            printf("%s%s", G->stations[path[i]].name, (i == 0 ? "" : " -> "));
        }
        printf("\n");
    }
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