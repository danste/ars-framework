#include <SimpleSocketConnection.hpp>

namespace ArsLexis
{

    SimpleSocketConnection::SimpleSocketConnection(SocketConnectionManager& manager):
        SocketConnection(manager),
        maxResponseSize_(32768),
        sending_(true),
        chunkSize_(256),
        requestBytesSent_(0)
    {}

    status_t SimpleSocketConnection::notifyWritable()
    {
        assert(sending_);
        ushort_t requestSize=request_.size();
        ushort_t requestLeft=requestSize-requestBytesSent_;
        if (requestLeft>chunkSize_)
            requestLeft=chunkSize_;
        ushort_t dataSize=0;
//        status_t error=socket().send(dataSize, request_.data()+requestBytesSent_, requestLeft, transferTimeout());
        status_t error=socket().send(dataSize, request_.data()+requestBytesSent_, requestLeft, 0);
        if (errNone==error || netErrWouldBlock==error)
        {
            registerEvent(SocketSelector::eventException);
            requestBytesSent_+=dataSize;
            if (requestBytesSent_==requestSize)
            {
                sending_=false;
                registerEvent(SocketSelector::eventRead);
                error=socket().shutdown(netSocketDirOutput);
                if (error)
                    log().debug()<<"notifyWritable(): Socket::shutdown() returned error, "<<error;
            }
            else
                registerEvent(SocketSelector::eventWrite);                
            error=notifyProgress();
        }
        else
            log().debug()<<"notifyWritable(): Socket::send() returned error, "<<error;

        return error;        
    }
    
    status_t SimpleSocketConnection::notifyReadable()
    {
        assert(!sending_);
        ushort_t dataSize=0;
        ushort_t responseSize=response_.size();
        status_t error=errNone;
        if (responseSize<maxResponseSize_-chunkSize_)
        {
            response_.resize(responseSize+chunkSize_);
//            error=socket().receive(dataSize, &response_[responseSize], chunkSize_, transferTimeout());
            error=socket().receive(dataSize, &response_[responseSize], chunkSize_, 0);
            if (!error)
            {
                assert(dataSize<=chunkSize_);
                response_.resize(responseSize+dataSize);
                if (0==dataSize)
                {   
                    error=notifyFinished();
                    abortConnection();
                }
                else
                {
                    registerEvent(SocketSelector::eventException);
                    registerEvent(SocketSelector::eventRead);
                    error=notifyProgress();
                }
            }
            else
                log().debug()<<"notifyReadable(): Socket::receive() returned error, "<<error;
        }
        else
            error=errResponseTooLong;                
            
        return error;
    }
        
}
