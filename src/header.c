#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFF 50

/* Reads file.
 * Takes a FILE * and a node *
 * as arguments, reads the file and
 * assign every term to a node. Finally,
 * returns the root node.
 * */
node * readFile( FILE * inputfile, node * rootOfCurrentFileTerms ){
    int currentChar;
    if ( inputfile == 0 ){
        printf( "Can not open the file" );
    }
    else {
        int wordFound = 0;
        int countForIndex = 0;
        char word[BUFF];
        while ( ( currentChar = fgetc( inputfile ) ) != EOF ){
            if( isalpha( currentChar ) ) {
                currentChar = tolower( currentChar );
                word[countForIndex++] = currentChar;
                wordFound = 1;
            }
            else if( wordFound ){
                word[countForIndex] = '\0';
                int wordLength = strlen( word );
                rootOfCurrentFileTerms = createNode( rootOfCurrentFileTerms, word, wordLength, 1 );
                countForIndex = 0;
                wordFound = 0;
            }
        }
        if( wordFound ){
            word[countForIndex] = '\0';
            int wordLength = strlen( word );
            rootOfCurrentFileTerms = createNode( rootOfCurrentFileTerms, word, wordLength, 1 );
        }
    }
    return rootOfCurrentFileTerms;
}


/* Creates a node for every term.
 * Takes the root of the linked list,
 * the term and the word's length.
 * Returns the root.
 * */
node * createNode( node * root, char * term, int wordLength, int freq ){
    char * currentWord;
    currentWord =( char* )malloc( sizeof( char ) * ( wordLength + 1 ) );

    /*Copying given term.*/
    char currentChar = term[0];
    int counter = 0;
    while( currentChar != '\0' ){
        currentWord[counter] = term[counter];
        counter++;
        currentChar = term[counter];
    }
    currentWord[wordLength] = '\0';

    if( root == NULL ){
        root = ( node * )malloc( sizeof( node ) );
        root->term = currentWord;
        root->freq = freq;
        root->next = NULL;
        return root;
    }
    node * iter = root;
    while ( iter->next != NULL ){
        iter = iter->next;
    }
    node * temp;
    temp = ( node * )malloc( sizeof( node ) );
    temp->term = currentWord;
    temp->freq = freq;
    temp->next = NULL;
    iter->next = temp;
    return root;

}


/* Comparing terms for frequency.
 * Takes terms as arguments.
 * Return 1 for true, 0 for false.
 * */
int compareTerms( char * firstTerm, char * secondTerm ){
    if( strlen( firstTerm ) != strlen( secondTerm ) ){
        return 0;
    }
    int count = 0;
    while( firstTerm[count] != '\0' ){
        if ( firstTerm[count] == secondTerm[count] ){
            count++;
        }
        else
            return 0;
        }
        if ( count == strlen( firstTerm ) )
            return 1;
        return 0;

}
/* Takes a root of node as an argument
 * and deallocates the root's memory.
 * */
void deallocateMemory ( node * root ){
    while ( root != NULL ){
        node * temp = root;
        root = root->next;
        free( temp->term );
        free( temp );
    }
}


/* Takes a node of a linked list. Deletes
 * the nodes which have the same terms with
 * the taken node for holding frequency.
 * Return the node.
 * */
node * deleteSameNode( node * currentNode ){
    node * iter = NULL;
    iter = currentNode;
    while ( iter->next != NULL ) {
        if ( compareTerms( iter->next->term, currentNode->term ) ) {
            currentNode->freq++;
            node *temp;
            temp = iter->next;
            iter->next = iter->next->next;
            free( temp->term );
            free( temp );
        }
        else
            iter = iter->next;
        if( iter == NULL ){
            break;
        }
    }
    return currentNode;
}


/* Takes a root node of a linked list.
 * Counts the term of given node's frequency.
 * Returns the root.
 * */
node * countFrequencies( node * rootNode ){
    node * iter = rootNode;
    while( iter != NULL ){
        iter = deleteSameNode( iter );
        iter = iter->next;
    }
    return rootNode;
}


/* Takes a root node and int value of the number of files.
 * Deletes the words which don't fulfill the conditions
 * from a linked list.
 * */
node * gettingTermsForSimilarity( node * rootNode, int numOfFiles ){
    while ( rootNode->freq >= 10 || rootNode->freq < numOfFiles ){
        node * tempForFree = rootNode;
        rootNode = rootNode->next;
        free( tempForFree->term );
        free( tempForFree );
    }
    node * iter = rootNode;
    while( iter->next != NULL ){
        if( iter->next->freq >= 10 || iter->next->freq < numOfFiles ){
            node * tempForFree = iter->next;
            iter->next = iter->next->next;
            free( tempForFree->term );
            free( tempForFree );
        }
        else
            iter = iter->next;
        if( iter == NULL ){
            return rootNode;
        }
    }
    return rootNode;
}

/* Takes a file and a root node.
 * Executes files and creates linked
 * lists and counts terms' frequencies.
 * Returns the root node.
 * */
node * executeFile( FILE * currentFile, node * currentRoot ){
    currentRoot = readFile( currentFile, currentRoot );
    currentRoot = countFrequencies( currentRoot );
    return currentRoot;
}

/* Takes an array of linked lists which hold terms in given
 * files and int value of the number of files.
 * Counts term' frequencies in all given
 * files and deletes which don't fulfill
 * the conditions.
 * Returns a node of linked list which contains terms which
 * fulfill the conditions.
 * */
node * countTermsInAllFiles( node *allFiles[], int numOfFiles ){
    node * termsInAllFiles;
    termsInAllFiles = NULL;
    int i;
    for( i = 0; i < numOfFiles; i++ ){
        while( allFiles[i] != NULL ){
            int wordLength = strlen( allFiles[i]->term );
            int freqOfCurrentWord = allFiles[i]->freq;
            int j;
            for ( j = 0; j < numOfFiles; j++ ){
                if ( i != j ){
                    node * iter;
                    while( allFiles[j] != NULL && compareTerms( allFiles[i]->term, allFiles[j]->term ) ){
                        node * tempForFree = allFiles[j];
                        freqOfCurrentWord += allFiles[j]->freq;
                        allFiles[j] = allFiles[j]->next;
                        free( tempForFree->term );
                        free( tempForFree );
                    }
                    if ( allFiles[j] != NULL ){
                        iter = allFiles[j];
                        while( iter->next != NULL ){
                            if( compareTerms( allFiles[i]->term, iter->next->term ) ){
                                node * tempForFree = iter->next;
                                freqOfCurrentWord += iter->next->freq;
                                iter->next = iter->next->next;
                                free( tempForFree->term );
                                free( tempForFree );
                            }
                            else
                                iter = iter->next;
                            if( iter == NULL ){
                                break;
                            }
                        }
                    }
                }
            }
            termsInAllFiles = createNode( termsInAllFiles, allFiles[i]->term, wordLength, freqOfCurrentWord );
            allFiles[i] = allFiles[i]->next;
        }
    }
    return termsInAllFiles;
}

