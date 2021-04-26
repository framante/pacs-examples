#include <iostream>
#include <cmath>
#include "GetPot"

#include "Adams_rule.hpp"
#include "Gauss_rule.hpp"
#include "numerical_integration.hpp"
#include "helperfunction.hpp"
#include "integrands.hpp"
#include "montecarlo.hpp"
#include "QuadratureRuleAdaptive.hpp"
using namespace apsc::NumericalIntegration;
using namespace Geometry;



int main(int argc, char** argv){
  using namespace std;
  FunPoint f=fsincos;

  double a,b;
  int nint;
  double targetError;
  readParameters(argc,argv, a, b,nint,targetError);
  cout<<"Integral from "<<a<<" to "<<b <<" on "<< nint <<" intervals"<<endl;
  double exactVal=exact(a,b);

  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  
  Quadrature s(Simpson(),mesh);
  Quadrature m(MidPoint(),mesh);
  Quadrature t(Trapezoidal(),mesh);
  Quadrature gL(GaussLegendre3p(),mesh);
  Quadrature gL4(GaussLegendre4p(),mesh);
  Quadrature gL5(GaussLegendre5p(),mesh);


  cout<<" Now the mesh has "<<mesh.numNodes()<<" nodes"<<endl;

  double approxs=s.apply(f);
  double approxm=m.apply(f);
  cout<<"MidPoint="<<approxm<<" Trapezoidal="<<t.apply(f)<<
      " Simpson="<<approxs<<" GaussLegendre3p="<<gL.apply(f)<<
    " GaussLegendre4p="<<gL4.apply(f)<<" GaussLegendre5p="<<gL5.apply(f)<<
    " Exact="<<exactVal<<endl;

  // Now with MonteCarlo

  MonteCarlo mcRule;
  // Desired error
  mcRule.setError(1.e-3);
  Quadrature mc(mcRule,mesh);
  
  auto approxmc = mc.apply(f);
  cout<<"MonteCarlo="<<approxmc<<" error="<<
    std::abs(exactVal-approxmc)<<" Estimated/error="<<
    dynamic_cast<MonteCarlo const &>(mc.myRule()).cumulatedError()/
    std::abs(exactVal-approxmc)<<endl;
  
  //Now the adaptive
  Quadrature sa(QuadratureRuleAdaptive<Simpson>(targetError,10000),mesh);
  double adaptiveResult=sa.apply(f);
  printout(adaptiveResult,exactVal,targetError,"SImpson Adaptive");

  //Now the adaptive
   Quadrature ga(QuadratureRuleAdaptive<GaussLobatto4p>(targetError,10000),mesh);
   adaptiveResult=ga.apply(f);
   printout(adaptiveResult,exactVal,targetError,"Gauss Lobatto Adaptive");

   //Now the adaptive
   Quadrature gL4adaptive(QuadratureRuleAdaptive<GaussLegendre4p>(targetError,10000),mesh);
   adaptiveResult=gL4adaptive.apply(f);
   printout(adaptiveResult,exactVal,targetError,"Gauss Legendre 4p Adaptive");

   //Now the adaptive
   Quadrature gL5adaptive(QuadratureRuleAdaptive<GaussLegendre5p>(targetError,10000),mesh);
   adaptiveResult=gL5adaptive.apply(f);
   printout(adaptiveResult,exactVal,targetError,"Gauss Legendre 5p Adaptive");

}
  
