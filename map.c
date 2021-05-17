#include "./includes/main.h"

int **map;
world *rooms;

unsigned rand_c(unsigned min, unsigned max){
    int r;
    unsigned range = 1 + max - min;
    unsigned buckets = RAND_MAX / range;
    unsigned limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int **adjacency_matrix(){  // create adjacency matrix for the maze map
    map = (int**) malloc (NROOM * sizeof(int*));
    for(int i=0; i < NROOM; i++)
        map[i] = (int*) calloc (NROOM,sizeof(int));
    for(int i=0; i < WORLD; i++){
        for(int j=0; j < WORLD; j++){
            if(j+1 < WORLD){
                map[(WORLD*i)+j][(WORLD*i)+j+1] = 1;
                map[(WORLD*i)+j+1][(WORLD*i)+j] = 1;
            }
            if(j-1 > 0){
                map[(WORLD*i)+j][(WORLD*i)+j-1] = 1;
                map[(WORLD*i)+j-1][(WORLD*i)+j] = 1;
            }
            if(i+1 < WORLD){
                map[(WORLD*i)+j][(WORLD*(i+1))+j] = 1;
                map[(WORLD*(i+1))+j][(WORLD*i)+j] = 1;
            }
            if(i-1 > 0){
                map[(WORLD*i)+j][(WORLD*(i-1))+j] = 1;
                map[(WORLD*(i-1))+j][(WORLD*i)+j] = 1;
            }
        }
    }
    rooms = (world*) malloc (NROOM * sizeof(world));
    for(int i=0; i < NROOM; i++){
        for(int j=0; j < 6; j++){
            rooms[i].state[j] = 0;
        }
    }
    return map;
}

world *randmap(int load_map, char *name){
    if(load_map == 1){
        printf("%s\n", name);
        FILE *file = fopen(name, "r");
        if(file == NULL){
            printf("error\n");
            exit(1);
        }  
        for(int i=0; i < NROOM; i++){
            fscanf(file, "%d%d%d%d%d\n", &rooms[i].state[0], &rooms[i].state[1], &rooms[i].state[2], &rooms[i].state[3], &rooms[i].state[4]);
        }
        fclose(file);
    }
    else {
        int n_pit = 3;

        //gold spawn
        int gold = rand_c(1, 15);
        rooms[gold].state[4] = 1;

        //wumpus spawn
        int wumpus;
        for (int i = 0; i < WUMPUS; i++) {
            wumpus = (rand() % (NROOM - 1)) + 1;
            rooms[wumpus].state[3] = 1;
        }

        //adding stench around
        for (int i = 0; i < NROOM; i++) {
            if (rooms[i].state[3] == 1) {
                for (int j = 0; j < NROOM; j++) {                
                   if (map[i][j] == 1 && rooms[j].state[3] != 1 && rooms[j].state[0] !=1 )
                        rooms[j].state[2] = 1;
                }
            }
        }

        int number, n_pills = 0;
        //pit spawn
        for (int i = 1; i < NROOM; i++) {
            number = rand_c(0, 4);
            if (number == 1 && n_pills < PILL) {
                if (rooms[i].state[4] != 1) {
                    rooms[i].state[0] = 1;
                    n_pills += 1;
                }
            }
        }

        //adding breeze 
        for (int j = 0; j < NROOM; j++) {
            if (rooms[j].state[0] == 1) {
                for (int k = 0; k < NROOM; k++) {               
                    if (map[j][k] == 1 && rooms[k].state[0] != 1)
                        rooms[k].state[1] = 1;
                }
            }
        }
    }
    return rooms;
}

void print_map(){
    char *info = (char*) malloc (6 * sizeof(char));
    printf("--------------g_map------------\n");
    printf("        1             2            3            4\n");
    for (int i = WORLD-1; i >= 0; i--){
        printf("%d- ",i+1);
        for (int j=0; j < WORLD; j++){
            printf("{ ");
            if(rooms[(i*WORLD)+j].state[0] == 1)
                info[0] = 'P';
            else
                info[0] = ' ';
            if(rooms[(i*WORLD)+j].state[1] == 1)
                info[1] = '~';
            else
                info[1] = ' ';
            if(rooms[(i*WORLD)+j].state[4] == 1)
                info[2] = 'G';
            else
                info[2] = ' ';
            if(rooms[(i*WORLD)+j].state[2] == 1)
                info[3] = '+';
            else
                info[3] = ' ';
            if(rooms[(i*WORLD)+j].state[3] == 1)
                info[4] = 'W';
            else
                info[4] = ' ';
            printf(" %c%c%c%c%c%c ", info[0], info[1], info[2], info[3], info[4], info[5]);
            printf("}  ");
        }
        printf("\n");
    }
    printf("\nA - Agent  W - Wumbus  G - Gold\n ~ - Breeze  + - Stench  P - Pit\n");
    printf("\n\n");
    free(info);
}

void free_map(int **map, world *rooms){
    
    for(int i=0; i<WORLD; i++){
        free(map[i]);
    }
    free(rooms);
    free(map);
}