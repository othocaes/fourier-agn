/*
 * mcmc.hpp
 *
 *  Created on: May 14, 2013
 *      Author: abduz
 */

#ifndef MCMC_HPP_
#define MCMC_HPP_

#include <ap.h>
#include <alglibmisc.h>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace alglib;
using namespace std;

typedef real_1d_array vec;
typedef real_2d_array vec2;
typedef integer_1d_array ivec;
typedef integer_2d_array ivec2;

class mcmc {
	int npar; double(*loglikelihood)(vec&,void*); void*ptr;
	hqrndstate rnd;
public:
	int nrun,nburn,nwk,ncheck; double avalue;
	mcmc(int np,double (*f)(vec&,void*),void *p);
	void setseed(int s1,int s2 ){hqrndseed(s1,s2,rnd);}
	void run( vec& , vec& , const char*fname="mcmc.dat");
	virtual ~mcmc();
};

#endif /* MCMC_HPP_ */
