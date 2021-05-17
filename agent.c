#include "./includes/main.h"

int wumpus_alive = 1;


// inference engine functions -------------------------------------------------------------------------------------

void inference(int **map, int coordinate, agent *jarvis, int sensor) {//make inferences based on sensors
    switch (sensor){
        case 1: //insert pill 
            for (int i = 0; i < NROOM; i++) {
                if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled == 0){
                    if(confirmed_inference(i, 1, jarvis, map) == 0)
                        (*jarvis).map[i].state[0] = 1;
                }
            }
            break;
        case -1:  //remove pill 
            for (int i = 0; i < NROOM; i++) {
                if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled == 0){
                    (*jarvis).map[i].state[0] = 0;
                }
            }
            break;
        case 2: //insert wumbus
            for (int i = 0; i < NROOM; i++) {
                if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled == 0){
                    if(confirmed_inference(i, 2, jarvis, map) == 0)
                        (*jarvis).map[i].state[3] = 1;
                }    
            }
            break;
        case -2: //remove wumbus
            for (int i = 0; i < NROOM; i++) {
                if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled == 0)
                    (*jarvis).map[i].state[3] = 0;
            }
            break;
        
        default:
            printf("ERROR switch\n");
            exit(0);
    }
}

void confirmed_dangers_inference(agent *jarvis, int **map) { //scan all map for confirm dangers
    
    int stench; //stench counter
    int breeze; // breeze counter

    for (int i = 0; i < NROOM; i++) { // for each room I search neighbors sensors
        breeze = 0;
        for (int j = 0; j < NROOM; j++) {
            if (map[i][j] == 1 && (*jarvis).map[j].state[1] == 1)
                breeze++;
            if (breeze == 2 && (*jarvis).map[i].traveled == 0) {
                if (i == WORLD - 1 || i == WORLD * (WORLD - 1) || i == NROOM - 1)
                    (*jarvis).map[i].state[0] = 2;
            }
        }
        stench = 0;
        for (int k = 0; k < NROOM; k++) {
            if (map[i][k] == 1 && (*jarvis).map[k].state[2] == 1)
                stench++;
            if (stench == 2 && (*jarvis).map[i].traveled == 0) {
                if (i == WORLD - 1 || i == WORLD * (WORLD - 1) || i == NROOM - 1)
                    (*jarvis).map[i].state[3] = 2;
            }
        }
        if (stench == 3 && (*jarvis).map[i].traveled == 0) // 3 stench around, I can confirm that is wumbus location
            (*jarvis).map[i].state[3] = 2;
    
    }
}

// kill wumpus functions---------------------------------------------------------------------

void clear_wumpus(agent *jarvis){ //after dead the agent eliminates the wumpus and its stinks from your own map 
    for (int i=0; i < NROOM; i++) {
        (*jarvis).map[i].state[2] = 0;
        (*jarvis).map[i].state[3] = 0;
    }
}

void kill_Wumpus(agent *jarvis, int die){
    
    if(die){
        wumpus_alive = 0;
        clear_wumpus(jarvis);
        if(MSG){  
            printf("The agent fired his arrow right into Wumpus's eyes\n");
            printf("his sad cry of pain running from the maze to the CTAN corridors.!!\n\n");
            printf("\n\tPRESS ENTER FOR CONTINUE...\n");
            getchar();
        }
        
    }else{
        if(MSG)
            printf("The agent has try to kill Wumpus but he miss your arrow! :C\n");
            printf("\n\tPRESS ENTER FOR CONTINUE...\n");
            getchar();
    }
    (*jarvis).points -= 10;
    (*jarvis).arrow = 0;

}

