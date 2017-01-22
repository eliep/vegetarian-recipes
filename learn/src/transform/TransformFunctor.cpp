//
// Created by elie on 10/01/17.
//

#include "TransformFunctor.h"
#include <shark/LinAlg/BLAS/matrix_expression.hpp>


AbsoluteDifference::AbsoluteDifference(double offset) : m_offset(offset), m_scalar(true) { }

AbsoluteDifference::AbsoluteDifference(const RealVector offset) : m_offsetv(offset), m_scalar(false) {}

RealVector AbsoluteDifference::operator()(RealVector input) const {
    if(m_scalar) {
        for(std::size_t i = 0; i != input.size(); ++i)
            input(i) -= m_offset;
    } else {
        SIZE_CHECK(m_offsetv.size() == input.size());
        for(std::size_t i = 0; i != input.size(); ++i)
            input(i) -= m_offsetv(i);
    }

    return shark::blas::abs(input);
}



SafeDivide::SafeDivide(double factor, double divideByZeroValue) : m_factor(factor), m_divideByZeroValue(divideByZeroValue), m_scalar(true) {}

SafeDivide::SafeDivide(const RealVector factor, double divideByZeroValue) : m_factor(0), m_factorv(factor), m_divideByZeroValue(divideByZeroValue), m_scalar(false) {}

RealVector SafeDivide::operator()(RealVector input) const {
    if(m_scalar) {
        for(std::size_t i = 0; i != input.size(); ++i)
            (m_factor != 0) ? input(i) /= m_factor : m_divideByZeroValue;
        return input;
    } else {
        SIZE_CHECK(m_factorv.size() == input.size());
        for(std::size_t i = 0; i != input.size(); ++i)
            (m_factorv(i) != 0) ? input(i) /= m_factorv(i) : m_divideByZeroValue;
        return input;
    }
}


Log::Log(double shift): m_shift(shift) {}

RealVector Log::operator()(RealVector input) const {
    return log(input + m_shift);
}