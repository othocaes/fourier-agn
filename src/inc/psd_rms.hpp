/*
 * psd.hpp
 *
 *  Created on: May 31, 2013
 *      Author: azoghbi
 */

#ifndef PSD_RMS_HPP_
#define PSD_RMS_HPP_

#include "mod.hpp"

class psd_rms: public mod {

	double	mean,mean2;

	void	_C( vec );
	void	_dC( vec , int );

public:
	psd_rms( lcurve , vec );
	virtual ~psd_rms();
};

// ---------------------- //

class psd10_rms: public mod {

	double	mean,mean2;

	void	_C( vec );
	void	_dC( vec , int );

public:
	psd10_rms( lcurve , vec );
	virtual ~psd10_rms();

	void	step_pars( int , vec& , vec& );
};

#endif /* PSD_RMS_HPP_ */
