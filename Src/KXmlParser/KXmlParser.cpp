/*
  Implementation of XmlPull XML parsing API. It's a port of Java's kXML 2
  (http://kxml.org, http://kobjects.dyndns.org/kobjects/auto?self=%23c0a80001000000f5ad6a6fb3)
*/

#include "KxmlParser.hpp"

using namespace KXml2;

KXmlParser::KXmlParser()
{
    fRelaxed_ = false;    
    srcBuf = new (char_t[128]);
}

void KXmlParser::setInput(XmlReader *reader)
{
    reader_ = reader;

    line = 1;
    column = 0;
    type = START_DOCUMENT;
    name = "";
    nameSpace = "";
    degenerated = false;
    attributeCount = -1;
    encoding = "";
    version_ = "";
    standalone_ = "";

    if (reader == NULL)
        return;
 
    srcPos = 0;
    srcCount = 0;
    peekCount = 0;
    depth_ = 0;

    if (entityMap_)
        delete entityMap_

    entityMap_ = new ArsLexis::Hashtable();
    entityMap_->put("amp", "&");
    entityMap_->put("apos", "'");
    entityMap_->put("gt", ">");
    entityMap_->put("lt", "<");
    entityMap_->put("quot", "\"");
}

void KXmlParser::setFeature(String feature, bool flag)
{
    if (feature==FEATURE_RELAXED)
    {
        fRelaxed_ = flag;
    }
    else
    {
        // TODO: throw an exception? do nothing?
    }
}

void KXmlParser::nextToken()
{
}

void KXmlParser::next()
{
}

String KXmlParser::getPositionDescription()
{
    return "";
}

int KXmlParser::getEventType()
{
    return END_DOCUMENT;
}
