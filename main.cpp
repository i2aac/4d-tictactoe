#include <stdio.h>
#include <malloc.h>

struct vector{ //A 4-D vector
    int w, x, y, z;
};

/*
void print_board_linear(char ****board, int s){
    for(int i = 0; i < s; i++){

        for(int j = 0; j < s; j++){

            for(int k = 0; k < s; k++){
                
                putchar(board[i][j][k][0]);
                for(int l = 1; l < s; l++){

                    printf("|%c", board[i][j][k][l]);
                }
                putchar('\n');
                if(k < s - 1){
                    for(int l = 0; l < 2 * s - 1; l++){
                        putchar('-');
                    }
                    putchar('\n');
                }

            }
            putchar('\n');
            
        }

    }
    
}
*/

void print_board_grid(char ****board, int s){ //Prints board.
    for(int i = 0; i < s; i++){

        for(int k = 0; k < s; k++){

            for(int j = 0; j < s; j++){

                for(int l = 0; l < s; l++){
                    putchar(board[i][j][k][l]);
                    if(l < s - 1){
                        putchar('|');
                    }
                }
                if(j < s - 1){
                    printf("   ");
                }

            }

            if(k < s - 1){
                putchar('\n');
                for(int a = 0; a < s; a++){
                    for(int b = 0; b < 2 * s - 1;  b++){
                        putchar('-');
                    }
                    if(a < s - 1){
                        printf("   ");
                    }
                }
                putchar('\n');
            }
            
        }

        putchar('\n');
        if(i < s - 1){
            putchar('\n');
        }
    }
}

bool in_bounds(int bound, vector position){ //Returns true if a vector is in bounds.
    return (0 <= position.w && position.w < bound && 0 <= position.x && position.x < bound && 0 <= position.y && position.y < bound && 0 <= position.z && position.z < bound);
}

int input_size(){ //Prompts user to input board size, and returns a user-selected size once a valid value has been selected.
    int size;
    char answer;

    do{
        printf("Choose a size (a positive integer) for the 4-D board: ");
        scanf("%d", &size);

        if(size <= 0){
            printf("\nThat is not a positive integer. ");
        }
        if(size > 5){
            printf("\n%d is a very large board size. Boards this large may have trouble rendering on some screens.\n", size);
            do{
                printf("\nAre you sure you wish to use %d as the board size? (y/n): ", size);
                scanf(" %c", &answer);
            }while(answer != 'y' && answer != 'n' && answer != 'Y' && answer != 'N');

            if(answer == 'n' || answer == 'N'){
                size = (-1);
                putchar('\n');
            }
        }

    }while(size <= 0);

    return size;
}

vector input_move(char ****board, int s, char turn){ //Prompts user to input a move, and returns a user-selected placement for their marker once a valid value has been chosen.

    vector placement;

    do{
        printf("%c's turn. Enter a coordinate (of the format w x y z) to mark: ", turn);
        scanf("%d %d %d %d", &placement.w, &placement.x, &placement.y, &placement.z);

        if(!in_bounds(s, placement) || board[placement.w][placement.x][placement.y][placement.z] != ' '){
            printf("\nThat is not a valid move. ");
        }

    }while(!in_bounds(s, placement) || board[placement.w][placement.x][placement.y][placement.z] != ' ');

    return placement;
}

vector add(vector a, vector b){ //Returns sum of vector A and B.
    vector r;
    r.w = a.w + b.w;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

int line(char ****board, int s, vector position, vector direction, int length){ //Follows a "line" through on a board until it stops. Returns its length.

    if(direction.w == 0 && direction.x == 0 && direction.y == 0 && direction.z == 0){ //Prevents an infinite loop
        return (-1);
    }

    vector new_position = add(position, direction);
    if(in_bounds(s, new_position) && board[new_position.w][new_position.x][new_position.y][new_position.z] == board[position.w][position.x][position.y][position.z]){
        return line(board, s, new_position, direction, length + 1);
    }

    return length;
}

bool victory(char p, char ****board, int s){ //This isn't a particularly efficient algorithm, but I'm hoping that a player will win before it gets too slow. Returns true if given marker has won.

    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            for(int k = 0; k < s; k++){
                for(int l = 0; l < s; l++){
                    if(board[i][j][k][l] == p){
                        for(int a = (-1); a <= 1; a++){
                            for(int b = (-1); b <= 1; b++){
                                for(int c = (-1); c <= 1; c++/*C++ hehehe*/){
                                    for(int d = (-1); d <= 1; d++){
                                        vector direction;
                                        direction.w = a;
                                        direction.x = b;
                                        direction.y = c;
                                        direction.z = d;

                                        vector position;
                                        position.w = i;
                                        position.x = j;
                                        position.y = k;
                                        position.z = l;

                                        if(line(board, s, position, direction, 1) == s){
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

int main(){

    int s = 3; //The length of your tic tac toe board. Typically set to 3 when in two dimensions.
    char turn = 'O';
    vector move;

    s = input_size();

    //this mess of code allocates the 4d array
    char ****board = (char****) malloc(s * sizeof(char***));
    for(int i = 0; i < s; i++){
        board[i] = (char***) malloc(s * sizeof(char**));
        for(int j = 0; j < s; j++){
            board[i][j] = (char**) malloc(s * sizeof(char*));
            for(int k = 0; k < s; k++){
                board[i][j][k] = (char*) malloc(s * sizeof(char));
            }
        }
    }

    //this tower of for loops makes everything a space
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            for(int k = 0; k < s; k++){
                for(int l = 0; l < s; l++){
                    board[i][j][k][l] = ' ';
                }
            }
        }
    }

    putchar('\n');
    print_board_grid(board, s);

    do{

        if(turn == 'O'){
            turn = 'X';
        }
        else{
            turn = 'O';
        }

        putchar('\n');
        move = input_move(board, s, turn);
        board[move.w][move.x][move.y][move.z] = turn;

        putchar('\n');
        print_board_grid(board, s);

    }while(!victory(turn, board, s));

    printf("\n%c's have won!\n", turn);

    //if god wanted my code to be memory safe, he wouldn't have given me access to malloc
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            for(int k = 0; k < s; k++){
                free(board[i][j][k]);
            }
            free(board[i][j]);
        }
        free(board[i]);
    }
    free(board);

    return 0;
}