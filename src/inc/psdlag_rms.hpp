/*
 * psdlag.hpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#include "mod.hpp"

#ifndef PSDLAG_RMS_HPP_
#define PSDLAG_RMS_HPP_

class psdlag_rms : public mod {

	int		n1;
	ivec	ip1,ip2,icx,iphi;
	double	mean1,mean2,mean1sq,mean2sq,mean12;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dP1( vec , int );
	void	_dP2( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );

public:
	psdlag_rms( lcurve , lcurve , vec );
	virtual ~psdlag_rms();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
};

class psdlag10_rms : public mod {

	int		n1;
	ivec	ip1,ip2,icx,iphi;
	double	mean1,mean2,mean1sq,mean2sq,mean12;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dP1( vec , int );
	void	_dP2( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );

public:
	psdlag10_rms( lcurve , lcurve , vec );
	virtual ~psdlag10_rms();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
	void	what_pars( int&, int& );
};

#endif /* PSDLAG_RMS_HPP_ */
