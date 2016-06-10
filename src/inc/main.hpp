/*
 * main.hpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_


#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>

#include "def.hpp"
#include "psd.hpp"
#include "lag.hpp"
#include "psdlag.hpp"
#include "mcmc.hpp"


void usage();
void do_work( char* );
void readLC( vector<vector<lcurve> >&, string , int , int , int , bool );


double mcmc_lag10( vec& x ,void*ptr ){
	int i,np = x.length(); for(i=0;i<np/2;i++){if(x[i]>7){x[i]=7;}if(x[i]<-7){x[i]=-7;}}
	Mod<lag10> *mod = (Mod<lag10>*) ptr; double logl=mod->loglikelihood(x);
	return logl;
}

double mcmc_psdlag10( vec& x ,void*ptr ){
	int i,np = x.length(); for(i=0;i<np/2;i++){if(x[i]>7){x[i]=7;}if(x[i]<-7){x[i]=-7;}}
	Mod<psdlag10> *mod = (Mod<psdlag10>*) ptr; double logl=mod->loglikelihood(x);
	return logl;
}

#endif /* MAIN_HPP_ */
