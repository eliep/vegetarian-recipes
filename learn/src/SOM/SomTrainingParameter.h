#ifndef LEARN_SOMTRAININGPARAMETER_H
#define LEARN_SOMTRAININGPARAMETER_H

#include <cmath>
#include <iostream>
#include <vector>
#include <boost/any.hpp>

enum class NeighborhoodFunction { gaussian, cutgauss };


class SomTrainingParameter {
public:
    SomTrainingParameter() {}
    SomTrainingParameter(int maxIterations, NeighborhoodFunction neighborhoodFunction, int radiusStart, int radiusEnd = 1)
            : m_radiusStart(radiusStart), m_radiusEnd(radiusEnd), m_maxIterations(maxIterations), m_neighborhoodFunction(neighborhoodFunction) { }

    inline int getRadiusStart() const { return m_radiusStart; }
    inline int getRadiusEnd() const { return m_radiusEnd; }
    inline int getMaxIterations() const { return m_maxIterations; }
    inline NeighborhoodFunction getNeighborhoodFunction() const { return m_neighborhoodFunction; }

    double radiusAt(int iter) const {
        return pow(m_radiusEnd + ((m_maxIterations-iter)/(double)(m_maxIterations)) * (m_radiusStart - m_radiusEnd), 2);
    }

    static std::vector<SomTrainingParameter> compose(std::vector<std::vector<boost::any>> parametersSet);

private:
    int m_radiusStart;
    int m_radiusEnd;
    int m_maxIterations;
    NeighborhoodFunction m_neighborhoodFunction;
};

std::ostream& operator<<(std::ostream& os, const NeighborhoodFunction& obj);
std::ostream& operator<<(std::ostream& os, const SomTrainingParameter& obj);

#endif //LEARN_SOMTRAININGPARAMETER_H
