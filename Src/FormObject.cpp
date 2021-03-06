#include <68k/Hs.h>

#include <DeviceInfo.hpp>
#include <SysUtils.hpp>

#include <Graphics.hpp>

#include <FormObject.hpp>
#include <FormGadget.hpp>
#include <Application.hpp>

FormObject::FormObject(Form& form, UInt16 id):
    form_(&form),
    id_(frmInvalidObjectId),
    index_(frmInvalidObjectId),
    object_(0)
{
    if (id!=frmInvalidObjectId)
        attach(id);
}

FormObject::~FormObject()
{}

void FormObject::attach(UInt16 id)
{
    assert(frmInvalidObjectId != id);
    index_ = FrmGetObjectIndex(*form_, id_ = id);
    assert(frmInvalidObjectId != index_);
    object_ = FrmGetObjectPtr(*form_, index_);
    assert(NULL != object_);
}

void FormObject::detach()
{   
    index_ = frmInvalidObjectId;
    id_ = frmInvalidObjectId;
    object_ = NULL;
}

void FormObject::bounds(Rect& out) const
{
    RectangleType rect;
    FrmGetObjectBounds(*form_, index(), &rect);
    out=rect;
}

void FormObject::setBounds(const Rect& bounds)
{
    assert(valid());
    FrmSetObjectBounds(*form_, index(), &bounds.native());
}

void FormObject::attachByIndex(UInt16 index)
{
    assert(frmInvalidObjectId != index);
    id_ = FrmGetObjectId(*form_, index_ = index);
    object_ = FrmGetObjectPtr(*form_, index_);
    assert(0 != object_);
}

void FormObject::anchor(const Rect& boundingBox, FormObjectAnchorStyle horizAnchor, Coord rightMargin, FormObjectAnchorStyle vertAnchor, Coord bottomMargin)
{
    RectangleType rect;
    FrmGetObjectBounds(*form_, index(), &rect);
    
    if (anchorLeftEdge == horizAnchor) 
        rect.topLeft.x = boundingBox.width() - rightMargin;
    else if (anchorRightEdge == horizAnchor)
        rect.extent.x = boundingBox.width() - rightMargin;
    
    if (anchorTopEdge == vertAnchor)
        rect.topLeft.y = boundingBox.height() - bottomMargin;
    else if (anchorBottomEdge == vertAnchor)
        rect.extent.y = boundingBox.height() - bottomMargin;
        
    FrmSetObjectBounds(*form_, index(), &rect);
}

void FormObject::hide()
{
    FrmHideObject(*form_, index_);
    if (frmGadgetObj == type())
    {
        FormGadget* gadget = static_cast<FormGadget*>(FrmGetGadgetData(*form_, index_));
        assert(NULL != gadget);
        gadget->usable_ = false;
        gadget->visible_ = false;
        gadget->notifyHide();      
    }
}

void FormObject::show()
{
    if (frmGadgetObj == type())
    {
        FormGadget* gadget = static_cast<FormGadget*>(FrmGetGadgetData(*form_, index_));
        assert(NULL != gadget);
        gadget->usable_ = true;
        gadget->visible_ = true;            
        gadget->notifyShow();
    }
    FrmShowObject(*form_, index_);
}

void FormObject::setVisible(bool value)
{
    if (value)
        show();
    else
        hide();
}


bool FormObject::hasFocus() const
{
    return index_ == form()->focusedControlIndex_;
}
    
void FormObject::focus()
{
    assert(valid());
    if (form_->focusedControlIndex_ != index_)
        form_->releaseFocus();          // To make sure event focus lost is sent before event focus taken on Treo 600
    FormObjectKind kind = type();
    bool isTreo = form_->application().runningOnTreo600();
    bool isPastCobalt61 = (romVersionMajor() > 6 || (romVersionMajor() == 6 && romVersionMinor() >= 1));
    if (isTreo && frmInvalidObjectId != id_)
        HsNavObjectTakeFocus(*form_, id_);
    else if (!isTreo && (frmFieldObj == kind || frmTableObj == kind))
        FrmSetFocus(*form_, index_);
    else if (isPastCobalt61 && frmInvalidObjectId != id_)
        FrmSetFocus(*form_, index_);            
    form_->focusedControlIndex_ = index_;
}

void FormObject::draw()
{
    assert(valid());
    FormObjectKind kind = type();
    switch (kind)
    {
        case frmFieldObj:
            FldDrawField(static_cast<FieldType*>(object_));
            break;
            
        case frmControlObj:
            CtlDrawControl(static_cast<ControlType*>(object_));
            break;
            
        case frmListObj:
            LstDrawList(static_cast<ListType*>(object_));
            break;
            
        case frmTableObj:
            TblDrawTable(static_cast<TableType*>(object_));
            break;
            
        case frmGadgetObj: {
            FormGadget* gadget = static_cast<FormGadget*>(FrmGetGadgetData(*form_, index_));
            assert(NULL != gadget);
            gadget->drawProxy();
            break;
        }
        
        case frmScrollBarObj:
            SclDrawScrollBar(static_cast<ScrollBarType*>(object_));
            break;

        default: // There's no other way to redraw other components 
            form_->draw();
    }        
}

void FormObject::setBounds(uint_t x, uint_t y, uint_t width, uint_t height)
{
    RectangleType rect = {{x, y}, {width, height}};
    FrmSetObjectBounds(*form_, index_, &rect);
}

void FormObject::setTopLeft(uint_t x, uint_t y)
{
    RectangleType rect;
    FrmGetObjectBounds(*form_, index_, &rect);
    rect.topLeft.x = x;
    rect.topLeft.y = y;
    FrmSetObjectBounds(*form_, index_, &rect);
}

