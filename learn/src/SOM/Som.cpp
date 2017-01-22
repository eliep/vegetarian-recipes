
#include <cmath>

#include <boost/range/algorithm/equal.hpp>

#include <shark/LinAlg/Metrics.h>
#include <shark/Models/Clustering/HardClusteringModel.h>

#include "Som.h"

using namespace shark;

Som::Som(Centroids& centroids, const uint height, const uint width)
        : m_centroids(centroids), m_height(height), m_width(width), m_neurons(height*width) { }

std::size_t Som::train(Data<RealVector> const& dataset, SomTrainingParameter parameter, bool trace) {
    SIZE_CHECK(m_neurons <= dataset.numberOfElements());

    auto start = std::chrono::steady_clock::now();

    // initialization
    std::size_t dimension = dataDimension(dataset);

    //if the centers are not already initialized, do it now
    if (m_centroids.numberOfClusters() != m_neurons){
        m_centroids.initFromData(dataset, m_neurons);
    }

    HardClusteringModel<RealVector> model(&m_centroids);
    ClassificationDataset clusterMembership(dataset, model(dataset));

    RealMatrix neuronDistances = interNeuronSqrDistances();
    size_t iter = 0;
    bool equal = false;
    for(; iter != parameter.getMaxIterations() && !equal; ++iter) {
        std::cout << '|'  << std::flush;
        double radius = parameter.radiusAt(iter);
        RealMatrix neuronNeighborhoods = shark::blas::exp(-neuronDistances/(2*radius));
        if (parameter.getNeighborhoodFunction() == NeighborhoodFunction::cutgauss)
            neuronNeighborhoods *= interNeuronRadiusMask(radius);

        RealMatrix neuronDataSum(m_neurons, dimension, 0);
        RealVector neuronDataCount(m_neurons, 0);
        BOOST_FOREACH(DataElementConstReference element, clusterMembership.elements()) {
            row(neuronDataSum, element.label) += element.input;
            neuronDataCount(element.label)++;
        }

        neuronDataSum = shark::blas::prod(neuronNeighborhoods, neuronDataSum);
        neuronDataCount = shark::blas::prod(neuronNeighborhoods, neuronDataCount);

        for (int i = 0; i < neuronDataSum.size1(); i++) {
            if (neuronDataCount(i) != 0)
                row(neuronDataSum, i) /= neuronDataCount(i);
            else
                row(neuronDataSum, i) = m_centroids.centroids().element(i);
        }

        Data<RealVector> weight = createDataFromRange(neuronDataSum);
        m_centroids.setCentroids(weight);

        //compute new cluster memberships and check whether they are
        // equal to the old one, in that case we stop after this iteration
        Data<unsigned int> newClusterMembership = model(dataset);
        equal = boost::equal(
                newClusterMembership.elements(),
                clusterMembership.labels().elements()
        );
        clusterMembership.labels() = newClusterMembership;

    }

    m_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

    return iter;
}

Neuron Som::bmu(RealVector const& input) {
    int indice = m_centroids.hardMembership(input);
    return neuron(indice);
}

RealMatrix Som::interNeuronSqrDistances()
{
    RealMatrix distances(m_neurons, m_neurons, 0);
    for (unsigned int i = 0; i < m_neurons; i++) {
        Neuron n1 = neuron(i);
        for (unsigned int j = 0; j < m_neurons; j++) {
            distances(i, j) =  n1.sqrDistance(neuron(j));
        }
    }
    return distances;
}
RealMatrix Som::interNeuronRadiusMask(double radius)
{
    RealMatrix mask(m_neurons, m_neurons, 0);
    for (unsigned int i = 0; i < m_neurons; i++) {
        Neuron n1 = neuron(i);
        for (unsigned int j = 0; j < m_neurons; j++) {
            double distance = n1.sqrDistance(neuron(j));
            mask(i, j) =  (distance <= radius) ? 1 : 0;
        }
    }
    return mask;
}

std::vector<Som::FeatureRanking> Som::neuronFeatureRanking(FeatureLabel nutrientIndices) {
    Data<RealVector> const& centroids = m_centroids.centroids();
    std::vector<FeatureRanking> neuronNutrientScore;

    for (int i=0; i<centroids.numberOfElements(); ++i) {
        FeatureRanking nutrientScores;
        RealVector centroid = centroids.element(i);
        for (auto nutrientIndice: nutrientIndices) {
            nutrientScores[centroid[nutrientIndice.second]] = nutrientIndice.first;
        }
        neuronNutrientScore.push_back(nutrientScores);
    }

    return neuronNutrientScore;
}

std::ostream& operator<<(std::ostream& os, const Som& obj) {

}