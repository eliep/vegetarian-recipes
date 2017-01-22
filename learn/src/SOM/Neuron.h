//
// Created by elie on 15/01/17.
//

#ifndef LEARN_NEURON_H
#define LEARN_NEURON_H

#include <shark/Data/Dataset.h>
#include <shark/LinAlg/BLAS/blas.h>
#include <shark/LinAlg/Metrics.h>

using namespace shark;

class Neuron {
private:
    int m_position, m_col, m_row, m_mapHeight, m_mapWidth;
    RealVector m_weight;

public:
    int getPosition() { return m_position; }
    int getRow() { return m_row; }
    int getCol() { return m_col; }
    double getDistance(RealVector const& input) {
        return blas::distance(input, m_weight);
    }

    Neuron(int position, int mapHeight, int mapWidth, RealVector const& weight);
    double distance(Neuron n);
    double sqrDistance(Neuron n);

};

#endif //LEARN_NEURON_H
