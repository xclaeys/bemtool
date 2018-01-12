#ifndef BEMTOOL_MISC_HTOOLWRAP_HPP
#define BEMTOOL_MISC_HTOOLWRAP_HPP

#include <htool/htool.hpp>
#include <bemtool/fem/dof.hpp>
#include <bemtool/operator/operator.hpp>

namespace bemtool{

template <typename KernelType, typename Discretization>
class BIO_Generator : public htool::IMatrix<Cplx>{
  Dof<Discretization> dof;
  BIOp<KernelType> V;

public:
  BIO_Generator(const Dof<Discretization>& dof0, const double& kappa):IMatrix(NbDof(dof0),NbDof(dof0)), dof(dof0), V(MeshOf(dof),MeshOf(dof),kappa){}

  Cplx get_coef(const int& i, const int& j) const {
      BIOp<KernelType> V_local=V;
    return V_local(dof.ToElt(i),dof.ToElt(j));
  }
  htool::SubMatrix<Cplx> get_submatrix(const std::vector<int>& I, const std::vector<int>& J) const{
    BIOp<KernelType> V_local = V;
    htool::SubMatrix<Cplx> mat(I,J);
    for (int i=0; i<mat.nb_rows(); i++)
        for (int j=0; j<mat.nb_cols(); j++)
            mat(i,j) = V_local(dof.ToElt(I[i]),dof.ToElt(J[j]));
    return mat;
}
};

template <typename KernelType, typename Discretization>
class SubBIO_Generator : public htool::IMatrix<Cplx>{
  Dof<Discretization> dof;
  SubBIOp<BIOp<KernelType>> subV;

public:
  SubBIO_Generator(const Dof<Discretization>& dof0, const double& kappa):IMatrix(NbDof(dof0),NbDof(dof0)), dof(dof0),subV(dof,dof,kappa) {}

  Cplx get_coef(const int& i, const int& j) const {
      std::vector<int> J(1,i);
      std::vector<int> K(1,j);
    htool::SubMatrix<Cplx> mat(J,K);
    SubBIOp<BIOp<KernelType>> subV_local = subV;
    subV_local.compute_block(J,K,mat);
    return mat(0,0);
  }

  htool::SubMatrix<Cplx> get_submatrix(const std::vector<int>& J, const std::vector<int>& K) const{
      // std::cout << "COUCOU"<<std::endl;
    htool::SubMatrix<Cplx> mat(J,K);
    SubBIOp<BIOp<KernelType>> subV_local = subV;
    subV_local.compute_block(J,K,mat);
    return mat;
  }

};


template <typename KernelType, typename Discretization>
class POT_Generator : public htool::IMatrix<Cplx>{
  Potential<KernelType>& V;
  Dof<Discretization>& dof;
  Geometry& geometry;

public:
  POT_Generator(Potential<KernelType>& V0, Dof<Discretization>& dof0, Geometry& geometry0):IMatrix(NbNode(geometry0),NbDof(dof0)), V(V0), dof(dof0), geometry(geometry0) {}

  Cplx get_coef(const int& i, const int& j) const {
      Potential<KernelType>& V_local=V;
    return V_local(geometry[i],dof.ToElt(j));
}
  htool::SubMatrix<Cplx> get_submatrix(const std::vector<int>& I, const std::vector<int>& J) const{
    Potential<KernelType> V_local = V;
    htool::SubMatrix<Cplx> mat(I,J);
    for (int i=0; i<mat.nb_rows(); i++)
        for (int j=0; j<mat.nb_cols(); j++)
            mat(i,j) = V_local(geometry[I[i]],dof.ToElt(J[j]));
    return mat;
}

};

}// namespace

#endif
