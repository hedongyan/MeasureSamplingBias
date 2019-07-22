/**
Paper Name: A Measurable Framework for Run-time Data Sampling in Large-scale Data center
Company: School of Computer Science, Beijing Institute of Technology
Author: Hedong Yan
Modified: 20190721
*/

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
using namespace std;

#include "dataset.h"

#include "sampling.h"
#include "collecting.h"
#include "recovering.h"
#include "comparing.h"

//#define DEBUG_MAIN_SAMPLING 1
//#define DEBUG_MAIN_COLLECTING 1
//#define DEBUG_MAIN_RECOVERING 1
#define DEBUG_MAIN_COMPARING 1

int main()
{
    ///----------------------------------read dataset----------------------------------
    Dataset alibaba2017;
    alibaba2017.clock_n=NUM_MOMENT;
    char filename[20]="new_contanier.txt";
    alibaba2017.ReadFile(filename);
    cout<<"Read dataset finished!"<<endl;

    ///---------------set sampling methods and recovering assumption used---------------
    const int sampling_method = R3S2_METHOD;
    const int recovering_assumption = ML_ASSUMPTION;

    ///evaluate biasness of sampling methods under certain recovering assumption accurately
    int i,j;
    double sum_cpu,sum_mem,sum_disk;
    sum_cpu=sum_mem=sum_disk=0;
    for(i=0;i<NUM_MOMENT;i++){
        ///----------------------------------sampling----------------------------------
        int ids[MAX_CONTAINER],ids_n;
        int population[MAX_CONTAINER_USED],population_n=MAX_CONTAINER_USED;
        for(j=0;j<MAX_CONTAINER_USED;j++){
            population[j]=j+1;///id is from 1 to MAX_CONTAINER+1
        }
        switch(sampling_method){
            case SRS_METHOD:
                srs(population,population_n,SAMPLING_RATE,ids,ids_n);
                break;
            case R3S2_METHOD:
                r3s2(population,population_n,SAMPLING_RATE,ids,ids_n);
                break;
        }

        #ifdef DEBUG_MAIN_SAMPLING
        for(j=0;j<ids_n;j++){
            cout<<ids[j]<<" ";
        }
        cout<<endl;system("pause");
        #endif

        ///----------------------------------collecting----------------------------------
        DatacenterMoment dc_moment;
        int time_n=i;
        collectbyid(alibaba2017,time_n,ids,ids_n,dc_moment);

        #ifdef DEBUG_MAIN_COLLECTING
        for(j=0;j<MAX_CONTAINER;j++){
            if(!dc_moment.containers[j].isnull()){
                cout<<dc_moment.containers[j].id<<" ";
            }
        }
        cout<<endl;system("pause");
        #endif

        ///----------------------------------recovering----------------------------------
        DatacenterMoment recovered;
        switch(recovering_assumption){
            case NU_ASSUMPTION:
                recover_NU(dc_moment,recovered);
                break;
            case ML_ASSUMPTION:
                recover_ML(dc_moment,recovered);
                break;
        }

        #ifdef DEBUG_MAIN_RECOVERING
        recovered.ShowFirstNContainers(100);
        system("pause");
        #endif

        ///----------------------------------comparing----------------------------------
        double loss_cpu,loss_mem,loss_disk;
        compare_MAD(recovered,alibaba2017.dc_moments[i],loss_cpu,loss_mem,loss_disk);
        sum_cpu+=loss_cpu;sum_mem+=loss_mem;sum_disk+=loss_disk;

        #ifdef DEBUG_MAIN_COMPARING
        cout<<"At clock "<<i*INTERVAL+START_TIME;
        cout<<" cpu ave util bias:"<<loss_cpu<<" mem ave util bias:"<<loss_mem<<" disk ave util bias:"<<loss_disk<<endl;
        #endif // DEBUG_MAIN_COMPARING
    }
    sum_cpu/=NUM_MOMENT;sum_mem/=NUM_MOMENT;sum_disk/=NUM_MOMENT;
    cout<<"For the dataset, ";
    cout<<"cpu util ave bias:"<<sum_cpu<<"  mem util ave bias:"<<sum_mem<<" disk util ave bias:"<<sum_disk<<endl;
    cout<<"cpu util relative error(%):"<<sum_cpu*100/9.52<<"  mem util relative error(%):"<<sum_mem*100/44.30<<" disk util relative error(%):"<<sum_disk*100/14.33<<" "<<(sum_cpu*100/9.52+sum_mem*100/44.30+sum_disk*100/14.33)/3<<endl;
    return 0;
}
