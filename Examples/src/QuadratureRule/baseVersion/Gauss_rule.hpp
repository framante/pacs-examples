/*
 * Gauss_rule.hpp
 *
 *  Created on: May 3, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_
#define EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_

#include <cmath>

#include "StandardQuadratureRule.hpp"
namespace apsc::NumericalIntegration{
  /*!
    \file numerical_rule.hpp
    \brief Some quadrature rules.

    Some standard quadrature rules.

   */
  //! Gauss  Legendre rule 2pt
  class GaussLegendre2p final : public StandardQuadratureRule<2>
  {
  public:
    GaussLegendre2p():StandardQuadratureRule<2>
    {
      {1,1},{-1./std::sqrt(3.),1./std::sqrt(3)},4
    }{}
    std::unique_ptr<QuadratureRuleBase> clone() const override
    {
      return std::unique_ptr<QuadratureRuleBase>(new GaussLegendre2p(*this));
    }
  };

  //! Gauss  Legendre rule 3pt
  class GaussLegendre3p final : public StandardQuadratureRule<3>
   {
   public:
     GaussLegendre3p():StandardQuadratureRule<3>
     {
       {5./9.,8./9.,5./9.},{-std::sqrt(3./5.),0,std::sqrt(3./5.)},6
     }{}
     std::unique_ptr<QuadratureRuleBase> clone() const override
     {
       return std::unique_ptr<QuadratureRuleBase>(new GaussLegendre3p(*this));
     }
   };

  //! Gauss  Legendre rule 4pt
  class GaussLegendre4p final : public StandardQuadratureRule<4>
   {
   public:
     GaussLegendre4p():StandardQuadratureRule<4>
     {
       {(18.-std::sqrt(30.))/36.,(18.-std::sqrt(30.))/36.,
	   (18.+std::sqrt(30.))/36.,(18.-std::sqrt(30.))/36.},
       {-std::sqrt(3./7.+2./7.*std::sqrt(6./5.)),
	   -std::sqrt(3./7.-2./7.*std::sqrt(6./5.)),
	   std::sqrt(3./7.-2./7.*std::sqrt(6./5.)),
	   std::sqrt(3./7.+2./7.*std::sqrt(6./5.))}
     }{}
     std::unique_ptr<QuadratureRuleBase> clone() const override
     {
       return std::unique_ptr<QuadratureRuleBase>(new GaussLegendre4p(*this));
     }
   };

  //! Gauss  Legendre rule 5pt
  class GaussLegendre5p final : public StandardQuadratureRule<5>
   {
   public:
     GaussLegendre5p():StandardQuadratureRule<5>
     {
       {(322-13*std::sqrt(70))/900.,(322+13*std::sqrt(70))/900.,
	   128./225.,(322+13*std::sqrt(70))/900.,
	   (322-13*std::sqrt(70))/900.},
	 {-std::sqrt(5.+2.*std::sqrt(10./7.))/3.,
	     -std::sqrt(5.-2.*std::sqrt(10./7.))/3.,
	     0,std::sqrt(5.-2.*std::sqrt(10./7.))/3.,
	     std::sqrt(5.+2.*std::sqrt(10./7.))/3.}
     }{}
     std::unique_ptr<QuadratureRuleBase> clone() const override
     {
       return std::unique_ptr<QuadratureRuleBase>(new GaussLegendre5p(*this));
     }
   };

  //! Gauss  Lobatto rule 4pt
    class GaussLobatto4p final : public StandardQuadratureRule<4>
    {
    public:
      GaussLobatto4p():StandardQuadratureRule<4>
      {
        {1./6, 5./6., 5./6., 1./6.},
	{-1.,-1./std::sqrt(5.),1./std::sqrt(5),1.},
	6
      }{}
      std::unique_ptr<QuadratureRuleBase> clone() const override
      {
        return std::unique_ptr<QuadratureRuleBase>(new GaussLobatto4p(*this));
      }
    };

    //! Gauss  Legendre rule 3pt
    class GaussLobatto5p final : public StandardQuadratureRule<5>
     {
     public:
       GaussLobatto5p():StandardQuadratureRule<5>
       {
         {1.10, 49./90, 32/45., 29./90, 1.10},
	 {-1, -std::sqrt(3./7.),0,std::sqrt(3./7.),1},8
       }{}
       std::unique_ptr<QuadratureRuleBase> clone() const override
       {
         return std::unique_ptr<QuadratureRuleBase>(new GaussLobatto5p(*this));
       }
     };


}



#endif /* EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_ */
