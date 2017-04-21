#include <iostream>

#include <flow.h>

using namespace std;

int main()
{
/**********************book p143 example4-8 ************/

/**********************basic parameters ****************/
    int n=4;            //number of nodes
    int l=4;            //number of branches between nodes, excluding the branches between nodes and ground
    int m=2;            //number of PQ nodes
    float  g[n*n];     //the matrix of inductors
    float  b[n*n];     //the matrix of imaginary part
    float  g1[l] = {0.58824,0,0.45386, 0.48077};   //the g of the i-th branch
    float  b1[l] = {-2.35294, -3.6667, -1.89107, -2.40385};    //the b of the i-th branch
    float  c1[l] = {0.01528, 0, 0.01920, 0.01413};     //the pi equal circuit parameters to the ground, symmetrical
    float  c[n*l] = {0,-0.36667,0,0, 0,0,0,0, 0,0.33333,0,0, 0,0,0,0 };    //the equal circuits of the branches, not symmetry, like transformer
    float  co[n] = {0,0,0,0};      //the pure resistor to the ground
    int k=1;
    int  s1[l] = {1,1,1,2};        //the start node of the i-th branch
    int  e1[l] = {2,3,4,4};        //the end node of the i-th branch

/********************calculating the Y matrix ********/

    ybus(n,l,m,g,b,g1,b1,c1,c,co, 0,s1,e1);

/********************* basic parameters ****************/

    float p[n-1] = {-0.3, -0.55, 0.5};      //the P of (n-1) nodes

    float q[m] = {-0.18, -0.13 };            //the Q of m PQ nodes
    float p0[n-1] = {1,1,1};            //the error of p of (n-1) nodes
    float q0[m] = {1,1};                //the error of q of m PQ nodes
    float v[n-1] ={0,0,1.1};              //the v of (n-1-m) PV nodes, but only the v[i](m<i<n) is meaningful
    float v0[n-1] ={0,0,1};               //the error of (n-1-m) PV nodes, delta v^2
    float e[n]={1,1,1,1.05};            //the e of (e+jf)
    float f[n]={0,0,0,0};            //the f of (e+jf)
    float dd[1]={1};                         //the error of the max element of deltaP & deltaQ
                                            //the criteria of the loop end
    float jm[4*(n-1)*(n-1)];            //the Jacob matrix, 2(n-1)*2(n-1)
    int n0 = 2*(n-1);                   //the rows of Jacob matrix
    float a[2*(n-1)*(1+2*(n-1))];       //the extended martrix of the linear equations,rows=2*(n-1),cols=2*n-1;
    int iter=0;                         //the number of iteration

    dpqc(p,q,p0,q0,v,v0,m,n,e,f,0,g,b,dd);

    while( dd[0] > 0.001)
    {
        std::cout<<std::endl;
        std::cout<<"iteration "<<(++iter)<< std::endl;
        jmcc(m,n,n0,e,f,g,b,jm,0);              //calculate the jacob matrix
        extendMatrix(a,n,m,jm,p0,q0,v0,0);           //extend the jacob matrix
        sevc(a,2*(n-1),0,2*(n-1)+1);                //solve the equations
        for(int ite=1;ite!=n;ite++)
        {
            e[f1(ite)] = e[f1(ite)]-a[f2(2*(ite)-1,2*n-1,2*n-1)];       //somehow, the order of the unknown is changed
            f[f1(ite)] = f[f1(ite)]-a[f2(2*(ite),2*n-1,2*n-1)];
        }

        cout<<"e";
        printMatrix(e,1,3);
        cout<<"f";
        printMatrix(f,1,3);

        dpqc(p,q,p0,q0,v,v0,m,n,e,f,0,g,b,dd);      //calculate the error of deltap,deltaq,deltaV

    }

/***********************calculating the power flow**********/
    float p1[l];
    float q1[l];
    float p2[l];
    float q2[l];
    float p3[l];
    float q3[l];
    int k1=0;           //the input is e+jf
    float angle[n];

    plsc(n, l, m,g,b,e,f,e1,s1,g1,b1,c1,c,co,p1,q1,p2,q2,p3,q3,p,q,v,angle,k1);

    cout<<endl<<"The program is finished!";
/**************************end of the code***************/
return 0;


}
