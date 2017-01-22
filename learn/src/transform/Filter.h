//
// Created by elie on 11/01/17.
//

#ifndef LEARN_FILTER_H
#define LEARN_FILTER_H


#include <boost/foreach.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/sort.hpp>

#include <shark/Data/Dataset.h>
#include <shark/Models/AbstractModel.h>

using namespace shark;

template <class DataType = RealVector>
class Filter : public AbstractModel<DataType, DataType> {
public:
    typedef AbstractModel<DataType, DataType> base_type;
    typedef Filter<DataType> self_type;

    typedef typename base_type::BatchInputType BatchInputType;
    typedef typename base_type::BatchOutputType BatchOutputType;
    typedef typename base_type::InputType InputType;
    typedef typename base_type::OutputType OutputType;

    Filter(): m_hasParameter(false) { }

    /// \brief From INameable: return the class name.
    std::string name() const { return "Filter"; }

    /// overwrite dimensionIndices
    void setParameters(RealVector const& offset, RealVector const& divider)
    {
        m_offset = offset;
        m_divider = divider;
        m_hasParameter = true;
    }

    using base_type::eval;

    /// \brief Evaluate the model
    void eval(BatchInputType const& input, BatchOutputType& output) const
    {
        std::vector<OutputType> rangeOutput;
        for(std::size_t i=0; i != boost::size(input); ++i) {
            OutputType out;
            eval(shark::get(input, i), out);

            if (out.size() > 0) {
                rangeOutput.push_back(shark::get(input, i));
            }
        }

        if (rangeOutput.size() > 0)
            output = Batch<OutputType>::createBatchFromRange(rangeOutput);
    }

    /// \brief Evaluate the model
    void eval(BatchInputType const& input, BatchOutputType& output, State& state) const
    {
        eval(input, output);
    }

    /// \brief Evaluate the model
    void eval(InputType const & pattern, OutputType& output) const
    {
        InputType norm = blas::abs(pattern - m_offset) / m_divider;
        bool keep = true;
        for (std::size_t j=0; (j < norm.size()) && keep; j++) {
            keep = (norm[j] < 1);
        }

        if (keep)
            output = pattern;
    }

private:
    RealVector m_offset;
    RealVector m_divider;
    bool m_hasParameter;
};


template<class T>
Data<T> filter(Data<T> data, Filter<T> const& f)
{
    int batches = (int) data.numberOfBatches();
    Data<T> result(batches);
    int k = 0;
    SHARK_PARALLEL_FOR(int i = 0; i < batches; ++i) {
        typename Data<T>::const_batch_reference batch = f(data.batch(i));
        if ((boost::size(batch) > 0)) {
            result.batch(k) = batch;
            k++;
        }
    }
    result.splice(k);

    return result;
}

template<class T, class L>
LabeledData<T, L> filterInputs(LabeledData<T, L> data, Filter<T> const& f)
{
    typedef typename LabeledData<T, L>::IndexSet IndexSet;
    typedef typename Filter<T>::OutputType OutputType;

    IndexSet keepIndex;
    for (size_t i=0; i< data.numberOfElements(); i++) {
        OutputType output = f(data.inputs().element(i));
        if (output.size() != 0) {
            keepIndex.push_back(i);
        }
    }

    DataView<LabeledData<T, L>> view(data);

    return toDataset(subset(view, keepIndex));
}

template<class T>
T filter(T input, Filter<T> const& f)
{
    return f(input);
}

#endif //LEARN_FILTER_H
