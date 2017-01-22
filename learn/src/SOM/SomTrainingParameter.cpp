#include "SomTrainingParameter.h"

using namespace std;

vector<SomTrainingParameter> SomTrainingParameter::compose(vector<vector<boost::any>> parametersSet)
{
    int moreThanOne = -1;
    for (int i=0; i < parametersSet.size() && moreThanOne < 0; i++) {
        if (parametersSet[i].size() > 1)
            moreThanOne = i;
    }
    if (moreThanOne < 0)
        return vector<SomTrainingParameter> {
                SomTrainingParameter(boost::any_cast<int>(parametersSet[0][0]), boost::any_cast<NeighborhoodFunction>(parametersSet[1][0]), boost::any_cast<int>(parametersSet[2][0]))
        };
    else {
        bool greater = false;
        vector<vector<boost::any>> head;
        vector<vector<boost::any>> tail;
        for (vector<vector<boost::any>>::iterator it = parametersSet.begin(); it != parametersSet.end(); ++it) {
            vector<boost::any>
                    headElem = ((*it).size() > 1 && !greater) ? vector<boost::any>((*it).begin(), (*it).begin() + 1) : *it,
                    tailElem = ((*it).size() > 1 && !greater) ? vector<boost::any>((*it).begin() + 1, (*it).end()) : *it;
            head.push_back(headElem);
            tail.push_back(tailElem);
            if (!greater)
                greater = ((*it).size() > 1);
        }
        vector<SomTrainingParameter> h = compose(head);
        vector<SomTrainingParameter> t = compose(tail);
        h.insert(h.begin(), t.begin(), t.end());

        return h;
    }
}

ostream& operator<<(ostream& os, const NeighborhoodFunction& obj)
{
    os << ((obj == NeighborhoodFunction::gaussian) ? "gaussian": "cutgauss");
    return os;
}

ostream& operator<<(ostream& os, const SomTrainingParameter& obj)
{
    os <<  "neighboorhood(" << obj.getNeighborhoodFunction() << "), iterations(" << obj.getMaxIterations() << "), radius start(" << obj.getRadiusStart() <<")";
    return os;
}