#pragma mark -

ScrollBar::~ScrollBar()
{}

Field::~Field()
{}

Control::~Control()
{}

List::~List()
{}

void Field::replace(const char* text, uint_t length)
{
    erase();
    insert(text, std::min(maxLength(), length));
}

void Field::setText(MemHandle handle)
{
    MemHandle oldHandle = FldGetTextHandle(object());
    FldSetTextHandle(object(), handle);
    if (NULL != oldHandle)
        MemHandleFree(oldHandle);
}

status_t Field::setEditableText(const char* data, uint_t length)
{
    MemHandle handle=MemHandleNew(length+1);
    if (NULL==handle)
        return memErrNotEnoughSpace;
    char* text=static_cast<char*>(MemHandleLock(handle));
    if (NULL==text)
        return memErrChunkNotLocked;
    StrNCopy(text, data, length);
    text[length]=chrNull;
    MemHandleUnlock(handle);
    setText(handle);
    return errNone;
}

void Field::setReadOnly(bool val)
{
    FieldAttrType attr;
    FldGetAttributes(object(), &attr);
    attr.editable = !val;
    FldSetAttributes(object(), &attr);
}

#pragma mark -
#pragma mark List


// At any given time an element of the list is selected. This function
// will set a new item selected of the pos curSelected + delta. delta
// can be negative. If asked to set selection to a inexistent item (<0 or
// >number of items) sets selection to the first/last item (this is so that
// the caller doesn't have to worry about this stuff)
void List::setSelectionDelta(int delta)
{
    if (0==delta || 0==itemsCount())
        return;
    // Why? If some calculations lead to delta==0, let it be.
    // assert(0!=delta);
    int sel=selection();
    if (noListSelection==sel)
        sel = 0;
    sel+=delta;
    sel=std::max(0, std::min<int>(sel, itemsCount()-1));
    setSelection(sel);
    makeItemVisible(sel);
}

List::CustomDrawHandler::CustomDrawHandler():
    form_(0),
    listId_(frmInvalidObjectId)
#ifdef __MWERKS__        
    , drawCallbackThunk_(drawCallback)
#endif
{}

void List::updateItemsCount(const CustomDrawHandler& handler)
{
    LstSetListChoices(object(), reinterpret_cast<Char**>(const_cast<CustomDrawHandler*>(&handler)), handler.itemsCount());
}

void List::setCustomDrawHandler(List::CustomDrawHandler* handler)
{
    if (handler)
    {
        handler->form_=form();
        handler->listId_=id();
#ifdef __MWERKS__        
        LstSetDrawFunction(object(), reinterpret_cast<ListDrawDataFuncPtr>(&handler->drawCallbackThunk_.thunkData));
#else
        LstSetDrawFunction(object(), CustomDrawHandler::drawCallback);
#endif
        updateItemsCount(*handler);
    }
    else
    {
        LstSetDrawFunction(object(), 0);
        LstSetListChoices(object(), 0, 0);
    }
}

void List::CustomDrawHandler::drawCallback(Int16 itemNum, RectangleType* rect, Char** itemsText)
{
    CustomDrawHandler* handler=reinterpret_cast<CustomDrawHandler*>(itemsText);
    assert(0!=handler);
    Rect bounds(*rect);
    List list(*handler->form_, handler->listId_);
    Graphics graphics(handler->form_->windowHandle());
    handler->drawItem(graphics, list, itemNum, bounds);
}

void List::adjustVisibleItems() 
{
    int total=itemsCount();
    if (0==total) 
        return;
    int top=topItem();
    int visible=visibleItemsCount();
    if (total-top<visible)
        top=std::max(0, total-visible);
    setTopItem(top);
}

List::CustomDrawHandler::~CustomDrawHandler()
{}


bool List::handleKeyDownEvent(const EventType& event, uint_t options)
{
    assert(valid());
    Form& form=*this->form();
    assert(keyDownEvent==event.eType);
    bool handled=false;
    int delta=0;
    int page=visibleItemsCount();
    if (form.fiveWayUpPressed(&event))
        delta=-1;
    else if (form.fiveWayDownPressed(&event))
        delta=1;
    else if ((optionScrollPagesWithLeftRight & options) && form.fiveWayLeftPressed(&event))
        delta=-page;
    else if ((optionScrollPagesWithLeftRight & options) && form.fiveWayRightPressed(&event))
        delta=page;
    else if ((optionFireListSelectOnCenter & options) && form.fiveWayCenterPressed(&event))
    {
        EventType e;
        MemSet(&e, sizeof(e), 0);
        if (noListSelection!=(e.data.lstSelect.selection=selection()))
        {
            e.eType=lstSelectEvent;
            e.data.lstSelect.listID=id();
            e.data.lstSelect.pList=object();
            EvtAddEventToQueue(&e);
            handled = true;
        }
    }
    else {
        switch (event.data.keyDown.chr)
        {
            case chrPageDown:
                delta=page;
                break;
                
            case chrPageUp:
                delta=-page;
                break;
            
            case chrDownArrow:
                delta=1;
                break;

            case chrUpArrow:
                delta=-1;
                break;
                
            case chrLeftArrow:
                if (optionScrollPagesWithLeftRight & options)
                    delta=-page;
                break;
                
            case chrRightArrow:
                if (optionScrollPagesWithLeftRight & options)
                    delta=page;
                break;
        }
    }
    if (0!=delta)
    {
        setSelectionDelta(delta);
        handled=true;
    }
    return handled;
}

