#ifndef PARAMETER_H
#define PARAMETER_H 1
///Data center scale
    //#define MAX_CONTAINER_USED 1000
    //#define MAX_CONTAINER_USED 2000
    //#define MAX_CONTAINER_USED 3000
    //#define MAX_CONTAINER_USED 4000
    //#define MAX_CONTAINER_USED 5000
    //#define MAX_CONTAINER_USED 6000
    //#define MAX_CONTAINER_USED 7000
    //#define MAX_CONTAINER_USED 8000
    //#define MAX_CONTAINER_USED 9000
    #define MAX_CONTAINER_USED 10000
///Sampling methods
    #define SRS_METHOD 1
    #define R3S2_METHOD 2
///Recovering assumption
    #define NU_ASSUMPTION 1
    #define ML_ASSUMPTION 2
///Sampling rate
    #define SAMPLING_RATE 0.1
    //#define SAMPLING_RATE 0.2
    //#define SAMPLING_RATE 0.4
///r3s2 parameter
    #define r3s2_q 0.99
///Alibaba Trace 2017
    #define NUM_MOMENT 144
    #define START_TIME 39600
    #define INTERVAL 300
    #define MAX_CONTAINER 10000
#endif
