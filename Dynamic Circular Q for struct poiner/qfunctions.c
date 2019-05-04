/**
 * Title: Dynamically Allocated Circular Queue for structure node pointer
 * File: qfunctions.c
 * Author: Joey Baek
 * Date: 2019 05 04
 * Explanation: 
 * This implementation is basically circular queue which contains dynamically allocated array that stores
 * pointers of a structure. This implementaion contains a few components, a structure, its pointer, pointers dynamic array.
 * 
 * 
 * 
 * **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Structure: struct _node
 * Explanation: 
 * This structure was made to implement Branch and Bound algorithm for instance.
 * In this dynamic circular queue for structure pointer, I am going to use the variable int benefit only
 * to check if the queue works fine.
 * **/
typedef struct _node{
    int benefit;
    int level;
    int weight;
    int total_weight;
    int bound; 
    //struct _node* left;
    //struct _node* right;
}node;

/**
 * Structure: struct _queue
 * Explanation:
 * This sturucture contains the array that stores the our target data, the pointer of structure node. 
 * It also has other information such as the size, capacity, head index, and tail index.
 * The head index is set for the Enqueue, and the tail index is set for the Dequeue.
 * 
*/
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

int main(){
    Q s = QcreateQ();   //initialize the queue
    int value = 0;  //example value to be saved in the target variable, benefit.
    const int N = 10;   //unit number of datas to enqueue and dequeue.
    char input; 
    while(1){
        int exitFlag = 0;
        printf("input u for push, o for pop, Stop n\n");
        fflush(stdin);

        while(1){   //loop until valid input
            scanf("%c",&input);
            if(input == 'n'){
                exitFlag = 1;
                break;
            }
            else if(input != 'u' && input != 'o'){
                printf("retry: ");
            }
            else if(input =='u'){   //push instruction input
                for(int i = 0 ; i < N; i ++){   
                    node* newNode = (node*)malloc(sizeof(node));    //create N number of new nodes dynamically
                    newNode -> benefit = value++;   //set its benefit variable.
                    pushQ (&s,  newNode);   //push each node pointer to the Q.
                }
                break;
            }
            else if(input =='o'){   //pop instuction input
                for(int i = 0 ; i < N; i ++){
                    popQ(&s);   //pop N number of nodes from the Q.
                }
                break;
            }
        }//input loop
        showQ(s);
        if(exitFlag == 1){  //valid input condition. break the loop.
            break;
        }
        printf("\n");
    }

    for(int i = 0 ; i < s.capa; i++){
        if(s.nodes[i] != NULL)
            free(s.nodes[i]);
    }
    free(s.nodes);
}

/**
 * Function: QcreatueQ
 * Explanation:
 * This function initializes the Q structure. 
 * 
*/
Q QcreateQ(){
    Q s;
    const int many  = 10;   //The Q array has default capacity 10
    s.nodes = (node**)malloc(sizeof(node*) * many); //since this array contains pointers, it should be declared as doulbe pointer
    s.size = 0;
    s.capa = 10;
    s.head = -1;    // when size is 0, the head and tail are set to -1
    s.tail = -1;
    return s;
}

