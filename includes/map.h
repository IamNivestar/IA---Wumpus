#ifndef WUMPUS_MAP_H
#define WUMPUS_MAP_H

typedef struct World{
    int state[5]; //0 pit //1 breeze //2 stench //3 wumpus //4 gold
}world;

int **adjacency_matrix();

world *randmap(int load_map, char *name);

void print_map();

unsigned rand_c(unsigned min, unsigned max);

void free_map(int **map, world *rooms);

#endif //WUMPUS_MAP_H
