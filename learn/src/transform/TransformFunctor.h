//
// Created by elie on 10/01/17.
//

#ifndef LEARN_TRANSFORM_FUNCTOR_H
#define LEARN_TRANSFORM_FUNCTOR_H

#include <shark/LinAlg/Base.h>

using namespace shark;
///
///\brief Transformation function taking the absolute difference between a vector or a scalar to the elements in a dataset
///
class AbsoluteDifference {
public:
    ///@param offset Scalar taken to all components of all vectors in the dataset before keeping abs
    AbsoluteDifference(double offset);
    ///@param offset Vector taken to vectors in the dataset before keeping abs
    AbsoluteDifference(const RealVector offset);

    typedef RealVector result_type;

    RealVector operator()(RealVector input) const;
private:
    double m_offset;
    RealVector m_offsetv;
    bool m_scalar;
};


///
///\brief Transformation function dividing the elements in a dataset by a scalar or component-wise by values stores in a vector
///
class SafeDivide {
public:
    ///@param factor All components of all vectors in the dataset are divided by this number
    SafeDivide(double factor, double divideByZeroValue);
    ///@param factor For all elements in the dataset, the i-th component is divided by the i-th component of this vector
    SafeDivide(const RealVector factor, double divideByZeroValue);

    typedef RealVector result_type;

    RealVector operator()(RealVector input) const;

private:
    double m_factor;
    RealVector m_factorv;
    double m_divideByZeroValue;
    bool m_scalar;
};

///
///\brief Transformation function taking the log of the elements in a dataset
///
class Log {
public:
    Log(double shift);

    typedef RealVector result_type;

    RealVector operator()(RealVector input) const;
private:
    double m_shift;
};


#endif //LEARN_TRANSFORM_FUNCTOR_H
