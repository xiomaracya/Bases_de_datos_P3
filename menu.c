#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "utils.h"

#define BUFFER 100

char *showUse();
void showInsert(char *dataname, char *indexname);
void showPrint(char *indexname);
int ShowMainMenu();

int main()
{
    char *dataname = "\0";
    int nChoice = 0;
    char *indexname;

    do
    {
        nChoice = ShowMainMenu();
        switch (nChoice)
        {
        case 1:
        {
            dataname = showUse();
            if(dataname == NULL){
                return 1;
            }
            indexname = (char*)malloc(BUFFER*sizeof(char));
            if(indexname == NULL){
                free(dataname);
                return 1;
            }
            replaceExtensionByIdx(dataname, indexname);
        }
        break;

        case 2:
        {
            if (strcmp(dataname, "\0")==0){
                printf("There is no table in use");
            }else {
                showInsert(dataname, indexname);
            }
        }
        break;

        case 3:
        {
            if (strcmp(dataname, "\0")==0){
                printf("There is no table in use");
            }else {
                showPrint(indexname);
            }
        }
        break;

        case 4:
        {
            printf("EXIT\n\n");
        }
        break;
        }
    } while (nChoice != 4);
    if(strcmp(dataname, "\0")){
        free(dataname);
        free(indexname);
    }
    return 0;
}

/**
 * @brief prints main menu and allows to select an option.
 *
 *
 * @return selected entry in the menu
 * @author rmarabini
 */

int ShowMainMenu()
{
    int nSelected = 0;
    char use[BUFFER] = "use\0";
    char insert[BUFFER] = "insert\0";
    char print[BUFFER] = "print\0";
    char exit[BUFFER] = "exit\0";
    char buf[BUFFER]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do
    {
        printf("\nMENU\n");
        printf("You can choose what you want to do\n\n");

        printf(" USE\n"
               " INSERT\n"
               " PRINT\n"
               " EXIT\n\n"
               "Enter your choice > ");
        strcpy(buf, "\0");
        if (!fgets(buf, BUFFER, stdin)){
            nSelected = 0;
        }
        buf[strlen(buf)-1]='\0';
            /* reading input failed, give up: */
        if (strcasecmp(buf, use)==0){
            nSelected = 1;
        }else if (strcasecmp(buf, insert)==0){
            nSelected = 2;
        }else if (strcasecmp(buf, print)==0){
            nSelected = 3;
        }else if (strcasecmp(buf, exit)==0){
            nSelected = 4;
        }else {
            nSelected = 0;
        }


        if ((nSelected < 1) || (nSelected > 4))
        {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

char *showUse(){
    char *name;
    name = (char*)malloc(BUFFER*sizeof(char));
    if(name == NULL){
        return NULL;
    }

    printf("Enter the table name >");
    if (!fgets(name , BUFFER, stdin)){
        free(name);
        return NULL;
    }
    name[strlen(name)-1]='\0';
    printf("The table %s is ready\n", name);
    createTable(name);
    return name;
}
void showInsert(char *dataname, char *indexname){
    Book *book = NULL;
    char pk[BUFFER] = "\0";
    char title[BUFFER] = "\0";

    book = (Book*)malloc(sizeof(Book));
    if (book == NULL){
        printf("Error");
        return;
    }
    printf("Enter the key >");
    if (!fgets(pk , BUFFER, stdin)){
        free(book);
        return;
    }
    printf("Enter the title >");
    if (!fgets(title , BUFFER, stdin)){
        free(book);
        return;
    }

    strcpy(book->book_id, pk);
    book->title = title;
    book->title_len = strlen(title);
    addTableEntry(book, dataname, indexname);
    return;
}
void showPrint(char *indexname){
    int level = 1;
    int number_nodes;
    int i;
    FILE *f = NULL;

    f = fopen(indexname, "rb");
    if(f==NULL){
        
        return;
    }
    printf("The table is going to be printed\n");
    fseek(f, 0, SEEK_END);
    number_nodes = ftell(f);
    number_nodes=(number_nodes-INDEX_HEADER_SIZE)/INDEX_REGISTER_SIZE;
    if(number_nodes==0){
        printf("The tree is empty");
        return;
    }
    for(i=0; 1<number_nodes; i++){
        number_nodes/=2;
        level++;
    }
    printTree(level+100, indexname);
    return;
}