#include <string>
#include <set>
#include <map>
#include <vector>
#include <deque>  
#include <fstream>  //reading from files
#include <iostream> 
#include <cmath>  //operations for cos-similarity
#include <filesystem> //reading files from training set folder

/*
###############################################
################PROGRAM USAGE##################
###############################################

- File(s) to be used for the training set must be placed in the dedicated folder, as shown below

TRAINING SET: (filepath: ./TrainingSet)
////filename: doc1
sport   ///classification
Lorem ipsum dolor....

////filename: doc2
languages   ///classification
Sator arepo opera tenet rotas...

etc...

- File(s) to be analyzed must be passed as parameter when executing the program

*/

class Document {
  private:
    std::string name;
    static std::set<std::string>* doctypes; //stores document classification types
    std::string assignation = ""; //document classification. DEFAULT: empty string
    std::map<std::string, unsigned int> word2occur;

    //design pattern singleton for "doctypes"
    inline static std::set<std::string>* GetDocTypes() { //writing access reserved for factory
        if (doctypes == nullptr) doctypes = new std::set<std::string>;
        return doctypes;
    }

    friend std::ostream& operator<<(std::ostream& out, const Document& doc);

  public:
    Document(const Document& to_copy) { }

    static Document* BuildDoc(const std::string& filename, bool is_training = false); //factory method

    inline Document(const std::string& def) { assignation = def; }

    static const inline std::set<std::string>* GetTypes() { return doctypes; }

    const inline std::string& GetFileName() const { return name; }

    const inline std::map<std::string, unsigned int>& GetWord2Occur() const { return word2occur; }

    const inline std::string& GetAssignation() const { return assignation; }

    inline void SetAssignation(const std::string& def) { assignation = def; }
    
    ~Document() { }
};

std::set<std::string>* Document::doctypes = nullptr;

std::ostream& operator<<(std::ostream& out, const Document& doc) {
    out << doc.assignation << std::endl;
    auto iter = doc.word2occur.begin();
    for (; iter != doc.word2occur.end(); iter++) {
        out << iter->first << ": " << iter->second << std::endl;
    }

    return out;
}

Document* Document::BuildDoc(const std::string& filename, bool is_training) {
    std::ifstream is(filename);
    if (!is.good()) {
        std::cerr << "Error in read process of file: " << filename
          << "! Proceeding with read and returning nullptr" << std::endl;
          return nullptr;
    } else {
        std::string type = "";
        if (is_training) {
            std::getline(is, type);
            std::set<std::string>::const_iterator iter_search = GetDocTypes()->find(type);
            if (iter_search == GetDocTypes()->end()) GetDocTypes()->insert(type);
        }
        Document* doc = new Document(type);
        doc->name = filename;
        std::string temp;
        while (is >> temp) {
            if (temp.length() >= 3) {
                doc->word2occur[temp]++;
            }
        }
        return doc;
    }
}



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

class Classifier { //abstract class
  public:
    virtual std::string Classify(Document* doc, TrainingSet& ts, bool overwrite) = 0;
    virtual ~Classifier() { }
};


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

    virtual std::string Classify(Document* doc, TrainingSet& ts, bool overwrite = false) {
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
            double output = DocCosSimilarity(doc, *iter);
            score2name.insert(std::make_pair(output, (*iter)->GetAssignation()));
            std::cout << "Score for: " << (*iter)->GetFileName() << 
              " (" << (*iter)->GetAssignation() << ") is: " << output << std::endl;
        }
        }

        std::multimap<int, std::pair<std::string, std::deque<double>>, std::greater<int> > out;
        { 
            //assigns keys:classifications values:#occurrences,scores
            std::map<std::string, std::pair<int, std::deque<double> > > results;
            { //Read score2name and evaluate the first k elements. 
            auto iter = score2name.begin();
            for (int i = 0; iter != score2name.end() && i < k_nearest; i++, iter++) {
                results[iter->second].first++;
                results[iter->second].second.push_back(iter->first);
            }
            }

            { //sorts results by number of classifications using multimap "out"
            auto iter = results.begin();
            for (; iter != results.end(); iter++) {
                out.insert(std::make_pair(iter->second.first, std::make_pair(iter->first, iter->second.second)));
                //std::cout << iter->second.first << "  " << iter->first << std::endl;
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

/* 
CosSimilarity(A, B) {  //LATEX
    \frac{\sum_{i=1}^n A_i \cdot B_i}{sqrt{\sum_{i = 1}^n A_i^2} \cdot sqrt{\sum_{i = 1}^n B_i^2}}
}
*/

double KNNClassifier::DocCosSimilarity(const Document* doc_in, const Document* doc_tr) {
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

int main(int argc, char** argv) { //args: files_to_analyze

    std::deque<std::string> ts_filenames;
    
    {
    const std::filesystem::path ts_path = "./TrainingSet"; //C++17: access to TrainingSet directory
    for (const auto& path_to_file: std::filesystem::directory_iterator(ts_path)) {
        ts_filenames.push_back(path_to_file.path());
    }
    }

    if (ts_filenames.empty()) {
        std::cerr << "Error: no files in training set!" << std::endl;
        return 1;
    } 

    TrainingSet ts(ts_filenames);

    std::deque<std::string> analyze_filenames;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            analyze_filenames.push_back(argv[i]);
        }
    } else {
        std::cerr << "Error: no files to be analyzed!" << std::endl;
        return 2;
    }

    std::vector<Document*> files_to_be_analyzed;
    {
    auto iter = analyze_filenames.begin();
    files_to_be_analyzed.reserve(analyze_filenames.size());
    for (; iter != analyze_filenames.end(); iter++) {
        Document* d = Document::BuildDoc(*iter);
        files_to_be_analyzed.push_back(d);
    }
    }

    KNNClassifier knn;
    knn.SetK(4);

    std::cout << "RESULTS: " << std::endl;
    auto iter = files_to_be_analyzed.begin();
    for (; iter != files_to_be_analyzed.end(); iter++) {
        if (*iter != nullptr) {
            std::cout << (*iter)->GetFileName() << ": "  << std::endl << knn.Classify(*iter, ts, true) << std::endl;
            delete *iter; //frees memory used by documents
        } else {
            std::cerr << "Warning: skipping nullptr element" << std::endl;
        }
    }

    delete Document::GetTypes(); //frees memory from static allocation

    return 0;
} 