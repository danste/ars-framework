#ifndef ARSLEXIS_TEXT_RENDERER_HPP__
#define  ARSLEXIS_TEXT_RENDERER_HPP__

#include <FormGadget.hpp>
#include <Definition.hpp>

namespace ArsLexis 
{
    class TextRenderer: public FormGadget 
    {
        Definition definition_;
        RenderingPreferences& renderingPreferences_;
        ScrollBar* scrollBar_;
        WinHandle drawingWindow_;
        bool drawingWindowIsOffscreen_;
        Rectangle drawingWindowBounds_;
        Err lastRenderingError_;
        
        void checkDrawingWindow();
        
        void updateForm(Graphics& graphics);
        
        void doUpdateScrollbar();
        
        bool handleMouseEvent(const EventType& event);
        
    public:
 
        TextRenderer(Form& form, RenderingPreferences& prefs, ScrollBar* scrollBar = NULL);

        ~TextRenderer();
        
        typedef Definition::const_iterator const_iterator;
        typedef Definition::iterator iterator;

        iterator  begin() {return definition_.begin();}
        const_iterator begin() const {return definition_.begin();}
        iterator end() {return definition_.end();}
        const_iterator end() const {return definition_.end();}
        bool empty() const {return definition_.empty();}

        typedef Definition::Elements_t Elements_t;
        void replaceElements(Elements_t& elements) {definition_.replaceElements(elements);}

        uint_t linesCount() const {return definition_.totalLinesCount();}

        uint_t visibleLinesCount() const {return definition_.shownLinesCount();}

        uint_t topLine() const {return definition_.firstShownLine();}

        void clear() {definition_.clear();}

        typedef Definition::HyperlinkHandler HyperlinkHandler;
        HyperlinkHandler* hyperlinkHandler() {return definition_.hyperlinkHandler();}

        void setHyperlinkHandler(HyperlinkHandler* handler) {definition_.setHyperlinkHandler(handler);}

        void selectionToText(ArsLexis::String& out) const {definition_.selectionToText(out);}

        typedef Definition::RenderingProgressReporter RenderingProgressReporter;
        void setRenderingProgressReporter(RenderingProgressReporter* reporter) {definition_.setRenderingProgressReporter(reporter);}
        
        class RenderingErrorListener {
        public:
        
            virtual void handleRenderingError(TextRenderer& renderer, Err error) = 0;
            
            virtual ~RenderingErrorListener();
            
        };
        
        void setRenderingErrorListener(RenderingErrorListener* listener) {renderingErrorListener_ = listener;}
        
        enum ScrollbarUpdateOption {
            updateScrollbarNot,
            updateScrollbar
        };
        
        void scroll(WinDirectionType direction, uint_t items, ScrollbarUpdateOption update = updateScrollbar);
        
        void draw() {drawProxy();}
        
        bool copySelection() const;
        
        enum InteractionBehaviorOption {
            behavDoubleClickSelection = Definition::behavDoubleClickSelection,
            behavMouseSelection = Definition::behavMouseSelection,
            behavUpDownScroll = Definition::behavUpDownScroll,
            behavHyperlinkNavigation = Definition::behavHyperlinkNavigation,
            behavMenuBarCopyButton = behavHyperlinkNavigation << 1,
        };
        
        void setInteractionBehavior(uint_t ib) {definition_.setInteractionBehavior(ib);}
        
        uint_t interactionBehavior() const {return definition_.interactionBehavior();}
        
        bool usesInteractionBehavior(InteractionBehaviorOption ibo) const {return 0 != (definition_.interactionBehavior() & ibo);}

        bool usesDoubleClickSelection() const { return usesInteractionBehavior(behavDoubleClickSelection);}

        bool usesMouseSelection() const { return usesInteractionBehavior(behavMouseSelection);}

        bool usesUpDownScroll() const { return usesInteractionBehavior(behavUpDownScroll);}

        bool usesHyperlinkNavigation() const { return usesInteractionBehavior(behavHyperlinkNavigation);}
        
        bool usesMenuBarCopyButton() const {return usesInteractionBehavior(behavMenuBarCopyButton);}
        
        bool handleEventInForm(EventType& event);
                
    protected:
    
        void drawFocusRing();
    
        void removeFocusRing();

        void drawProxy();

        void handleDraw(Graphics& graphics);
        
        bool handleEvent(EventType& event);
        
        bool handleEnter(const EventType& event);

        void notifyHide();
       
    private:

        bool handleScrollRepeat(const EventType& event);
        
        bool handleKeyDownEvent(const EventType& event);
    
        bool handleMenuCmdBarOpen(EventType& event);

        bool handleMenuEvent(const EventType& event);

        void disposeOffscreenWindow();
        
        RenderingErrorListener* renderingErrorListener_;
	
        UInt32 scheduledNilEventTicks_;
        
        enum ScheduledScrollDirection {
            scheduledScrollAbandoned,
            scheduledScrollUp,
            scheduledScrollDown
        };
        
        ScheduledScrollDirection scheduledScrollDirection_;
        
        void handleNilEvent();
        
    };
    
}

#endif