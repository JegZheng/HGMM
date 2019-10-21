#ifndef __HGMM_H__
#define __HGMM_H__

#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<dirent.h>
#include<map>
#include<set>
#include<vector>
#include<math.h>
#include<time.h>
#include<pthread.h>

using namespace std;

const double pi = 3.1416926;

struct e_parameters{
	e_parameters(int tid,int pro,int k,int c,int un,int dim,vector<int>inu,double ***v,double *alp,double **ph,double**gu,double*gsigma,double**theta_r,double***ps,double***Ph):thread_id(tid),pron(pro),K(k),C(c),usern(un),dimension(dim),inum(inu),V(v),alpha(alp),phi(ph),u(gu),sigma(gsigma),r(theta_r),psi(ps),Phi(Ph){}
	e_parameters(){}
	int thread_id;
	int pron;
	int K;
	int C;
	int usern;
	int dimension;
	vector<int> inum;
	double ***V;
	double *alpha;
	double **phi;
	double **u;
	double *sigma;
	double **r;
	double ***psi;
	double ***Phi;
};

struct m_parameters{
	m_parameters(int tid,int pro,int k,int c,int un,int dim,vector<int>inu,double ***v,double *alp,double **ph,double**gu,double*gsigma,double**theta_r,double***ps,double***Ph,double *g,double *h,double c_):thread_id(tid),pron(pro),K(k),C(c),usern(un),dimension(dim),inum(inu),V(v),alpha(alp),phi(ph),u(gu),sigma(gsigma),r(theta_r),psi(ps),Phi(Ph),G(g),H(h),C_(c_){}
	m_parameters(){}
	int thread_id;
	int pron;
	int K;
	int C;
	int usern;
	int dimension;
	vector<int> inum;
	double ***V;
	double *alpha;
	double **phi;
	double **u;
	double *sigma;
	double **r;
	double ***psi;
	double ***Phi;
	double *G;
	double *H;
	double C_;
};

struct c_parameters{
	c_parameters(int tid,int pro,int c,int un,int dim,vector<int>inu,double ***v,double**gu,double*gsigma,double***ps,double *likeli):thread_id(tid),pron(pro),C(c),usern(un),dimension(dim),inum(inu),V(v),u(gu),sigma(gsigma),psi(ps),likelihoods(likeli){}
	c_parameters(){}
	int thread_id;
	int pron;
	int C;
	int usern;
	int dimension;
	vector<int> inum;
	double ***V;
	double **u;
	double *sigma;
	double ***psi;
	double *likelihoods;
};

double digamma ( double x )
{
  double euler_mascheroni = 0.57721566490153286060;
  double r;
  double value;
  double x2;
//
//  Check the input.
//
  if ( x <= 0.0 )
  {
    value = 0.0;
    cout<<"input error of digamma function"<<endl;
    return value;
  }
//
//  Initialize.
//
  x2 = x;
  value = 0.0;
//
//  Use approximation for small argument.
//
  if ( x2 <= 0.00001 )
  {
    value = - euler_mascheroni - 1.0 / x2;
    return value;
  }
//
//  Reduce to DIGAMA(X + N).
//
  while ( x2 < 8.5 )
  {
    value = value - 1.0 / x2;
    x2 = x2 + 1.0;
  }
//
//  Use Stirling's (actually de Moivre's) expansion.
//
  r = 1.0 / x2;
  value = value + log ( x2 ) - 0.5 * r;
  r = r * r;
  value = value 
    - r * ( 1.0 / 12.0
    - r * ( 1.0 / 120.0 
    - r *   1.0 / 252.0 ) );

  return value;
}

double de_digamma(double x){

	double sum = 0;
	for(int i=0;i<100;i++)
		sum += 1.0/((x+i)*(x+i));

	return sum;
}

class hgmm{
	public:
		hgmm(string p,int k,int c,int it,int pron);
		~hgmm();

		string user_path;
		int K,C,Itern,Pron;

		int usern,dimension,uin;
		vector<int>inum;
		double ***V;

		double *alpha,**phi,**u,*sigma;
		double **r,***psi,***Phi;

		double likelihood;

		void load_data();
		void initial_params();
		void e_step();
		void m_step();
		void convergence();
		void run_em();
		void output();
		void release_params();
};

#endif
