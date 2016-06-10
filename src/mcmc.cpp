/*
 * mcmc.cpp
 *
 *  Created on: May 14, 2013
 *      Author: abduz
 */

#include "inc/mcmc.hpp"

mcmc::mcmc(int np,double (*f)(vec&,void*),void *p) {
	npar	=	np;
	loglikelihood	=	f;
	ptr		=	p;
	hqrndrandomize(rnd);
	nrun = 100; nburn = 100; nwk = 100; ncheck = 10;avalue = 2.0;
}

mcmc::~mcmc() {}

void mcmc::run( vec& p, vec& pe, const char*fname){
	int		i,j,k,nacc,nr,n,nsum;
	vec2	pars; pars.setlength(nwk,npar);
	vec		prob,currp,bestp,sum,sum2;
	ivec2	compj;
	double	bestlike,tmplike=-1e20,z,q,a1=1./sqrt(avalue),a2=sqrt(avalue);;
	prob.setlength(nwk);currp.setlength(npar);bestp.setlength(npar);sum.setlength(npar);sum2.setlength(npar);
	compj.setlength(nwk,nwk-1);
	nsum=0;for(i=0;i<npar;i++){sum[i]=0;sum2[i]=0;}
	ofstream fout(fname);

	for(k=0;k<nwk;k++){j=0;for(i=0;i<k;i++){compj[k][j]=i;j++;}for(i=k+1;i<nwk;i++){compj[k][j]=i;j++;}}

	bestlike	=	-1e20;
	for(k=0;k<nwk;k++){
		//pars[k][0] = p[0];currp[0] = p[0];
		for(i=0;i<npar;i++){pars[k][i] = p[i]*(1.0+hqrndnormal(rnd)*0.05); currp[i]=pars[k][i];}
		prob[k]	=	loglikelihood( currp , ptr );
		if(bestlike<prob[k]){bestlike=prob[k];for(i=0;i<npar;i++){bestp[i]=pars[k][i];}}
	}

	nr = 0;nacc = 0;
	for(n=1;n<=(nrun+nburn);n++){
		for(k=0;k<nwk;k++){
			nr++;j = compj[k][hqrnduniformi(rnd,nwk-1)];
			z	=	hqrnduniformr(rnd)*(a2-a1) + a1; z = z*z;
			for(i=0;i<npar;i++){currp[i]=pars[j][i] + z*(pars[k][i]-pars[j][i]);}
			tmplike	=	loglikelihood(currp,ptr);
			q = pow(z,npar-1)*exp(tmplike-prob[k]);
			if( hqrnduniformr(rnd)<=q and tmplike>-1e10 ){
				nacc++;for(i=0;i<npar;i++){pars[k][i]=currp[i];} prob[k] = tmplike;
				if(bestlike<tmplike){bestlike=tmplike;for(i=0;i<npar;i++){bestp[i]=currp[i];}}
				if(n>nburn) {
					for(i=0;i<npar;i++){
						fout << currp[i] << "  ";
						sum[i] += currp[i];sum2[i] += currp[i]*currp[i]; nsum++;
					} fout << tmplike << endl;
				}
			}
		}
		if(n%ncheck==0){
			q = 1.0*nacc/nr; nr=0;nacc=0;
			cout << setw(8) << n << setw(12) << q << setw(12) << tmplike << setw(12) << bestlike << endl;
		}
	}
	for(i=0;i<npar;i++){
		p[i]=bestp[i];
		pe[i] = sqrt((sum2[i] - sum[i]*sum[i]/nsum)/(nsum+1));
	}cout << endl;


	fout.close();

}
