#ifndef __ARSLEXIS_SOCKET_CONNECTION_HPP__
#define __ARSLEXIS_SOCKET_CONNECTION_HPP__

#include <Socket.hpp>
#include <ErrBase.h>
#include <Logging.hpp>
#include <SocketAddress.hpp>
#include <vector>

namespace ArsLexis
{

    class SocketConnection;

    class SocketConnectionManager: private NonCopyable
    {
        NetLibrary& netLib_;
        SocketSelector selector_;
        typedef std::vector<SocketConnection*> Connections_t;
        Connections_t connections_;
        
        void addConnection(SocketConnection& connection);
    
        void registerEvent(SocketConnection& connection, SocketSelector::EventType event);
        
        void unregisterEvents(SocketConnection& connection);
        
        void removeConnection(SocketConnection& connection);
        
    public:
    
        SocketConnectionManager(NetLibrary& netLib);

        ~SocketConnectionManager();
        
        bool active() const
        {return selector_.active();}
        
        Err manageConnectionEvents(Int32 timeout=evtWaitForever);
        
        NetLibrary& netLibrary()
        {return netLib_;}
        
        friend class SocketConnection;
    };

    class SocketConnection: private NonCopyable
    {
        SocketConnectionManager& manager_;
        Int32 transferTimeout_;
        SocketAddress address_;
        mutable ChildLogger log_;
        
    protected:

        Socket socket_;
    
        Err getSocketErrorStatus() const;
    
        virtual void notifyWritable()
        {}
        
        virtual void notifyReadable()
        {}
        
        virtual void notifyException();
        
        virtual void abortConnection()
        {delete this;}
        
        SocketConnection(SocketConnectionManager& manager);
        
    public:
    
        Logger& log() const
        {return log_;}
    
        enum Error
        {
            errResponseTooLong=sockConnErrorClass,
            errResponseMalformed,
            errFirstAvailable
        };
    
        virtual void handleError(Err)
        {abortConnection();}
        
        void setTransferTimeout(Int32 timeout)
        {
            transferTimeout_=timeout;
        }
        
        Int32 transferTimeout() const
        {return transferTimeout_;}
        
        void setAddress(const SocketAddress& address)
        {address_=address;}

        virtual Err open();
        
        void registerEvent(SocketSelector::EventType event)
        {
            manager_.registerEvent(*this, event);
        }
        
        virtual ~SocketConnection();
        
        SocketConnectionManager& manager()
        {return manager_;}
        
        friend class SocketConnectionManager;
    };

}

#endif