#ifndef BSXFUN_H
#define BSXFUN_H
#include <string>
#include <iostream>
#include <assert.h>
#include <cstring>
#include <vector>
using namespace std;
void bsxfun(char method, double **X, std::pair<int,int>xSize, double *Y, std::pair<int,int>ySize);
void arrange_elements(double *Arr,std::pair<int,int>arrSize,double *Indices, std::pair<int,int>indSize);
//used for arrange  arrays
void arrange_elements2(double *X, std::pair<int,int>XSize,int *Tt, std::pair<int,int>tSize, bool forward=true);
void arrange_elements3(double *Arr,int arrSize,int *Indices, bool reverse=true);//TODO: inefficient version.
double vec_norm(std::vector<double> &v);
vector<double> diff_vec(std::vector<double> V);
#endif
