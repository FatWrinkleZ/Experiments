#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#include <sys/types.h>
#endif
#include <time.h>

#define WIDTH 60
#define HEIGHT 30

void RenderScreen();
int PlayLoop();
void GetInput();

typedef struct Vector2{
    int x;
    int y;
}Vector2;

Vector2 *snakePieces;

Vector2 dir;
Vector2 goalPos;
int snakeLen = 3;

int SCORE = 0;

bool isAlive = true;

#ifndef _WIN32
WINDOW* scrn;
#endif

#ifndef _WIN32
int kbhit(void)
{
    struct timeval timeout;
    fd_set readfds;
    int how;

    /* look only at stdin (fd = 0) */
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    /* poll: return immediately */
    timeout.tv_sec = 0L;
    timeout.tv_usec = 0L;

    how = select(1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);
    /* Change "&timeout" above to "(struct timeval *)0" ^^^^^^^^
    * if you want to wait until a key is hit
    */

    if ((how > 0) && FD_ISSET(0, &readfds))
    return 1;
    else
    return 0;
}
#endif

void GenerateGoalPos(){
    srand(time(0));
    goalPos.x = 1 + (rand()%(WIDTH-1));
    goalPos.y = 1 + (rand()%(HEIGHT-1));

}

int main(){
    #ifdef _WIN32
    #else
    scrn = initscr();
    //notimeout();
    //nodelay(scrn, TRUE);
    printw("Epic style linux detected");
    #endif
    dir.x = 0;
    dir.y=1;
    GenerateGoalPos();
    snakePieces = (Vector2*)malloc(3 * sizeof(Vector2));
    for(int i = 0; i < snakeLen;i++){
        snakePieces[i].x = i;
    }
    PlayLoop();

}

void RenderScreen(){
    #ifdef _WIN32
    system("cls");
    #else
    clear();
    #endif

    for(int y = HEIGHT-1; y>=0;y--){
        for(int x = 0; x < WIDTH; x++){
            int beenPlaced = 0;
            for(int i = 0; i < snakeLen; i++){
                if(x == snakePieces[i].x && y == snakePieces[i].y){
                    #ifdef _WIN32
                    printf("O");
                    #else
                    printw("O");
                    #endif
                    beenPlaced = 1;
                }
            }
            if(beenPlaced != 1){
            #ifdef _WIN32
            if(x == goalPos.x && y == goalPos.y){
                printf("$");
            }else{printf(".");}
            #else
            if(x==goalPos.x && y==goalPos.y){printw("$");}
            else{printw(".");}
            #endif
            }
        }
        #ifdef _WIN32
        printf("\n");
        #else
        printw("\n");
        #endif
    }
    

    #ifdef _WIN32
    printf("SCORE : %d\n", SCORE);
    Sleep(100);
    #else
    printw("SCORE : %d\n", SCORE);
    refresh();
    usleep(100000);
    #endif
}

int PlayLoop(){
    Vector2 oldPos;
    for(int i = 0; i < snakeLen;i++){
        Vector2 temp = snakePieces[i];
        if(i == 0){
            temp = snakePieces[i];
            snakePieces[i].x += dir.x;
            snakePieces[i].y += dir.y;
            oldPos = temp;
            //continue;
        }else{
            snakePieces[i].x = oldPos.x;
            snakePieces[i].y = oldPos.y;
            oldPos = temp;
        }
    }
    Vector2 snakeHead = snakePieces[0];
    if(snakeHead.x == goalPos.x && snakeHead.y == goalPos.y){
        GenerateGoalPos();
        SCORE++;
        snakeLen++;
        snakePieces = (Vector2*)realloc(snakePieces, snakeLen*sizeof(Vector2));
        snakePieces[snakeLen-1].x = snakePieces[snakeLen-2].x-dir.x;
        snakePieces[snakeLen-1].y = snakePieces[snakeLen-2].y - dir.y;
    }
    if(snakeHead.x < 0 || snakeHead.x >= WIDTH || snakeHead.y < 0 || snakeHead.y >= HEIGHT){
        isAlive = false;
    }
    for(int i = 1; i < snakeLen; i++){
        if(snakeHead.x == snakePieces[i].x && snakePieces[i].y == snakeHead.y){
            isAlive = false;
        }
    }

    RenderScreen();
    GetInput();
    if(isAlive){return PlayLoop();}else{
        #ifdef _WIN32
        system("cls");
        #else
        clear();
        nodelay(scrn, FALSE);
        notimeout(scrn, FALSE);
        refresh();
        endwin();
        system("clear");
        #endif
    }
    if(!isAlive){
        free(snakePieces);
        printf("GAME OVER! FINAL SCORE : %d\n",SCORE);
        exit(0);
        return 0;
    }
    return 0;
    
}

void GetInput(){
    char c = 0;
    while(kbhit()!=0){
        char c = wgetch(scrn);
        switch(c){
            case 'w':
            if(dir.y == 0){
            dir.x = 0;
            dir.y = 1;
            }
            break;
            case 'a':
            if(dir.x == 0){
            dir.x = -1;
            dir.y = 0;
            }
            break;
            case 'd':
            if(dir.x==0){
            dir.x = 1;
            dir.y = 0;
            }
            break;
            case 's':
            if(dir.y==0){
            dir.x = 0;
            dir.y = -1;
            }
            break;
            case '.':
            #ifndef _WIN32
            endwin();
            #endif
            exit(0);
            break;
            default:
            break;
        }
        //endwin();
    }
}