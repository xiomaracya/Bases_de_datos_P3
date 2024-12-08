#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BufferLength 100

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char *indexName)
{
    char *name;
    char *extension;
    char *aux;

    aux = (char*)malloc((strlen(fileName)+1)*sizeof(char));
    if(aux == NULL){
        return;
    }

    strcpy(aux, fileName);
    name = strtok(aux, ".");
    extension = strtok(NULL, ".");

    if (extension != NULL && strlen(extension) == 3) {
        sprintf(indexName, "%s.idx", name);
    } else {
        strcpy(indexName, fileName);
    }
    free(aux);
}

bool createTable(const char *tableName)
{
    FILE *f = NULL;
    int element = -1;
    char *indexName;
    indexName = (char*)malloc(sizeof(char)*BufferLength);
    if(indexName==NULL){
        return false;
    }
    strcpy(indexName, "\0");

    f = fopen(tableName, "rb");
    if (f == NULL)
    {
        f = fopen(tableName, "wb");
        if (f == NULL)
        {
            free(indexName);
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            free(indexName);
            return false;
        }
    }

    replaceExtensionByIdx(tableName, indexName);

    if (createIndex(indexName) == false)
    {
        fclose(f);
        free(indexName);
        return false;
    }

    fclose(f);
    free(indexName);
    return true;
}

bool createIndex(const char *indexName)
{
    FILE *f = NULL;
    int element = -1;
    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        f = fopen(indexName, "wb");
        if (f == NULL)
        {
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            return false;
        }
        if (fwrite(&element, sizeof(int), 1, f) != 1)
        {
            fclose(f);
            return false;
        }
    }

    fclose(f);
    return true;
}
void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side)
{
    int h1 = 0, h2 = 0, p = 0, datos = 0, i;
    char pk[5] = {0};

    long offset = INDEX_HEADER_SIZE + node_id * INDEX_REGISTER_SIZE;
    if (fseek(indexFileHandler, offset, SEEK_SET) != 0)
    {
        return;
    }

    if (_level > level)
    {
        return;
    }

    if (fread(pk, sizeof(char), sizeof(pk) - 1, indexFileHandler) != sizeof(pk) - 1)
    {
        return;
    }
    if (fread(&h1, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&h2, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&p, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }
    if (fread(&datos, sizeof(int), 1, indexFileHandler) != 1)
    {
        return;
    }

    for (i = 0; i < (int)_level; i++)
    {
        printf("\t");
    }
    printf("%c %s (%d) : %d\n", side, pk, node_id, datos);

    if (h1 != -1)
    {
        printnode(_level+1, level, indexFileHandler, h1, 'l');
    }
    if (h2 != -1)
    {
        printnode(_level+1, level, indexFileHandler, h2, 'r');
    }
    return;
}

void printTree(size_t level, const char *indexName)
{
    FILE *f = NULL;
    int loc_root = -1, borrados = -1;
    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        return;
    }
    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0)
    {
        fclose(f);
        return;
    }
    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return;
    }
    if (fseek(f, loc_root, SEEK_CUR) != 0)
    {
        return;
    }
    printnode(0, level, f, loc_root, ' ');
    if (f)
    {
        fclose(f);
    }
    return;
}

