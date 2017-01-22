#ifndef LEARN_STAT_H
#define LEARN_STAT_H

#include <shark/Data/Dataset.h>
#include <shark/Statistics/Statistics.h>

using namespace shark;

RealVector stat(const Data<RealVector>& data, const statistics::BaseStatisticsObject& object);


#endif //LEARN_STAT_H
