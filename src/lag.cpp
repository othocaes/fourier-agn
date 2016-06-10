/*
 * lag.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#include "inc/lag.hpp"

lag::lag( lcurve lc1 , lcurve lc2 , vec fqL , vec pars ) {

	// ----------- initial parameters ------------ //
	n1		=	lc1.len;
	n		=	n1 + lc2.len;
	dt		=	lc1.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	lc1.demean(); lc2.demean();
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
		p1[i]	=	pars[i];
		p2[i]	=	pars[i+nfq];
		icx[i]	=	i;
		iphi[i]	=	i+nfq;
	}
	// ------------------------------------------ //

}

lag::~lag() {}

void lag::_C( vec p ){
	int	i,j,k; double d;
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

void lag::_dC( vec p , int k ){
	if( k<nfq ){	_dCx( p , k );
	}else{			_dPhi( p , k-nfq );}
}

void lag::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void lag::_dPhi( vec p , int k ){
	int	i,j; double cx=p[k],phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}


void lag::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3000 ){dpar[i] = 3000;} if( dpar[i]<-3000 ){dpar[i] = -3000;}
		pars[i] += dpar[i]/((n<10)?10:1);
	}
}


void lag::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		if( pars[i]<0 ){ pars[i]*=-1; pars[i+nfq] += M_PI;}
		while( pars[i+nfq] > M_PI ){ pars[i+nfq] -= 2*M_PI; }
		while( pars[i+nfq] <-M_PI ){ pars[i+nfq] += 2*M_PI; }
	}
	mod::print_pars( pars , errs );
}


// ****************************************** //


lag10::lag10( lcurve lc1 , lcurve lc2 , vec fqL , vec pars ) {

	// ----------- initial parameters ------------ //
	n1		=	lc1.len;
	n		=	n1 + lc2.len;
	dt		=	lc1.dt;
	// ------------------------------------------ //


	// ----------- light curve setup ------------ //
	setlc();
	lc1.demean(); lc2.demean();
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
		p1[i]	=	pow(10,pars[i]);
		p2[i]	=	pow(10,pars[i+nfq]);
		icx[i]	=	i;
		iphi[i]	=	i+nfq;
	}
	// ------------------------------------------ //

}

lag10::~lag10() {}

void lag10::_C( vec p ){
	int	i,j,k; double d; for(i=0;i<nfq;i++){p[i] = pow(10,p[i]);}
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

void lag10::_dC( vec p , int k ){
	if( k<nfq ){	_dCx( p , k );
	}else{			_dPhi( p , k-nfq );}
}

void lag10::_dCx( vec p , int k ){
	int	i,j; double phi=p[iphi[k]],cx = log(10)*pow(10,p[k]);
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( Cfq[k](i,j)*cos(phi) - Sfq[k](i,j)*sin(phi) ); dC(j,i) = dC(i,j);}
	}
}

void lag10::_dPhi( vec p , int k ){
	int	i,j; double cx=pow(10,p[k]),phi=p[iphi[k]];
	for( i=0 ; i<n1 ; i++ ){ for( j=0 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;} }
	for( i=n1 ; i<n ; i++ ){
		for( j=n1 ; j<=i ; j++ ){ dC(i,j) = 0; dC(j,i) = 0;}
		for( j=0 ; j<n1 ; j++ ){
			dC(i,j) = cx * ( -Cfq[k](i,j)*sin(phi) - Sfq[k](i,j)*cos(phi) ); dC(j,i) = dC(i,j);}
	}
}


void lag10::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		if( dpar[i]>3 ){dpar[i] = 3;} if( dpar[i]<-3 ){dpar[i] = -3;}
		//pars[i] += dpar[i];
		pars[i] += dpar[i]/((n<5)?10:1);
	}
}


void lag10::print_pars( vec& pars , vec& errs ){
	for( int i=0 ; i<nfq ; i++ ){
		while( pars[i+nfq] > M_PI ){ pars[i+nfq] -= 2*M_PI; }
		while( pars[i+nfq] <-M_PI ){ pars[i+nfq] += 2*M_PI; }
	}
	mod::print_pars( pars , errs );
}

void lag10::what_pars( int& ip1 , int& ip2 ){
	ip1 = nfq; ip2 = npar;
}
