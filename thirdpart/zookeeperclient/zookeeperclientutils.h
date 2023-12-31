#ifndef INCLUDE_ZKCLIENTUTILS_H
#define INCLUDE_ZKCLIENTUTILS_H

#include "Logger/src/Macro.h"
#include <zookeeper/zookeeper.h>

//摘自zookeeper源文件 zk_adptor.h
/* zookeeper event type constants */
#define CREATED_EVENT_DEF 1
#define DELETED_EVENT_DEF 2
#define CHANGED_EVENT_DEF 3
#define CHILD_EVENT_DEF 4
#define SESSION_EVENT_DEF -1
#define NOTWATCHING_EVENT_DEF -2

//摘自zookeeper源文件 zk_adptor.h
/* zookeeper state constants */
#define EXPIRED_SESSION_STATE_DEF -112
#define AUTH_FAILED_STATE_DEF -113
#define CONNECTING_STATE_DEF 1
#define ASSOCIATING_STATE_DEF 2
#define CONNECTED_STATE_DEF 3
#define NOTCONNECTED_STATE_DEF 999


#define destroyPointer(pointer)\
    delete pointer;\
    pointer = nullptr;

//static const int IGNORE_VERSION = -1;



class ZookeeperClientUtils
{
public:
    ZookeeperClientUtils();


    static void printString(const std::string &name);
    static void printStringPair(const std::pair<std::string, std::string> &stringPair);

    static void transStringVector2VectorString(const String_vector &children, std::vector<std::string> &vecString);
    static void printPathList(const std::vector<std::string> &nodeNameVector);
    static void printPathValueList(const std::map<std::string, std::string> &pathValueMap);

    static void transACLVector2VectorACL(const ACL_vector &childs, std::vector<ACL> &vectorACL);

    static const std::string watcherEventType2String(int type);
    static const std::string state2String(int state);


};


#endif

