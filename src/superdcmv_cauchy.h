#ifndef SUPERDCMV_CAUCHY_H
#define SUPERDCMV_CAUCHY_H
#include <iostream>
#include <utility>
#include "eigenmatrix.h"
double *superdcmv_cauchy(nonleaf *Q,std::pair<int, int>qSize, double *X,std::pair<int, int>xSize,int ifTrans,double N = 1024);

#endif