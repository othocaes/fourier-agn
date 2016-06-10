/*
 * psd.hpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#ifndef PSD_HPP_
#define PSD_HPP_

#include "mod.hpp"

class psd: public mod {

	void	_C( vec );
	void	_dC( vec , int );

public:
	psd( lcurve , vec );
	virtual ~psd();
};

// ---------------------- //

class psd10: public mod {

	void	_C( vec );
	void	_dC( vec , int );

public:
	psd10( lcurve , vec );
	virtual ~psd10();

	void	step_pars( int , vec& , vec& );
};

#endif /* PSD_HPP_ */