bool findKey(const char *book_id, const char *indexName,
             int *nodeIDOrDataOffset)
{
    int h1 = 0, h2 = 0, p = 0, datos = 0, loc_root = -1, borrados = -1;
    char pk[5] = {0};
    FILE *f;
    int current_node;
    long offset;

    f = fopen(indexName, "rb");
    if (f == NULL)
    {
        return false;
    }

    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0)
    {
        fclose(f);
        return false;
    }
    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return false;
    }
    if (fseek(f, loc_root, SEEK_CUR) != 0)
    {
        return false;
    }
    current_node = loc_root;

    while (current_node != -1)
    {
        offset = INDEX_HEADER_SIZE + current_node * INDEX_REGISTER_SIZE;
        if (fseek(f, offset, SEEK_SET) != 0)
        {
            return false;
        }

        if (fread(pk, sizeof(char), sizeof(pk) - 1, f) != sizeof(pk) - 1)
        {
            return false;
        }
        if (fread(&h1, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&h2, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&p, sizeof(int), 1, f) != 1)
        {
            return false;
        }
        if (fread(&datos, sizeof(int), 1, f) != 1)
        {
            return false;
        }

        if (strncmp(book_id, pk, 4 * sizeof(char)) == 0)
        {
            *nodeIDOrDataOffset = datos;
            fclose(f);
            return true;
        }
        else if (h1 == -1 && h2 == -1)
        {
            *nodeIDOrDataOffset = current_node;
            fclose(f);
            return false;
        }
        else if (strcmp(book_id, pk) < 0)
        {
            *nodeIDOrDataOffset = current_node;
            current_node = h1;
        }
        else
        {
            *nodeIDOrDataOffset = current_node;
            current_node = h2;
        }
    }
    fclose(f);
    return false;
}
bool addIndexEntry(char *book_id, int bookOffset, char const *indexName)
{
    int nodeIDorDataOffset, loc_root, borrados;
    long new_node_id;
    char pk[5] = {0};
    FILE *f = NULL;
    int sin_nodo = -1;
    long offset;
    printf("La clave a añadir es %s\n", book_id);
    printf("El bookoffset es %d\n", bookOffset);

    if (findKey(book_id, indexName, &nodeIDorDataOffset) == true)
    {
        printf("La clave ya está en el índice\n");
        return false;
    }
    printf("El nodeIDorDataOffSet hallado con findKey es %d\n", nodeIDorDataOffset);
    f = fopen(indexName, "r+b");
    if (f == NULL)
    {
        printf("ERROR1");
        return false;
    }

    if (fread(&loc_root, sizeof(int), 1, f) != 1 || loc_root < 0)
    {
        fclose(f);
        return false;
    }
    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if(borrados == -1){
        fseek(f, 0, SEEK_END);
        new_node_id = ftell(f);
        new_node_id=(new_node_id-INDEX_HEADER_SIZE)/INDEX_REGISTER_SIZE;
    } else {
        new_node_id = borrados;
        printf("Hay registros borrados\n");
    } 
    printf("new_node_id = %ld\n", new_node_id);


    offset = INDEX_HEADER_SIZE + nodeIDorDataOffset * INDEX_REGISTER_SIZE;

    if (fseek(f, offset, SEEK_SET) != 0)
    {
        return false;
    }

    if (fread(pk, sizeof(char), sizeof(pk) - 1, f) != sizeof(pk) - 1)
    {
        return false;
    }

    printf("Leemos la pk: %s\n", pk);

    if(strncmp(pk, book_id, 4*sizeof(int))<0){
        printf("Insertamos al lado derecho\n");
        if(fseek(f, offset+8, SEEK_SET) != 0){
            return false;
        }
        fwrite(&new_node_id, sizeof(char), 4, f);
    } else{
        printf("Insertamos al lado iquierdo\n");
        if(fseek(f, offset+4, SEEK_SET) != 0){
            return false;
        }
        fwrite(&new_node_id, sizeof(char), 4, f);
    }

    offset = INDEX_HEADER_SIZE + new_node_id * INDEX_REGISTER_SIZE;

    printf("El registro borrado incial es %d\n", borrados);

    if(borrados!=-1){
        if (fseek(f, offset+4, SEEK_SET) != 0)
        {
            return false;
        }
        if(fread(&borrados, sizeof(int), 1, f)!=1){
            return false;
        }
        printf("El nuevo registro borrado es %d\n", borrados);
        if (fseek(f, 4, SEEK_SET) != 0)
        {
            return false;
        }
        fwrite(&borrados, sizeof(int), 1, f);
    }

    if (fseek(f, offset, SEEK_SET) != 0)
    {
        return false;
    }

    fwrite(book_id, sizeof(char), 4, f);
    fwrite(&sin_nodo, sizeof(int), 1, f);
    fwrite(&sin_nodo, sizeof(int), 1, f);
    fwrite(&nodeIDorDataOffset, sizeof(int), 1, f);
    fwrite(&bookOffset, sizeof(int), 1, f);

    fclose(f);
    printf("Se ha añadido la clave\n\n");

    return true;
}

bool addTableEntry(Book *book, const char *dataName,
                   const char *indexName)
{
    int borrados;
    int nodeIDorDataOffset;
    FILE *f = NULL;
    int bookoffset = 0;
    int borrado_size = 0;
    int new_borrado;

    if(dataName == NULL || indexName == NULL){
        return false;
    }

    f = fopen(dataName, "rb+");
    if (f == NULL)
    {
        return false;
    }


    if (findKey(book->book_id, indexName, &nodeIDorDataOffset) == true)
    {
        printf("La clave ya existe\n");
        return false;
    }

    fseek(f,0,SEEK_SET);

    if (fread(&borrados, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    printf("Borrados es igual a %d\n", borrados);

    if(borrados == 0){
        new_borrado = -1;
        fseek(f, 0, SEEK_SET);
        fwrite(&new_borrado, sizeof(int),1,f);
        fseek(f, 0, SEEK_END);
        bookoffset = ftell(f);
        fwrite(book->book_id, sizeof(char), 4, f);
        fwrite(&book->title_len, sizeof(size_t), 1, f);
        fwrite(book->title, sizeof(char), book->title_len, f);
    } else if (borrados == -1)
    {
        fseek(f, 0, SEEK_END);
        bookoffset = ftell(f);
        fwrite(book->book_id, sizeof(char), 4, f);
        fwrite(&book->title_len, sizeof(size_t), 1, f);
        fwrite(book->title, sizeof(char), book->title_len, f);
    }
    else
    {
        bookoffset = borrados;
        printf("Hay registros borrados y el offset es %d\n", bookoffset);
        while(book->title_len+5>(size_t)borrado_size){
            fseek(f, borrados, SEEK_SET);
            fread(&borrados, sizeof(int), 1, f);
            printf("El siguiente registro borrado es %d\n", borrados);
            fread(&borrado_size, sizeof(int), 1, f);
            printf("El tamaño de este registro es %d\n", borrado_size);
        }


        fseek(f, bookoffset, SEEK_SET);
        fwrite(book->book_id, sizeof(char), 4, f);
        fwrite(&book->title_len, sizeof(size_t), 1, f);
        fwrite(book->title, sizeof(char), book->title_len, f);
        if(borrado_size-5-book->title_len >= 6){
            new_borrado = bookoffset + 4 + 1 + book->title_len;
            fwrite(&borrados, sizeof(int), 1, f);
            borrado_size = borrado_size-5-book->title_len;
            fwrite(&borrado_size, sizeof(int),1,f);
            borrados = new_borrado;
        }
        printf("Borrados tiene el valor de %d\n", borrados);
        fseek(f, 0, SEEK_SET);
        fwrite(&borrados, sizeof(int), 4, f);
    }

    addIndexEntry(book->book_id, bookoffset, indexName);
    fclose(f);
    return true;
}
