#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "qheader.h"

int totalW(node* oneNode, int totalW, int totalB, item* items, int capa, int N, int index);
int bound(node* oneNode, item* items, int capa, int N, int index);

int main(){
    

    int N = 10;
    int capa = N * 40;
    srand(time(NULL));

    item* items = (item*)malloc(sizeof(item)*N);
    
    for(int i = 0 ; i < N; i ++){
        items[i].benefit = rand() % 300 + 1;
        items[i].weight = rand() % 100 + 1;
        items[i].bw = (float)items[i].benefit / (float)items[i].weight;
    }
    // DPRINT(printf("item input done!\n"))
    quickSort(items,0,N-1);
    printf("  i  : benefit : weight  :  bw\n");
    for(int i = 0 ; i < N; i ++){
        printf("  %-3d   %-4d   %7d %15f\n",i, items[i].benefit, items[i].weight,items[i].bw) ;   
    }
    printf("\n---------------------------\n\n\n");

    Q s = QcreateQ(N);
    node* newNode = (node*)malloc(sizeof(node));
    newNode -> benefit = 0;//items[0].benefit;
    newNode -> weight = 0;//items[0].weight;
    newNode -> level = 0;
    // DPRINT(printf("first node set\n"))
    
    int max_benefit = 0;
    // int i = -1;
    int totalWeight = 0;
    int totalBenefit = 0;
    
    // while(totalWeight + items[++i].weight < capa){
    //     totalWeight += items[i].weight;
    //     totalBenefit += items[i].benefit; 
    //     if(i >=N ){
    //         printf("may take all them out!\n");
    //         break;
    //     }
    // }
    // DPRINT(printf("total weight: %d, totalBenefit: %d\n",totalWeight, totalBenefit));


    // DPRINT(printf("calculating the first total node\n"))

    
    // totalWeight = totalW(newNode,totalWeight,totalBenefit,items,capa,N,i);
    newNode -> bound = bound(newNode,items,capa,N,newNode->level);

    // DP2(printf("benefit: %d\nweight: %d\nbound: %d\n-----\n",newNode->benefit, newNode->weight,newNode->bound))

    // DPRINT(printf("calculate the first bound\n"))
    // DPRINT(printf("newNode -> bound : %d\n",newNode->bound))
    if(newNode -> bound >= max_benefit){
        // DP("if bound > max benefit")
        pushQ(&s,newNode);
        // DP("pushQ")
        while(1){
            // DP("\n***IN HWILE***\n")


            node* parentNode = popQ(&s);
            // DPRINT(printf("%d\n",parentNode->benefit))
            // DP2(printf("POP!\n"))
            // DPRINT(printf("parent node benefit : %d, parent level: %d\n",parentNode -> benefit, parentNode -> level));
           
            if( parentNode -> level >= N ) break;

            node* Lnode = createLNode(parentNode,items);
            // node* Lnode = (node*)malloc(sizeof(node));
            // if(Lnode == NULL)
            //     printf("malloc error in Lnode!\n");
            // DP("LEFT NODE")
            // Lnode -> level = parentNode->level+1;
            // DP("LEVEL")
            // DPRINT(printf("parent level: %d left level: %d\n",parentNode -> level, Lnode -> level));
            // Lnode -> benefit = parentNode -> benefit + items[Lnode->level].benefit;
            // DP("LEFT BENFIT")
            // Lnode -> weight = parentNode -> weight + items[Lnode->level].weight;
            
            // totalWeight = totalW(Lnode, totalWeight, totalBenefit, items, capa, N, i);
            // totalBenefit = totalB(Lnode, totalWeight, totalBenefit, items, capa, N, i);
            Lnode -> bound = bound(Lnode,items,capa,N,Lnode->level-1);
            // DP2(printf("Lnode\nbenefit: %d\nweight: %d\nbound: %d\n,level: %d\n-----\n",Lnode->benefit, Lnode->weight,Lnode->bound, Lnode->level))

            // DPRINT(printf("left bound calculated: %d\n", Lnode -> bound ))

            node* Rnode = createRNode(parentNode);
            // node* Rnode = (node*)malloc(sizeof(node));
            // if(Rnode == NULL)
            //     printf("malloc error in Rnode!\n");
            // Rnode -> level = parentNode->level+1;
            // Rnode -> benefit = parentNode -> benefit;
            // Rnode -> weight = parentNode -> weight;

            // totalWeight = totalW(Rnode, totalWeight, totalBenefit, items, capa, N, i);
            // totalBenefit = totalB(Rnode, totalWeight, totalBenefit, items, capa, N, i);
            Rnode -> bound = bound(Rnode,items,capa,N,Rnode->level-1);
            // DP2(printf("Rnode\nbenefit: %d\nweight: %d\nbound: %d\n,level: %d\n-----\n",Rnode->benefit, Rnode->weight,Rnode->bound, Rnode->level))


            // DPRINT(printf("Right node bound: %d\n",Rnode->bound))

            if(Lnode -> weight <= capa && Lnode -> bound >= max_benefit){
                pushQ(&s, Lnode);
                if(max_benefit < Lnode -> benefit) max_benefit = Lnode -> benefit;
                // DP("PUSH left DONE")
            }
            else{
                free(Lnode);
                // printf("one node removed\n");
            }


             if(Rnode -> weight <= capa && Rnode -> bound >= max_benefit){
                pushQ(&s, Rnode);
                if(max_benefit < Lnode -> benefit) max_benefit = Lnode -> benefit;
                // DP("PUSH right DONE")

            }
            else{
                free(Rnode);
                // printf("one node removed\n");
            }

            if(isEmptyQ(s)){
                // printf("??????????\n");
                // printf("parentnode -> benefit: %d\n", parentNode -> benefit);
                break;
                // printf("%d : parentnode bound\n",parentNode -> bound);
            }
            
        }

    }
    // printf("done? Q capacity: %d\n",s.capa);
}


