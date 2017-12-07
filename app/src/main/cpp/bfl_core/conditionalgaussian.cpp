// $Id: conditionalgaussian.cpp 29890 2009-02-02 10:22:01Z tdelaet $
// Copyright (C) 2003 Klaas Gadeyne <first dot last at gmail dot com>
// Copyright (C) 2008 Tinne De Laet <first dot last at mech dot kuleuven dot be>
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

#include <random>
#include "include/conditionalgaussian.h"

using namespace Eigen;
using namespace std;

namespace bfl_core
{
    ConditionalGaussian::ConditionalGaussian(unsigned int dim,
                                             unsigned int num_conditional_arguments)
            : ConditionalPdf<Vector3d, Vector3d>(dim, num_conditional_arguments),
              _diff(), _Mu(), _Low_triangle(), _samples(), _SampleValue()
    {}

/// Destructor
    ConditionalGaussian::~ConditionalGaussian()
    {}

//Clone function
    ConditionalGaussian *ConditionalGaussian::Clone() const
    {
        return new ConditionalGaussian(*this);
    }

    double
    ConditionalGaussian::ProbabilityGet(const Vector3d &input) const
    {
        // Update Mu
        _Mu = ExpectedValueGet();
        _diff = input - _Mu;

        Vector3d vector3d = CovarianceGet().inverse() * _diff;
        double temp = _diff.transpose().x() * vector3d.x() +
                      _diff.transpose().y() * vector3d.y() +
                      _diff.transpose().z() * vector3d.z();
        double result = exp(-0.5 * temp) /
                        sqrt(pow(M_PI * 2, (double) DimensionGet()) *
                             CovarianceGet().determinant());
        return result;
    }

    bool
    ConditionalGaussian::SampleFrom(vector<Sample<Vector3d>> &samples,
                                    const unsigned int num_samples,
                                    int method, void *args) const
    {
        return Pdf<Vector3d>::SampleFrom(samples, num_samples, method, args);
    }

    bool
    ConditionalGaussian::SampleFrom(Sample<Vector3d> &sample, int method, void *args) const
    {
        // Sampling from a Gaussian is simple if DIMENSION = 1 or 2 (and the
        // 2 variables are independant!)
        // Then we can use inversion sampling (Box-Muller method)
        // So for 1D, we use Box-Muller, else we use the cholesky method
        // These are both methods that don't require any arguments

        // Update mu
        _Mu = ExpectedValueGet();

        switch (method)
        {
            case DEFAULT: // Cholesky, see althere (bad implementation)
            {
                LLT<MatrixXd> lltOfA(CovarianceGet());
                _Low_triangle = lltOfA.matrixL();
                //bool result = CovarianceGet().cholesky_semidefinite(_Low_triangle);

                default_random_engine generator;
                normal_distribution<double> distribution(0.0,1.0);

                for (unsigned int j = 1; j < DimensionGet() + 1; j++)
                    _samples[j] = distribution(generator);

                _SampleValue = (_Low_triangle * _samples) + _Mu;
                sample.ValueSet(_SampleValue);
                return true;
            }
            case BOXMULLER: /// @todo Implement box-muller here
            {
                //cerr << "Box-Muller not implemented yet!" << endl;
                return false;
            }
            case CHOLESKY: // Cholesky Sampling
            {
                LLT<MatrixXd> lltOfA(CovarianceGet());
                _Low_triangle = lltOfA.matrixL();
                //bool result = CovarianceGet().cholesky_semidefinite(_Low_triangle);
                /* For now we keep it simple, and use the scythe library
                 (although wrapped) with the uRNG that it uses itself only */
                /* Sample Gaussian._dimension samples from univariate
                 gaussian This could be done using several available
                 libraries, combined with different uniform RNG.  Both the
                 library to be used and the uRNG could be implemented as
                 #ifdef conditions, although I'm sure there must exist a
                 cleaner way to implement this!
              */
                default_random_engine generator;
                normal_distribution<double> distribution(0.0,1.0);

                for (unsigned int j = 1; j < DimensionGet() + 1; j++)
                    _samples[j] = distribution(generator);

                _SampleValue = (_Low_triangle * _samples) + _Mu;
                sample.ValueSet(_SampleValue);
                return true;
            }
            default:
                return false;
        }
    }

} // End namespace