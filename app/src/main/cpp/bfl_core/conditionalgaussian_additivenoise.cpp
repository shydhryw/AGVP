// $Id: conditionalgaussian_additivenoise.cpp 29495 2008-08-13 12:57:49Z tdelaet $
// Copyright (C) 2003 Klaas Gadeyne <first dot last at gmail dot com>
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

#include "include/conditionalgaussian_additivenoise.h"

using namespace Eigen;

namespace bfl_core
{

#define CondGausAddNoise ConditionalGaussianAdditiveNoise

    /// Constructor
    /** @note Because of the virtual public inheritance, we create the Pdf
        baseclass ourselves
    */
    CondGausAddNoise::CondGausAddNoise(const Gaussian &additiveNoise,
                                       unsigned int num_conditional_arguments)
            : ConditionalGaussian(additiveNoise.DimensionGet(),
                                  num_conditional_arguments),
              _additiveNoise_Mu(additiveNoise.ExpectedValueGet()),
              _additiveNoise_Sigma()
    {}

    CondGausAddNoise::CondGausAddNoise(unsigned int dim,
                                       unsigned int num_conditional_arguments)
            : ConditionalGaussian(dim, num_conditional_arguments)
    {
        _additiveNoise_Mu.setZero();
        _additiveNoise_Sigma.setZero();
    }

    /// Destructor
    CondGausAddNoise::~CondGausAddNoise()
    {}

    Matrix3d CondGausAddNoise::CovarianceGet() const
    {
        return AdditiveNoiseSigmaGet();
    }

    const Vector3d &CondGausAddNoise::AdditiveNoiseMuGet() const
    {
        return _additiveNoise_Mu;
    }

    const Matrix3d &CondGausAddNoise::AdditiveNoiseSigmaGet() const
    {
        return _additiveNoise_Sigma;
    }

    void CondGausAddNoise::AdditiveNoiseMuSet(const Vector3d &mu)
    {
        _additiveNoise_Mu = mu;
    }

    void CondGausAddNoise::AdditiveNoiseSigmaSet(const Matrix3d &sigma)
    {
        _additiveNoise_Sigma = sigma;
    }

} // End namespace