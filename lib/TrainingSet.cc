#include "TrainingSet.h"

std::ostream& operator<<(std::ostream& out, const TrainingSet& ts) {
    auto iter = ts.doc_set.begin();
    for(; iter != ts.doc_set.end(); iter++) {
        out << (*iter)->GetFileName() << ":" << std::endl;
        out << "\t" << (*iter)->GetAssignation() << std::endl;
        auto iter1 = (*iter)->GetWord2Occur().begin();
        for(; iter1 != (*iter)->GetWord2Occur().end(); iter1++) {
            out << "\t" << iter1->first << ": " << iter1->second << std::endl; 
        } 
    }

    return out;
}

TrainingSet::TrainingSet(const std::deque<std::string>& filenames) {
    auto iter = filenames.begin();
    doc_set.reserve(filenames.size());
    for (; iter != filenames.end(); iter++) {
        doc_set.push_back(Document::BuildDoc(*iter, true));
    }
}

TrainingSet::~TrainingSet() {
    auto iter = doc_set.begin();
    for (; iter != doc_set.end(); iter++) {
        if (*iter != nullptr) {
            delete *iter;
        }
    }
}
