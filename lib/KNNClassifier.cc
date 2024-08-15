/* 
CosSimilarity(A, A) {  //LATEX
    \frac{\sum_{i=1}^n A_i \cdot B_i}{sqrt{\sum_{i = 1}^n A_i^2} \cdot sqrt{\sum_{i = 1}^n B_i^2}}
}
*/

#include "KNNClassifier.h"

double KNNClassifier::DocCosSimilarity(const Document* doc_in, const Document* doc_tr) {  //doc_in: to be analyzed, doc_tr: from training set
    auto iter_tr = doc_tr->GetWord2Occur().begin();
    auto iter_in = doc_in->GetWord2Occur().begin();
    double num = 0.0; //nominator for cos-similarity
    double denom = 0.0; //denominator for cos-similarity

    for (; iter_tr != doc_tr->GetWord2Occur().end(); iter_tr++) {
        denom += pow(iter_tr->second,2); //Computation of denominator's first part
        auto iter_in_search = doc_in->GetWord2Occur().find(iter_tr->first);
        if (iter_in_search == doc_in->GetWord2Occur().end()) { //Avoid overwrite for doc_in
            continue; //equivalent to 0-sum: occurrence is 0 for word in doc_in
        } else {
            num += ((iter_tr->second) * (iter_in_search->second)); 
        }
    }
    denom = sqrt(denom);

    double denom2 = 0.0;
    for (; iter_in != doc_in->GetWord2Occur().end(); iter_in++) {
        denom2 += pow(iter_in->second,2);
    }
    denom2 = sqrt(denom2);
    denom = denom * denom2;

    double out = num/denom;
    return out;
}
