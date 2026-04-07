#ifndef AG_H
#define AG_H
typedef struct Individual;
typedef struct Solution;

void allocate_individual(Individual* ind, int gene_size);
typedef void (*InitFunc)(Individual*, int);
typedef float (*FitnessFunction)(float x, float y);
Individual** allocate_individual_matrix(int rows, int cols, int gene_size);
#endif