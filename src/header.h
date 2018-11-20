#ifndef ASSIGNMENTIV_HEADER_H
#define ASSIGNMENTIV_HEADER_H

#include <stdio.h>

typedef struct node{
    struct node * next;
    int freq;
    char * term;
}node;

node * readFile( FILE *, node *);
node * createNode( node *, char *, int, int );
int compareTerms( char *, char *);
node * deleteSameNode( node * );
node * countFrequencies( node * );
node * gettingTermsForSimilarity( node *, int );
void deallocateMemory ( node * );
node * executeFile( FILE *, node *);
node * countTermsInAllFiles( node *[], int );
#endif

