//
// Created by elie on 10/01/17.
//

#include "Stat.h"

RealVector stat(const Data<RealVector>& data, const statistics::BaseStatisticsObject& object) {
    std::size_t dimension = dataDimension(data);
    statistics::ResultTable<double> table(dimension, "statistics");
    table.setDimensionName(0, "data");

    for (std::size_t i = 0; i < data.numberOfElements(); i++) {
        table.update(0, data.element(i));
    }

    statistics::Statistics<double> stats(&table);
    stats.addStatistic(object);

    return stats[0].at(object.name());
}