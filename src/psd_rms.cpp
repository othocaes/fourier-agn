/*
 * psd.cpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#include "inc/psd_rms.hpp"

psd_rms::psd_rms( lcurve inlc , vec fqL ) {

	// ----------- initial parameters ------------ //
	n		=	inlc.len;
	dt		=	inlc.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	mean	=	inlc.demean();
	mean2	=	mean*mean;
	int		i;
	for( i=0 ; i<n ; i++ ){
		lc[i]	=	inlc.lc[i];
		lce[i]	=	inlc.lce[i]*inlc.lce[i];
		t[i]	=	inlc.t[i];
	}
	// ------------------------------------------ //

	// ----------- parent initilizer ------------ //
	init( fqL , 1 );
	// ------------------------------------------ //
}

psd_rms::~psd_rms() {}

void psd_rms::_C( vec p ){
	int	i,j,k; double d;
	for( i=0 ; i<npar ; i++ ){ p[i] *= mean2; }
	for( i=0 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,i); }
		d += f1*p[0]*Cfq2[0](i,i) + f2*p[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,j); }
		d += f1*p[0]*Cfq2[0](i,j) + f2*p[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
	}
}

void psd_rms::_dC( vec p , int k ){
	int	i,j;
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = Cfq[k](i,j); dC(j,i) = dC(i,j);} }
}


// ********************************************** //
// ********************************************** //

psd10_rms::psd10_rms( lcurve inlc , vec fqL ) {

	// ----------- initial parameters ------------ //
	n		=	inlc.len;
	dt		=	inlc.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	mean		=	inlc.demean();
	mean2	=	mean*mean;
	int		i;
	for( i=0 ; i<n ; i++ ){
		lc[i]	=	inlc.lc[i];
		lce[i]	=	inlc.lce[i]*inlc.lce[i];
		t[i]	=	inlc.t[i];
	}
	// ------------------------------------------ //

	// ----------- parent initilizer ------------ //
	init( fqL , 1 );
	// ------------------------------------------ //
}

psd10_rms::~psd10_rms() {}

void psd10_rms::_C( vec p ){
	int	i,j,k; double d; for(i=0;i<npar;i++){p[i]=pow(10,p[i])*mean2;}
	for( i=0 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,i); }
		d += f1*p[0]*Cfq2[0](i,i) + f2*p[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,j); }
		d += f1*p[0]*Cfq2[0](i,j) + f2*p[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
	}
}

void psd10_rms::_dC( vec p , int k ){
	int	i,j; double d = log(10)*pow(10,p[k])*mean2;
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = d*Cfq[k](i,j); dC(j,i) = dC(i,j);} }
}

void psd10_rms::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3 ){dpar[i] = 3;} if( dpar[i]<-3 ){dpar[i] = -3;}
		pars[i] += dpar[i];
	}
}
