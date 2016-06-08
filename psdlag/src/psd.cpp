/*
 * psd.cpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#include "inc/psd.hpp"

psd::psd( lcurve inlc , vec fqL ) {

	// ----------- initial parameters ------------ //
	n		=	inlc.len;
	dt		=	inlc.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	inlc.demean();
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

psd::~psd() {}

void psd::_C( vec p ){
	int	i,j,k; double d;
	for( i=0 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,i); }
		d += f1*p[0]*Cfq2[0](i,i) + f2*p[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,j); }
		d += f1*p[0]*Cfq2[0](i,j) + f2*p[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
	}
}

void psd::_dC( vec p , int k ){
	int	i,j;
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = Cfq[k](i,j); dC(j,i) = dC(i,j);} }
}


// ********************************************** //
// ********************************************** //

psd10::psd10( lcurve inlc , vec fqL ) {

	// ----------- initial parameters ------------ //
	n		=	inlc.len;
	dt		=	inlc.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	inlc.demean();
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

psd10::~psd10() {}

void psd10::_C( vec p ){
	int	i,j,k; double d; for(i=0;i<npar;i++){p[i]=pow(10,p[i]);}
	for( i=0 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,i); }
		d += f1*p[0]*Cfq2[0](i,i) + f2*p[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[k]*Cfq[k](i,j); }
		d += f1*p[0]*Cfq2[0](i,j) + f2*p[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
	}
}

void psd10::_dC( vec p , int k ){
	int	i,j; double d = log(10)*pow(10,p[k]);
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = d*Cfq[k](i,j); dC(j,i) = dC(i,j);} }
}

void psd10::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3 ){dpar[i] = 3;} if( dpar[i]<-3 ){dpar[i] = -3;}
		pars[i] += dpar[i];
	}
}
