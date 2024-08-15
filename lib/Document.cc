#include "Document.h"

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