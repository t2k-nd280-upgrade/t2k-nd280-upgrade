#ifndef LinearFit_hxx_seen
#define LinearFit_hxx_seen

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include "TVector3.h"

#include "Math/GSLSimAnMinimizer.h"
#include "Math/Functor.h"


void line(double t, const double *p, double &x, double &y, double &z) {
    x = p[0]*t + p[1];
    y = p[2]*t + p[3];
    z = p[4]*t + p[5];
};


double distance(double x,double y,double z, const double* p ){
    //t=0 point on the line
    TVector3 x_i(x,y,z);
    TVector3 x0(p[1],p[3],p[5]);
    TVector3 x1(p[0]+p[1],p[2]+p[3],p[4]+p[5]);
    TVector3 u=(x0-x1).Unit();;
    double b=((x_i-x0).Cross(u)).Mag();
    return b;
};

double dir_constrain(const double* p){
    //direction constrain
    return (p[0]*p[0]+p[2]*p[2]+p[4]*p[4]-1)/(p[0]*p[0]+p[2]*p[2]+p[4]*p[4]);
};

double s_i(double x,double y,double z, const double* p){
    //distance along the line
    TVector3 x_i(x,y,z);
    TVector3 x0(p[1],p[3],p[5]);
    TVector3 x1(p[0]+p[1],p[2]+p[3],p[4]+p[5]);
    TVector3 u=(x0-x1).Unit();
    double s = (x_i-x0).Dot(u);
    return s;
};

struct Lagrangian{
    std::vector<TVector3>* fPoints;
    double fLambda;
    double fBetta;
    
    Lagrangian(std::vector<TVector3>* points, double lambda, double betta) : fPoints(points),fLambda(lambda),fBetta(betta) {};
    
    
    double operator() (const double* p){
        assert((int)fPoints->size()!=0);
        int nPoints=(int)fPoints->size();
        double totDist=0;
        double totS=0;
        for(int i=0;i<nPoints;++i){
            totDist+=pow(distance((*fPoints)[i].X(),(*fPoints)[i].Y(),(*fPoints)[i].Z(),p),2);
            totS+=s_i((*fPoints)[i].X(),(*fPoints)[i].Y(),(*fPoints)[i].Z(),p);
        }
        totS=totS*totS;
        
        return totDist+fLambda*dir_constrain(p)+fBetta*totS;
        
    };
    
};


std::vector<double> LinearFit(std::vector<TVector3> points, const double* p0){
    
    double variable[6] = {p0[0],p0[1],p0[2],p0[3],p0[4],p0[5]};
    for(int i=0;i<6;++i){
       // std::cout<<p0[i]<<std::endl;
    }
    Lagrangian L(&points,0.01,0.01);
    
#ifdef __CINT__
    ROOT::Math::Functor f(&L,6,"Lagrangian");
#else
    ROOT::Math::Functor f(L,6);
#endif
    
    ROOT::Math::GSLSimAnMinimizer min;
        //1000000
    min.SetMaxFunctionCalls(1000);
       //100000
    min.SetMaxIterations(1000);
    min.SetTolerance(0.1);
    double step[6] = {0.1,0.1,0.1,0.1,0.1,0.1};
    
    min.SetFunction(f);
    
    min.SetVariable(0,"dx",variable[0], step[0]);
    min.SetVariable(1,"x0",variable[1], step[1]);
    min.SetVariable(2,"dy",variable[2], step[2]);
    min.SetVariable(3,"y0",variable[3], step[3]);
    min.SetVariable(4,"dz",variable[4], step[4]);
    min.SetVariable(5,"z0",variable[5], step[5]);
    
    min.Minimize();
    
    const double* pm = min.X();

    std::vector<double> output;
    for(int i=0;i<6;++i){
        output.push_back(pm[i]);
    }
    
    return output;
    
};

#endif