/**
 * Function: popQ
 * Explanation:
 * pop the element with index tail and return the node pointer.
*/
node* popQ(Q* s){
    if(s->size <= 0) printf("empry!\n");

    else{   //when size > 0
        if( s->size <= s->capa / 4 && s-> capa > 10 ){  //when size is smaller then current capacity devied by 4, 
                                                        //it decreases the capacity to half size.
                                                        //Do not reduce the size below the default capacity 10.
            /**
             * Since this is circular queie, we have to consider variety cases. 
             * The head index may be higher than the 
             
             
             * ARRAY
             * case 1: When the head is bigger then the tail.
             * [T O O O O O O O O H 0 0 0 0 0 0 0]
             * [0 0 0 0 0 T O O O O H 0 0 0 0 0 0 0]
             * 
             * To reduce the size by half with quater size of capacity, we move all data to leftest.
             * [T O O O O O O H 0 0 0 ...]
             * Then, reduce the size from the rightest.
             * Adjust the tail and head index value.
             * 
            */
            if(s->tail < s->head){
                s->capa /= 2;
                memmove(&(s->nodes[0]),&(s->nodes[s->tail]),sizeof(node*)*s->size);
                s->nodes =(node**)realloc(s->nodes ,sizeof(node*) * s->capa );
                s->tail = 0; //Because tail is the leftest, the value is 0.
                s->head = s->size-1;    // The head is the rightest data, so size - 1.
            }

            /**
             * case 2: When the head is smaller then the tail. 
             * [O O O O O H 0 0 0 ... 0 0 0 T O O O O ...]
             *                     ^       (T           capa) : capa - T = how many datas to move
             *                     ^         
             *                  capa/2 - (capa -tail) -> 새 데이터가 들어갈 자리는 중간 위치에서 들어갈 데이터의 수 만큼.
             *
             * To reduce the size by half, we remove the empty in the middle of) the array, [0 0 0 ... 0 0 0]
             * 
            */

            else if(s-> tail > s->head){
                for(int i = 0 ; i < (s->capa - s->tail) +1 ; i ++){ //how many datas to move
                    s->nodes[s->capa/2 - (s->capa - s->tail) +i ] = s->nodes[s->tail + i];    //move the right side datas to the at some point in the middle area.
                             //new locations.                                //old locations
                }
                s->capa /= 2;
                s->nodes =(node**)realloc( s->nodes ,sizeof(node*) * s->capa ); 
                s->tail = s->capa - (s->capa - s->tail);     //head는 그대로 자리를 지킨다. tail은 땡겨진다. 중간자리에서 이동한 데이터의 수 만큼.
            }
        }

        node* temp = s->nodes[s->tail];
        if(s-> head == s-> tail)
            s-> head = s-> tail = -1;   //Both head and tail index are same, which menas it pops all the datas. So the q is empty.
        else s->tail = (s->tail+1) % (s->capa); // if not empty, the tail index incereases by one after pop.
                                                // Mod should be by capa, not size, because of the head < tail case.
                                                //[O O O O O H 0 0 0 ... 0 0 0 T O O O O ...] -> while tail increases, it goes beyond the capadicy index. 
                                                //The tail index should be 0 when it reaches the capacity. 
        
        s->size--;
        return temp;
    }
    return NULL;
}

int isEmptyQ(Q s){
    return s.head == -1;
}

void pushQ(Q* s, node* newNode){
    if(s->head == -1){  //While the Q is empty, the index should be turned to 0. 
        s->head = s->tail = 0;
        s->nodes[s->head]=newNode;
    }
    else {
        s->head =  (s->head+1) % s->capa;   
        s->nodes[s->head]=newNode;
    }
    s->size ++;  

    if(s->size >= s->capa){ //When the size reaches to capacity, it doubles the size. 
        s->nodes =(node**)realloc(s->nodes,sizeof(node*) * s->capa*2 );
        /**
             * case 1: When the head is smaller then the tail. 
             * [O O O O O H 0 0 0 ... 0 0 0 T O O O O ...]
             *                               Tail       size
             *                               size - tail -> how many datas should be moved?
             * To increase the size by double, we inserts more space in the middle points, maintaing the head < tail satus. 
             * For this, we move Tail side data to the rightest AFTER doubling the size. 
             * 두 배 한 다음에 size - tail 의 수 만큼 tail에서부터 capa*2 - 옮길 수 만큼의 자리로 옮긴다. 
             * 
             * //그러나 push는 꽉 찼을 때 double 된다. 이 경우, head + 1 = tail이다. 
             * size - 1 - head = size - tail.
             * 
            */
        if(s-> head < s-> tail){
            //moving the datas to the rightest
            memmove(&(s->nodes[s->capa*2 - (s->size -1 - s->head)]),&(s->nodes[s->tail]), sizeof(node*) * (s->size - s-> head -1));
            //adjusting the tail index. we do not have to change the head index. 
            s->tail = s->capa*2 - (s->size -1 - s->head);
        }
         /**ARRAY
             * case 2: When the head is bigger then the tail.
             * [T O O O O O O O O H 0 0 0 0 0 0 0]
             * [0 0 0 0 0 T O O O O H 0 0 0 0 0 0 0]
             * In this case, increasing the array size does not require any further process. It works well.
        */
        else{
            ;
        }
        s->capa *= 2;
    }   
}

int QSize(Q s){
    return s.size;
}

void showQ(Q s){
    if(s.size >0){
        for(int i = s.tail ; i < s.tail + s.size; i++){
            if(s.tail > s. head)
                printf("%d ",s.nodes[i%s.capa]->benefit);
            if(s.tail < s. head)
                printf("%d ",s.nodes[i]->benefit);
            if (s.tail == s.head && s.tail != -1)
                printf("%d ", s.nodes[i]->benefit);
        }
    }
}
