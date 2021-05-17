#include "./includes/main.h"

//Amaury Mario Ribeiro Neto
//Antonio Pereira de Souza Junior


int main(int argc, char **argv){
    
    srand(time(NULL));
    int exit = 0;

    do{
        printf("----------- WUMBUS WORLD -----------\n\n");
        int **map = adjacency_matrix();  
        world *rooms;
        

        if(argc > 1){
            rooms = randmap(1, argv[1]);
        }
        else {
            rooms = randmap(0, NULL);
        }

        agent *jarvis = create_agent_world(); //create agent data structure

        if(MSG){
            print_map(rooms);
            printf("\n\tPRESS ENTER FOR CONTINUE...\n\n");
            getchar();
            printf("Agent Starts Exploration...\n");
        }

        explore(jarvis, map, rooms); 
        if(MSG){
            printf("The real map was...\n");
            print_map(rooms);
        }
        
        free_map(map,rooms);
        free_agent(jarvis);
        printf("Play again? 1)Start\n 0)Exit\n");
        scanf("%d",&exit);
    }
    while(exit != 0);

    return 0;
}