
#include "RecipeClient.h"
#include <cprtypes.h>
#include <response.h>
#include <api.h>

#include "../../vendor/json/json.hpp"

using json = nlohmann::json;

RecipeClient::RecipeClient(string hostname, string port) : m_hostname(hostname), m_port(port) {}

map<string, int> RecipeClient::getNutrientIndices() {
    if (m_nutrientIndices.size() != 0)
        return m_nutrientIndices;

    cpr::Url url;
    url = cpr::Url{"http://" + m_hostname + ":" + m_port +"/recipes/recipe/_search"};

    cpr::Response request = cpr::Get(url, cpr::Parameters{{"size", "1"}});


    json j = json::parse(request.text);
    std::vector<json> v_digest = j["hits"]["hits"][0]["_source"]["digest"];
    for (int i=0; i < v_digest.size(); i++) {
        m_nutrientIndices[v_digest[i]["label"]] = i;
    }
    return m_nutrientIndices;


}

LabeledData<RealVector, string> RecipeClient::getRecipes(int maxRecipe)
{
    if (m_recipesData.numberOfBatches() != 0)
        return m_recipesData;

    cpr::Url url;
    url = cpr::Url{"http://" + m_hostname + ":" + m_port +"/recipes/recipe/_search"};

    bool moreHits = true;
    int size = (maxRecipe > 1000) ? 1000 : maxRecipe,
        from = 0,
        querySize;

    vector<RealVector> inputs;
    vector<string> ids;
    do {
        cout << '|' << std::flush;
        cpr::Response request = cpr::Get(url, cpr::Parameters{{"size", std::to_string(size)},
                                                              {"from", std::to_string(from)}});
        json j = json::parse(request.text);
        auto hits = j["hits"]["hits"];
        querySize = hits.size();
        for (int i = 0; i < hits.size(); i++) {
            std::vector<json> v_ingredients = hits[i]["_source"]["ingredients"];
            bool exclude = false;
            for (int i=0; i < v_ingredients.size() && !exclude; ++i) {
                json ingredient = v_ingredients[i];
                if (ingredient["weight"] == 0.0)
                    exclude = true;
            }

            if (!exclude) {
                std::vector<json> v_digest = hits[i]["_source"]["digest"];
                RealVector features;
                for (json digest: v_digest) {
                    features.push_back(digest["daily"]);
                }
                inputs.push_back(features);
                ids.push_back(hits[i]["_id"]);
            }
        }
        from += querySize;
    } while (querySize == size && from < maxRecipe);

    m_recipesData = createLabeledDataFromRange(inputs, ids, 1000);

    return m_recipesData;
}


int RecipeClient::updateElastisearchDocuments(LabeledData<RealVector, string> recipesData, Som som)
{
    int recipeCount = 0;
    map<string, string> updateDocumentMap;

    map<string, int> nutrientIndices = getNutrientIndices();
    vector<Som::FeatureRanking> featureRankingPerNeuron = som.neuronFeatureRanking(nutrientIndices);
    vector<json> jsonFeatureRankingPerNeuron;
    for (int i=0; i< featureRankingPerNeuron.size(); i++) {
        json jsonFeatureRanking;
        for (auto featureRank: featureRankingPerNeuron[i]) {
            json jsonFeatureRank;
            jsonFeatureRank["label"] = featureRank.second;
            jsonFeatureRank["weight"] = featureRank.first;
            jsonFeatureRanking.push_back(jsonFeatureRank);
        }
        jsonFeatureRankingPerNeuron.push_back(jsonFeatureRanking);
    }

    auto updateDocument = [](string id, Neuron n, RealVector const& input, json featureRanking) {
        string i_tpl = (boost::format(R"({ "update" : {"_id" : "%s" } })") % id).str();
        string d_tpl = (boost::format(R"({ "doc": { "som": { "pos" : %d, "row" : %d, "col" : %d, "dist" : %f, "nutrient": %s } } })")
                        % n.getPosition() % n.getRow() % n.getCol() %n.getDistance(input) % featureRanking.dump() ).str();
        return i_tpl + "\n" + d_tpl;
    };

    for (int i=0; i< recipesData.numberOfElements(); i++) {
        RealVector input = recipesData.element(i).input;
        string id = recipesData.element(i).label;
        Neuron bmu = som.bmu(input);

        updateDocumentMap[id] = updateDocument(id, bmu, input, jsonFeatureRankingPerNeuron[bmu.getPosition()]);

        if (i % 100 == 0) {
            bulkUpdateRecipe(updateDocumentMap);
            updateDocumentMap.clear();
            cout << "|" << flush;
        }
        recipeCount++;
    }

    if (updateDocumentMap.size() > 0)
        bulkUpdateRecipe(updateDocumentMap);

    return recipeCount;
}

bool RecipeClient::bulkUpdateRecipe(map<string, string> updateDocumentMap)
{
    std::string bulkString;
    for (auto updateDocument: updateDocumentMap) {
        bulkString += updateDocument.second + "\n";
    }

    cpr::Url url{"http://" + m_hostname + ":" + m_port +"/recipes/recipe/_bulk"};
    cpr::Response request = cpr::Post(url, cpr::Body{bulkString});

    auto response = json::parse(request.text);

    return (request.status_code == 200) && (!response["errors"]);
}