int try_kill(int coordinate, int **map, agent *jarvis, world *rooms ){ 
    for (int i = 0; i < NROOM; i++) {
        if (map[coordinate][i] == 1) {
            if ((*jarvis).map[i].state[3] == 2) { //the agent is sure wumpus is there
                (*jarvis).map[i].state[3] = 0;
                kill_Wumpus(jarvis,1);
                return 1;
            }
        }
    }
    
    for (int i = 0; i < NROOM; i++) { //the agent is not sure wumpus is there
        if (map[coordinate][i] == 1 && (*jarvis).map[i].state[3]==1) {
            if(rooms[i].state[3]==1){  //got it
                (*jarvis).map[i].state[3] = 0; //has wumpus
                kill_Wumpus(jarvis,1);
                return 1;
            }else{  //miss
                (*jarvis).map[i].state[3] = 0; // do not have Wumpus
                kill_Wumpus(jarvis,0);
                return 0;
            }
        }
    }
}

//travel functions -----------------------------------------------------------------------------------------

int safe_way(int coordinate, agent *jarvis) { //if there is a room is safe

    if ((*jarvis).map[coordinate].state[0] == 0 && (*jarvis).map[coordinate].state[3] == 0)
        return 1;
    return 0;
}

int confirmed_inference(int coordinate, int perception, agent *jarvis, int **map){ //check if neighborhood is valid 
    
    int confirmation= 0;
    for (int i = 0; i < NROOM; i++) { 
        if (map[coordinate][i] == 1) {
            if ((*jarvis).map[i].traveled == 1){ //traveled
                if((*jarvis).map[i].state[perception] == 0) //the neighbors also has perception
                    return 1;    
            }
        }
    }
    return 0;
}


int check_neighborhood (int coordinate, agent *jarvis, int **map){ //return how many neighbors are safe and unexplored

    int neighbors = 0;
    for (int i = 0; i < NROOM; i++) { 
        if (map[coordinate][i] == 1) {
            if ((*jarvis).map[i].traveled == 0){
                if (safe_way(i, jarvis))
                    neighbors++;        
            }
        }
    }
    return neighbors;
}

