#include <string>
#include <vector>
#include <deque>  
#include <iostream> 
#include <filesystem> //reading files from training set folder
#include "lib/Document.h"
#include "lib/TrainingSet.h"
#include "lib/KNNClassifier.h"

/*
###############################################
################PROGRAM USAGE##################
###############################################

- File(s) to be used for the training set must be placed in the dedicated folder, as shown below

TRAINING SET: (filepath: ./TrainingSet)

////filename: doc1.txt
sport   ///classification
Lorem ipsum dolor....

////filename: doc2.txt
languages   ///classification
Sator arepo opera tenet rotas...

etc...

- File(s) to be analyzed must be passed as parameter when executing the program

*/

/* 
CosSimilarity(A, A) { //LATEX
    \frac{\sum_{i=1}^n A_i \cdot B_i}{sqrt{\sum_{i = 1}^n A_i^2} \cdot sqrt{\sum_{i = 1}^n B_i^2}}
}
*/

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
        std::cout << std::endl;
        if (*iter != nullptr) {
            std::cout << (*iter)->GetFileName() << ": "  << std::endl << knn.Classify(*iter, ts) << std::endl;
            delete *iter; //frees memory used by documents
        } else {
            std::cerr << "Warning: skipping nullptr element" << std::endl;
        }
    }

    delete Document::GetTypes(); //frees memory from static data allocation
    
    return 0;
} 