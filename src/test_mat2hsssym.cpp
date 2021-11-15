#include<bits/stdc++.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stack>
#include<algorithm>
#include<assert.h>
#include"test_mat2hsssym.h"
#include "compr.h"
#include "QR.h"
#include "compr_new.h"

using namespace std;
//assuming the 16x16 matrix
void merge_arr(std::pair<vector<double>,int>*tempSt,int ns, double *tempT,int tRowWidth,int row){
   // vector<double> current=tempSt[0].first;
    
    for(int itr=0;itr<row;itr++){
        int filled_upto=0;
    for(int i=0;i<ns;i++){
       // vector<double> current=tempSt[i].first;
        for(int k = 0; k < tempSt[i].second;k++){
            tempT[filled_upto + itr*tRowWidth] = tempSt[i].first[k+itr*tempSt[i].second]; 
            filled_upto++;
           // tempT[filled_upto + itr*tRowWidth] = 1;; 
        }
    }
    filled_upto =0;
    }
}


tHSSMat* t_mat2hsssym(double* A, int aSize, BinTree* bt, int* m, int mSize, char* tol, double par){
    tHSSMat* ret          = new tHSSMat();
    int n                 = mSize; //number of leaf nodes
    int N                 = bt->GetNumNodes();
    int aRowWidth         = sqrt(aSize);
    int aColWidth         = aRowWidth;    
    //storing info about node starting and ending block 
    //Index range of each node 
    std::pair<int,int>* l = new std::pair<int,int>[N];

    for(int i = 0; i < N; i++)
		l[i] = std::make_pair(0,0);    

    l[0]                  = {0,m[0]-1};
    int it                = 0;
    int lt                = 0;

    for(int i = 0; i < N; i++)
    {
        std::vector<int> ch = bt->GetChildren(i+1);
        if(ch.size() == 0)
        {
            l[i] = {lt,lt+m[it]-1};
            lt   = l[i].second +1;
            it   = it+1;
        }
        else
        {
            l[i] = {l[ch[0]-1].first,l[ch[1]-1].second};

        }
    }
    //To store the diagonal blocks
    double** D = new double*[N];

    //create a list to hold the matrix dimensions of the diagonal matrices 
	std::pair<int, int>* dSizes = new std::pair<int, int>[N];
    for(int i = 0; i < N; i++)
		dSizes[i]=std::make_pair(0,0); 

    //to store generator U
    double** U = new double*[N];

    //Pair to hold the matrix dimension of generator U
    std::pair<int, int>* uSizes = new std::pair<int, int>[N];
    for(int i = 0; i < N; i++)
		uSizes[i]=std::make_pair(0,0); 
    //to store the generator R
    double** R = new double*[N];

    //Pair to hold the matrix dimension of generator R
    std::pair<int, int>* rSizes = new std::pair<int, int>[N];
    for(int i = 0; i < N; i++)
		rSizes[i]=std::make_pair(0,0);

    //to store the generator B     
    double** B = new double*[N];

    //Pair to hold the matrix dimension of generator B
    std::pair<int, int>* bSizes = new std::pair<int, int>[N];
    for(int i = 0;i < N; i++)
		bSizes[i]=std::make_pair(0,0); 

    std::stack<vector<double>>S;
    //creating temmporary array toa hold entire row-block except the diagonal block
    double** T = new double*[N];
   
   //creating temporary list to hold dimension of temporary block
    std::pair<int, int>* tSizes = new std::pair<int, int>[N];
    for(int i = 0; i < N; i++)
		tSizes[i]=std::make_pair(0,0); 

    //temp stack
    std::vector<int>St;
    int ns=0;
    //visiting every nodes in postorder    
    for(int i = 0; i < N-1; i++){
        std::vector<int> ch = bt->GetChildren(i+1);
        //if the current node is a leaf node
        if(ch.size() == 0)
        {   
            int dRowWidth = l[i].second-l[i].first+1;
            int dColWidth = dRowWidth;
            D[i]          = new double[dRowWidth*dColWidth];
            double *temp  = D[i];
            dSizes[i]     ={dColWidth,dRowWidth};

            //row and column start indices
            int rSI       = l[i].first;            
            int rEI       = l[i].second;
            int cSI       = rSI;
            int cEI       = rEI;

            //creating or copying diagonal blocks from original matrix
            for(int k = 0, j = rSI; j <= rEI; j++, k++)
                memcpy(temp+(k*dRowWidth),A+j*aRowWidth+cSI, sizeof(double)*dRowWidth);
            
            //dimension of row and column of temporary array 
            int tRowWidth = aColWidth-dColWidth;
            int tColWidth = dRowWidth;
            
            tSizes[i]     = {tColWidth,tRowWidth}; 
            //size of the temporary matrix           
            T[i]          = new double[tColWidth*tRowWidth];
            
            //temporary pointer to hold matrix data corresponding to index i.
            double* tempT = T[i];   
            memset(tempT,0,sizeof(*tempT)*tColWidth*tRowWidth);
            
            if (i == 0)
            {
                for(int k = 0, j = rSI; j<= rEI; j++, k++)
                    memcpy(tempT+(k*tRowWidth),A+j*aRowWidth+cEI+1, sizeof(double)*tRowWidth);

                std::pair<int,int>uSize(0,0);
                double* R               = NULL;
                std::pair<int,int>rSize = uSize;
                printf("LeafNode: Computing U%d\n",i+1);
                compr_new(tempT, tSizes[i], &(U[i]),uSize, &R, rSize, tol, par);
                delete [] tempT;
                T[i]      = R;
                tSizes[i] = rSize;
                uSizes[i] = uSize;
            }
            // for i greater the zero(except the first node)
            else
            {
                St.push_back(i-1);
                ns++;
                std::pair<vector<double>,int>tempSt[ns];
                int current_pos_col = 0;//used to keep track of the columns filled.
                for(int k = 0; k < ns; k++)
                {
                    rSI                = 0;
                    rEI                = tSizes[St[k]].first;
                    cSI                = tSizes[St[k]].second-(aRowWidth-l[i].first);
                    cEI                = tSizes[St[k]].second-(aRowWidth-l[i].second);
                    double *tempTT     = T[St[k]];
                    vector<double>tempVec;
                    double* temp_array = new double[(rEI)*(cEI-cSI+1)]; 
                    int index          = 0;

                    //copying elements into an temporary array
                    for(int itr = rSI; itr < rEI; itr++){
                        for(int j = cSI; j <= cEI; j++){
                            temp_array[index++]=tempTT[j+itr*(tSizes[St[k]].second)];
                            //tempVec.push_back(tempTT[j+i*(tSizes[St[k]].second)]);
                        }
                    }
                    
                    GetTransposeInPlace(temp_array,(rEI),(cEI-cSI+1));
                    for(int itr=0;itr<index;itr++){
                        tempVec.push_back(temp_array[itr]);
                    }
                    tempSt[k]={tempVec,rEI};
                /*
                    index = 0;
                    int tempRow = tSizes[i].first;
                    for(int i = 0; i < tempRow; i++){
                        for(int j = current_pos_col; j < current_pos_col+rEI; j++){
                            tempT[j+i*tRowWidth]=temp_array[index++];                        
                        }
                    }

                    current_pos_col = current_pos_col+rEI;
                    delete [] temp_array;
                    */
                }

            
                merge_arr(tempSt,ns,tempT,tRowWidth,tColWidth);
                for(int itr =0;itr<ns;itr++)
                    current_pos_col += tempSt[itr].second;     

                //copying remaining element to tempT array from A
                int cp = current_pos_col;
                rSI    = l[i].first;
                rEI    = l[i].second;
                cSI    = l[i].second+1;
                cEI    = aRowWidth;

                for(int itr=rSI, tp=0; itr<=rEI; itr++, tp++){
                    for(int j = cSI, k = cp; j < cEI; j++, k++){
                        tempT[k+tp*tRowWidth]=A[j+itr*aRowWidth];   
                    }
                }

                std::pair<int,int>uSize(0,0);
                double* R               = NULL;
                std::pair<int,int>rSize = uSize;
                printf("LeafNode: Computing U%d\n",i+1);
                compr_new(tempT, tSizes[i], &(U[i]),uSize, &R, rSize, tol, par);
                delete [] tempT;
                T[i]      = R;
                tSizes[i] = rSize;
                uSizes[i] = uSize;

            }   
        }
        //if the currrent node is non-leaf node
        else
        {
            St.pop_back();
            ns--;
            int left      = ch[0]-1;
            int right     = ch[1]-1;
             //sizes of left and right child
            int r1        = tSizes[left].first;
            int n1        = tSizes[left].second;
            int r2        = tSizes[right].first;
            int n2        = tSizes[right].second;
            
            int tColWidth = r1+r2;
            int tRowWidth = aRowWidth-tColWidth;
            T[i]          = new double[tRowWidth*tColWidth];
            tSizes[i]     = {tColWidth,tRowWidth};

            double* tempT = T[i];
            memset(tempT,0,sizeof(*tempT)*tColWidth*tRowWidth);
                
            double* tempL = T[left];
            double* tempR = T[right];
           
            //coloums to copy from the left child 
            int cSI_L_1   = n1-(aRowWidth-1-l[left].second);
            int cSI_L_2   = n1-(aRowWidth-1-l[i].second);
             
            //columns to copy from right child
            int cSI_R_1   = n1-(aRowWidth-1-l[left].second);
            int cSI_R_2   = n2-(aRowWidth-1-l[i].second);
            
            int limit     = 0;
            //copying from the left child
            if(cSI_L_1 >= 0)
            {
                for(int itr = 0; itr < r1; itr++){
                    limit=0;
                    for(int j = 0;j < cSI_L_1; j++){
                     tempT[limit + itr*tRowWidth]=tempL[j+itr*tSizes[left].second];
                     limit++;   
                    }
                    
                }
            }

            if(cSI_L_2 >= 0)
            {
                for(int  itr = 0; itr < r1; itr++){
                    for(int j = cSI_L_2,k=limit;j < tSizes[left].second; j++,k++){
                     tempT[k+itr*tRowWidth]=tempL[j+itr*tSizes[left].second];   
                    }
                }
            }

           //copying from right child
           limit = 0;
            if(cSI_R_1 >= 0)
            {
                for(int itr = 0,row=r1; itr < r2; itr++,row++){
                    limit = 0;
                    for(int j = 0; j < cSI_R_1; j++){
                     tempT[limit+row*tRowWidth]=tempR[j+itr*tSizes[right].second];
                     limit++;   
                    }
                }
            }

            if(cSI_R_2 >= 0)
            {
                for(int itr = 0,row=r1; itr<r1; itr++,row++){
                    for(int j = cSI_R_2,k=limit; j < tSizes[right].second; j++,k++){
                     tempT[k+row*tRowWidth]=tempR[j+itr*tSizes[right].second];   
                    }
                }
            }
            
            //computing B
           // int bColWidth = tSizes[right].first;
            int rSI        = 0;
            int rEI        = tSizes[right].first;
            int cSI        = n1-(aRowWidth-1-l[left].second);
            int cEI        = n2-(aRowWidth-1-l[i].second);
            int bRowWidth  = cEI-cSI;  
            int bColWidth  = rEI;
            bSizes[left]   = {bColWidth,bRowWidth};
            B[left]        = new double[bColWidth*bRowWidth];
            double* tempB  = B[left];
            double* tempTT = T[right];
            int index      = 0;

            printf("Computing B%d\n",left+1);
            for(int itr = rSI; itr < rEI; itr++){
                for(int j = cSI;j < cEI; j++){
                    tempB[index++] = tempTT[j+itr*tSizes[right].second];
                }
            }

            GetTransposeInPlace(tempB,bColWidth,bRowWidth);

            std::pair<int,int>uSize(0,0);
            double* R               = NULL;
            std::pair<int,int>rSize = uSize;

            printf("Computing U%d\n",i+1);            
            compr_new(tempT, tSizes[i], &(U[i]),uSize, &R, rSize, tol, par);

            delete [] tempT;
            T[i]                    = R;
            tSizes[i]               = rSize;
            uSizes[i]               = uSize;

            //deallocating space as we won't need T[left] and T[right]
            T[left]                 = NULL;
            T[right]                = NULL;
            delete [] tempL;
            delete [] tempR;
            tSizes[left]            = {0,0};
            tSizes[right]           = {0,0};
        }
    }

    //Remaining B
    int node            = N;
    std::vector<int> ch = bt->GetChildren(node);
    int left            = ch[0]-1;
    int right           = ch[1]-1;
     //sizes of left and right child
    int r1              = tSizes[left].first;
    int n1              = tSizes[left].second;
    int r2              = tSizes[right].first;
    int n2              = tSizes[right].second;
    int rSI             = 0;
    int rEI             = tSizes[right].first;
    int cSI             = n1-(aRowWidth-1-l[left].second);
    int cEI             = n2-(aRowWidth-1-l[node-1].second);
    int bRowWidth       = cEI-cSI;
    int bColWidth       = rEI;
    bSizes[left]        = {bColWidth,bRowWidth};
    B[left]             = new double[bColWidth*bRowWidth];
    double* tempB       = B[left];
    double* tempTT      = T[right];
    int index           = 0;

    printf("Computing B%d\n", left+1);

    for(int i = rSI; i < rEI; i++){
        for(int j = cSI;j < cEI; j++){
            tempB[index++] = tempTT[j+i*tSizes[right].second];
        }
    }

    GetTransposeInPlace(tempB,bColWidth,bRowWidth);

//Computing R
    for(int i = N-1; i >= 1; i--){
        std::vector<int> ch = bt->GetChildren(i);
        if(ch.size() != 0){
            int left         = ch[0]-1;
            int right        = ch[1]-1;
            int sz           = uSizes[left].second;

            //for left 
            int rColWidth    = sz;
            int rRowWidth    = uSizes[i-1].second;
            R[left]          = new double[rRowWidth*rColWidth];
            rSizes[left]     = {rColWidth,rRowWidth};

            //for right
            rColWidth        = uSizes[i-1].first-sz;            
            R[right]         = new double[rRowWidth*rColWidth];            
            rSizes[right]    = {rColWidth,rRowWidth};

            double *tempR_L  = R[left];
            double *tempR_R  = R[right];
            double *tempU    = U[i-1];

            //copying R[left]
            
            index            = 0;
            printf("computing R%d\n",left);
            for(int i = 0; i < sz; i++){
                for(int j = 0;j < rRowWidth; j++){
                    tempR_L[index] = tempU[j+i*rRowWidth];
                    index++;
                }
            }
           
           //copying R[right]
           rColWidth    += sz;
           index = 0;
           printf("computing R%d\n",right);
           for(int i = sz; i < rColWidth; i++){
               for(int j = 0; j<rRowWidth;j++){
                   tempR_R[index] = tempU[j+i*rRowWidth];
                   index++;
               }
           }

           U[i-1]      = NULL;
           uSizes[i-1] = {0,0};
           printf("Deleting U%d\n",i);
           delete [] tempU;    
        }

    }

    ret->D=D;ret->U=U;ret->R=R;ret->B=B;
	ret->dSizes=dSizes;ret->uSizes=uSizes;ret->rSizes=rSizes;ret->bSizes=bSizes; 
    return ret;
    
}