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



/*
 * this function gets a key from the keyboard. Its a function given by the instructors.
 */
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
/*
 * This function checks if the char is a correct char with a meaning in the game
 */
bool checkChar(char c){
    if (c=='a' || c=='s' || c=='d' || c=='w' || c=='q'){
        return true;
    }
    else{
        return false;
    }
}
/*
 * This function checks if the key is a q of not.
 */
bool checkIfQ(char q){
    if (q == 'q'){
        return true;
    }
    else{
        return false;
    }
}

/*
 * This function runs the main program. It runs "draw.out" and writes to the pipe.
 */
void run() {
    int filedes[2];
    //chdir(getenv("HOME"));
    int ret = pipe(filedes);
    if (ret == -1){
        char error_message[] = "Failed to open a pipe\n";
        write(STDERR_FILENO, error_message, sizeof(error_message));
        exit(1);
    }
    //create the output compile line (runs ex31)
    char *argv[2] = {"./draw.out", NULL};
    //Spawn a child to run the program.
    pid_t pid = fork();
    if (pid < 0){
        char error_message[] = "Error in System call\n";
        write(STDERR_FILENO, error_message, sizeof(error_message));
    }
    // child process
    if (pid == 0) {
        //#######check if this is correct
        close(filedes[1]);
        //use filedes[0] to read
        dup2(filedes[0], STDIN_FILENO);

        if ((execvp(argv[0], argv) < 0)) {
            char error_message[] = "Error in System call\n";
            write(STDERR_FILENO, error_message, sizeof(error_message));
        }

        //parent process
    } else {
        while (1) {
            char c = getch();

            if (checkChar(c)) {
                if (checkIfQ(c)) {
                    //exit
			        system("clear");

                    //kill(pid, SIGKILL);
                    break;
                }
                //######check if this is right.
                close(filedes[0]);
                //use filedes[1] to write
                if (write(filedes[1], &c, 1) < 0) {
                    char error_message[] = "Failed to write to pipe\n";
                    write(STDERR_FILENO, error_message, sizeof(error_message));
                }
                if (kill(pid, SIGUSR2) < 0) {
                    char error_message[] = "Failed to send signal\n";
                    write(STDERR_FILENO, error_message, sizeof(error_message));
                }
            }
        }
    }
}

/*
 * This is the main function of the program.
 */
int main() {

    run();
    return 0;
}



