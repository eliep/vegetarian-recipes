#ifndef LEARN_RECIPECLIENT_H
#define LEARN_RECIPECLIENT_H

#include <shark/Data/Dataset.h>
#include "../SOM/Som.h"

using namespace shark;
using namespace std;

class RecipeClient {
public:

    RecipeClient(string hostname, string port);
    LabeledData<RealVector, string> getRecipes(int maxRecipe = 7000);
    map<string, int> getNutrientIndices();
    int updateElastisearchDocuments(LabeledData<RealVector, string> recipesData, Som som);

private:
    string m_hostname;
    string m_port;

    LabeledData<RealVector, string> m_recipesData;
    map<string, int> m_nutrientIndices;

    bool bulkUpdateRecipe(map<string, string> updateDocumentMap);
};


#endif //LEARN_RECIPECLIENT_H
