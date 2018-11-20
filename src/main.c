#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "header.h"

int main ( int argc, char **argv ) {
    /* Reading txt files in the current
     * directory and assign them into an array.
     * */
    char *files[10];
    int numOfFiles = 0;
    DIR *dir;
    struct dirent *currentFile;
    if ( ( dir = opendir ("./") ) != NULL ) {
        /* Printing all the txt files within directory */
        while ( ( currentFile = readdir ( dir ) ) != NULL ) {
            int length = strlen( currentFile->d_name );
            if (strncmp( currentFile->d_name + length - 4, ".txt", 4 ) == 0 ) {
                files[numOfFiles] = malloc( sizeof(char) * (strlen(currentFile->d_name) + 1) );
                strcpy( files[numOfFiles], currentFile->d_name );
                numOfFiles++;
            }
        }
        closedir (dir);
    }
    else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    char **ptrToFiles = files;

    /* Assigning terms to nodes and
     * creating linked lists.
     * */
    node * allFiles[numOfFiles];
    FILE * ptrCurrentFile;
    int i;
    for( i = 0; i < numOfFiles; i++){
        ptrCurrentFile = fopen( ptrToFiles[i], "r" );
        allFiles[i] = NULL;
        allFiles[i] = executeFile( ptrCurrentFile, allFiles[i] );
        fclose(ptrCurrentFile);
    }
    /* Creating nodes and linked list again.
     * Because we will delete some nodes of former
     * linked lists in upcoming part.
     * */
    node * allFilesForMatrix[numOfFiles];
    FILE * ptrCurrentFile1;
    int c;
    for( c = 0; c < numOfFiles; c++ ){
        ptrCurrentFile1 = fopen( ptrToFiles[c], "r" );
        allFilesForMatrix[c] = NULL;
        allFilesForMatrix[c] = executeFile( ptrCurrentFile1, allFilesForMatrix[c] );
        fclose( ptrCurrentFile1 );
    }

    /* Creating a linked list for terms which
     * will be used for calculating similarity.*/
    node * termsInAllFiles;
    termsInAllFiles = countTermsInAllFiles( allFiles, numOfFiles );
    termsInAllFiles = gettingTermsForSimilarity( termsInAllFiles, numOfFiles );

    /* Counting terms for Doc-Term matrix. */
    int numOfTerms = 0;
    node * iter = termsInAllFiles;
    while( iter != NULL ){
        numOfTerms++;
        iter = iter->next;
    }

    /* Creating Doc-Term Matrix for
     * calculating the similarities
     * between files.
     * */
    int docTermMatrix [numOfFiles][numOfTerms];
    int x ;
    int y = 0;
    for(x = 0; x < numOfFiles; x++ ){
        node * iterForAllTerms = termsInAllFiles;
        while( iterForAllTerms != NULL ){
            node * iterForCurrentFileTerms = allFilesForMatrix[x];
            int count = 0;
            while( iterForCurrentFileTerms != NULL ){
                if( compareTerms( iterForAllTerms->term, iterForCurrentFileTerms->term ) ){
                    docTermMatrix[x][y] = iterForCurrentFileTerms->freq;
                    y++;
                    count++;
                    break;
                }
                iterForCurrentFileTerms = iterForCurrentFileTerms->next;
            }
            if( count == 0){
                docTermMatrix[x][y] = 0;
                y++;
            }
            iterForAllTerms = iterForAllTerms->next;
        }
        y = 0;
    }

    /* Calculating similarities. */
    int firstFileIndex;
    int secondFileIndex;
    int indexForTerms;
    for( firstFileIndex = 0; firstFileIndex < numOfFiles; firstFileIndex++ ){
        for( secondFileIndex = firstFileIndex + 1; secondFileIndex < numOfFiles; secondFileIndex++ ){
            float similarityVal = 0;
            float firstRow = 0;
            float secondRow = 0;
            float multVal = 0;
            for ( indexForTerms = 0; indexForTerms < numOfTerms; indexForTerms++ ){
                multVal = multVal + ( docTermMatrix[firstFileIndex][indexForTerms] * docTermMatrix[secondFileIndex][indexForTerms] );
                firstRow = firstRow + pow( docTermMatrix[firstFileIndex][indexForTerms], 2);
                secondRow = secondRow + pow( docTermMatrix[secondFileIndex][indexForTerms], 2);
            }
            firstRow = sqrt( firstRow );
            secondRow = sqrt( secondRow );
            if( ( firstRow * secondRow ) == 0 ){
                printf( "CosSim of %s and %s is: %d\n", ptrToFiles[firstFileIndex], ptrToFiles[secondFileIndex], 0);
            }
            else {
                similarityVal = multVal / ( firstRow * secondRow );
                printf( "CosSim of %s and %s is: %1.3f\n", ptrToFiles[firstFileIndex], ptrToFiles[secondFileIndex], similarityVal );
            }

        }
    }

    /* Deallocation for txt files */
    int indexForFree;
    for( indexForFree = 0; indexForFree < numOfFiles; indexForFree++ ){
        free( ptrToFiles[indexForFree] );
    }
    int indexForDeallocation;
    for( indexForDeallocation = 0; indexForDeallocation < numOfFiles; indexForDeallocation++ ){
        deallocateMemory( allFiles[indexForDeallocation] );
        deallocateMemory( allFilesForMatrix[indexForDeallocation] );
    }
    deallocateMemory( termsInAllFiles );

    return 0;
}