int partition(item items[], int lo, int hi){
    int left = lo;
    item key = items[lo];

    int right = hi;
    while( left < right){
        while(items[left].benefit>= key.benefit && left < hi){
            left++;
        }
        // printf("left ++ work?\n");
        while(items[right].benefit < key.benefit && right > lo){
            right --;
        }
        if(left < right){
            item temp = items[left];
            items[left] = items[right];
            items[right] = temp;
        }
    }
    item temp = items[right];
    items[right] = key;
    items[lo] = temp;

    return right;
}

void quickSort(item items[], int lo, int hi){

    if(lo < hi){
        int pivot = partition(items, lo, hi);
        // printf("pivot : %d\n",pivot);
        quickSort(items,pivot + 1, hi);
        quickSort(items, lo,pivot-1);
    }

}



int totalW(node* oneNode, int totalW, int totalB, item* items, int capa, int N, int index){
    totalW = oneNode -> weight;
    while(totalW + items[index+1].weight < capa){
        totalW += items[index++].weight;
        if(index >=N ){
            printf("may take all them out!\n");
            break;
        }

    }
    return totalW;
}

int bound(node* oneNode, item* items, int capa, int N, int index){
    int totalB = oneNode -> benefit;
    int totalW = oneNode -> weight;
    index = index - 1;
    if(index + 1 >=N ){
        // DP2(printf("index overflow! get overvlow\n"));
        // DP2(printf("boud = totalB\n"));
        // DP2(printf("boud = %d\n",totalB));
        return totalB; 
    }
    else{
        while(totalW + items[index+1].weight < capa){
            totalW += items[++index].weight;
            totalB += items[index].benefit; 
            if(index + 1 >=N ){
                // DP2(printf("index overflow! make overflow\n"));
                break;
            }
        }
    
        if( index + 1>= N){
            // DP2(printf("boud = totalB\n"));
            // DP2(printf("boud = %d\n",totalB));
            return totalB; 
    
        }
    } 
    
    // DP2(printf("bound = totalB + (capa - totalW)*(items[%d].benefit) /(items[%d].weight)\n",index+1,index+1));
    // DP2(printf("%d + (%d - %d )  *  ( %d ) /   (%d)\n",totalB, capa, totalW,(items[index+1].benefit),(items[index+1].weight )));

    return totalB + (capa - totalW)*(float)(items[index+1].benefit) /(float)(items[index+1].weight); 
}

node* createRNode(node* parentNode){
    node* oneNode = (node*)malloc(sizeof(node));
    if(oneNode == NULL)
        printf("malloc error in Rnode!\n");
    oneNode -> level = parentNode->level+1;
    oneNode -> benefit = parentNode -> benefit;
    oneNode -> weight = parentNode -> weight;
    // DP2(printf("parentnode benefit: %d, node benefit: %d\n",parentNode->benefit, oneNode->benefit));

    return oneNode;
}


node* createLNode(node* parentNode, item* items){
    node* oneNode = (node*)malloc(sizeof(node));
    if(oneNode == NULL)
        printf("malloc error in Rnode!\n");
    oneNode -> level = parentNode->level+1;
    oneNode -> benefit = parentNode -> benefit + items[parentNode->level].benefit;
    oneNode -> weight = parentNode -> weight+ items[parentNode->level].weight;
    
    // DP2(printf("parentnode benefit: %d, node benefit: %d\n",parentNode->benefit, oneNode->benefit));
    return oneNode;
}


// int limitWeightIdx(int totalWeight, int totalBenefit, int i, item* items, int capa){
//     while(totalWeight + items[i+1].weight < capa){
//         totalWeight += items[i++].weight;
//         totalBenefit += items[i].benefit; 
     
//         if(i >=N ){
//             printf("may take all them out!\n");
//         }
//     }
// }

