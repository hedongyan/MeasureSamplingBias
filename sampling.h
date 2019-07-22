#ifndef SAMPLING_H
#define SAMPLING_H 1
    ///different sampling methods
    void srs(int population_ids[], int population_n, double sampling_rate, int sample_ids[],int &ids_n);
    void r3s2(int population_ids[], int population_n, double sampling_rate, int sample_ids[],int &ids_n);
#endif
