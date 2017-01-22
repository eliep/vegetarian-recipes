#include <cmath>

#include "Neuron.h"

Neuron::Neuron(int position, int mapHeight, int mapWidth, RealVector const& weight)
        : m_position(position), m_mapHeight(mapHeight), m_mapWidth(mapWidth), m_weight(weight)
{
    m_row = m_position / m_mapWidth;
    m_col = m_position % m_mapWidth;
}

double Neuron::sqrDistance(Neuron n)
{
    auto sqr = [](int x) { return x * x; };
    return sqr(getRow() - n.getRow()) + sqr(getCol() - n.getCol());
}

double Neuron::distance(Neuron n)
{
    return sqrt(sqrDistance(n));
}