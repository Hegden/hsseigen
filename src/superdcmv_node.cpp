#include "superdcmv_node.h"
#include "BinTree.h"
#include <assert.h>
#include "eigenmatrix.h"
#include "superdcmv_cauchy.h"
extern "C"
{
    #include<lapacke.h>
    #include<lapack.h>
    #include<cblas.h>
}

void superdcmv_node(EIG_MAT *Q,std::pair<int, int>qSize, double *X,std::pair<int, int>xSize,BinTree *bt, int index, int ifTrans,double N){
/*
%%% Input:
%%% Q: hss sturctured cauchylike eigenmatrix of descendants of i
%%% X: vectors to be multiplied
%%% ifTrans = 0: not transpose
%%% ifTrans = 1: transpose
%%% N: size threshold to use fmm
%%% bt: hss tree

%%% Output
%%% ifTrans = 0: X = Qi * X 
%%% ifTrans = 1: X = Qi^T * X;
*/
    double alpha = 1;
    double beta  = 0;
    std::vector<int> ch = bt->GetChildren(index+1);
    if(ch.size()==0){
        if(ifTrans == 0){
            if(qSize.second != xSize.first){
                cout<<"Multiplication is not possible";
                assert(false);
            }

            double *tempQX = new double[qSize.first*xSize.second];
            cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,qSize.first,xSize.second,qSize.second,alpha,Q->Q0_leaf,qSize.second,X,xSize.second,beta,tempQX,xSize.second);
            delete [] X;
            X = tempQX;
            tempQX = NULL;
        }
        else{
            if(qSize.first != xSize.first){
                cout<<"Multiplication is not possible";
                assert(false);
            }

            double *tempQX = new double[qSize.second*xSize.second];
            cblas_dgemm(CblasRowMajor,CblasTrans,CblasNoTrans,qSize.second,xSize.second,qSize.first,alpha,Q->Q0_leaf,qSize.second,X,xSize.second,beta,tempQX,xSize.second);
            delete [] X;
            X = tempQX;
            tempQX = NULL;
        }
    }
    else{
        int r = qSize.second;
        if(ifTrans == 0){
            //for j = r:-1:1
            //    [X, nflops1] = superdcmv_cauchy(Qi{j}, X, t, N);
        }
        else{
            for(int j = 0; j <r; j++){
              //superdcmv_cauchy()  
            }



        }




    }



}