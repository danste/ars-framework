#ifndef __KXML2_XML_PULL_HPP__
#define __KXML2_XML_PULL_HPP__

#include <vector>
#include "XmlReader.hpp"
#include "KXml2.hpp"

// Defines a public, abstract interface for pull XML parsing
// This api is based on Java's XmlPull API (http://www.xmlpull.org/)
namespace KXml2{
    class XmlPullParser
    {
        public:
            static const int START_DOCUMENT = 0;
            static const int START_TAG = 2;
            static const int TEXT = 4;
            static const int END_TAG = 3;
            static const int END_DOCUMENT = 1;

            static const int ENTITY_REF = 6;
            static const int IGNORABLE_WHITESPACE = 7;

            static const int COMMENT = 9;
            static const int CDSECT = 5;
            static const int DOCDECL = 10;
            static const int PROCESSING_INSTRUCTION = 8;
                                      
            static const String TYPES[];

            static String FEATURE_PROCESS_NAMESPACES;

            virtual error_t setInput(XmlReader *reader) = 0;
            virtual error_t setFeature(String feature, bool flag) = 0;
            virtual error_t nextToken(int& ret) =0;
            virtual error_t next(int& ret) = 0;
            virtual error_t getPositionDescription(String& ret) = 0;
            virtual int     getEventType() = 0;
            virtual void    defineEntityReplacementText(String entity, String value) = 0;
    };
}

#endif
