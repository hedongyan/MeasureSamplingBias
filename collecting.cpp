#include "collecting.h"

void collectbyid(const Dataset& dataset,int time_index,int ids[],int ids_n,DatacenterMoment& dc_moment){
    dc_moment.Clear();
    dc_moment.containers_n=ids_n;
    int i;
    ///fill up dc_moment from dataset by ids
    for(i=0;i<ids_n;i++){
        int index=ids[i]-1;
        dc_moment.containers[index]=dataset.dc_moments[time_index].containers[index];
    }
}
