#include <vector>
#include <deque>
#include "Document.h"

#ifndef TRAININGSET_H
#define TRAININGSET_H
class TrainingSet {
  private:
    std::vector<Document*> doc_set;

    friend std::ostream& operator<<(std::ostream& out, const TrainingSet& ts);

  public:
    inline const std::vector<Document*>& GetTrainingSet() const { return doc_set; }

    TrainingSet(const std::deque<std::string>& filenames);
    TrainingSet(const TrainingSet& to_copy) { }

    ~TrainingSet();
};
#endif
