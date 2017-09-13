//===================================================================
//
//  Copyright 2017 Xavier Claeys
//
//  bemtool is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  bemtool is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//  (see ../LICENSE.txt)
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with bemtool.  If not, see <http://www.gnu.org/licenses/>
//
//====================================================================
#ifndef EIGEN_WRAP_H
#define EIGEN_WRAP_H

#include <Eigen/Dense>
#include "../calculus/calculus.hpp"
#include "misc.hpp"

namespace bemtool {

  /*===========================
    ||   Interface avec eigen  ||
    ===========================*/
  
  template <class EigenMatType,class ValType>
  class EigenMat{
    
  private:
    EigenMatType          mat;
    std::vector<size_t>   ipvt;
    
  public:
    
    typedef EigenMat<EigenMatType,ValType>          m_t;
    typedef ValType                                 v_t;
    typedef std::vector<v_t>                        VectorType;
    typedef Eigen::Matrix<v_t,Eigen::Dynamic,1>     EigenVectorType;
    
    EigenMat(){};
    EigenMat(const int& n, const int& m): mat(n,m), ipvt(n){};
    
    v_t& operator()(const int& i, const int& j){
      return mat(i,j);}
    
    const v_t& operator()(const int& i, const int& j) const{
      return mat(i,j);}
    
    template <class r_t, class c_t> submat<m_t,r_t,c_t>
    operator()(const r_t& I0,const c_t& J0 ){
      return submat<m_t,r_t,c_t>(*this,I0,J0);}
    
    template <class r_t, class c_t> submat<const m_t,r_t,c_t>
    operator()(const r_t& I0,const c_t& J0 ) const{
      return submat<const m_t,r_t,c_t>(*this,I0,J0);}
    
    template <class c_t> submat<m_t,int,c_t>
    operator()(const int& I0,const c_t& J0 ){
      return submat<m_t,int,c_t>(*this,I0,J0);}
    
    template <class c_t> submat<const m_t,int,c_t>
    operator()(const int& I0,const c_t& J0 ) const {
      return submat<m_t,int,c_t>(*this,I0,J0);}
    
    template <class r_t> submat<m_t,r_t,int>
    operator()(const r_t& I0,const int& J0 ) {
      return submat<m_t,r_t,int>(*this,I0,J0);}
    
    template <class r_t> submat<const m_t,r_t,int>
    operator()(const r_t& I0,const int& J0 ) const {
      return submat<m_t,r_t,int>(*this,I0,J0);}
    
    void operator=(const m_t& m){
      for(int j=0; j<m.mat.rows(); j++){
	for(int k=0; k<m.mat.cols(); k++){
	  mat(j,k) = m(j,k);
	}
      }
    }
    
    friend m_t operator+(const m_t& m1, const m_t& m2){
    m_t m;
    for(int j=0; j<m.mat.rows(); j++){
      for(int k=0; k<m.mat.cols(); k++){
	m(j,k) = m1(j,k) + m2(j,k);
      }
    }
    return m;
    }
    
    friend m_t operator-(const m_t& m1, const m_t& m2){
      m_t m;
      for(int j=0; j<m.mat.rows(); j++){
	for(int k=0; k<m.mat.cols(); k++){
	  m(j,k) = m1(j,k) - m2(j,k);
	}
      }
      return m;
    }
    
    friend int NbRows(const m_t& m){
      return m.mat.rows();}
    
    friend int NbCols(const m_t& m){
      return m.mat.cols();}
    
    friend std::ostream& operator<<(std::ostream& os, m_t& m){
      for(int j=0; j<NbRows(m); j++){
	for(int k=0; k<NbCols(m); k++){
	  os << m(j,k) << "\t\t";}
	os << std::endl;
      }
      return os;
    }
    
    friend void Clear(m_t& m){
      for(int j=0; j<NbRows(m); j++){
	for(int k=0; k<NbCols(m); k++){
	  m(j,k)= 0.;
	}
      }
    }
    
    /*=======================
      Produit matrice-vecteur
      =======================*/
    
    friend void mv_prod(VectorType& b0, const m_t& A, const VectorType& x0){
      assert( b0.size()==NbRows(A) && x0.size()==NbCols(A) );
      EigenVectorType x(NbCols(A));
      EigenVectorType b(NbRows(A));
      for(int j=0; j<NbCols(A); j++){x[j]=x0[j];} b = A.mat*x;
      for(int j=0; j<NbRows(A); j++){b0[j]=b[j];}
    }

    friend void add_mv_prod(VectorType& b0, const m_t& A, const VectorType& x0){
      assert( b0.size()==NbRows(A) && x0.size()==NbCols(A) );
      EigenVectorType x(NbCols(A));
      EigenVectorType b(NbRows(A));
      for(int j=0; j<NbCols(A); j++){x[j]=x0[j];} b = A.mat*x;
      for(int j=0; j<NbRows(A); j++){b0[j]+=b[j];}
    }
  
    
    /*========
      Solveurs
      ========*/
    
    friend void lu_solve(m_t& A, const VectorType& b0, VectorType& x0){
      assert( b0.size()==NbRows(A) && x0.size()==NbCols(A) );
      EigenVectorType b(NbRows(A));
      for(int j=0; j<NbRows(A); j++){b[j]=b0[j];}
      EigenVectorType x = A.mat.lu().solve(b);
      for(int j=0; j<NbCols(A); j++){x0[j]=x[j];}
    }
    
  };
  
  
  /*===========================
    ||   Definitions de type   ||
    ===========================*/
  
  typedef Cplx Field;
  static const int Dynamic = Eigen::Dynamic;
  typedef Eigen::Matrix<Field, Dynamic, Dynamic>  DenseMatrix;
  typedef EigenMat< DenseMatrix ,Field >          EigenDense;
  
  
}
#endif
