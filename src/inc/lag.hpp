/*
 * lag.hpp
 *
 *  Created on: Jun 1, 2013
 *      Author: azoghbi
 */

#ifndef LAG_HPP_
#define LAG_HPP_

#include "mod.hpp"

class lag : public mod {

	int		n1;
	vec		p1,p2;
	ivec	icx,iphi;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );


public:
	lag( lcurve , lcurve , vec , vec );
	virtual ~lag();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
};

// ----------------------------------- //

class lag10 : public mod {

	int		n1;
	vec		p1,p2;
	ivec	icx,iphi;

	void	_C( vec );
	void	_dC( vec , int );
	void	_dCx( vec , int );
	void	_dPhi( vec , int );


public:
	lag10( lcurve , lcurve , vec , vec );
	virtual ~lag10();

	void	step_pars( int , vec& , vec& );
	void	print_pars( vec& , vec& );
	void	what_pars( int& , int& );
};

#endif /* LAG_HPP_ */
