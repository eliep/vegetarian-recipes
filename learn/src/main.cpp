#include <iostream>
#include <fstream>
#include <chrono>

#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>
#include <shark/Algorithms/KMeans.h>
#include <shark/Data/Csv.h>

#include "../vendor/json/json.hpp"

#include "elasticsearch/RecipeClient.h"
#include "SOM/SomEvaluation.h"
#include "transform/Reducer.h"
#include "transform/ReducerOnQuantileValue.h"
#include "transform/SampleFilterMedianOutlierDetection.h"


using json = nlohmann::json;

using namespace shark;

int main()
{
    LabeledData<RealVector, string> recipesData;
    LabeledData<RealVector, string> transformedRecipesData;

    RecipeClient recipeClient("localhost", "9200");
    cout << "loading recipe: " ;
    recipesData = recipeClient.getRecipes();
    cout << " (" << recipesData.numberOfElements() << "x" << dataDimension(recipesData.inputs()) << ")" << endl;

    cout << "dimension reduction: " ;
    Reducer<RealVector> dimensionReducer;
    ReducerOnQuantileValue<RealVector> filterTrainer(0.5, 0);
    filterTrainer.train(dimensionReducer, recipesData.inputs());
    transformedRecipesData = reduceInputs(recipesData, dimensionReducer);
    cout << "(" << transformedRecipesData.numberOfElements() << "x" << dataDimension(transformedRecipesData.inputs()) << ")" << endl;

    cout << "log transform: ";
    Log logTransform = Log(1.0);
    transformedRecipesData = transformInputs(transformedRecipesData, logTransform);
    cout << "(" << transformedRecipesData.numberOfElements() << "x" << dataDimension(transformedRecipesData.inputs()) << ")" << endl;

    cout << "outlier filtering: " ;
    Filter<RealVector> sampleFilter;
    SampleFilterMedianOutlierDetection<RealVector> sampleFilterTrainer(4);
    sampleFilterTrainer.train(sampleFilter, transformedRecipesData.inputs());
    transformedRecipesData = filterInputs(transformedRecipesData, sampleFilter);
    cout << "(" << transformedRecipesData.numberOfElements() << "x" << dataDimension(transformedRecipesData.inputs()) << ")" << endl;

    cout << "unit variance normalization: " ;
    Normalizer<RealVector> normalizer;
    NormalizeComponentsUnitVariance<RealVector> normalizingTrainer(true);
    normalizingTrainer.train(normalizer, transformedRecipesData.inputs());
    transformedRecipesData = transformInputs(transformedRecipesData, normalizer);
    cout << "(" << transformedRecipesData.numberOfElements() << "x" << dataDimension(transformedRecipesData.inputs()) << ")" << endl;

    Centroids centroids;
    Som som(centroids, 6, 8);
    SomEvaluation eval(&som);

    cout << "parameters search: " << endl;
    std::vector<std::vector<boost::any>> parametersSet {
            { 5, 10, 15, 20, 30, 50 }, { NeighborhoodFunction::gaussian, NeighborhoodFunction::cutgauss }, { 5, 4, 3, 2 } };
    vector<SomTrainingParameter> parameters = SomTrainingParameter::compose(parametersSet);

    double minError = std::numeric_limits<double>::max();
    SomTrainingParameter bestParameter;
    for (SomTrainingParameter p: parameters) {
        som.train(transformedRecipesData.inputs(), p, false);
        cout << p << " ";
        double error = eval.eval(transformedRecipesData.inputs()).getTopographicError(); //QuantizationError();
        cout << " -> " << eval << endl;
        if (error < minError) {
            bestParameter = p;
            minError = error;
        }
    }
    cout << "best parameters: " << bestParameter <<endl;

    cout << "train: " ;
    som.train(transformedRecipesData.inputs(), bestParameter, false);
    eval.eval(transformedRecipesData.inputs());
    cout << " -> " << bestParameter << " " << eval << endl;

    cout << "updating: " ;

    int recipeCount = recipeClient.updateElastisearchDocuments(transformedRecipesData, som);

    cout << " (" << recipeCount << " recipes updated)" << endl;

    return 0;
}