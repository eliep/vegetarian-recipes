#ifndef LEARN_SOMEVALUATION_H
#define LEARN_SOMEVALUATION_H

#include <shark/Data/Dataset.h>
#include <shark/LinAlg/BLAS/blas.h>

#include "Som.h"

using namespace std;

class SomEvaluation {
public:
    typedef LabeledData<RealVector, uint>::const_element_reference DataElementConstReference;
    typedef std::pair<Neuron, Neuron> NeuronPair;

    SomEvaluation(Som const *som): m_som(som) { };
    SomEvaluation eval(Data<RealVector> const& dataset);

    inline double getQuantizationError() const { return m_quantizationError; }
    inline double getTopographicError() const { return m_topographicError; }
    inline Som::TrainingDuration getTrainingDuration() const { return m_som->getTrainingDuration(); }

    vector<map<double, string>> nutrient(map<string, int> nutrientIndices);

protected:
    NeuronPair bmuPair(RealVector const& input);

private:
    Som const *m_som ;
    double m_quantizationError;
    double m_topographicError;

};

std::ostream& operator<<(std::ostream& os, const SomEvaluation& obj);

#endif //LEARN_SOMEVALUATION_H
