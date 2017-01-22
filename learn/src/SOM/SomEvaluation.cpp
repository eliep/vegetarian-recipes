
#include <shark/LinAlg/Metrics.h>
#include <shark/Models/Clustering/Centroids.h>
#include <shark/Models/Clustering/HardClusteringModel.h>

#include "SomEvaluation.h"

SomEvaluation SomEvaluation::eval(Data<RealVector> const& dataset) {
    Centroids centroids = m_som->getCentroids();
    HardClusteringModel<RealVector> model(&centroids);
    ClassificationDataset clusterMembership(dataset, model(dataset));

    RealVector neuronDataCount(m_som->getNeurons(), 0);
    RealVector neuronQuantizationError(m_som->getNeurons(), 0);
    m_topographicError = 0;
    BOOST_FOREACH(DataElementConstReference element, clusterMembership.elements()) {
        neuronQuantizationError(element.label) += distance(element.input, centroids.centroids().element(element.label));
        neuronDataCount(element.label)++;
        NeuronPair bmus = bmuPair(element.input);
        m_topographicError += (bmus.first.distance(bmus.second) > 1);
    }
    for (int i  =0; i < m_som->getNeurons(); i++)
        neuronQuantizationError[i] = (neuronDataCount[i]!=0) ? neuronQuantizationError[i] / neuronDataCount[i] : 0;
    m_quantizationError = sum(neuronQuantizationError) /  neuronQuantizationError.size();
    m_topographicError /=  dataset.numberOfElements();

    return (*this);
}


SomEvaluation::NeuronPair SomEvaluation::bmuPair(RealVector const& input) {
    RealVector membership = m_som->getCentroids().softMembership(input);
    std::map<double, int> member;
    for (uint k=0; k<membership.size();k++)
        member[membership[k]] = k;
    std::map<double, int>::iterator it = member.end();

    std::map<double, int>::iterator firstBmu = (--it);
    std::map<double, int>::iterator secondBmu = (--it);
    RealVector firstBmuWeight = m_som->getCentroids().centroids().element(firstBmu->second);
    RealVector secondBmuWeight = m_som->getCentroids().centroids().element(secondBmu->second);
    return NeuronPair(m_som->neuron(firstBmu->second), m_som->neuron(secondBmu->second));
}

std::ostream& operator<<(std::ostream& os, const SomEvaluation& obj)
{
    os << " Q(" << obj.getQuantizationError() << "), T(" << obj.getTopographicError() << ")" << " duration(" << obj.getTrainingDuration().count() << "ms)";
    return os;
}