/*
 * lag.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#include "inc/lag_rms.hpp"

lag_rms::lag_rms( lcurve lc1 , lcurve lc2 , vec fqL , vec pars ) {

	// ----------- initial parameters ------------ //
	n1		=	lc1.len;
	n		=	n1 + lc2.len;
	dt		=	lc1.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	mean1	=	lc1.demean(); mean2	=	lc2.demean();
	mean1sq	=	mean1*mean1; mean2sq = mean2*mean2; mean12 = mean1*mean2;
	int		i;
	for( i=0 ; i<n1 ; i++ ){
		lc[i]	=	lc1.lc[i];
		lce[i]	=	lc1.lce[i]*lc1.lce[i];
		t[i]	=	lc1.t[i];
	}
	for( i=n1 ; i<n ; i++ ){
		lc[i]	=	lc2.lc[i-n1];
		lce[i]	=	lc2.lce[i-n1]*lc2.lce[i-n1];
		t[i]	=	lc2.t[i-n1];
	}
	// ------------------------------------------ //

	// ----------- parent initilizer ------------ //
	init( fqL , 2 );
	// ------------------------------------------ //

	// --------- constants and indices ---------- //
	p1.setlength(nfq); p2.setlength(nfq); icx.setlength(nfq); iphi.setlength(nfq);
	for( i=0 ; i<nfq ; i++ ){
		p1[i]	=	pars[i] * mean1sq;
		p2[i]	=	pars[i+nfq] * mean2sq;
		icx[i]	=	i;
		iphi[i]	=	i+nfq;
	}
	// ------------------------------------------ //

}

lag_rms::~lag_rms() {}

void lag_rms::_C( vec p ){
	int	i,j,k; double d;
	for( i=0 ; i<nfq ; i++ ){ p[icx[i]] *= mean12;}
	for( i=0 ; i<n1 ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p1[k]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p1[k]*Cfq[k](i,j); } C(i,j) = d;}
	}
	for( i=n1 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p2[k]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=n1 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p2[k]*Cfq[k](i,j); } C(i,j) = d;}
		for( j=0 ; j<n1 ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){
			d += p[icx[k]] * ( Cfq[k](i,j)*cos(p[iphi[k]]) - Sfq[k](i,j)*sin(p[iphi[k]])); } C(i,j) = d;}
	}
}

void lag_rms::_dC( vec p , int k ){
	if( k<nfq ){	_dCx( p , k );
	}else{			_dPhi( p , k-nfq );}
}

void lag_rms::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void lag_rms::_dPhi( vec p , int k ){
	int	i,j; double cx=p[k]*mean12,phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}


void lag_rms::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3000 ){dpar[i] = 3000;} if( dpar[i]<-3000 ){dpar[i] = -3000;}
		pars[i] += dpar[i]/((n<10)?10:1);
	}
}


void lag_rms::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		if( pars[i]<0 ){ pars[i]*=-1; pars[i+nfq] += M_PI;}
		while( pars[i+nfq] > M_PI ){ pars[i+nfq] -= 2*M_PI; }
		while( pars[i+nfq] <-M_PI ){ pars[i+nfq] += 2*M_PI; }
	}
	mod::print_pars( pars , errs );
}


// ****************************************** //


lag10_rms::lag10_rms( lcurve lc1 , lcurve lc2 , vec fqL , vec pars ) {

	// ----------- initial parameters ------------ //
	n1		=	lc1.len;
	n		=	n1 + lc2.len;
	dt		=	lc1.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	mean1	=	lc1.demean(); mean2	=	lc2.demean();
	mean1sq	=	mean1*mean1; mean2sq = mean2*mean2; mean12 = mean1*mean2;
	int		i;
	for( i=0 ; i<n1 ; i++ ){
		lc[i]	=	lc1.lc[i];
		lce[i]	=	lc1.lce[i]*lc1.lce[i];
		t[i]	=	lc1.t[i];
	}
	for( i=n1 ; i<n ; i++ ){
		lc[i]	=	lc2.lc[i-n1];
		lce[i]	=	lc2.lce[i-n1]*lc2.lce[i-n1];
		t[i]	=	lc2.t[i-n1];
	}
	// ------------------------------------------ //

	// ----------- parent initilizer ------------ //
	init( fqL , 2 );
	// ------------------------------------------ //

	// --------- constants and indices ---------- //
	p1.setlength(nfq); p2.setlength(nfq); icx.setlength(nfq); iphi.setlength(nfq);
	for( i=0 ; i<nfq ; i++ ){
		p1[i]	=	pow(10,pars[i]) * mean1sq;
		p2[i]	=	pow(10,pars[i+nfq]) * mean2sq;
		icx[i]	=	i;
		iphi[i]	=	i+nfq;
	}
	// ------------------------------------------ //

}

lag10_rms::~lag10_rms() {}

void lag10_rms::_C( vec p ){
	int	i,j,k; double d; for(i=0;i<nfq;i++){p[icx[i]] = pow(10,p[i]) * mean12;}
	for( i=0 ; i<n1 ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p1[k]*Cfq[k](i,i); }
		d += f1*p1[0]*Cfq2[0](i,i) + f2*p1[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p1[k]*Cfq[k](i,j); }
		d += f1*p1[0]*Cfq2[0](i,j) + f2*p1[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
	}
	for( i=n1 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p2[k]*Cfq[k](i,i); }
		d += f1*p2[0]*Cfq2[0](i,i) + f2*p2[nfq-1]*Cfq2[2](i,i);
		d += lce[i]; C(i,i) = d;
		for( j=n1 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p2[k]*Cfq[k](i,j); }
		d += f1*p2[0]*Cfq2[0](i,j) + f2*p2[nfq-1]*Cfq2[2](i,j);
		C(i,j) = d;}
		for( j=0 ; j<n1 ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){
			d += p[icx[k]] * ( Cfq[k](i,j)*cos(p[iphi[k]]) - Sfq[k](i,j)*sin(p[iphi[k]])); }
		d += f1*p[icx[0]] * ( Cfq2[0](i,j)*cos(p[iphi[0]]) - Sfq2[0](i,j)*sin(p[iphi[0]]) );
		d += f2*p[icx[nfq-1]] * ( Cfq2[2](i,j)*cos(p[iphi[nfq-1]]) - Sfq2[2](i,j)*sin(p[iphi[nfq-1]]) );
		C(i,j) = d;}
	}
}

void lag10_rms::_dC( vec p , int k ){
	if( k<nfq ){	_dCx( p , k );
	}else{			_dPhi( p , k-nfq );}
}

void lag10_rms::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]],cx = log(10)*pow(10,p[k])*mean12;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void lag10_rms::_dPhi( vec p , int k ){
	int	i,j; double cx=pow(10,p[k])*mean12,phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}


void lag10_rms::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3 ){dpar[i] = 3;} if( dpar[i]<-3 ){dpar[i] = -3;}
		//pars[i] += dpar[i];
		pars[i] += dpar[i]/((n<5)?10:1);
	}
}


void lag10_rms::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		while( pars[i+nfq] > M_PI ){ pars[i+nfq] -= 2*M_PI; }
		while( pars[i+nfq] <-M_PI ){ pars[i+nfq] += 2*M_PI; }
	}
	mod::print_pars( pars , errs );
}

void lag10_rms::what_pars( int& ip1 , int& ip2 ){
	ip1 = nfq; ip2 = npar;
}
