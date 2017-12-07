// $Id: conditionalgaussian_additivenoise.h 29830 2009-01-14 15:10:41Z kgadeyne $
// Copyright (C) 2002 Klaas Gadeyne <first dot last at gmail dot com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#ifndef __CONDITIONAL_GAUSSIAN_ADDITIVE_NOISE__
#define __CONDITIONAL_GAUSSIAN_ADDITIVE_NOISE__

#include "conditionalgaussian.h"
#include "gaussian.h"

namespace bfl_core
{

    /// Abstract Class representing all Conditional Gaussians with additive gaussian noise
    /** This class represents all Pdf's of the type
        \f[ P ( A | B, C, D, ... ) \f] where
        \f[ \mu_A = f(B,C,D, ...) + mu_{additiveNoise} \f] and
        \f[ \Sigma_A = \Sigma_{additiveNoise} \f] and

        \f[ A = N(\mu_A, \Sigma_A) \f]

        f is not necessarily a analytical function

    */
    class ConditionalGaussianAdditiveNoise : public ConditionalGaussian
    {
    public:
        /// Constructor
        /**
       @param gaus Gaussian representing the additive uncertainty
       @param num_conditional_arguments The number of conditional
       arguments.
        */
        ConditionalGaussianAdditiveNoise(const Gaussian &gaus, 
                                         unsigned int num_conditional_arguments = 1);

        /// Constructor 2, Gaussian not yet known
        /**
       @param dim Dimension of state
       @param num_conditional_arguments The number of conditional
       arguments.
        */
        ConditionalGaussianAdditiveNoise(unsigned int dim = 0,
                                         unsigned int num_conditional_arguments = 0);

        // Default copy constructor will do

        /// Destructor
        virtual ~ConditionalGaussianAdditiveNoise();

        // implement virtuals!
        virtual Eigen::Matrix3d CovarianceGet() const;

        // data access functions
        /// Get the mean Value of the Additive Gaussian uncertainty
        /**
       @return the mean Value of the Additive Gaussian uncertainty
        */
        const Eigen::Vector3d &AdditiveNoiseMuGet() const;

        /// Get the covariance matrix of the Additive Gaussian uncertainty
        /**
       @return the mean Value of the Additive Gaussian uncertainty
        */
        const Eigen::Matrix3d &AdditiveNoiseSigmaGet() const;

        /// Set the mean Value of the Additive Gaussian uncertainty
        /**
       @param mu the mean Value of the Additive Gaussian uncertainty
        */
        void AdditiveNoiseMuSet(const Eigen::Vector3d &mu);

        /// Set the covariance of the Additive Gaussian uncertainty
        /**
       @param sigma the covariance matrix of the Additive Gaussian uncertainty
        */
        void AdditiveNoiseSigmaSet(const Eigen::Matrix3d &sigma);


    protected:
        /// additive noise expected value
        Eigen::Vector3d _additiveNoise_Mu;

        /// additive noise covariance
        Eigen::Matrix3d _additiveNoise_Sigma;
    };

} // End namespace BFL

#endif // __CONDITIONAL_GAUSSIAN_ADDITIVE_NOISE__
