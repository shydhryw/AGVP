// $Id: analyticsystemmodel_gaussianuncertainty.h 29830 2009-01-14 15:10:41Z kgadeyne $
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
#ifndef __SYSTEM_MODEL_GAUSSIANUNCERTAINTY__
#define __SYSTEM_MODEL_GAUSSIANUNCERTAINTY__

#include "systemmodel.h"
#include "analyticconditionalgaussian.h"

namespace bfl_core
{

    /// Class for analytic system models with additive Gauss. uncertainty
    /** Class representing all analytic system Models with Additive
        Gaussian noise
    */
    class AnalyticSystemModelGaussianUncertainty 
            : public SystemModel<Eigen::Vector3d>
    {
    public:
        /// Constructor
        /** @param Systempdf AnalyticConditionalGaussian representing \f$
        P(X_k | X_{k-1}, U_{k}) \f$
        */
        AnalyticSystemModelGaussianUncertainty(AnalyticConditionalGaussian *Systempdf);

        /// Destructor
        virtual ~AnalyticSystemModelGaussianUncertainty();

        /// Returns F-matrix
        /** \f[ F = \frac{df}{dx} \mid_{u,x} \f] used by kalman filter variants
        @param u The value of the input in which the derivate is evaluated
        @param x The value in the state in which the derivate is
        evaluated
        @bug Should actually be defined for _any_ continuous system
        model!  There should be a class between this one and system
        model tout court, not assuming gaussian uncertainty!
        */
        Eigen::Matrix3d
        df_dxGet(const Eigen::Vector3d &u, const Eigen::Vector3d &x);

        /// Returns prediction of state
        Eigen::Vector3d
        PredictionGet(const Eigen::Vector3d &u, const Eigen::Vector3d &x);

        /// Covariance of system noise
        Eigen::Matrix3d
        CovarianceGet(const Eigen::Vector3d &u, const Eigen::Vector3d &x);
    };

} // End namespace BFL

#endif // __SYSTEM_MODEL_GAUSSIANUNCERTAINTY__

