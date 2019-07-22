#ifndef RECOVERING_H
#define RECOVERING_H 1
    #include "dataset.h"
    ///neighbor uniform assumption
    void recover_NU(const DatacenterMoment& orign,DatacenterMoment& recovered);
    ///markov assumption based on logical clock
    void recover_ML(const DatacenterMoment& orign,DatacenterMoment& recovered);
#endif
