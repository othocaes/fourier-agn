/*
 * psdlag.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#include "inc/psdlag_rms.hpp"

psdlag_rms::psdlag_rms( lcurve lc1, lcurve lc2 , vec fqL ) {

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
	init( fqL , 4 );
	// ------------------------------------------ //

	// --------- constants and indices ---------- //
	ip1.setlength(nfq); ip2.setlength(nfq); icx.setlength(nfq); iphi.setlength(nfq);
	for( i=0 ; i<nfq ; i++ ){
		ip1[i]	=	i;
		ip2[i]	=	i+nfq;
		icx[i]	=	i+2*nfq;
		iphi[i]	=	i+3*nfq;
	}
	// ------------------------------------------ //

}

psdlag_rms::~psdlag_rms() {}

void psdlag_rms::_C( vec p ){
	int	i,j,k; double d;
	for( i=0 ; i<nfq ; i++ ){ p[ip1[i]] *= mean1sq; p[ip2[i]] *= mean2sq; p[icx[i]] *= mean12;}
	for( i=0 ; i<n1 ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip1[k]]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip1[k]]*Cfq[k](i,j); } C(i,j) = d;}
	}
	for( i=n1 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip2[k]]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=n1 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip2[k]]*Cfq[k](i,j); } C(i,j) = d;}
		for( j=0 ; j<n1 ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){
			d += p[icx[k]] * ( Cfq[k](i,j)*cos(p[iphi[k]]) - Sfq[k](i,j)*sin(p[iphi[k]])); } C(i,j) = d;}
	}
}

void psdlag_rms::_dC( vec p , int k ){
	if( k<nfq ){			_dP1( p , k );
	}else if( k<2*nfq ){	_dP2( p , k%nfq );
	}else if( k<3*nfq ){	_dCx( p , k%nfq );
	}else{					_dPhi( p , k%nfq );}
}

void psdlag_rms::_dP1( vec p , int k ){
	int	i,j;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = Cfq[k](i,j); dC(j,i) = dC(i,j);} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);}
	}
}

void psdlag_rms::_dP2( vec p , int k ){
	int	i,j;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = Cfq[k](i,j); dC(j,i) = dC(i,j);}
		for( j=0 ; j<n1 ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);}
	}
}

void psdlag_rms::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void psdlag_rms::_dPhi( vec p , int k ){
	int	i,j; double cx=p[icx[k]]*mean12,phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}

void psdlag_rms::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3000 ){dpar[i] = 3000;} if( dpar[i]<-3000 ){dpar[i] = -3000;}
		pars[i] += dpar[i]/((n<10)?10:1);
	}
}


void psdlag_rms::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		if( pars[i+2*nfq]<0 ){ pars[i+2*nfq]*=-1; pars[i+3*nfq] += M_PI;}
		while( pars[i+3*nfq] > M_PI ){ pars[i+3*nfq] -= 2*M_PI; }
		while( pars[i+3*nfq] <-M_PI ){ pars[i+3*nfq] += 2*M_PI; }
		mod::print_pars( pars , errs );
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

psdlag10_rms::psdlag10_rms( lcurve lc1, lcurve lc2 , vec fqL ) {

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
	init( fqL , 4 );
	// ------------------------------------------ //

	// --------- constants and indices ---------- //
	ip1.setlength(nfq); ip2.setlength(nfq); icx.setlength(nfq); iphi.setlength(nfq);
	for( i=0 ; i<nfq ; i++ ){
		ip1[i]	=	i;
		ip2[i]	=	i+nfq;
		icx[i]	=	i+2*nfq;
		iphi[i]	=	i+3*nfq;
	}
	// ------------------------------------------ //

}

psdlag10_rms::~psdlag10_rms() {}

void psdlag10_rms::_C( vec p ){
	int	i,j,k; double d; for(i=0;i<nfq;i++){ p[ip1[i]] = pow(10,p[ip1[i]])*mean1sq; p[ip2[i]] = pow(10,p[ip2[i]])*mean2sq; p[icx[i]] = pow(10,p[icx[i]])*mean12;}
	for( i=0 ; i<n1 ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip1[k]]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=0 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip1[k]]*Cfq[k](i,j); } C(i,j) = d;}
	}
	for( i=n1 ; i<n ; i++ ){
		d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip2[k]]*Cfq[k](i,i); } d += lce[i]; C(i,i) = d;
		for( j=n1 ; j<i ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){ d += p[ip2[k]]*Cfq[k](i,j); } C(i,j) = d;}
		for( j=0 ; j<n1 ; j++ ){ d = 0; for( k=0 ; k<nfq ; k++ ){
			d += p[icx[k]] * ( Cfq[k](i,j)*cos(p[iphi[k]]) - Sfq[k](i,j)*sin(p[iphi[k]])); } C(i,j) = d;}
	}
}

void psdlag10_rms::_dC( vec p , int k ){
	if( k<nfq ){			_dP1( p , k );
	}else if( k<2*nfq ){	_dP2( p , k%nfq );
	}else if( k<3*nfq ){	_dCx( p , k%nfq );
	}else{					_dPhi( p , k%nfq );}
}

void psdlag10_rms::_dP1( vec p , int k ){
	int	i,j; double p1 = log(10)*pow(10,p[k])*mean1sq;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = p1 * Cfq[k](i,j); dC(j,i) = dC(i,j);} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);}
	}
}

void psdlag10_rms::_dP2( vec p , int k ){
	int	i,j; double p2 = log(10) * pow(10,p[ip2[k]])*mean2sq;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = p2 * Cfq[k](i,j); dC(j,i) = dC(i,j);}
		for( j=0 ; j<n1 ; j++ ){ dC(i,j) = 0; dC(j,i) = dC(i,j);}
	}
}

void psdlag10_rms::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]], cx=log(10)*pow(10,p[icx[k]])*mean12;
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void psdlag10_rms::_dPhi( vec p , int k ){
	int	i,j; double cx=pow(10,p[icx[k]])*mean12,phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}

void psdlag10_rms::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3 ){dpar[i] = 3;} if( dpar[i]<-3 ){dpar[i] = -3;}
		//pars[i] += dpar[i];
		pars[i] += dpar[i]/((n<5)?10:1);
	}
}


void psdlag10_rms::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		while( pars[i+3*nfq] > M_PI ){ pars[i+3*nfq] -= 2*M_PI; }
		while( pars[i+3*nfq] <-M_PI ){ pars[i+3*nfq] += 2*M_PI; }
	}
	mod::print_pars( pars , errs );
}

void psdlag10_rms::what_pars( int& ip1 , int& ip2 ){
	ip1 = 3*nfq; ip2 = 4*nfq;
}
