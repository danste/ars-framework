#include <Debug.hpp>
#include <Text.hpp>
#include <WindowsCE/WinGraphics.hpp>
#include <DefinitionStyle.hpp>

Graphics::Graphics(const NativeGraphicsHandle_t& handle, DeleteOption delHandle):
    handle_(handle), 
	hwnd_(NULL),
	originalFont_(NULL),
	originalPen_(NULL),
	deleteHandle_(deleteHandle == delHandle)
{
	init();
}

Graphics::Graphics(HWND hwnd):
    handle_(GetDC(hwnd)), 
	hwnd_(hwnd),
	deleteHandle_(true)
{
	init();
}

void Graphics::init()
{
	fontHeight_ = 0;
	fontBaseline_ = 0;
	fontMetricsFlags_ = 0;

	SetBkMode(handle_, TRANSPARENT);

	currentFont_.getSystemFont();
	originalFont_ = (HFONT)SelectObject(handle_, currentFont_.handle());
	assert(NULL != originalFont_);
	queryFontMetrics();

#ifdef DEBUG
    statePushCounter_ = 0;
#endif

	HGDIOBJ blackPen = GetStockObject(BLACK_PEN);
	assert(NULL != blackPen);
	GetObject(blackPen, sizeof(pen_), &pen_);
	penColor_ = pen_.lopnColor;
	originalPen_ = (HPEN)SelectObject(handle_, blackPen);
}

void Graphics::setFont(const WinFont& font)
{
	assert(font.valid());
	if (!font.valid())
		return;

	if (currentFont_.handle() != font.handle())
	{
		SelectObject(handle_, font.handle());
		queryFontMetrics();
	}
	currentFont_ = font;
}


Graphics::State_t Graphics::pushState()
{
    State_t st;
    st.state = SaveDC(handle_);
    st.fontHeight = fontHeight_;
    st.fontBaseline = fontBaseline_;
	st.fontMetricsFlags = fontMetricsFlags_;
	st.font = currentFont_;

#ifdef DEBUG
    statePushCounter_ += 1;
#endif
    return st;
}

void Graphics::popState(const Graphics::State_t& state)
{
    RestoreDC(handle_, state.state);
    fontHeight_ = state.fontHeight;
    fontBaseline_ = state.fontBaseline;
	currentFont_ = state.font;
	fontMetricsFlags_ = state.fontMetricsFlags;

#ifdef DEBUG
    statePushCounter_ -= 1;
    assert(statePushCounter_>=0);
#endif
}

Graphics::~Graphics()
{
	if (NULL != originalFont_)
		SelectObject(handle_, originalFont_);

	if (NULL != originalPen_)
		SelectObject(handle_, originalPen_);

#ifdef DEBUG
    assert(0 == statePushCounter_);
#endif

    if(NULL != hwnd_)
        ReleaseDC(hwnd_, handle_);
    else if (deleteHandle_)
        DeleteDC(handle_);
}

static void InvertRectangle(HDC dc, const Point& topLeft, uint_t width, uint_t height)
{
	BitBlt(dc, topLeft.x, topLeft.y, width, height, dc, topLeft.x, topLeft.y, DSTINVERT);	
}

void Graphics::drawText(const char_t* text, ulong_t length, const Point& topLeft, bool inverted)
{
    ulong_t len=length;
    uint_t width=0x7fffffff;
    charsInWidth(text, len, width);

    uint_t height=fontHeight();
	uint_t base = fontBaseline();

    uint_t top=topLeft.y;
	if (0 != (winFontMetricSmall &  fontMetricsFlags_))
		top += base / 3;

	if (0 != (winFontMetricSubscript & fontMetricsFlags_))
		top += height / 3;

    ExtTextOut(handle_, topLeft.x, top, 0, NULL, text, length, NULL);

	if (inverted)
		InvertRectangle(handle_, topLeft, width, height);
}

void Graphics::erase(const Rect& rect)
{
    HBRUSH hbr = CreateSolidBrush(GetBkColor(handle_));
    FillRect(handle_, &rect, hbr);
    DeleteObject(hbr);
}

void Graphics::copyArea(const Rect& sourceArea, Graphics& targetSystem, const Point& targetTopLeft)
{
    BitBlt(targetSystem.handle_, targetTopLeft.x, targetTopLeft.y,
        sourceArea.width(), sourceArea.height(), 
        handle_,
        sourceArea.left, sourceArea.top, SRCCOPY);
}

