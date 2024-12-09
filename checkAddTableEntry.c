/*
// Created by roberto on 30/8/20.
*/
#include "tester.h"
#define BUFFER 100

void checkAddTableEntry(const char * tableName)
/**
 *  check AddIndexEntry
 */
{
    bool result;
    int nodeIDOrDataOffset;
    int tmp=-1;
    FILE * fp;
    int deleted;
    char *indexName;
    Book *book;

    indexName = (char*)malloc(BUFFER*sizeof(char));
    if(indexName == NULL){
        exit(EXIT_FAILURE);
    }
    strcpy(indexName,"\0");

    createTestIndexFile(tableName);
    replaceExtensionByIdx(tableName, indexName);

    /* 1) add existing node should complain*/

    result = addTableEntry(&b[0], tableName, indexName);
    if (result){
        fprintf(stderr,
                "Error in addTableEntry, inserted existing key %s",
                b[0].book_id);
        free(indexName);
        exit(EXIT_FAILURE);
    }

    book = (Book*)malloc(sizeof(Book));
    if(book == NULL){
        free(indexName);
        exit(EXIT_FAILURE);
    }
    book->title = (char*)malloc(8*sizeof(char));
    if(book->title == NULL){
        free(indexName);
        free(book);
        exit(EXIT_FAILURE);
    }
    /* add a new node using a existing deleted node */
    printf("-----Original tree-------\n");
    printTree(5, indexName);
    strcpy(book->book_id, "BAR1");
    strcpy(book->title, "titulo1");
    book->title_len = 7;
    deleted = -1;
    result = addTableEntry(book, tableName, indexName);
    findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR2-------\n");
    printTree(4, indexName);
    /* get first to integer shuld be 0 (root) and 8 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be %d"
                " but it is %d", deleted, tmp);
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }

    /* add a new node using the last  existing deleted node */
    strcpy(book->book_id,"RAR3");
    strcpy(book->title, "titulo2");
    book->title_len = 7;
    deleted = -1;
    result = addTableEntry(book, tableName, indexName);
    findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR3-------\n");
    printTree(4, indexName);
    /* get first to integer shuld be 0 (root) and -1 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in acheckAddIndexEntry.cddIndexEntry, first int in file should be %d"
                " but ir is %d", deleted, tmp);
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }

    /* add a new node by expanding file */
    strcpy(book->book_id, "MAR4");
    strcpy(book->title, "titulo3");
    book->title_len = 7;
    deleted = -1;
    result = addTableEntry(book, tableName, indexName);
    findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR4-------\n");
    printTree(5, indexName);
    /* get first to integer shuld be 0 (root) and -1 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be %d"
                " but ir is %d", deleted, tmp);
        free(book->title);
        free(book);
        free(indexName);
        exit(EXIT_FAILURE);
    }
    free(book->title);
    free(book);
    free(indexName);
}

