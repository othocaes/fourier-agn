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
#include "psd_rms.hpp"
#include "lag.hpp"
#include "lag_rms.hpp"
#include "psdlag.hpp"
#include "psdlag_rms.hpp"
#include "mcmc.hpp"


void usage();
void do_work( char* );
void readLC( vector<vector<lcurve> >&, string , int , int , int , bool );


double mcmc_lag10( vec& x ,void*ptr ){
	int i,hnp,np = x.length();hnp=np/2; double phi,twopi=2*M_PI;
	for(i=0;i<hnp;i++){
		if(x[i]>7){x[i]=7;}if(x[i]<-7){x[i]=-7;}
		phi = x[i+hnp]+M_PI;
		while(phi>twopi){phi -= twopi;}
		while(phi<0){phi += twopi;}
		x[i+hnp] = phi-M_PI;
	}
	Mod<lag10_rms> *mod = (Mod<lag10_rms>*) ptr; double logl=mod->loglikelihood(x);
	return logl;
}

double mcmc_psdlag10( vec& x ,void*ptr ){
	int i,hnp,np = x.length();hnp=np/2; double phi,twopi=2*M_PI;
	for(i=0;i<hnp;i++){
		if(x[i]>7){x[i]=7;}if(x[i]<-7){x[i]=-7;}
		phi = x[i+hnp]+M_PI;
		while(phi>twopi){phi -= twopi;}
		while(phi<0){phi += twopi;}
		x[i+hnp] = phi-M_PI;
	}
	Mod<psdlag10_rms> *mod = (Mod<psdlag10_rms>*) ptr; double logl=mod->loglikelihood(x);
	return logl;
}

#endif /* MAIN_HPP_ */
