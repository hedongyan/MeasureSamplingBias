#ifndef DATASET_H
#define DATASET_H 1

#include "parameters.h"

#define DEBUG_DATASET 1
#ifdef DEBUG_DATASET
    #include <iostream>
    #include <stdlib.h>
    #include <vector>
    #include <fstream>
    using namespace std;
#endif // DEBUG_DATASET

///Container Runtime data
struct Container{
    int time;int id;
    double cpu,mem,disk;
    bool isnull()const{
        return id==-1;
    }
    void copy_data(const Container& t){
        cpu=t.cpu;mem=t.mem;disk=t.disk;
    }
};

///Datacenter runtime data at a certain clock
class DatacenterMoment{
public:
    int containers_n;
    Container containers[MAX_CONTAINER];

    ///container id is exactly its index+1 in containers
    DatacenterMoment(){
        int i;
        for(i=0;i<MAX_CONTAINER;i++){
            containers[i].id=containers[i].time=-1;
            containers[i].cpu=containers[i].mem=containers[i].disk=-9999;
        }
        containers_n=0;
    }

    void concat_update(const DatacenterMoment& t){
        int i;
        for(i=0;i<MAX_CONTAINER;i++){
            if(!t.containers[i].isnull()){
                if(containers[i].isnull()){
                    containers_n++;
                }
                containers[i].id=i+1;
                containers[i].time=t.containers[i].time;
                containers[i].copy_data(t.containers[i]);
            }
        }
    }

    ///for input DatacenterMoment t, we did nearest_interpolation and save result to this object.
    void nearest_interpolation_from(const DatacenterMoment& t){
        int i,j;
        for(i=0;i<MAX_CONTAINER;i++){
            j=i;
            if(t.containers[i].isnull()){
                j=FindNearestNeighborIndex(t,i);
            }
            containers[i].id=i+1;
            containers[i].time=t.containers[j].time;
            containers[i].copy_data(t.containers[j]);
        }
    }

    ///find the nearest neighbor of index i whose id is not -1
    int FindNearestNeighborIndex(const DatacenterMoment&t,int i){///i is index,and its id is i+1
        int step,left,right;
        for(step=1;left>=0||right<MAX_CONTAINER;step++){
            left=i-step;
            right=i+step;
            if(left>=0&&!t.containers[left].isnull()){
                return left;
            }
            if(right<MAX_CONTAINER&&!t.containers[right].isnull()){
                return right;
            }
        }
        return 1;
    }

    ///clear data of datacenter in this moment
    void Clear(){
        int i;
        for(i=0;i<MAX_CONTAINER;i++){
            containers[i].id=containers[i].time=-1;
            containers[i].cpu=containers[i].mem=containers[i].disk=-9999;
        }
        containers_n=0;
    }

    ///show first n containers
    void ShowFirstNContainers(int n){
        int i;
        for(i=0;i<n;i++){
            cout<<containers[i].id<<" "<<containers[i].cpu<<" "<<containers[i].mem<<" "<<containers[i].disk<<" "<<endl;
        }
    }

    ~DatacenterMoment(){
    }
};

class Dataset{
public:
    int clock_n;
    DatacenterMoment* dc_moments;
    Dataset(){
        ///inilized containers' id to -1
        dc_moments = new DatacenterMoment[NUM_MOMENT];
        int i,j;
        for(i=0;i<NUM_MOMENT;i++){
            for(j=0;j<MAX_CONTAINER;j++){
                dc_moments[i].containers[j].id=-1;
            }
        }
    }

    ///read the dataset file,
    ///which includes data of 144 clocks, no more than 10000 containers in each clock, id is from 1 to 10000.
    void ReadFile(char filename[]){
        ///read containers' data
        ifstream in(filename,ios::in);
        while(!in.eof()){
            int time,id;
            double cpu,mem,disk;
            ///read timestamp and container id
            in>>time>>id;
            int t=(time-START_TIME)/INTERVAL;///index of dc_moments
            ///remove five data whose one indicator is null, we had found them in mysql
            if((time==47400&&id==490)||(time==48000&&id==5965)||(time==48000&&id==8377)||(time==61800&&id==5342)||(time==74700&&id==3403)){
                    in>>cpu>>mem;continue;
            }
            ///read utilization information
            in>>cpu>>mem>>disk;
            dc_moments[t].containers[id].time=time;
            dc_moments[t].containers[id].id=id;
            dc_moments[t].containers[id].cpu=cpu;
            dc_moments[t].containers[id].mem=mem;
            dc_moments[t].containers[id].disk=disk;
        }
        in.close();
    }
    ~Dataset(){
        delete dc_moments;
    }
};

#endif // DATASET_H
