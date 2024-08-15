#include <set>
#include <string>
#include <map>
#include <iostream>
#include <fstream>


#ifndef DOCUMENT_H
#define DOCUMENT_H
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
};

#endif