void Graphics::drawLine(Coord_t x0, Coord_t y0, Coord_t x1, Coord_t y1)
{
    POINT p[2];
    p[0].x=x0;
    p[0].y=y0;
    p[1].x=x1;
    p[1].y=y1;
    Polyline(handle_, p, 2);
}

NativeColor_t Graphics::setForegroundColor(NativeColor_t color)
{
	// @bug this code below will have to be reworked as it is possible currently to use non-existant pens after restoring draw state
    COLORREF oldColor = pen_.lopnColor;
    pen_.lopnColor = color;
    HPEN pen = CreatePenIndirect(&pen_);
    DeleteObject(SelectObject(handle_, pen));
    return oldColor;
}

NativeColor_t Graphics::setBackgroundColor(NativeColor_t color)
{
    return SetBkColor(handle_, color);
}

NativeColor_t Graphics::setTextColor(NativeColor_t color)
{
    return SetTextColor(handle_, color); 
}

ulong_t Graphics::wordWrap(const char_t* text, uint_t availableDx, uint_t& textDx)
{
    int     lenThatFits;
    SIZE    size;
    int     textLen = Len(text);

DoItAgain:
    GetTextExtentExPoint(handle_, text, textLen, availableDx, &lenThatFits, NULL, &size);

    if ( (lenThatFits==textLen) || (0==lenThatFits))
    {
        textDx = size.cx;
        return lenThatFits;
    }

    // the string is too big so try to find a line-breaking place (a whitespace)
    int curPos = lenThatFits-1;
    while (curPos>=0)
    {
        if (isSpace(text[curPos]))
        {
            textLen = curPos+1;
            // this time it should succeed 100%
            goto DoItAgain;
        }
        --curPos;
    }
    
    textDx = textWidth(text, lenThatFits);
    return lenThatFits;
}

// given a string text, determines how many characters of this string can
// be displayed in a given width. If text is bigger than width, a line
// break will be on newline, tab or a space.
// Mimics Palm OS function FntWordWrap()
ulong_t Graphics::wordWrap(const char_t* text, uint_t width)
{
    uint_t textDx;
    ulong_t lenThatFits = wordWrap(text, width, textDx);
    return lenThatFits;
}

uint_t Graphics::textWidth(const char_t* text, ulong_t length)
{
    SIZE size;
    GetTextExtentPoint(handle_, text, length, &size);
    return size.cx;
}

void Graphics::charsInWidth(const char_t* text, ulong_t& length, uint_t& width)
{
    int   len;
    SIZE  size;
    GetTextExtentExPoint(handle_, text, length, width, &len, NULL, &size ); 
    length = len;
    width = size.cx;
}

void Graphics::queryFontMetrics()
{
	TEXTMETRIC metrics;
	BOOL res = GetTextMetrics(handle_, &metrics);
	if (!res)
		return;

	fontHeight_ = metrics.tmHeight;
	fontBaseline_ = metrics.tmAscent;
	if (0 == fontMetricsFlags_)
		return;

	fontHeight_ *= 3;
	fontHeight_ /= 2;

	fontBaseline_ *= 3;
	fontBaseline_ /= 2;
}

void Graphics::applyStyle(const DefinitionStyle* style, bool isHyperlink)
{
	const DefinitionStyle* def = StyleGetStaticStyle(styleIndexDefault);
    DefinitionStyle s = *def;

    if (isHyperlink)
        s |= *StyleGetStaticStyle(styleIndexHyperlink);

    if (NULL != style && def != style)
        s |= *style;

	WinFont font = s.font();

	fontMetricsFlags_ = 0;
	if (s.small == s.yes)
		fontMetricsFlags_ |= winFontMetricSmall;
	if (s.subscript == s.yes)
		fontMetricsFlags_ |= winFontMetricSubscript;
	if (s.superscript == s.yes)
		fontMetricsFlags_ |= winFontMetricSuperscript;

    setFont(font);

	assert(DefinitionStyle::colorNotDefined != s.backgroundColor);
	SetBkColor(handle_, s.backgroundColor);
	assert(DefinitionStyle::colorNotDefined != s.foregroundColor);
	SetTextColor(handle_, s.foregroundColor);
}

