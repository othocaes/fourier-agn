/*
 * main.cpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#include "inc/main.hpp"

/**************************************
 * Main function
 * Checks if there is an input and
 * raises an error.
 * otherwise, call do_work(fname)
 **************************************/
int main( int argc , char* argv[] ){
	if ( argc < 2 ){
		cerr << "** Error ** No input file given." << endl;
		usage();
	}else {
		do_work( argv[1] );
	}
	return 0;
}




/**************************************
 * Main function that does the work
 * INPUT:	char*	fname: input file name
 * RETURN:	void
 **************************************/

void do_work( char* fname ){

	/************   Reading the input file  ***********/

	ifstream fp(fname);
	string line,mcmcfile; stringstream ss;
	int i,nfiles,nfq,mode,npar,bin1,bin2,fit_type,nrun,nburn,nwk; bool strict;
	getline(fp,line);ss.str(line); ss >> nfiles;								ss.clear();
	vector<string> files(nfiles);ivec secL;secL.setlength(nfiles);for(i=0;i<nfiles;i++){
	getline(fp,line); ss.str(line); ss >> files[i] >> secL[i];					ss.clear();}
	getline(fp,line); ss.str(line); ss >> nfq;vec fqL;fqL.setlength(nfq+1);
	for(i=0;i<=nfq;i++){ss >> fqL[i];}											ss.clear();
	getline(fp,line); ss.str(line); ss >> mode;									ss.clear();
	npar = (mode==0)?4*nfq:nfq; vec pars; pars.setlength(npar);
	getline(fp,line); ss.str(line); for(i=0;i<npar;i++){ss >> pars[i];}			ss.clear();
	getline(fp,line); ss.str(line); ss >> bin1 >> bin2;							ss.clear();
	if(mode!=0){bin1 = (mode==-1)?-1:(mode-1);bin2=-1;}
	getline(fp,line); ss.str(line); ss >> fit_type;								ss.clear();
	getline(fp,line); ss.str(line); ss >> i; strict	= i!=0;						ss.clear();
	getline(fp,line); ss.str(line); ss >> nrun >> nburn >> nwk >> mcmcfile;		ss.clear();
	fp.close();
	/*********   END Reading the input file  **********/

	/**********  Read the light curves  **************/
	vector<vector<lcurve> > LC;
	for(i=0;i<nfiles;i++) readLC( LC , files[i] , secL[i] , bin1 , bin2 , strict );
	/********  END Read the light curves  ************/


	if( mode > 0 or mode==-1 ){
		vec errs; errs.setlength(nfq);
		vector<lcurve> lc1; for( i=0 ; i<int(LC.size()) ; i++ ){ lc1.push_back( LC[i][0]); }
		Mod<psd10>	p1( lc1 , fqL );

		p1.optimize( pars , errs );
		if( fit_type==1 ) p1.errors( pars , errs );
	}else if( mode == 0 or mode==-1 ){
		vec ec,e1,e2,errs; ec.setlength(2*nfq);e1.setlength(nfq);e2.setlength(nfq);errs.setlength(4*nfq);
		vector<lcurve> lc1,lc2; for( i=0 ; i<int(LC.size()) ; i++ ){ lc1.push_back( LC[i][0]); lc2.push_back( LC[i][1]); }
		Mod<psd10>	p1( lc1 , fqL ), p2( lc2 , fqL );
		vec		pars1,pars2;pars1.setlength(nfq);pars2.setlength(nfq);for(i=0;i<nfq;i++){pars1[i]=pars[i];pars2[i]=pars[i+nfq];}

		p1.optimize( pars1 , e1 );
		p2.optimize( pars2 , e2 );
		if( fit_type==1 ) {
			p1.errors_avg( pars1 , e1 );
			p2.errors_avg( pars2 , e2 );
		}

		vec		pc; pc.setlength(2*nfq);
		for(i=0;i<nfq;i++){pc[i]=pars[i+2*nfq];pc[i+nfq]=pars[i+3*nfq];pars[i]=pars1[i];pars[i+nfq]=pars2[i];pc[i]=(pars1[i]+pars2[i])/2;}
		Mod<lag10>		l( lc1 , lc2 , fqL , pars );
		l.optimize( pc , ec );
		if( fit_type==1 ) l.errors_avg( pc , ec );
		if( fit_type==2 ) {
			mcmc mc( 2*nfq , mcmc_lag10 , (void*)&l );
			mc.nrun=nrun; mc.nburn=nburn; mc.nwk=nwk;
			mc.run( pc , errs , mcmcfile.c_str() );
		}

		for(i=0;i<nfq;i++){ pars[i+2*nfq]=pc[i]; pars[i+3*nfq]=pc[i+nfq];}
		for(i=0;i<nfq;i++){ errs[i] = e1[i]; errs[i+nfq] = e2[i]; errs[i+2*nfq]=ec[i]; errs[i+3*nfq]=ec[i+nfq];}

		Mod<psdlag10>	pl( lc1 , lc2 , fqL );
		if( fit_type==3 ) pl.errors_avg( pars , errs );
		if( fit_type==4 ) {
			mcmc mc( 4*nfq , mcmc_psdlag10 , (void*)&pl );
			mc.nrun=nrun; mc.nburn=nburn; mc.nwk=nwk;
			mc.run( pars , errs , mcmcfile.c_str() );
		}

		pl.print_pars( pars , errs );
	}
}







