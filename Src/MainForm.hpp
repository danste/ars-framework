#ifndef __MAINFORM_HPP__
#define __MAINFORM_HPP__

#include "iPediaForm.hpp"

class Definition;
class LookupHistory;
class RenderingPreferences;

class MainForm: public iPediaForm
{
    Definition* getDefinition();
    const LookupHistory* getLookupHistory() const;
    const RenderingPreferences& renderingPreferences() const
    {return static_cast<const iPediaApplication&>(application()).renderingPreferences();}
    
    void handleScrollRepeat(const EventType& data);
    void handlePenUp(const EventType& event);
    void handleControlSelect(const EventType& data);
    bool handleKeyPress(const EventType& event);
    
    void drawSplashScreen(ArsLexis::Graphics& graphics, const ArsLexis::Rectangle& bounds);
    void drawDefinition(ArsLexis::Graphics& graphics, const ArsLexis::Rectangle& bounds);
    
    void updateScrollBar(const Definition& def);
 
    void randomArticle();
    void copySelectionToClipboard();

    void updateAfterLookup();
    
    void moveHistory(bool forward);
    void switchServer(const char* server);
    
    void setControlsState(bool enabled);
    
protected:

    void resize(const ArsLexis::Rectangle& screenBounds);
    
    void draw(UInt16 updateCode=frmRedrawUpdateCode);
    
    bool handleWindowEnter(const struct _WinEnterEventType& data);
    
    bool handleEvent(EventType& event);
    
    bool handleMenuCommand(UInt16 itemId);

public:
    
    MainForm(iPediaApplication& app);
    
    ~MainForm();

    enum DisplayMode
    {
        showSplashScreen,
        showDefinition
    };
    
    DisplayMode displayMode() const
    {return displayMode_;}

    void setDisplayMode(DisplayMode displayMode)
    {displayMode_=displayMode;}
    
    enum ScrollUnit
    {
        scrollLine,
        scrollPage
    };
    
    void scrollDefinition(int units, ScrollUnit unit);
    
private:
    
    DisplayMode displayMode_;
    
};

#endif