int draw_routes(int coordinate, int r_choice, agent *jarvis, int **map){
             
    int number_routes = 0;
    for (int i = 0; i < NROOM; i++) { //search as coordinates of the drawn route
        if (map[coordinate][i] == 1) {
            if ((*jarvis).map[i].traveled == 0){
                if (safe_way(i, jarvis)) {
                    number_routes ++;
                    if(number_routes == r_choice){
                        (*jarvis).location = i;
                        (*jarvis).points -= 1;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;

}

void risk_life(int coordinate, agent *jarvis, int **map){
    //draws a place to risk life
    int neighbor= 0;
    for (int i = 0; i < NROOM; i++) { //find  neighbor
        if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled==0 ){
            neighbor++;
        }          
    }
    //draw a neighbor
    if(MSG){
        printf("The agent will risk a random neighbor!\n");
    }
    int r_choice = rand_c(1, neighbor); 
    neighbor = 0;
    for (int i = 0; i < NROOM; i++) { //go to neighbor draw
        if(map[coordinate][i] == 1 && (*jarvis).map[i].traveled==0 ){
            neighbor++;
            if(neighbor == r_choice){      
                //talvez coloque condicoes pra ele atirar aqui
                (*jarvis).location = i;
                (*jarvis).points -=1;
            }
        }          
    }

}

int dead_lock(int coordinate, int **map, agent *jarvis) {  //verification if there is available way using backtracker

    for (int i = 0; i < NROOM; i++) {
        if (map[coordinate][i] == 1) {    
            if(safe_way(i,jarvis))
                return 0;
        }
    }
    return 1;
}

int backtracking(int coordinate, int *sucess, agent *jarvis, int **map){//check all places already visited if have some neighbor safe and unexplored...
    
    int *checkeds = (int*) malloc(NROOM*sizeof(int));
    for(int k=0; k<NROOM; k++){  
        checkeds[k]=-1;
    }
    
    int number_rooms = 0;  // counter number of rooms visited
    for(int n=0 ; n<NROOM; n++){       
        if((*jarvis).map[n].traveled == 1){
            number_rooms++;
        }
    }

    int cont=0;
    int new_place;
    int points_spent=0;
    int last_travel; //safe place backtracker

    for (int i = 0; i < NROOM; i++) { //find  neighbor
        if (map[coordinate][i] == 1 && (*jarvis).map[i].traveled == 1){ //backtracker to visited neighbor
            
            new_place = 1;
            for(int j=0; j<NROOM; j++){
                if(checkeds[j]== i){
                    new_place = 0; //this place already checked
                }
            }
            if(new_place){  //backtrack
                points_spent++; //decrease point because the agent moves
                coordinate = i;  //coordinate updated    
                
                if(check_neighborhood(coordinate, jarvis, map) != 0){ //if there are neighbors safe so stop backtracking I finded a way
                    (*jarvis).points -= points_spent;
                    free(checkeds);
                    *sucess = 1;
                    return coordinate;  
                }else{ //back
                    checkeds[cont] = coordinate;
                    last_travel = coordinate;
                    cont++;
                    i = 0;
                }
            }
        }          
    }
    free(checkeds);
    *sucess = 0;
    (*jarvis).points -= points_spent;
    return last_travel;        
}

int travel(int coordinate, int **map, agent *jarvis, world *rooms) {
    
    int number_routes = 0;
    number_routes = check_neighborhood(coordinate, jarvis, map);
    if(MSG){
        printf("Number of routes available: %d\n",number_routes);
    }
    
    if(number_routes >= 1){  
        
        int r_choice = 1;
        if(number_routes>1){
            r_choice = rand_c(1, number_routes); 
        } 
        int success = draw_routes(coordinate, r_choice, jarvis, map);
        if(success){
            return 1;
        }else{
            printf("ERROR IN DRAW ROUTES\n");
        }
    }
    
    if(dead_lock(coordinate, map, jarvis)){ //check if deadlock at the start
        if(MSG){
            printf("Deadlock at the Begining!\n\n");
        }
        risk_life(coordinate, jarvis, map);
        return 1;
    }
    else{  //check backtrack
        int *sucess = (int*) malloc(sizeof(int));
        coordinate =  backtracking(coordinate,sucess, jarvis, map); 
        (*jarvis).location = coordinate; //backtrack to new position
        
        if(*sucess == 0){ //deadlock 
            free(sucess);
            if(MSG){
                printf("\nDeadlock!\n");
            }

            if(!try_kill(coordinate, map, jarvis, rooms)){
                risk_life(coordinate, jarvis, map);
                return 1;
            }else{
                return 1; 
            }      

        }else{ //is not a deadlock
            if(MSG){
                printf("\nBacktrack!\n");
            }
            
            free(sucess);
            return 1;
        }
    }
    return 0;        
}

// agent functions ------------------------------------------------------------------------------

agent *create_agent_world() {  //function to create the world in the agent's view
    
    agent *jarvis = (agent *) malloc(sizeof(agent));

    (*jarvis).map = (inf_base *) malloc(NROOM * sizeof(inf_base));
    (*jarvis).arrow = 1;
    //all world start with 0 in all states
    for (int i = 0; i < NROOM; i++) {
        for (int j = 0; j < 6; j++)
            (*jarvis).map[i].state[j] = 0;
    }
    (*jarvis).location = 0;
    (*jarvis).map[0].traveled = 1;
    (*jarvis).points = 0;
    return jarvis;
}

void get_perception(int room, agent *jarvis, world *rooms) {  //the agent feels the perceptions of the world

    for (int i = 0; i < 5; i++)
        (*jarvis).map[room].state[i] = rooms[room].state[i];

    if (wumpus_alive == 0){
        (*jarvis).map[room].state[2] = 0;
        (*jarvis).map[room].state[3] = 0;
    }
}

void explore(agent *jarvis, int **map, world *rooms) {
    
    int end = 0; //end game?
    int current = 0;
    
    do {
        get_perception(current, jarvis, rooms);
        (*jarvis).map[current].traveled = 1;

        if ((*jarvis).map[current].state[0] == 1) {
            (*jarvis).map[current].state[0] = 2;  
            
            if(MSG){
                print_agent_map(jarvis);
                printf("The agent fell into a pit! \n");
            }
            (*jarvis).points -= 1000;
            end = 1;
        } else if ((*jarvis).map[current].state[3] == 1) {
            (*jarvis).map[current].state[3] = 2;
            
            if(MSG){
                print_agent_map(jarvis);
                printf("The agent was devoured by Wumpus! \n");
            }
            (*jarvis).points -= 1000;
            end = 1;
        } else if ((*jarvis).map[current].state[4] == 1) {
            
            if(MSG){          
                print_agent_map(jarvis);
                printf("The agent found the gold! \n");
            }

            (*jarvis).points += 1000;
            end = 1;
        } 
        else { //neither won nor lost

            if(MSG){
                print_agent_map(jarvis);
                printf("\nThe agent is performing inferences...\n");
            }

            //-------inference---------------
            if ((*jarvis).map[current].state[1] == 1)
                inference(map, current, jarvis, 1); //breeze
            else
                inference(map, current, jarvis, -1);  //no breeze

            if ((*jarvis).map[current].state[2] == 1) //stench
                inference(map, current, jarvis, 2);
            else 
                inference(map, current, jarvis, -2); // no stench
            
            confirmed_dangers_inference(jarvis, map);
            
            //---------travel -----------------
            if(MSG){
                print_agent_map(jarvis);
                printf("\n\tPRESS ENTER FOR CONTINUE TRAVELING...\n\n");
                getchar();
            }

            if(!travel(current, map, jarvis, rooms)){
                printf("ERROR NO TRAVEL FINDED\n");
                exit(0);
            }
            current = (*jarvis).location;           
        }
    } while (end==0);

    printf("END!!! \n The Agent socored: %d\n\n", (*jarvis).points);
}

void print_agent_map(agent *jarvis) {
    char *info = (char *) malloc(10 * sizeof(char));
    printf("--------------g_map------------\n");
    printf("        1             2            3            4\n");
    for (int i = WORLD - 1; i >= 0; i--) {
        printf("%d- ", i + 1);
        for (int j = 0; j < WORLD; j++) {
            printf("{ ");
            if ((*jarvis).map[(i * WORLD) + j].state[0] == 1) {
                info[0] = 'P';
                info[1] = '?';
            } else if ((*jarvis).map[(i * WORLD) + j].state[0] == 2) {
                info[0] = 'P';
                info[1] = ' ';
            } else if((*jarvis).map[(i * WORLD) + j].traveled == 0){
                info[0] = ' ';
                info[1] = '?';
            } else{
                info[0] = ' ';
                info[1] = ' ';
            }
            if ((*jarvis).map[(i * WORLD) + j].state[1] == 1)
                info[2] = '~';
            else
                info[2] = ' ';
            if ((*jarvis).map[(i * WORLD) + j].state[4] == 1)
                info[3] = 'G';
            else
                info[3] = ' ';
            if ((*jarvis).map[(i * WORLD) + j].state[2] == 1)
                info[4] = '+';
            else
                info[4] = ' ';
            if ((*jarvis).map[(i * WORLD) + j].state[3] == 1) {
                info[5] = 'W';
                info[6] = '?';
            } else if ((*jarvis).map[(i * WORLD) + j].state[3] == 2) {
                info[5] = 'W';
                info[6] = ' ';
            } else {
                info[5] = ' ';
                info[6] = ' ';
            }
            if ((*jarvis).location == (i * WORLD) + j)
                info[7] = 'A';
            else
                info[7] = ' ';
            printf(" %c%c%c%c%c%c%c%c ", info[0], info[1], info[2], info[3], info[4], info[5], info[6], info[7]);
            printf("}  ");
        }
        printf("\n");
    }
    printf("\nA - Agent  W - Wumpus  G - Gold\n ~ - Breeze  + - Stench  P - Pit\n");
    printf("The Agent socored: %d\n", (*jarvis).points);
    printf("\n");
    free(info);
}

void free_agent (agent *jarvis){

    free(jarvis->map);
    free(jarvis);
}
