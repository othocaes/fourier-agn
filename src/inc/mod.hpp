/*
 * mod.hpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#ifndef MOD_HPP_
#define MOD_HPP_

#include "def.hpp"
#include <specialfunctions.h>
#include <solvers.h>

class mod {

	void	_CSfq( vec , vector<vec2>& , vector<vec2>& );

public:

	int				n,nfq,npar;
	double			dt,f1,f2;
	vec				lc,lce,t,FqL;
	vector<vec2>	Cfq,Sfq,Cv,Cfq2,Sfq2;

	vec2			C,Ci,I,yyT,yyTmC,dC,C2,C3;
	vec				Cilc;

	ae_int_t		info; densesolverreport rep;

	mod();
	virtual ~mod();

	virtual void _C( vec );
	virtual void _dC( vec , int );
	virtual void step_pars( int , vec& , vec& );
	virtual void print_pars( vec& , vec& );
	virtual void print_pars( vec& , vec& , vec& );
	virtual void what_pars( int&, int& );

	void	setlc(){ lc.setlength(n); lce.setlength(n); t.setlength(n);}
	void	init( vec , int );
	double	loglikelihood( vec );
	void	dlikelihood( vec , double& , vec& , vec2& );
	void	optimize( vec& , vec& );
};

// ----------------------------------------- //
// ---------- mod container ---------------- //
// ----------------------------------------- //

template <class M>
class Mod {

	int			nmod,nfq;
	vector<M>	mods;

public:
	int			npar;
	Mod( vector<lcurve> inlc , vec fqL ){
		int		i;
		nmod	=	inlc.size();
		for( i=0 ; i<nmod ; i++ ){ mods.push_back( M( inlc[i] , fqL ) ); }
		npar	=	mods[0].npar;
		nfq		=	mods[0].nfq;
	}
	Mod( vector<lcurve> lc1, vector<lcurve> lc2 , vec fqL , vec pars ){
		int		i;
		nmod	=	lc1.size();
		for( i=0 ; i<nmod ; i++ ){ mods.push_back( M( lc1[i] , lc2[i] , fqL , pars ) ); }
		npar	=	mods[0].npar;
		nfq		=	mods[0].nfq;
	}
	Mod( vector<lcurve> lc1, vector<lcurve> lc2 , vec fqL ){
		int		i;
		nmod	=	lc1.size();
		for( i=0 ; i<nmod ; i++ ){ mods.push_back( M( lc1[i] , lc2[i] , fqL ) ); }
		npar	=	mods[0].npar;
		nfq		=	mods[0].nfq;
	}
	virtual ~Mod(){}

	double	loglikelihood( vec p ){
		double l=0; for( int i=0 ; i<nmod ; i++ ){ l += mods[i].loglikelihood(p); }
		return l;
	}

	void	dlikelihood( vec p , double& loglike , vec& grad , vec2& hess ){
		int i,j,nm;vec g; vec2 h; double l;g.setlength(npar);h.setlength(npar,npar);
		loglike = 0; for( i=0 ; i<npar ; i++ ){ grad[i]=0; for(j=0;j<npar;j++){ hess(i,j) = 0;}}

		for( nm=0 ; nm<nmod ; nm++ ){
			mods[nm].dlikelihood( p , l , g , h );
			loglike	+=	l;
			for( i=0 ; i<npar ; i++ ){ grad[i] += g[i]; for(j=0;j<npar;j++){ hess(i,j) += h(i,j);}}
		}
	}

	void	step_pars( int n, vec& dpar , vec& pars ){ mods[0].step_pars( n , dpar , pars );}
	void	print_pars( vec& pars , vec& errs ){ mods[0].print_pars( pars , errs );}
	void	print_pars( vec& pars , vec& errs1 , vec& errs2 ){ mods[0].print_pars( pars , errs1 , errs2 );}
	void	what_pars( int& ip1 , int& ip2 ){ mods[0].what_pars( ip1 , ip2 );}

	double	optimize( vec& pars , vec& errs ){
		int		nmax = 400;
		double	tol = 1e-3;

		ae_int_t info; densesolverreport rep;
		int		i,j,n,sing=0,np;
		double	loglike,dpmax,dl,prevl=-1e20;
		vec		tmppars,dpar,grad,prevp; vec2 hess,hessi,ii;
		tmppars.setlength(npar); dpar.setlength(npar); grad.setlength(npar); prevp.setlength(npar);
		hess.setlength( npar , npar );ii.setlength(npar,npar);hessi.setlength(npar,npar);
		for( i=0 ; i<npar ; i++ ){
			tmppars[i] = pars[i]; dpar[i] = 0.01*pars[i]; ii(i,i)=1;for(j=0;j<i;j++){ii(i,j)=0;ii(j,i)=0;}
			prevp[i] = pars[i];
		}

		for( n=1 ; n<= nmax ; n++ ){

			dlikelihood( tmppars , loglike , grad , hess );
			// ---------- //
			if( loglike != loglike ){ for( i=0 ; i<npar ; i++ ){ dpar[i]/=10;tmppars[i] = prevp[i] + dpar[i]; } continue;}
			dl = loglike - prevl;
			// ---------- //
			vector<int> ic,iv; sing = 0;
			for( i=0; i <npar ; i++ ){ if( fabs(grad[i]) < 1e-5 ){ sing=1;ic.push_back(i); }else { iv.push_back(i);}}
			if( sing == 1 and iv.size()!=0 ){
				np = iv.size();
				vec	g; vec2 h,hi,iii; g.setlength(np); h.setlength(np,np); hi.setlength(np,np);iii.setlength(np,np);
				for( i=0 ; i<np ; i++ ){
					iii(i,i) = 1; h(i,i) = hess(iv[i],iv[i]);
					g[i] = grad[iv[i]]; for(j=0;j<i;j++){ h(i,j) = hess(iv[i],iv[j]);h(j,i)=h(i,j);iii(i,j)=0;iii(j,i)=0;}
				}
				spdmatrixcholesky( h , np , false );
				spdmatrixcholeskysolvem( h , np , false , iii , np , info , rep , hi );

				dpmax = -1e20;
				for( i=0 ; i<np ; i++ ){
					dpar[iv[i]] = 0; for( j=0 ; j<np ; j++ ){ dpar[iv[i]] += g[j] * hi(i,j);}
					dpmax	=	max( dpmax , fabs(dpar[iv[i]]) );
				}
				for( i=0 ; i<int(ic.size()); i++ ){ dpar[ic[i]] = 0;}
			}else {
			// --------- //

				spdmatrixcholesky( hess , npar , false );
				spdmatrixcholeskysolvem( hess , npar , false , ii , npar , info , rep , hessi );

				dpmax = -1e20;
				for( i=0 ; i<npar ; i++ ){
					dpar[i] = 0; for( j=0 ; j<npar ; j++ ){ dpar[i] += grad[j] * hessi(i,j);}
					dpmax	=	max( dpmax , fabs(dpar[i]) );
				}
			}
			for(i=0;i<npar;i++){prevp[i] = tmppars[i];}
			prevl = loglike;
			if ( dl < 2 and n>10 ){for(i=0;i<npar;i++){dpar[i]/=2;}}
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
		return loglike;
	}

	double	opti( vec pars , vec errs ,int k ){
		int		nmax = 400;
		double	tol = 1e-3;

		ae_int_t info; densesolverreport rep;
		int		i,j,n,np;
		double	loglike,dpmax,dl,prevl=-1e20;
		vec		tmppars,dpar,grad,prevp; vec2 hess,hessi,ii;
		tmppars.setlength(npar); dpar.setlength(npar); grad.setlength(npar); prevp.setlength(npar);
		hess.setlength( npar , npar );ii.setlength(npar,npar);hessi.setlength(npar,npar);
		vector<int> ic,iv; np = npar - 1;
		vec	g; vec2 h,hi,iii; g.setlength(np); h.setlength(np,np); hi.setlength(np,np);iii.setlength(np,np);
		ic.push_back(k);

		for( i=0 ; i<npar ; i++ ){
			tmppars[i] = pars[i]; dpar[i] = 0.01*pars[i]; ii(i,i)=1;for(j=0;j<i;j++){ii(i,j)=0;ii(j,i)=0;}
			prevp[i] = pars[i];
			if(i==k){continue;} iv.push_back(i);
		}


		for( n=1 ; n<= nmax ; n++ ){

			dlikelihood( tmppars , loglike , grad , hess );
			// ---------- //
			if( loglike != loglike ){ for( i=0 ; i<npar ; i++ ){ tmppars[i] = prevp[i] + dpar[i]/10; } continue;}
			dl = loglike - prevl;
			// ---------- //


			for( i=0 ; i<np ; i++ ){
				iii(i,i) = 1; h(i,i) = hess(iv[i],iv[i]);
				g[i] = grad[iv[i]]; for(j=0;j<i;j++){ h(i,j) = hess(iv[i],iv[j]);h(j,i)=h(i,j);iii(i,j)=0;iii(j,i)=0;}
			}
			spdmatrixcholesky( h , np , false );
			spdmatrixcholeskysolvem( h , np , false , iii , np , info , rep , hi );

			dpmax = -1e20;
			for( i=0 ; i<np ; i++ ){
				dpar[iv[i]] = 0; for( j=0 ; j<np ; j++ ){ dpar[iv[i]] += g[j] * hi(i,j);}
				dpmax	=	max( dpmax , fabs(dpar[iv[i]]) );
			}
			for( i=0 ; i<int(ic.size()); i++ ){ dpar[ic[i]] = 0;}

			for(i=0;i<npar;i++){prevp[i] = tmppars[i];}
			prevl = loglike;
			if ( dl < 2 and n>10 ){for(i=0;i<npar;i++){dpar[i]/=2;}}
			step_pars( n , dpar , tmppars );

			cout << n << " " << dpmax << " " << loglike << " "; for(j=0;j<npar;j++){cout << tmppars[j] << " ";} cout << endl;
			if ( dpmax<tol ) break;
		}
		return loglike;
	}

	void	errors_avg( vec& pars , vec& errs ){
		vec errs1,errs2;errs1.setlength(npar);errs2.setlength(npar);
		errors( pars , errs1 , 1 );
		errors( pars , errs2 , -1 );
		for(int i=0;i<npar;i++){errs[i] = (errs1[i]+errs2[i])/2;}
	}

	void	errors( vec& pars , vec& errs1 , vec& errs2 ){
		errors( pars , errs1 , 1 );
		errors( pars , errs2 , -1 );
	}

	void	errors( vec& pars , vec& errs , int sign=-1){
		double		tol = 1e-3 , delchi = 1;
		int			i,ip,ip1,ip2;
		double		pu,pd,phalf=0,bestlike,tmplike,dl;
		vec			pars0,errs0,tmpp; pars0.setlength(npar);errs0.setlength(npar);tmpp.setlength(npar);
		bestlike	=	optimize( pars , errs );
		for(i=0;i<npar;i++){pars0[i] = pars[i];errs0[i]=errs[i];}

		what_pars( ip1 , ip2 );

		for( ip=ip1 ; ip<ip2 ; ip++ ){
			cout << endl << "******* par " << ip << " *******" << endl << endl;
			pd	=	pars[ip];
			for( i=0 ;i<npar ; i++ ){tmpp[i] = pars0[i]; }

			i=2; dl = 0;
			while( dl < delchi ){
				pu			=	pars[ip] + sign*i*errs0[ip];
				tmpp[ip]	=	pu;
				tmplike		=	opti( tmpp , errs , ip );
				dl			=	2*(bestlike - tmplike);
				i++;
			}
			while( fabs(dl-delchi)>tol ){
				phalf		=	(pu+pd)/2.0;
				tmpp[ip]	=	phalf;
				tmplike		=	opti( tmpp , errs , ip );
				dl			=	2*(bestlike - tmplike);
				cout << "+++ " << bestlike << " " << tmplike << " " << pars[ip] << " "<< phalf << " " << dl << endl;
				if( dl<delchi ){ pd = phalf;}else{ pu = phalf;}
			}
			errs0[ip]		=	sign* ( phalf - pars0[ip] );
		}
		for( i=0 ; i<npar ; i++ ){ pars[i] = pars0[i]; errs[i] = errs0[i];}
		print_pars( pars0 , errs0 );
		//optimize( pars0 , errs );
	}
};

#endif /* MOD_HPP_ */
