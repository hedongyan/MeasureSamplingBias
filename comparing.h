#ifndef COMPARING_H
#define COMPARING_H 1
    #include "dataset.h"
    void compare_MAD(const DatacenterMoment &recover,const DatacenterMoment &real,double &loss_cpu,double &loss_mem,double &loss_disk);
#endif
