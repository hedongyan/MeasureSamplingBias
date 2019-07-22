#include "recovering.h"
///use neighbor uniform assumption
void recover_NU(const DatacenterMoment& sample,DatacenterMoment& recovered){
    recovered.nearest_interpolation_from(sample);
}

///use markov assumption based on logical clock
DatacenterMoment history_ml;
void recover_ML(const DatacenterMoment& orign,DatacenterMoment& recovered){
    history_ml.concat_update(orign);
    recovered.nearest_interpolation_from(history_ml);
}
