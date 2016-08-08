/*
 * lag.hpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#ifndef LAG_RMS_HPP_
#define LAG_RMS_HPP_

#include "mod.hpp"

class lag_rms : public mod {

	int		n1;
	vec		p1,p2;
	ivec	icx,iphi;
	double	mean1,mean2,mean1sq,mean2sq,mean12;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );


public:
	lag_rms( lcurve , lcurve , vec , vec );
	virtual ~lag_rms();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
};

// ----------------------------------- //

class lag10_rms : public mod {

	int		n1;
	vec		p1,p2;
	ivec	icx,iphi;
	double	mean1,mean2,mean1sq,mean2sq,mean12;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );


public:
	lag10_rms( lcurve , lcurve , vec , vec );
	virtual ~lag10_rms();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
	void	what_pars( int& , int& );
};

#endif /* LAG_HPP_ */
