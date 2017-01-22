//
// Created by elie on 10/01/17.
//

#ifndef LEARN_DIMENSIONFILTERONQUANTILEVALUE_H
#define LEARN_DIMENSIONFILTERONQUANTILEVALUE_H

#include <shark/Data/Statistics.h>

#include "Reducer.h"
#include "../statistics/Stat.h"

using namespace shark;

template <class DataType = RealVector>
class ReducerOnQuantileValue {
public:
    ReducerOnQuantileValue(double quantile, double value): m_quantile(quantile), m_value(value) { };

    void train(Reducer<DataType>& model, UnlabeledData<DataType> const& input)
    {

        RealVector m = stat(input, statistics::Median());
        UIntVector dimensionIndices; //std::size_t nDimensionToRemove = 0;
        for (int i = 0; i < m.size(); i++)
            if (m[i] != 0)
                dimensionIndices.push_back(i); //nDimensionToRemove++;

        model.setDimensionFilter(dimensionIndices);
    }

    template <class L>
    void train(Reducer<DataType>& model, LabeledData<DataType, L> const& input)
    {
        train(model, input.inputs());
    }

private:
    double m_quantile;
    double m_value;
};


#endif //LEARN_DIMENSIONFILTERONQUANTILEVALUE_H
