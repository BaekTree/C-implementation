#ifndef QHEADER_H
#define QHEADER_H

// #define DEBUG 
#ifdef DEBUG
#define DPRINT(func) func;
#define DP(string) printf(string); printf("\n");
#else
#define DPRINT(func) ;
#define DP(string) ;
#endif

// #define DEBUG2
#ifdef DEBUG2
#define DP2(func) func;
#else
#define DP2(func) ;
#endif


typedef struct _item{
    int benefit;
    int weight;
    float bw;
}item;

typedef struct _node{
    int benefit;
    int level;
    int weight;
    int total_weight;
    int bound; 
    struct _node* left;
    struct _node* right;
}node;

typedef struct _queue{
    node** nodes;
    int size;
    int capa;
    int head;
    int tail;

}Q;


Q QcreateQ();
void showQ(Q s);
node* popQ(Q* s);
int isEmptyQ(Q s);
void pushQ(Q* s, node* newNode);
int QSize(Q s);
int partition(item items[], int lo, int hi);
void quickSort(item items[], int lo, int hi);
int totalW(node* oneNode, int totalW, int totalB, item* items, int capa, int N, int index);
int bound(node* oneNode, item* items, int capa, int N, int index);
node* createRNode(node* parentNode);
node* createLNode(node* parentNode, item* items);




#endif