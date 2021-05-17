#ifndef WUMPUS_AGENT_H
#define WUMPUS_AGENT_H

typedef struct Inf_base{
    int traveled; //number of times if a place was visited
    int state[5]; //0 pit //1 breeze //2 stench //3 wumpus //4 gold
}inf_base;

typedef struct Agent{
    inf_base *map;
    int location;
    int arrow;
    int points;
}agent;

// inference engine functions -------------------------------------------------------------------------------------
void inference(int **map, int coordinate, agent *jarvis, int sensor);

void confirmed_dangers_inference(agent *jarvis, int **map);

// kill wumpus functions---------------------------------------------------------------------

void clear_wumpus(agent *jarvis);

void kill_Wumpus(agent *jarvis, int die);

int try_kill(int coordinate, int **map, agent *jarvis, world *rooms);

//travel functions -----------------------------------------------------------------------------------------

int safe_way(int coordinate, agent *jarvis);

int confirmed_inference(int coordinate, int perception, agent *jarvis, int **map);

int check_neighborhood (int coordinate, agent *jarvis, int **map);

int draw_routes(int coordinate, int r_choice, agent *jarvis, int **map);

void risk_life(int coordinate, agent *jarvis, int **map);

int dead_lock(int coordinate, int **map, agent *jarvis);

int backtracking(int coordinate, int *sucess, agent *jarvis, int **map);

int travel(int coordinate, int **map, agent *jarvis, world *rooms);

// agent functions ------------------------------------------------------------------------------

agent *create_agent_world();

void get_perception(int room, agent *jarvis, world *rooms);

void explore(agent *jarvis, int **map, world *rooms);

void print_agent_map(agent *jarvis);

void free_agent(agent *jarvis);

#endif //WUMPUS_AGENT_H
