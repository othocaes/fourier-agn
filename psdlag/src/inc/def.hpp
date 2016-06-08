/*
 * def.hpp
 *
 *  Created on: May 24, 2013
 *      Author: abduz
 */

#ifndef DEF_HPP_
#define DEF_HPP_

#include <ap.h>
#include <iostream>
#include <iomanip>

using namespace alglib;
using namespace std;

typedef real_1d_array vec;
typedef real_2d_array vec2;
typedef integer_1d_array ivec;


class lcurve {
public:
	int len; double dt; vec lc,lce,t;
	lcurve(int n=1){t.setlength(n); lc.setlength(n); lce.setlength(n) ;len=n;dt=1.0;}
	lcurve(vec& t_,vec& lc_,vec& lce_,double dt_){t=t_;lc=lc_;lce=lce_;len=t.length();dt=dt_;}
	void demean(){double m=0;int i;for(i=0;i<len;i++){m+=lc[i];} m/=len;for(i=0;i<len;i++){lc[i]-=m;}}
	friend ostream& operator<<(ostream&o,lcurve&l){ o << setprecision(12);
			for(int i=0;i<l.len;i++){o << l.t[i] << " " << l.lc[i] << " " << l.lce[i] << endl;}
			return(o);}
};



#endif /* DEF_HPP_ */
