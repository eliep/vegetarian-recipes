//
// Created by elie on 11/01/17.
//

#ifndef LEARN_SAMPLEFILTERMEDIANOUTLIERDETECTION_H
#define LEARN_SAMPLEFILTERMEDIANOUTLIERDETECTION_H

#include <shark/Data/Statistics.h>

#include "../transform/TransformFunctor.h"
#include "../statistics/Stat.h"

using namespace shark;

template <class DataType = RealVector>
class SampleFilterMedianOutlierDetection {
public:
    SampleFilterMedianOutlierDetection(uint medianCount): m_medianCount(medianCount) { };

    template <class L>
    void train(Filter<DataType>& model, LabeledData<DataType, L> const& input)
    {
        train(model, input.inputs());
    }

    void train(Filter<DataType>& model, UnlabeledData<DataType> const& input)
    {
        RealVector m = stat(input, statistics::Median());
        UnlabeledData<RealVector> d = transform(input, AbsoluteDifference(m));
        RealVector mdev = stat(d, statistics::Median()) * m_medianCount;
        model.setParameters(m, mdev);
    }

private:
    double m_medianCount;
};
#endif //LEARN_SAMPLEFILTERMEDIANOUTLIERDETECTION_H
