#include "comparing.h"
#include "parameters.h"
#include <cmath>
///compare MAD of runtime data of recovered data(rt) and real data(state),output their difference in three indicators
void compare_MAD(const DatacenterMoment &recover,const DatacenterMoment &real,double &loss_cpu,double &loss_mem,double &loss_disk){
    int i,num=0;
    loss_cpu=loss_mem=loss_disk=0;
    for(i=0;i<MAX_CONTAINER;i++){
        if(real.containers[i].id!=-1&&recover.containers[i].id!=-1){
            num++;
            loss_cpu+=fabs(real.containers[i].cpu-recover.containers[i].cpu);
            loss_mem+=fabs(real.containers[i].mem-recover.containers[i].mem);
            loss_disk+=fabs(real.containers[i].disk-recover.containers[i].disk);
        }
    }
    loss_cpu/=num;loss_mem/=num;loss_disk/=num;
}
