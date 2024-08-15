#include <cmath> //operations for cos-similarity
#include <string>
#include <map>
#include "Document.h"
#include "TrainingSet.h"



#ifndef KNNCLASSIFIER_H
#define KNNCLASSIFIER_H

#ifndef CLASSIFIER_H
#define CLASSIFIER_H
class Classifier { //abstract class
  public:
    virtual std::string Classify(Document* doc, const TrainingSet& ts, bool overwrite) = 0;
    virtual ~Classifier() { }
};
#endif


class KNNClassifier : public Classifier {

  private:
    std::multimap<double, std::string, std::greater<double> > score2name;
    int k_nearest = 5; //default value

  public:
    inline KNNClassifier() { }
    inline KNNClassifier(int k) { k_nearest = k; }

    inline void SetK(int k) { k_nearest = k; }
    //doc_tr --> Training set document
    double DocCosSimilarity(const Document* doc_in, const Document* doc_tr);

    virtual std::string Classify(Document* doc, const TrainingSet& ts, bool overwrite = false) {
        score2name.clear();
        if (doc->GetAssignation() != "" && 
          doc->GetTypes()->find(doc->GetAssignation()) != doc->GetTypes()->end()) {
            return doc->GetAssignation();
        }

        { //evaluate cos-similarity by assigning a score from 0 to 1 (key) to every file
          //from TrainingSet (value)
        auto iter = ts.GetTrainingSet().begin();
        for (; iter != ts.GetTrainingSet().end(); iter++) {
            if (*iter == nullptr || doc == nullptr) continue;
            //score2name.insert(std::make_pair(DocCosSimilarity(&doc, *iter), (*iter)->GetAssignation()));
            double output = DocCosSimilarity(doc, *iter);//100000
            score2name.insert(std::make_pair(output, (*iter)->GetAssignation()));
            std::cout << "Score for: " << (*iter)->GetFileName() << 
              " (" << (*iter)->GetAssignation() << ") is: " << output << std::endl;
        }
        }

        std::multimap<int, std::pair<std::string, std::deque<double>>, std::greater<int> > out;
        { 
            ////assigns keys:classifications values:#occurrences,scores
            std::map<std::string, std::pair<int, std::deque<double> > > results;
            { //Read score2name and evaluate the first k elements. 
            auto iter = score2name.begin();
            for (int i = 0; iter != score2name.end() && i < k_nearest; i++, iter++) {
                results[iter->second].first++;
                results[iter->second].second.push_back((iter->first));
            }
            }

            { //sorts results by number of classifications using multimap "out"
            auto iter = results.begin();
            for (; iter != results.end(); iter++) {
                out.insert(std::make_pair(iter->second.first, std::make_pair(iter->first, iter->second.second)));
            }
            }
        } //frees memory used by "results"

        //classification is chosen as the definition with the highest number of occurrences
        //in case of equal # occurrences, definition with highest score is chosen

        auto iter1 = out.begin();

        int best_score = iter1->first;
        std::deque<double> max = iter1->second.second;
        auto itermax = iter1;
        iter1++;

        for (; iter1 != out.end() && iter1->first == best_score; iter1++) {
            if (iter1->second.second > max) {
                max = iter1->second.second;
                itermax = iter1;
            }
        }

        if (overwrite) { doc->SetAssignation(itermax->second.first); }

        return ("Classification: " + itermax->second.first);
    }
    
    virtual ~KNNClassifier() { }
};
#endif
