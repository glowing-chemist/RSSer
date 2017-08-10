//
// Created by ollie on 07/05/17.
//

#ifndef RSSHUNGER_EXTRACTOR_H
#define RSSHUNGER_EXTRACTOR_H

#include <string>

//tinyXML parsing library DOM based
#define TIXML_USE_STL
#include <tinyxml.h>



template<int N> // how many links to fetch starting at most resent
class XMLDecoder {
private:
    std::string& XMLDoc;

public:
    XMLDecoder(std::string&);

    const std::string parse() const;
};

template<int N>
XMLDecoder<N>::XMLDecoder(std::string& doc) : XMLDoc{doc} {}

template<int N>
const std::string XMLDecoder<N>::parse() const {
    TiXmlDocument docP;
    docP.Parse(XMLDoc.c_str(), 0, TIXML_ENCODING_UTF8);
    TiXmlElement* root = docP.RootElement();
    TiXmlElement* itemNode = root -> FirstChild("item") -> ToElement();
    const char* val = itemNode -> FirstChild("link") -> ToElement() -> GetText();
    if(val == nullptr) return "";
    return val;
}

#endif //RSSHUNGER_EXTRACTOR_H
