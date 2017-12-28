//
// Created by kosuke on 11/21/17.
//
//Polynomial Fit
#include<iostream>
#include<iomanip>
#include<cmath>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include "polyfit.h"

using namespace std;
using namespace cv;

void makePonts(Mat img, vector<int>& x, vector<int>& y) {
    //Loop over each pixel and create a point
    for (int xP = 0; xP < img.cols; xP++)
        for (int yP = 0; yP < img.rows; yP++){
            int temp = int(img.at<uchar>(yP, xP));
            if(temp == 255){
                x.push_back(xP);
                y.push_back(yP);
            }
        }
}

// code is mainly from here "http://www.bragitoff.com/2015/09/c-program-for-polynomial-fit-least-squares/"
void makePolyFit(vector<int> x, vector<int> y, vector<double>& a) {
    int i,j,k;
    int N = x.size();
    int n = 2;
    double X[2*n+1];                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    for (i=0;i<2*n+1;i++)
    {
        X[i]=0;
        for (j=0;j<N;j++)
            X[i]=X[i]+pow(x[j],i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    }
    double B[n+1][n+2];//B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
//    double a[n+1];
    for (i=0;i<=n;i++)
        for (j=0;j<=n;j++)
            B[i][j]=X[i+j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
    double Y[n+1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    for (i=0;i<n+1;i++)
    {
        Y[i]=0;
        for (j=0;j<N;j++)
            Y[i]=Y[i]+pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    }
    for (i=0;i<=n;i++){
        B[i][n+1]=Y[i];
    }//load the values of Y as the last column of B(Normal Matrix but augmented)
    n=n+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations

    for (i=0;i<n;i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
        for (k=i+1;k<n;k++)
            if (B[i][i]<B[k][i])
                for (j=0;j<=n;j++)
                {
                    double temp=B[i][j];
                    B[i][j]=B[k][j];
                    B[k][j]=temp;
                }

    for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
        for (k=i+1;k<n;k++)
        {
            double t=B[k][i]/B[i][i];
            for (j=0;j<=n;j++)
                B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
    for (i=n-1;i>=0;i--)                //back-substitution
    {                        //x is an array whose values correspond to the values of x,y,z..
        a[i]=B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
        for (j=0;j<n;j++)
            if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                a[i]=a[i]-B[i][j]*a[j];
        a[i]=a[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    }
//    cout<<"\nThe fitted Polynomial is given by:\ny=";
//    for (i=0;i<n;i++) {
//        cout << " + (" << a[i] << ")" << "x^" << i;
//    }
    cout<<"\n";
}