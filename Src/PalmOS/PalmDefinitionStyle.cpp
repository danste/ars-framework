#include <DefinitionStyle.hpp>
#include <Graphics.hpp>
#include <Debug.hpp>
#include <Text.hpp>

struct StaticStyleEntry
{
    const char* name;
    const DefinitionStyle style;
    
    // For use by binary search (std::lower_bound())
    bool operator<(const StaticStyleEntry& other) const
    {
        return tstrcmp(name, other.name) < 0;
    }
    
};

#define COLOR_NOT_DEF  {-1,0,0,0}
#define RGB(r,g,b)  {0,r,g,b}
#define WHITE    RGB(255,255,255)
#define BLACK    RGB(0,0,0)
#define GREEN    RGB(0,196,0)
#define BLUE     RGB(0,0,255)
#define RED      RGB(255,0,0)
#define YELLOW   RGB(255,255,0)
#define GRAY     RGB(127,127,127)

#define FONT_NOT_DEF  FontID(-1)
#define NOT_DEF          DefinitionStyle::notDefined
#define TRUE            DefinitionStyle::yes
#define FALSE           DefinitionStyle::no
#define UNDERLINE_NOT_DEF UnderlineModeTag(-1)

#define COLOR(name, color) \
    {(name), {color, COLOR_NOT_DEF, FONT_NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, UNDERLINE_NOT_DEF}}
#define COLOR_BOLD(name, color) \
    {(name), {color, COLOR_NOT_DEF, FONT_NOT_DEF, TRUE, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, UNDERLINE_NOT_DEF}}
#define COLOR_AND_FONT(name, color, font) \
    {(name), {color, COLOR_NOT_DEF, font, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, UNDERLINE_NOT_DEF}}
#define COLOR_AND_FONT_BOLD(name, color, font) \
    {(name), {color, COLOR_NOT_DEF, font, TRUE, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, UNDERLINE_NOT_DEF}}

static const StaticStyleEntry staticStyleTable[] =
{
    //TODO: add more and more:)
    COLOR("black",BLACK),
    COLOR("blue",BLUE),
    COLOR_BOLD("bold",COLOR_NOT_DEF),
    COLOR("gray",GRAY),
    COLOR("green",GREEN),
    {"hyperlink", {BLUE, COLOR_NOT_DEF, FONT_NOT_DEF, NOT_DEF, NOT_DEF, NOT_DEF, TRUE, NOT_DEF, NOT_DEF, grayUnderline}},
    COLOR_AND_FONT("large", COLOR_NOT_DEF, largeFont),
    COLOR("red",RED)
};



uint_t getStaticStyleCount()
{
    return ARRAY_SIZE(staticStyleTable);
}

const char* getStaticStyleName(uint_t index)
{
    assert(index < ARRAY_SIZE(staticStyleTable));
    return staticStyleTable[index].name;
}

const DefinitionStyle* getStaticStyle(uint_t index)
{
    assert(index < ARRAY_SIZE(staticStyleTable));
    return &staticStyleTable[index].style;
}

const DefinitionStyle* getStaticStyle(const char* name, uint_t length)
{
    if (NULL == name || 0 == length)
        return NULL;
    char* nameBuf = StringCopy2N(name, length);
    if (NULL == nameBuf)
        return NULL;
    
    StaticStyleEntry entry = COLOR(nameBuf, COLOR_NOT_DEF);
    const StaticStyleEntry* end = staticStyleTable + ARRAY_SIZE(staticStyleTable);
    const StaticStyleEntry* res = std::lower_bound(staticStyleTable, end, entry);

    free(nameBuf);

    if (end == res)
        return NULL;
    return &res->style; 
}

DefinitionStyle* parseStyle(const char* style, ulong_t length)
{
    //TODO: 



    return new DefinitionStyle();
}

void DefinitionStyle::reset()
{
    foregroundColor = (RGBColorType) COLOR_NOT_DEF;
    backgroundColor = (RGBColorType) COLOR_NOT_DEF;
    fontId = FONT_NOT_DEF;
    bold = NOT_DEF;
    italic = NOT_DEF;
    superscript = NOT_DEF;
    subscript = NOT_DEF;
    small = NOT_DEF;
    strike = NOT_DEF;
    underline = UNDERLINE_NOT_DEF;
}

#ifdef DEBUG
void test_StaticStyleTable()
{
    // Validate that fields_ are sorted.
    for (uint_t i = 0; i < ARRAY_SIZE(staticStyleTable); ++i)
    {
        if (i > 0 && staticStyleTable[i] < staticStyleTable[i-1])
        {
            const char_t* prevName = staticStyleTable[i-1].name;
            const char_t* nextName = staticStyleTable[i].name;
            assert(false);
        }
    }
}
#endif