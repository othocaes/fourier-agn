/*
 * psdlag.hpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#include "mod.hpp"

#ifndef PSDLAG_HPP_
#define PSDLAG_HPP_

class psdlag : public mod {

	int		n1;
	ivec	ip1,ip2,icx,iphi;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dP1( vec , int );
	void	_dP2( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );

public:
	psdlag( lcurve , lcurve , vec );
	virtual ~psdlag();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
};

class psdlag10 : public mod {

	int		n1;
	ivec	ip1,ip2,icx,iphi;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dP1( vec , int );
	void	_dP2( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );

public:
	psdlag10( lcurve , lcurve , vec );
	virtual ~psdlag10();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
	void	what_pars( int&, int& );
};

#endif /* PSDLAG_HPP_ */
