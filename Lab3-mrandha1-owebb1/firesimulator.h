/*
 * Swarthmore College, CS 87
 * Copyright (c) 2020 Swarthmore College Computer Science Department,
 * Swarthmore PA, Professor Tia Newhall
 */

// TODO: add top-level comment here: and comment methods and data

#pragma once

#include <animator.h>
#include <curand_kernel.h>
#include <curand.h>
#include <cuda.h>

/* NOTE: other constants are declared in firesimulator.cu (ones that are
 * only used by code in that file).  If a constant's value is needed by
 * both main.ccp and firesimulator.cu, it needs to be declated in this file.
 */
static const int N             = 512;   // dimension of square world

static const int NUMITERS      = 2000;  // default values:



void process_args(int ac, char *av[], int* iters, int* step, float* prob,
  int* lighting_strike, int* numb_lakes, int** lakes, char* filename);


class fireSimulatorKernel: public Animator {

  public:
    // TODO: implement constructor: should parse command line args
    fireSimulatorKernel(int w, int h, int arc, char *argv[]);
    // TODO: implement destructor
    ~fireSimulatorKernel();
    // TODO: implement update
    void update(ImageBuffer *img);
    // TODO: set the total_iters data member in constructor
    int totalIters() { return total_iters; }

    // TODO: you can add more public and private methods

  private:
    int total_iters;
    int num_iters;
    int steps;
    float prob;
    int lighting_strike[2];
    int* m_dev_grid;
    curandState* dev_random;
    int m_rows;
    int m_cols;
    int m_argc;
    int* burned_keeper;
    char** m_argv;
    float time;

    // TODO: add more data members here

};
