#include "sampling.h"
#include "parameters.h"
#include <cstdlib>
#include <ctime>

#define DEBUG 1
#ifdef DEBUG
    #include <stdio.h>
    #include <iostream>
    using namespace std;
#endif

///compute complement of history_ids in population_ids
void complement_U_A(int population_ids[],int population_n,int r3s2_history_ids[],int r3s2_history_n,int lefts_ids[],int& left_n);

///simple random sampling
void srs(int population_ids[], int population_n, double sampling_rate, int sample_ids[],int & ids_n){
    if(sampling_rate>1)sampling_rate=1;
    ///random seed
    srand((unsigned)time(NULL)+rand());
    ///num of sample
    int num_sample=(int)(sampling_rate*population_n);
    ///simple random sampling
    int i;
    int flag[MAX_CONTAINER]={};
    for(i=0;i<num_sample;i++){
        int index = (int)(1.0*population_n*rand()/RAND_MAX);
        ///check is this id has been sampled
        if(flag[index]==0){
            sample_ids[i]=population_ids[index];
            flag[index]=1;
        }else{///if yes, then sample again
            i--;
        }
    }
    ids_n=num_sample;
}

///repeat-reducing simple random sampling
int r3s2_history_ids[MAX_CONTAINER]={};int r3s2_history_n=0;
void merge12(int samples1[],int n1,int sample2[],int n2,int sample[],int& n){
    n=0;
    int i;
    for(i=0;i<n1;i++){
        sample[n++]=samples1[i];
    }
    for(i=0;i<n2;i++){
        sample[n++]=sample2[i];
    }
}
void r3s2(int population_ids[], int population_n, double sampling_rate, int sample_ids[],int &ids_n){
    ///check is almost all ids had been sampled once at least
    if(r3s2_history_n<population_n*r3s2_q){///if no
        ///compute lefts
        int lefts_ids[MAX_CONTAINER]={},lefts_n=0;
        complement_U_A(population_ids,population_n,r3s2_history_ids,r3s2_history_n,lefts_ids,lefts_n);
        double left_sampling_rate = sampling_rate*population_n/lefts_n;
        ///sampling
        int sample_ids1[MAX_CONTAINER],sample_ids2[MAX_CONTAINER],ids_n1,ids_n2;
        ids_n1=ids_n2=0;
        ///if lefts is not enough,sampling from history_ids
        if(left_sampling_rate>1){
            srs(r3s2_history_ids,r3s2_history_n,(sampling_rate*population_n-lefts_n)/r3s2_history_n,sample_ids1,ids_n1);
        }
        ///sampling from left
        srs(lefts_ids,lefts_n,left_sampling_rate,sample_ids2,ids_n2);
        ///merge ids1 and ids2
        merge12(sample_ids1,ids_n1,sample_ids2,ids_n2,sample_ids,ids_n);
        ///update history ids
        int i;
        for(i=0;i<ids_n;i++){
            r3s2_history_ids[r3s2_history_n++]=sample_ids[i];
        }
    }else{///if yes
        r3s2_history_n=0;
        srs(population_ids,population_n,sampling_rate,sample_ids,ids_n);
    }
}



///compute complement of history_ids in population_ids
void complement_U_A(int population_ids[], int population_n,int r3s2_history_ids[],int r3s2_history_n,int lefts_ids[],int& lefts_n){
    int i,j;
    lefts_n=0;
    for(i=0;i<population_n;i++){
        int flag=0;
        for(j=0;j<r3s2_history_n;j++){
            if(population_ids[i]==r3s2_history_ids[j]){
                flag=1;break;
            }
        }
        if(flag==0){
            lefts_ids[lefts_n++]=population_ids[i];
        }
    }
}
