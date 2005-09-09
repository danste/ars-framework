#include <WindowsCE/Controls.hpp>

static BOOL InitCC(DWORD control)
{
    INITCOMMONCONTROLSEX icc;
    ZeroMemory(&icc, sizeof(icc));
    icc.dwSize = sizeof(icc);
    icc.dwICC = control;   
    return InitCommonControlsEx(&icc); 
}

ScrollBar::ScrollBar(AutoDeleteOption ad):
	Widget(ad)
{}

bool ScrollBar::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance)
{
	assert(parent != NULL);
	style |= WS_CHILD;
	return Widget::create(WINDOW_CLASS_SCROLLBAR, NULL, style, x, y, width, height, parent, NULL, instance);
}

EditBox::EditBox(AutoDeleteOption ad):
	Widget(ad)
{}

bool EditBox::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, const char_t* text, DWORD styleEx)
{
	assert(parent != NULL);
	style |= WS_CHILD;
	return Widget::create(WINDOW_CLASS_EDITBOX, text, style, x, y, width, height, parent, NULL, instance, styleEx);
}

ulong_t EditBox::charAtPoint(const Point& p, ulong_t* line) const
{
    LRESULT res = sendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(p.x, p.y));
    if (NULL != line)
        *line = HIWORD(res);
    return LOWORD(res);
}

ProgressBar::ProgressBar(AutoDeleteOption ad):
    Widget(ad)
{
}

bool ProgressBar::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance)
{
	assert(parent != NULL);
	style |= WS_CHILD;
	return Widget::create(PROGRESS_CLASS, NULL, style, x, y, width, height, parent, NULL, instance);
}



TabControl::TabControl(AutoDeleteOption ad):
    Widget(ad)
{
    InitCC(ICC_TAB_CLASSES);
} 

TabControl::TabControl(HWND wnd, AutoDeleteOption ad):
    Widget(wnd, ad)
{
    InitCC(ICC_TAB_CLASSES);
} 

TabControl::~TabControl()
{
}

bool TabControl::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, DWORD styleEx)
{
	assert(parent != NULL);
	style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_TABCONTROL, NULL, style, x, y, width, height, handle(), NULL, instance, styleEx);
}


ListView::ListView(AutoDeleteOption ad):
    Widget(ad)
{
    InitCC(ICC_LISTVIEW_CLASSES);
}

ListView::ListView(HWND wnd, AutoDeleteOption ad):
    Widget(wnd, ad)
{
    InitCC(ICC_LISTVIEW_CLASSES);
}

ListView::~ListView()
{}

bool ListView::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, DWORD styleEx)   
{
    assert(parent != NULL);
    style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_LISTVIEW, NULL, style, x, y, width, height, parent, NULL, instance, styleEx);    
}

Button::Button(AutoDeleteOption ad):
    Widget(ad)
{
}

Button::Button(HWND wnd, AutoDeleteOption ad):
    Widget(wnd, ad)
{
}

Button::~Button()
{}

bool Button::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, DWORD styleEx)   
{
    assert(parent != NULL);
    style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_BUTTON, NULL, style, x, y, width, height, parent, NULL, instance, styleEx);    
}


ListBox::ListBox(AutoDeleteOption ad):
    Widget(ad)
{
}

ListBox::ListBox(HWND wnd, AutoDeleteOption ad):
    Widget(wnd, ad)
{
}

ListBox::~ListBox()
{}

bool ListBox::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, DWORD styleEx)   
{
    assert(parent != NULL);
    style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_LISTBOX, NULL, style, x, y, width, height, parent, NULL, instance, styleEx);    
}

ComboBox::ComboBox(AutoDeleteOption ad):
    Widget(ad)
{
}

ComboBox::ComboBox(HWND wnd, AutoDeleteOption ad):
    Widget(wnd, ad)
{
}

ComboBox::~ComboBox()
{}

bool ComboBox::create(DWORD style, int x, int y, int width, int height, HWND parent, HINSTANCE instance, UINT controlId, DWORD styleEx)   
{
    assert(parent != NULL);
    style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_COMBOBOX, NULL, style, x, y, width, height, parent, HMENU(controlId), instance, styleEx);    
}

bool ComboBox::create(DWORD style, const RECT& rect, HWND parent, HINSTANCE instance, UINT controlId, DWORD styleEx)
{
    assert(parent != NULL);
    style |= WS_CHILD;
    return Widget::create(WINDOW_CLASS_COMBOBOX, NULL, style, rect, parent, HMENU(controlId), instance, styleEx);    
}

bool ComboBox::setDroppedRect(const RECT& r)
{
    COMBOBOXINFO info = {sizeof(info)};
    if (0 == sendMessage(CB_GETCOMBOBOXINFO, 0, (LPARAM)&info))
        return false;

    if (NULL == info.hwndList)
        return false;

    return FALSE != MoveWindow(info.hwndList, r.left, r.top, r.right - r.left, r.bottom - r.top, FALSE); 
}
