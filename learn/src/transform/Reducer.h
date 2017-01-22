//
// Created by elie on 10/01/17.
//

#ifndef LEARN_DIMENSIONFILTER_H
#define LEARN_DIMENSIONFILTER_H

#include "Filter.h"

using namespace shark;

template <class DataType = RealVector>
class Reducer : public AbstractModel<DataType, DataType> {
public:
    typedef AbstractModel<DataType, DataType> base_type;
    typedef Reducer<DataType> self_type;

    typedef typename base_type::InputType InputType;
    typedef typename base_type::OutputType OutputType;
    typedef typename base_type::BatchInputType BatchInputType;
    typedef typename base_type::BatchOutputType BatchOutputType;

    /// \brief From INameable: return the class name.
    std::string name() const { return "Reducer"; }

    Reducer(): m_hasDimensionIndices(false) { }

    /// overwrite dimensionIndices
    void setDimensionFilter(UIntVector const& dimensionIndices)
    {
        m_dimensionIndices = dimensionIndices;
        m_hasDimensionIndices = true;
    }

    /// \brief Evaluate the model
    void eval(BatchInputType const& input, BatchOutputType& output) const
    {
        std::vector<OutputType> rangeOutput;
        for(std::size_t i=0; i != boost::size(input); ++i) {
            OutputType out;
            InputType in = shark::get(input, i);
            eval(in, out);
            rangeOutput.push_back(out);
        }

        output = Batch<OutputType>::createBatchFromRange(rangeOutput);
    }

    /// \brief Evaluate the model
    void eval(BatchInputType const& input, BatchOutputType& output, State& state) const
    {
        eval(input, output);
    }

    /// \brief Evaluate the model
    void eval(InputType const& input, OutputType& output) const
    {
        if (output.size() != m_dimensionIndices.size())
        output.resize(m_dimensionIndices.size());
        for (std::size_t j = 0; j != m_dimensionIndices.size(); j++) {
            output[j] = input[m_dimensionIndices[j]];
        }
    }

private:
    UIntVector m_dimensionIndices;
    bool m_hasDimensionIndices;
};


template<class T>
Data<T> reduce(Data<T> data, Reducer<T> const& f)
{
    int batches = (int) data.numberOfBatches();
    Data<T> result(batches);
    SHARK_PARALLEL_FOR(int i = 0; i < batches; ++i) {
        result.batch(i) = f(data.batch(i));
    }

    return result;
}

template<class T, class L>
LabeledData<T, L> reduceInputs(LabeledData<T, L> data, Reducer<T> const& f)
{
    return LabeledData<T, L>(reduce(data.inputs(),f),data.labels());
}

template<class T>
T reduce(T input, Reducer<T> const& f)
{
    return f(input);
}


#endif //LEARN_DIMENSIONFILTER_H
