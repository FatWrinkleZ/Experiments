#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int numWords = 1;
size_t size = 1;

typedef struct Word{
    char word[24];
}Word;

Word* dictionary;

char lastTypedWord[24];
char* goalWord;
bool gameOver = false;

void red () {
  printf("\033[1;31m");
}
void white(){
  printf("\033[0;37m");
}
void reset () {
  printf("\033[0m");
}

int main(){
    srand(time(0));
    size = sizeof(Word)* numWords;
    dictionary = (Word*)malloc(sizeof(Word) * numWords);
    FILE* wordFile;
    wordFile = fopen("WORDS", "r");
    char buf[24];

    while(fscanf(wordFile,"%s",buf) > 0){

        dictionary = (Word*)realloc(dictionary, (sizeof(Word)*numWords+1));
        size+=strlen(buf)*sizeof(char);
        sprintf(dictionary[numWords-1].word,"%s", buf);
        //printf("%s", dictionary(wor))
        numWords++;
        
    }
    goalWord = dictionary[rand()%numWords].word;
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    for(int i = 0; i < strlen(goalWord); i++){
        printf("[_]");
        //printf("[%c]", goalWord[i]);
    }
    printf("\n");
    while(!gameOver){
        printf(">");
        scanf("%s", lastTypedWord);
        if(strcmp(lastTypedWord, goalWord)==0){
            gameOver = true;
        }
        for(int i = 0; i < strlen(goalWord); i++){
            if(lastTypedWord[i] == goalWord[i]){
                white();
            }else{
                red();
            }
            printf("[%c]", lastTypedWord[i]);
        }
        reset();
        printf("\n");
    }
    printf("Congragulations You Won!\n");

}