/**************************************
 * Usage function
 * prints a sample input text
 * INPUT:	none
 * RETURN:	void
 **************************************/
void usage(){
	cerr << setw(40) << setfill('*') << "\n";
	cout << setw(30) << left << "1"							<< "# of lc"			<< endl;
	cout << setw(30) << left << "lc.dat 0"					<< "name1 secL1"		<< endl;
	cout << setw(30) << left << "3 1e-5 1e-4 5e-4 1e-3"		<< "nfq fqL1 fqL2.." 	<< endl;
	cout << setw(30) << left << "1"							<< "mode? 0:lag, n:psd" << endl;
	cout << setw(30) << left << "1 1 1 1 1 1 1 1 1 1 1 1"	<< "init. pars" 		<< endl;
	cout << setw(30) << left << "0 1"						<< "bin1, bin2,-1: all" << endl;
	cout << setw(30) << left << "0"							<< "fit_type" 			<< endl;
	cout << setw(30) << left << "0"							<< "strict" 			<< endl;
	cout << setw(30) << left << "100 50 50 mcmc.dat"		<< "nrun,nburn,nwk,file"<< endl;
	cerr << setw(40) << setfill('*') << "\n";
}





/***************************************
 * READ LIGHTCURVES
 * Read a standard light curve file that has a description at the top
 * the first line is something like: # dT nlc b0 b1 b2 ... b_lc (b0 .. are bins boundaries)
 * Then the content with: time lc1 err1 lc2 err2 ... etc
 *
 * INPUT:
 * 		LC		: a vector of vectors of light curves, where the first dimension is the number of
 * 				light curves (i.e. segments), this is incremented in this function when reading a new file.
 * 				The other dimension is either 1 for psd and 2 for lags, which contains either one light curve
 * 				or two for the lag. Passed by reference
 * 		fname	: name of the light curve file
 * 		secL	: the section length to split the light curves to.
 * 		b1		: the bin1 to read for the first light curve. 0-based, -1 for all
 * 		b2		: the bin2 to read, 0-based, -1 for none (.i.e just psd)
 * 		strict	: a boolean on whether to be strict or not in segmenting the light curve
 * 				e.g. for secL=220: 0 gives 100,100 and 1 gives 100,120
 *
 **************************************/
void readLC( vector<vector<lcurve> >&LC , string fname , int secL , int b1 , int b2 , bool strict ){

	/*		Initialize file stream, define some variables		*/
	ifstream fp(fname.c_str()); string line,sdum; stringstream ss;
	int i,j,nlc,n,nsec,ns,sl; double dt;
	/*		read dt from the description line 					*/
	getline(fp,line); ss.str(line); ss >> sdum >> dt >> nlc; ss.clear();

	/*		get the number of lines 							*/
	n=0;while(getline(fp,line)){if(line!="")n++;} fp.clear(); fp.seekg(0,ios::beg);getline(fp,line);

	/*		calculate section lengths							*/
	if(secL==0){secL=n;}
	nsec = n/secL; vector<int> secl(nsec,secL); if(not strict) secl[nsec-1] += n-nsec*secL;

	/*		Start looping through the number of sections		*/
	for(ns=0;ns<nsec;ns++){
		sl	=	secl[ns]; vector<lcurve> Lc;
		vec t; vector<vec> lc(nlc),lce(nlc); t.setlength( sl ); for(i=0;i<nlc;i++){lc[i].setlength(sl);lce[i].setlength(sl);}

		// Read the first chunk
		for( i=0 ; i<sl ; i++ ){
			getline(fp,line);ss.str(line);ss>>t[i];for(j=0;j<nlc;j++){ss>>lc[j][i]>>lce[j][i];} ss.clear();
		}

		// if we want the whole light curves in bin1
		if( b1==-1 ){
			vec lc_,lce_; lc_.setlength( sl );lce_.setlength( sl );for(i=0;i<sl;i++){lc_[i]=0;lce_[i]=0;}
			for( i=0 ; i<sl ; i++ ){
				for( j=0 ; j<nlc ; j++ ){ if(j==b2){continue;} lc_[i] += lc[j][i]; lce_[i] += lce[j][i]*lce[j][i]; }
				lce_[i] = sqrt(lce_[i]);
			}
			Lc.push_back(lcurve(t,lc_,lce_,dt));

		// else: i.e. light curve 1 is bin1
		}else {
			Lc.push_back(lcurve(t,lc[b1],lce[b1],dt));
		}
		// Do we need a second light curve?
		if(b2!=-1){Lc.push_back(lcurve(t,lc[b2],lce[b2],dt));}

		// push the resulting vector<lcurve> to LC
		LC.push_back(Lc);
	}
	fp.close();
}
