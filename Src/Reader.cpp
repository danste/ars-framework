
#include <Reader.hpp>

namespace ArsLexis {

    status_t Reader::readLine(bool& eof, String& out, char_t delimiter)
    {
        while (true)
        {
            int chr;
            status_t error=read(chr);
            if (errNone!=error)
                return error;
            if (npos!=chr && delimiter!=chr)
                out.append(1, char_t(chr));
            else 
            {
                if (npos==chr)
                    eof=true;
                break;
            }
        }
        return errNone;
    }
    
    Reader::~Reader()
    {}
    
}