#include "AG.h"

#include<stdio.h>
#include<stdlib.h>

typedef struct{
    unsigned char* chromosome;
    float value;
    int id;
} Individual;
typedef struct{
    Individual x,y;
    float fit;
}Solution;

void allocate_individual(Individual* ind, int gene_size){
    ind->chromosome = (unsigned char*) malloc(gene_size * sizeof(unsigned char));
    ind->value = 0.0f;
    ind->id = -1;
}
void allocate_solution(Solution* sol, int gene_size){
    allocate_individual(&sol->x, gene_size);
    allocate_individual(&sol->y, gene_size);
    sol->fit = 0.0f;
}

Individual** allocate_individual_matrix(int rows, int cols, int gene_size){
    Individual** matrix = (Individual**) malloc(rows * sizeof(Individual*)); //aloca a linha de ind
    if (matrix == NULL) return NULL;

    for (int i = 0; i < rows; i++){
        matrix[i] = (Individual*) malloc(cols * sizeof(Individual)); //aloca a coluna de ind
        if (matrix[i] == NULL) return NULL;

        for (int j = 0; j < cols; j++){ //aloca cada ind
            Individual ind = matrix[i][j];
            allocate_individual(&ind, gene_size);
            ind.id = i;
        }
        
    }
    return matrix;
}
Solution* allocate_solution_array(int pop_size, int gene_size){
    Solution* pop = (Solution*) malloc(pop_size * sizeof(Solution));
    if (pop == NULL) return NULL;

    for (int i = 0; i < pop_size; i++) {
        allocate_solution(&pop[i], gene_size);
    }
    return pop;
}

void initialize_population(Individual **matrix, int n_rows, int n_cols, int gene_size, 
    InitFunc initializer){
    for (int i = 0; i < n_rows; i++){
        for (int j = 0; j < n_cols; j++){
            initializer(&matrix[i][j], gene_size);
        }   
    }   
}
void evaluate_population(Solution* pop, int pop_size, FitnessFunction fit_func){
    for (int i = 0; i < pop_size; i++){
        pop[i].fit = fit_func(pop[i].x.value, pop[i].y.value);
    }
}


Solution** tournament_selection(Solution* population, int pop_size, float selection_pressure){
    Solution** selected_parents = (Solution**) malloc(pop_size * sizeof(Solution*));
    if (selected_parents == NULL) return NULL;

    for (int i = 0; i < pop_size; i++){
        int parent_x = rand() % pop_size;
        int parent_y = rand() % pop_size;

        while (parent_x == parent_y) parent_y = rand() % pop_size;

        float roll = (float)rand() / (float)RAND_MAX;
        int winner_idx;

        if (population[parent_x].fit < population[parent_y].fit)
            winner_idx = (roll < selection_pressure) ? parent_x : parent_y;
        else
            winner_idx = (roll < selection_pressure) ? parent_y : parent_x;

        selected_parents[i] = &population[winner_idx]; 
    }
    return selected_parents;
}