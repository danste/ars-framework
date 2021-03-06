#ifndef ARSLEXIS_HISTORY_CACHE_HPP__
#define ARSLEXIS_HISTORY_CACHE_HPP__

#include <DataStore.hpp>
#include <Utility.hpp>

class Serializer;

class HistoryCache: private NonCopyable 
{
public:

    enum {
        maxCacheEntries = 10, 
        maxCacheEntryUrlLength = 255,
        maxCacheEntryTitleLength = 63
    };
    
private:
    
    typedef char CacheEntryUrlBuffer_t[maxCacheEntryUrlLength + 1];
    typedef char_t CacheEntryTitleBuffer_t[maxCacheEntryTitleLength + 1];
    typedef char StreamNameBuffer_t[DataStore::maxStreamNameLength + 1];

    struct IndexEntry 
    {
        CacheEntryUrlBuffer_t url;
        StreamNameBuffer_t streamName;
        CacheEntryTitleBuffer_t title;
        bool onlyLink;
        
        IndexEntry();
    };
    
    IndexEntry* indexEntries_;    
    ulong_t indexEntriesCount_;
    ulong_t indexCapacity_;
    
    bool dataStoreOwner_;
    
    status_t readIndex();
    
    status_t writeIndex();
    
    status_t serializeIndexIn(Serializer& serialize);
    
    status_t serializeIndexOut(Serializer& serialize);
    
public:
    
    DataStore* dataStore;

    HistoryCache();
    
    ~HistoryCache();

    status_t open(DataStore& ds);
    status_t open(const char_t* dataStoreName);
    
    ulong_t entriesCount() const {return indexEntriesCount_;}
    
    enum {entryNotFound = ulong_t(-1)};
    
    ulong_t entryIndex(const char* entry) const;
    
    const char* entryUrl(ulong_t index) const;
    
    const char_t* entryTitle(ulong_t index) const;
    
    bool entryIsOnlyLink(ulong_t index) const;
    
    void setEntryTitle(ulong_t index, const char_t* title);
    
    status_t removeEntry(ulong_t index);
    
    status_t removeEntry(const char* url);
    
    status_t removeEntriesAfter(ulong_t startIndex);
    
    status_t appendEntry(const char* url, ulong_t& index);
    
    status_t insertLink(ulong_t index, const char* url, const char_t* title);
    
    status_t appendLink(const char* url, const char_t* title);
    
    status_t replaceEntries(ulong_t from, const char* newUrl);
    
    DataStoreReader* readerForEntry(ulong_t index);
    
    DataStoreWriter* writerForEntry(ulong_t index);
    
    void close();
    
    // Moves the entry to be the last in the cache. 
    // On succesful return index is updated to new value.
    status_t moveEntryToEnd(ulong_t& index);
   
};

#ifdef DEBUG
void test_HistoryCache();
#endif

#endif


