#ifndef __BULLET_ELEMENT_HPP__
#define __BULLET_ELEMENT_HPP__

#include "GenericTextElement.hpp"

class BulletElement: public GenericTextElement
{
    uint_t childIndentation_;
    
protected:

    void applyFormatting(ArsLexis::Graphics& graphics, const RenderingPreferences& prefs);
    
    uint_t childIndentation() const
    {return childIndentation_;}

public:
    
    
    BulletElement();

    bool requiresNewLine(const RenderingPreferences& preferences) const
    {return true;}
    
    void calculateLayout(LayoutContext& mc);
    
    void toText(ArsLexis::String& appendTo, uint_t from, uint_t to) const
    {
        appendTo.append("\n* ");
    }
    

};

#endif