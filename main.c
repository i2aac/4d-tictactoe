#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

#define MAX_FILENAME 64 //The maximum length of any files printed by the program
#define MAX_YES_NO 256 //The maximum length of any yes-or-no question asked by the program

char texts[][MAX_FILENAME] = { //None of these strings actually need to be stored beforehand, but it avoids compiler warnings if they are
"lesson.txt",
"tutorial.txt"
}, questions[][MAX_YES_NO] = {
"\nIt is understood that 4-D tic-tac-toe may be difficult to understand as a game to mere 3-D beings. Would you like a lesson on 4-D tic-tac-toe?",
"\nIt is understood that the used coordinate system may not be immediately obvious to mere 3-D beings. Would you like a tutorial on how the board works?",
"\nContinue?",
"\nAre you sure you wish to use this number as the board size?"
};

struct vector{ //A 4-D struct vector
    int w, x, y, z;
};

/*
void print_board_linear(char ****board, int s){ //Old function
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

bool in_bounds(int bound, struct vector position){ //Returns true if a struct vector is in bounds.
    return (0 <= position.w && position.w < bound && 0 <= position.x && position.x < bound && 0 <= position.y && position.y < bound && 0 <= position.z && position.z < bound);
}

bool yes_or_no(char question[MAX_YES_NO]){ //Returns true if yes, false if no
    char answer;

    do{
        printf("%s (y/n): ", question);
        scanf(" %c", &answer);
    }while(answer != 'y' && answer != 'n' && answer != 'Y' && answer != 'N');

    if(answer == 'y' || answer == 'Y'){
        return true;
    }

    return false;
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
            if(!yes_or_no(questions[3])){
                size = (-1);
                putchar('\n');
            }
        }

    }while(size <= 0);

    return size;
}

struct vector input_move(char ****board, int s, char turn){ //Prompts user to input a move, and returns a user-selected placement for their marker once a valid value has been chosen.

    struct vector placement;

    do{
        printf("%c's turn. Enter a coordinate (of the format w x y z) to mark: ", turn);
        scanf("%d %d %d %d", &placement.w, &placement.x, &placement.y, &placement.z);

        if(!in_bounds(s, placement) || board[placement.w][placement.x][placement.y][placement.z] != ' '){
            printf("\nThat is not a valid move. ");
        }

    }while(!in_bounds(s, placement) || board[placement.w][placement.x][placement.y][placement.z] != ' ');

    return placement;
}

struct vector add(struct vector a, struct vector b){ //Returns sum of struct vector A and B.
    struct vector r;
    r.w = a.w + b.w;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

bool zero_vector(struct vector a){ //Returns false if vector is a zero vector
    return (a.w + a.x + a.y + a.z == 0);
}

/*
The next two functions are where the magic happens. They're an implementation of a simple but rather inefficient algorithm to check for
straight lines on a 4-D tic-tac-toe board.

First, two things must be established:

1. Every space on the board has a position vector associated with it. This position vector is the sum of an integer amount of 
unit vectors, corresponding to that space's coordinates.

2. Every line on the board can be expressed as a position vector, where the endpoint of the vector is the endpoint of the line,
relative to one of two ends of the line. This position vector can be divided by the board side length, n, so produce a direction
vector. The direction vector comprises of at most 1 unit vector in each axis, and always travels directly from one space to
another.

The algorithm cycles through every single space with the desired marking in it. It then assumes that the space represents one
of two ends of a line.

For that space, it then checks through all possible direction vectors. (Yes, it is a finite amount, directly corresponding to
the amount of dimensions of the board.) For every direction vector that leads to another space with the desired marking when
added to the current space's positional vector, the algorithm continues to add that direction vector to the resultant vector
until it stops leading to spaces with the desired marking. Once this occurs, the algorithm compares the amount of times it had
to add the directional vector, to the side length of the board. Since all winning lines must be n spaces long, this comparison
immediately reveals if the discovered line has won the game or not.

There is much optimization to be done to prevent redundant checking of spaces, but I have implemented none of it here.
*/

int line(char ****board, int s, struct vector position, struct vector direction, int length){ //Follows a "line" through on a board until it stops. Returns its length.

    struct vector new_position = add(position, direction); //A new position vector is created by adding both vectors passed in. This results in a new position vector

    if(in_bounds(s, new_position) && board[new_position.w][new_position.x][new_position.y][new_position.z] == board[position.w][position.x][position.y][position.z]){
        //If the new position vector leads to another space with the desired marking, the function calls itself again to repeat the previous addition.
        return line(board, s, new_position, direction, length + 1);
    }

    return length; //Returns length if there ended up being no valid continuation of the line
}

bool victory(char p, char ****board, int s){ //This function returns true if it finds a winning pattern made from the given marking

    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            for(int k = 0; k < s; k++){
                for(int l = 0; l < s; l++){

                    //The above loops cycle through every space

                    if(board[i][j][k][l] == p){ //This limits the next part to only spaces with the desired marking

                        for(int a = (-1); a <= 1; a++){
                            for(int b = (-1); b <= 1; b++){
                                for(int c = (-1); c <= 1; c++/*C++ hehehe*/){
                                    for(int d = (-1); d <= 1; d++){

                                        //The above loops cycle through all possible direction vectors for a winning line

                                        struct vector direction; //See? the direction vector
                                        direction.w = a;
                                        direction.x = b;
                                        direction.y = c;
                                        direction.z = d;

                                        struct vector position; //Its most memory efficient if the position vector doesn't get made in an earlier loop
                                        position.w = i;
                                        position.x = j;
                                        position.y = k;
                                        position.z = l;

                                        if(!zero_vector(direction) && line(board, s, position, direction, 1) == s){ //This passes off both vectors to the line() function to see how far it can go
                                            //It should also be noted that passing in zero vectors for the direction vector would cause line() to loop infinitely, so they are pruned beforehand
                                            return true; //There is no need to check for more winning patterns after one is found
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

    return false; //Returns false if no winning lines found
}

void print_file(char name[MAX_FILENAME]){

    char c, answer;
    FILE *fptr = fopen(name, "r");
    
    if(fptr == NULL){
        printf("%s not found.\n", name);
        return;
    }

    c = fgetc(fptr);
    while(c != '!'){

        if(c == '/'){
            if(yes_or_no(questions[2])){
                c = ' ';
            }
            else{
                break;
            }
        }

        putchar(c);
        c = fgetc(fptr);
    }

    fclose(fptr);

    return;
}

int main(){

    int s = 3; //The length of your tic tac toe board. Typically set to 3 when in two dimensions.
    char turn = 'O';

    struct vector move;

    printf("Welcome to 4-D tic-tac-toe.\n");

    if(yes_or_no(questions[0])){
        putchar('\n');
        print_file(texts[0]);
    }

    if(yes_or_no(questions[1])){
        putchar('\n');
        print_file(texts[1]);
    }

    putchar('\n');
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

    do{ //This is the actual game part of the program

        if(turn == 'O'){ //after every turn, this bit of code switches whose turn it is
            turn = 'X';
        }
        else{
            turn = 'O';
        }

        putchar('\n'); //this code handles taking player input, and allowing them to make a move
        move = input_move(board, s, turn);
        board[move.w][move.x][move.y][move.z] = turn;

        putchar('\n'); //prints board
        print_board_grid(board, s);

    }while(!victory(turn, board, s)); //after every turn, the game checks if the most recent move has won the game

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