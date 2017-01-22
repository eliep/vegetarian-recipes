#ifndef LEARN_SOM_H
#define LEARN_SOM_H

#include <chrono>
#include <ratio>

#include <shark/Data/Dataset.h>
#include <shark/LinAlg/BLAS/blas.h>
#include <shark/Models/Clustering/Centroids.h>

#include "Neuron.h"
#include "SomTrainingParameter.h"

using namespace shark;

class Som {
public:
    typedef LabeledData<RealVector, uint>::const_element_reference DataElementConstReference;
    typedef std::chrono::duration<long, std::ratio<1, 1000>> TrainingDuration;
    typedef std::map<std::string, int> FeatureLabel;
    typedef std::map<double, std::string> FeatureRanking;

    Som(Centroids& centroids, const uint height, const uint width);

    std::size_t train(Data<RealVector> const& dataset, SomTrainingParameter parameter, bool trace = false);

    Neuron bmu(RealVector const& input);

    //void eval(Data<RealVector> const& dataset, bool trace = false);

    inline Centroids getCentroids() const { return m_centroids; }
    inline Neuron neuron(int position) const { return Neuron(position, m_height, m_width, m_centroids.centroids().element(position)); }
    inline int getNeurons() const { return m_neurons; }
    inline TrainingDuration getTrainingDuration() const { return m_duration; }

    std::vector<FeatureRanking> neuronFeatureRanking(FeatureLabel nutrientIndices);
private:

    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_neurons;
    Centroids m_centroids;
    TrainingDuration m_duration;

    RealMatrix interNeuronSqrDistances();
    RealMatrix interNeuronRadiusMask(double radius);

};

std::ostream& operator<<(std::ostream& os, const Som& obj);

#endif //LEARN_SOM_H
