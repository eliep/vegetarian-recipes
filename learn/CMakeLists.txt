cmake_minimum_required(VERSION 3.6)
project(learn)

set(CMAKE_CXX_STANDARD 11)
set(USE_SYSTEM_CURL true)
set(Shark_DIR vendor/Shark/build)

find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

add_subdirectory(vendor/cpr)
include_directories(${CPR_INCLUDE_DIRS})
include_directories(${CMAKE_HOME_DIRECTORY}/vendor/json)

set(SOURCE_FILES
        src/main.cpp
        src/SOM/Som.cpp src/SOM/Som.h src/SOM/Neuron.cpp src/SOM/Neuron.h src/SOM/SomTrainingParameter.cpp src/SOM/SomTrainingParameter.h src/SOM/SomEvaluation.cpp src/SOM/SomEvaluation.h
        src/statistics/Stat.cpp src/statistics/Stat.h
        src/transform/TransformFunctor.cpp src/transform/TransformFunctor.h src/transform/ReducerOnQuantileValue.h src/transform/Reducer.h src/transform/Filter.h src/transform/SampleFilterMedianOutlierDetection.h
        src/elasticsearch/RecipeClient.cpp src/elasticsearch/RecipeClient.h
        ${CMAKE_HOME_DIRECTORY}/vendor/json/json.hpp
)
add_executable(learn ${SOURCE_FILES})
target_link_libraries(learn ${SHARK_LIBRARIES} ${CPR_LIBRARIES})