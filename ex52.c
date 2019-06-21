//
// Noam Schwartz
// 200160042
//




#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <stdbool.h>
#include <error.h>

typedef struct Environment{
    char matrix[20][20];
    int x;
    int y;
    int alignment;
}Environment;

Environment environment;

/*
 * This function changes the objects y value from vertical to horizontal and vica verca.
 */
Environment* changeYSpace(Environment* env, int minus, int x, int y){
    if (!minus) {
        env->matrix[env->x][env->y] = ' ';
        env->matrix[env->x][env->y + 1] = ' ';
        env->matrix[env->x][env->y + 2] = ' ';
        return env;
    }
    //If the sign is minus
    else{
        env->matrix[x][y] = '-';
        env->matrix[x][y + 1] = '-';
        env->matrix[x][y + 2] = '-';
    return env;
    }

}
/*
 * This function changes the objects x value from vertical to horizontal and vica verca.
 */
Environment* changeXSpace(Environment* env, int minus, int x, int y){
    if (!minus) {
        env->matrix[env->x][env->y] = ' ';
        env->matrix[env->x + 1][env->y] = ' ';
        env->matrix[env->x + 2][env->y] = ' ';
        return env;
    }
    //change the - sign
    else{
        env->matrix[x][y] = '-';
        env->matrix[x + 1][y] = '-';
        env->matrix[x + 2][y] = '-';
    return env;
    }

}

/*
 * This function creates the gameBoard envirenment using * and blank spaces
 */
void newEnvironment(Environment *env){
    int i, j;
    //create bottom line
    for (j = 0; j < 20; j++) {
        env->matrix[19][j] = '*';
    }
    for (i = 0; i < 20 - 1; i++) {
        env->matrix[i][0] = '*';
        env->matrix[i][19] = '*';
        //clear game surface.
        for (j = 1; j < 19; j++) {
            env->matrix[i][j] = ' ';
            }

        }
    }
/*
 * This function prints the board to the surface. It cleas the previous one and prints the new one.
 */
void print(Environment* env) {
    if (system("clear") < 0) {
        char error_message[] = "Error clearing board\n";
        write(STDERR_FILENO, error_message, sizeof(error_message));
    }
    int i;
    int j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            printf("%c", env->matrix[i][j]);
        }
        printf("\n");
    }
}
/*
 * This function updates the objects place when a key is pressed. It also makes sure the object stays
 * within the gameboard borders
 */
void update(Environment* env, int x, int y, int flip) {
    //vertical
    if (env->alignment == 1 ) {
        if ((y <= 0) || (y >= 19))
            return;
    }
    //horizontal
    else{
        if ((y <= 0)|| (y + 2 >= 19 ))
            return;
    }
    //keep as is
    if (flip == 0) {
        if (env->alignment == 1) {
          changeXSpace(env, 0, 0, 0);
        } else {
          changeYSpace(env, 0, 0, 0);
        }
    }
    environment.x = x;
    environment.y = y;
    //horizontal
    if (env->alignment == 0) {
      changeYSpace(env, 1, x, y);
      //if the object goes beyond its borders
        if (x >= 19) {
            //initialize
            env->alignment = 0;
            env->x = 0;
            env->y = 9;
            newEnvironment(env);
            update(env, env->x, env->y, env->alignment);

        }

    } else {
     changeXSpace(env, 1, x, y);
     //if the object goes beyond its borders
        if (x + 2 >= 19) {
            //initialize
            env->alignment = 0;
            env->x = 0;
            env->y = 9;
            newEnvironment(env);
            update(env, env->x, env->y, env->alignment);
        }
    }
}
/*
 * THis function moves the object to its new spot according to the key recieved.
 */
void move() {
    signal(SIGALRM, move);
    alarm(1);
    update(&environment, environment.x + 1, environment.y, 0);
    print(&environment);
}
/*
 * This function flips the object from horizontal to vertical and otherwise.
 */
void flip(Environment *env) {
    if (env->alignment == 0) {
        changeYSpace(env, 0,0,0);
    } else {
        changeXSpace(env, 0,0,0);
    }

    if (env->alignment == 0) {
        env->alignment = 1;
        update(env , env->x - 1, env->y + 1, 1);
    } else  {
        int y = env->y - 1;

        if (y + 2 >= 19) {
            y = 16;
        }
        env->alignment = 0;
        update(env, env->x + 1, y - 1, 1);
    }
}

/*
 * This function returns the key pressed and perfomes the action according to the keys meaning.
 */
void option() {
    signal(SIGUSR2, option);
    char c = getchar();
    if (c == 'q') {
        exit(1);
    }
    if (c == 'a') {
        update(&environment, environment.x, environment.y - 1, 0);
    }
    if (c == 's'){
        update(&environment, environment.x + 1, environment.y, 0);
    }
    if (c == 'd'){
        update(&environment, environment.x, environment.y + 1, 0);
    }
   if (c == 'w') {
       flip(&environment);
   }
    print(&environment);
}

/*
 * This function runs the program it creates an envirenment and then coordinates with the signals
 */
void run(){
    system("clear");
    environment.alignment = 0;
    environment.x = 0;
    environment.y = 9;
    newEnvironment(&environment);
    update(&environment, environment.x, environment.y, environment.alignment);
    print(&environment);
    signal(SIGALRM, move);
    alarm(1);
    signal(SIGUSR2, option);
    while (1) {
        pause();
    }


}

/*
 * THis function the the main function of the program.
 */
int main() {
    run();
    return 0;
}

