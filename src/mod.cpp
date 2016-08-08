/*
 * mod.cpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#include "inc/mod.hpp"

mod::mod() {
	n = 0 ; nfq = 0 ; npar = 0; dt = 0; info = 0;f1=0;f2=0;

}

mod::~mod() {}

void mod::init( vec fqL , int fac ){

	// --------------- Freq stuff----------------- //
	int		i,j;
	nfq		=	fqL.length() - 1;
	npar	=	nfq * fac ;
	FqL.setlength(nfq+1); for( i=0 ; i<=nfq ; i++ ){ FqL[i] = fqL[i]; }
	// ------------------------------------------ //


	// ---------------- Integrals ---------------- //
	Cfq.resize( nfq );Sfq.resize( nfq );
	for( i=0 ; i<nfq ; i++ ){ Cfq[i].setlength( n , n ); Sfq[i].setlength( n , n );}
	_CSfq( fqL , Cfq , Sfq );

	Cfq2.resize( 3 );Sfq2.resize( 3 );
	for( i=0 ; i<3 ; i++ ){ Cfq2[i].setlength( n , n ); Sfq2[i].setlength( n , n );}
	vec		fqL2;fqL2.setlength(4); fqL2[0] = 1e-1*fqL[0]; fqL2[1] = fqL[0]; fqL2[2] = fqL[nfq]; fqL2[3] = 10*fqL[nfq];
	//f1 = 1.5; f2 = 0.6;
	f1 = 0.5; f2 = .0;
	_CSfq( fqL2 , Cfq2 , Sfq2 );
	// ------------------------------------------ //


	// -------------- Containers ---------------- //
	C.setlength( n , n );
	Ci.setlength( n , n );
	I.setlength( n , n );
	for( i=0 ; i<n ; i++ ){ I(i,i) = 1.0; for( j=0 ; j<i ; j++ ){ I(i,j)=0; I(j,i)=0;}}
	Cilc.setlength( n );
	yyT.setlength( n , n );
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ yyT(i,j) = lc[i]*lc[j];}}
	yyTmC.setlength( n , n );
	dC.setlength( n , n );
	Cv.resize( npar ); for( i=0 ; i<npar ; i++ ){ Cv[i].setlength( n , n ); }
	C2.setlength( n , n );
	C3.setlength( n , n );

	// ------------------------------------------ //

}

void mod::_CSfq( vec fqL , vector<vec2>&cfq , vector<vec2>& sfq ){
	//fqL[0] *= 1e-6; fqL[nfq]*=2;
	int		i,j,k, nfq=fqL.length()-1;
	double	tt,norm,t1,t2,pi=M_PI,pidt=pi*dt,sdt2,sdt1,cdt2,cdt1,s2dt2,s2dt1,c2dt2,c2dt1,dtmt,dtpt;
	double	st2,st1,ct2,ct1, sm2,sm1,cm2,cm1,  sp2,sp1,cp2,cp1;
	vec		w; w.setlength(nfq+1); for( k=0 ; k<=nfq ; k++ ){ w[k] = 2*pi*fqL[k]; }
	norm	=	1./( pidt*pidt );

	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<n ; j++ ){
		tt	=	t[j] - t[i];
		dtmt = dt - tt; dtpt = dt+tt;
		for( k=0 ; k<nfq ; k++ ){
			if( tt==0 ){
				sinecosineintegrals( w[k+1]*dt , sdt2 , cdt2 );
				sinecosineintegrals( w[k]  *dt , sdt1 , cdt1 );
				t1	=	( cos( w[k+1]*dt ) - 1 ) / ( 2 * fqL[k+1] ) + pidt * sdt2;
				t1	-=	( cos( w[k]  *dt ) - 1 ) / ( 2 * fqL[k]   ) + pidt * sdt1;
				cfq[k](i,j)		=	norm * t1;
				sfq[k](i,j)		=	0;
			}else if( fabs(tt)==dt ){
				sinecosineintegrals( w[k+1]*dt , sdt2 , cdt2 );
				sinecosineintegrals( w[k]  *dt , sdt1 , cdt1 );
				sinecosineintegrals( 2*w[k+1]*dt , s2dt2 , c2dt2 );
				sinecosineintegrals( 2*w[k]  *dt , s2dt1 , c2dt1 );
				t1	=	-( cos( w[k+1]*dt )*pow( sin( 0.5*w[k+1]*dt ) , 2 ) ) / fqL[k+1];
				t1	-=	-( cos( w[k]  *dt )*pow( sin( 0.5*w[k]  *dt ) , 2 ) ) / fqL[k]  ;
				t2	=	pidt * ( s2dt2 - sdt2 );
				t2	-=	pidt * ( s2dt1 - sdt1 );
				cfq[k](i,j)		=	norm * ( t1 + t2 );

				t1	=	-( 2*cos( 0.5*w[k+1]*dt )*pow( sin( 0.5*w[k+1]*dt ) , 3 ) ) / fqL[k+1];
				t1	-=	-( 2*cos( 0.5*w[k]  *dt )*pow( sin( 0.5*w[k]  *dt ) , 3 ) ) / fqL[k]  ;
				t2	=	pidt * ( cdt2 - c2dt2 );
				t2	-=	pidt * ( cdt1 - c2dt1 );
				sfq[k](i,j)		=	norm * fabs(tt)/tt * ( t1 + t2 );
			}else{
				sinecosineintegrals( w[k+1]*tt , st2 , ct2 );
				sinecosineintegrals( w[k]  *tt , st1 , ct1 );
				sinecosineintegrals( w[k+1]*dtmt , sm2 , cm2 );
				sinecosineintegrals( w[k]  *dtmt , sm1 , cm1 );
				sinecosineintegrals( w[k+1]*dtpt , sp2 , cp2 );
				sinecosineintegrals( w[k]  *dtpt , sp1 , cp1 );
				t1	=	( cos( w[k+1]*dtmt ) - 2*cos( w[k+1]*tt ) + cos( w[k+1]*dtpt ) )/(4*fqL[k+1]);
				t1	-=	( cos( w[k]  *dtmt ) - 2*cos( w[k]  *tt ) + cos( w[k]  *dtpt ) )/(4*fqL[k]  );
				t2	=	( dtmt*sm2 - 2*tt*st2 + dtpt*sp2 ) * (pi/2);
				t2	-=	( dtmt*sm1 - 2*tt*st1 + dtpt*sp1 ) * (pi/2);
				cfq[k](i,j)		=	norm * ( t1 + t2 );

				t1	=	( -sin( w[k+1]*dtmt ) - 2*sin( w[k+1]*tt ) + sin( w[k+1]*dtpt ) )/(4*fqL[k+1]);
				t1	-=	( -sin( w[k]  *dtmt ) - 2*sin( w[k]  *tt ) + sin( w[k]  *dtpt ) )/(4*fqL[k]  );
				t2	=	( dtmt*cm2 + 2*tt*ct2 - dtpt*cp2 ) * (pi/2);
				t2	-=	( dtmt*cm1 + 2*tt*ct1 - dtpt*cp1 ) * (pi/2);
				sfq[k](i,j)		=	norm * ( t1 + t2 );
			}
		}

	}}
}


void mod::_C( vec ){
	cerr << endl << "mod::_C is an empty function. It should not be called directly" << endl << endl;;
	exit(1);
}

void mod::_dC( vec , int ){
	cerr << endl << "mod::_C is an empty function. It should not be called directly" << endl << endl;;
	exit(1);
}

double mod::loglikelihood( vec p ){
	double		logdet , chi2 , loglike ; int i;
	_C( p );
	if (not spdmatrixcholesky( C , n , false )) {return -1e20;};
	logdet = 0 ; for( i=0 ; i<n ; i++ ){ logdet += log( C(i,i) ); } logdet *= 2;
	spdmatrixcholeskysolvem( C , n , false , I , n , info , rep , Ci );
	rmatrixmv( n , n , Ci , 0 , 0 , 0 , lc , 0 , Cilc , 0 );
	chi2 = 0 ; for( i=0 ; i<n ; i++ ){ chi2 += Cilc[i]*lc[i] ; }
	loglike	=	-0.5 * ( chi2 + logdet + n*log(2*M_PI) );
	return loglike;
}

void mod::dlikelihood( vec p , double& loglike , vec& grad , vec2& hess ){
	double		logdet , chi2 , res; int i,j,k;
	_C( p );
	for( i=0 ; i<n ; i++ ){ for( j=0 ; j<=i ; j++ ){ yyTmC(i,j) = yyT(i,j) - C(i,j); yyTmC(j,i) = yyTmC(i,j);}}
	spdmatrixcholesky( C , n , false );
	logdet = 0 ; for( i=0 ; i<n ; i++ ){ logdet += log( C(i,i) ); } logdet *= 2;
	spdmatrixcholeskysolvem( C , n , false , I , n , info , rep , Ci );
	rmatrixmv( n , n , Ci , 0 , 0 , 0 , lc , 0 , Cilc , 0 );
	chi2 = 0 ; for( i=0 ; i<n ; i++ ){ chi2 += Cilc[i]*lc[i] ; }
	loglike	=	-0.5 * ( chi2 + logdet + n*log(2*M_PI) );


	for( i=0 ; i<npar ; i++ ){
		_dC( p , i );
		rmatrixgemm( n , n , n , 1.0 , Ci , 0 , 0 , 0 , dC , 0 , 0 , 0 , 0.0 , Cv[i] , 0 , 0 );
	}

	for( i=0 ; i<npar ; i++ ){
		rmatrixgemm( n , n , n , 1.0 , Cv[i] , 0 , 0 , 0 , Ci , 0 , 0 , 0 , 0.0 , C2 , 0 , 0 );
		rmatrixgemm( n , n , n , 1.0 , yyTmC , 0 , 0 , 0 , C2 , 0 , 0 , 0 , 0.0 , C3 , 0 , 0 );
		res = 0; for( k=0 ; k<n ; k++ ){ res += C3(k,k); }
		grad[i]		=	0.5 * res;
		for( j=0 ; j<=i ; j++ ){
			rmatrixgemm( n , n , n , 1.0 , Cv[i] , 0 , 0 , 0 , Cv[j] , 0 , 0 , 0 , 0.0 , C3 , 0 , 0 );
			res = 0; for( k=0 ; k<n ; k++ ){ res += C3(k,k); }
			hess(i,j)	=	0.5 * res; hess(j,i) = 0.5*res;
		}
	}
}


void mod::optimize( vec& pars , vec& errs ){
	int		nmax = 200;
	double	tol = 1e-3;

	int		i,j,n;
	double	loglike,dpmax;
	vec		tmppars,dpar,grad; vec2 hess,hessi,ii;
	tmppars.setlength(npar); dpar.setlength(npar); grad.setlength(npar);
	hess.setlength( npar , npar );ii.setlength(npar,npar);hessi.setlength(npar,npar);
	for( i=0 ; i<npar ; i++ ){
		tmppars[i] = pars[i]; dpar[i] = 0.01*pars[i]; ii(i,i)=1;for(j=0;j<i;j++){ii(i,j)=0;ii(j,i)=0;}
	}

	for( n=1 ; n<= nmax ; n++ ){

		dlikelihood( tmppars , loglike , grad , hess );
		spdmatrixcholesky( hess , npar , false );
		spdmatrixcholeskysolvem( hess , npar , false , ii , npar , info , rep , hessi );

		dpmax = -1e20;
		for( i=0 ; i<npar ; i++ ){
			dpar[i] = 0; for( j=0 ; j<npar ; j++ ){ dpar[i] += grad[j] * hessi(i,j);}
			dpmax	=	max( dpmax , fabs(dpar[i]) );
		}
		step_pars( n , dpar , tmppars );

		cout << n << " " << dpmax << " " << loglike << " "; for(j=0;j<npar;j++){cout << tmppars[j] << " ";} cout << endl;
		if ( dpmax<tol ) break;
	}
	for( i=0 ; i<npar ; i++ ){
		pars[i] = tmppars[i];
		errs[i]	= sqrt(hessi(i,i));
	}
	cout << setfill('-') << setw(40) << "\n";
	for(i=0;i<npar;i++){cout << pars[i] << " ";} cout << endl;
	for(i=0;i<npar;i++){cout << errs[i] << " ";} cout << endl;
	for(i=0;i<npar;i++){cout << grad[i] << " ";} cout << endl;
	cout << setfill('-') << setw(40) << "\n" << setfill(' ');
	print_pars( pars, errs );
}


void mod::step_pars( int n , vec& dpar , vec& pars ){
	for( int i=0 ; i<npar ; i++ ){
		pars[i] += dpar[i];
	}
}

void mod::print_pars( vec& pars, vec& errs ){
	cout << setfill('*') << setw(40) << "\n" << setfill(' ');
	cout << endl << "# "; for(int i=0;i<=nfq;i++){cout << FqL[i] << " ";} cout << endl;
	for(int i=0;i<npar;i++){ printf( "%3.3e %3.3e\n" , pars[i] , errs[i] );}
	cout << setfill('*') << setw(40) << "\n" << setfill(' ');
}

void mod::print_pars( vec& pars, vec& errs1 , vec& errs2 ){
	cout << setfill('*') << setw(40) << "\n" << setfill(' ');
	cout << endl << "# "; for(int i=0;i<=nfq;i++){cout << FqL[i] << " ";} cout << endl;
	for(int i=0;i<npar;i++){ printf( "%3.3e %3.3e %3.3e\n" , pars[i] , errs1[i] , errs2[i] );}
	cout << setfill('*') << setw(40) << "\n" << setfill(' ');
}

void mod::what_pars( int& ip1 , int& ip2 ){
	ip1 = 0; ip2 = npar;
}
