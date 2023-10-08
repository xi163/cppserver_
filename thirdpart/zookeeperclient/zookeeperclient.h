#ifndef INCLUDE_ZKCLIENT_H
#define INCLUDE_ZKCLIENT_H

#include "Logger/src/Macro.h"
#include "zookeeper/zookeeper.h"

class ZookeeperClient;

static const int IGNORE_VERSION = -1;
static const int MAX_BUFF_LEN = 1024;

typedef std::function<void()> ConnectedWatcherHandler;
/**
 * watcher handler
 */
typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
                       void *context)>  SessionWatcherHandler;

typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
                       const std::string &path, void *context)>  ExistsNodeWatcherHandler;

typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
                       const std::string &path, void *context)>  GetNodeWatcherHandler;

//typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
//                       const std::string &path, const std::string &value, void *context)>  CreateNodeWatcherHandler;

//typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
//                       const std::string &path, void *context)>  DeleteNodeWatcherHandler;

//typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
//                       const std::string &path, void *context)> NodeChangeWatcherHandler;

typedef std::function<void (int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr,
                       const std::string &path, void *context)> GetChildrenWatcherHandler;


/**
 * completion function
 */
typedef std::function<void (int rc, const char *value, const void *data)> StringCompletionHandler;

typedef std::function<void (int rc, const void *data)> VoidCompletionHandler;

typedef std::function<void (int rc, const struct Stat *stat, const void *data)> StatCompletionHandler;

typedef std::function<void (int rc, const char *value, int value_len,
                       const struct Stat *stat, const void *data)> DataCompletionHandler;

typedef std::function<void (int rc,const struct String_vector *strings, const void *data)> StringsCompletionHandler ;

typedef std::function<void (int rc,const struct String_vector *strings,
                       const struct Stat *stat,const void *data)> StringsStatCompletionHandler;

typedef std::function<void (int rc, struct ACL_vector *acl,struct Stat *stat, const void *data)> AclCompletionHandler;



/**
 * 监视器 通用上下文结构
 */
class ZkWatcherOperateContext
{
public:
    ZkWatcherOperateContext(const std::string &path, void *context, const std::shared_ptr<ZookeeperClient> &zkClientPtr)
    {
        m_path = path;
        m_context = context;
        m_zkClientPtr = zkClientPtr;
    }

    std::string m_path;
    void *m_context;
    std::shared_ptr<ZookeeperClient> m_zkClientPtr;

    SessionWatcherHandler m_sessionWatcherHandler;
    ExistsNodeWatcherHandler m_existsNodeWatcherHandler;
    GetNodeWatcherHandler m_getNodeWatcherHandler;
//    CreateNodeWatcherHandler m_createNodeWatcherHandler;
//    DeleteNodeWatcherHandler m_deleteNodeWatcherHandler;
//    NodeChangeWatcherHandler m_nodeChangeWatcherHandler;
    GetChildrenWatcherHandler m_getChildrenWatcherHandler;
};


class ZkAsyncCompletionContext
{
public:
    ZkAsyncCompletionContext(const std::string &path, void *context, const std::shared_ptr<ZookeeperClient> &zkClientPtr)
    {
        m_path = path;
        m_context = context;
        m_zkClientPtr = zkClientPtr;
    }

    std::string m_path;
    void *m_context;
    std::shared_ptr<ZookeeperClient> m_zkClientPtr;

    StringCompletionHandler m_stringCompletionHandler;
    DataCompletionHandler m_dataCompletionHandler;
    StatCompletionHandler m_statCompletionHandler;
    StringsCompletionHandler m_stringsCompletionHandler;
    StringsStatCompletionHandler m_stringsStatCompletionHandler;
    AclCompletionHandler m_aclCompletionHandler;
    VoidCompletionHandler m_voidCompletionHandler;
};



class ZookeeperClient : public std::enable_shared_from_this<ZookeeperClient>
{
public:
//    ZookeeperClient(const ZookeeperClient &) = delete;
//    ZookeeperClient & operator=(const ZookeeperClient &) = delete;

    ZookeeperClient();
    /**
     * ZookeeperClient constructor
     * @param server e.g. "127.0.0.1" or "127.0.0.1:2181,127.0.0.1:2182,127.0.0.1:2183"
     * @param handler  session handler defined by yourself
     * @param timeout default 3000 ms
     */
    ZookeeperClient(const std::string &server, int timeout = 30000, bool debug = false);

    /**
     * 释放资源
     */
    ~ZookeeperClient();


    /*
     * set server ips
     */

    void setServerIP(const std::string &server, int timeout = 30000, bool debug = false);

    /**
     * 连接服务端
     */
    bool connectServer();

    /**
     * 关闭连接
     */
    void closeServer();

    /**
     * 重新连接
     */
     bool reConnectServer();

     /**
      * 获取连接状态
      */
     int getConnectStatus();

 //    /**
 //     * 设置连接状态
 //     */
 //    void setConectStatus(bool connectStatus);

     zhandle_t *getZKHandle()
     {
         return m_zkHandle;
     }




    /**
     * sync api
     */

    /**
     * 根据结点路径，获取结点数据
     * @param path 结点路径
     * @param value 结点数据
     * @param version 返回对应的版本号
     * @param watch 0-不启用监视  1-启用连接的默认监视
     * @return 返回结果  成功ZOK
     */
    int getNodeValue(const std::string &path, std::string &value, int &version);

    /**
     * 带watcher的getValue(注意:结点不存在的情况,watcher不起作用)
     * @param path 结点路径
     * @param value 结点数据
     * @param defaultWatcher  传true,会使用默认的watcher,传false,需要传自己定义的watcher
     * @param watcher 当defaultWatcher传false时,需要传
     * @param context 上下文
     * @return 返回结果  成功ZOK
     */
    int getNodeValueWithWatcher(const std::string &path, std::string &value, int &version,
                                GetNodeWatcherHandler getNodeWatcherHandler = nullptr, void *context = nullptr);

    /**
     * 获取结点路径对应的ACL权限列表
     * @param path 结点路径
     * @param vectorACL ACL权限列表
     * @return 返回结果  成功ZOK
     */
    int getNodeACL(const std::string &path, std::vector<ACL> &vectorACL);

    /**
     * 设置结点数据
     * @param path 结点路径
     * @param value 结点数据
     * @param version 结点版本  -1表示不校验版本,其他表示验证版本，版本正确才能正确修改
     * @return 返回结果  成功ZOK
     */
    int setNodeValue(const std::string &path, const std::string &value, int version = IGNORE_VERSION,
                     struct Stat *stat = nullptr);

    /**
     * 不带监视的结点时候存在的判断
     * @param path 结点路径
     * @param stat 支持返回结点的Stat信息
     * @return 返回结果  成功ZOK
     */
    int existsNode(const std::string &path, struct Stat *stat = nullptr);

    /**
     * 带监视的exists的实现
     * @param path
     * @param existsNodeHandler  传入自行定义的session handler
     * @param context  传入自行定义的session handler
     * @return 返回结果  成功ZOK
     */
    int existsNodeWithWatch(const std::string &path, ExistsNodeWatcherHandler existsNodeWatcherHandler = nullptr, void *context = nullptr);

    /**
     * 创建结点(支持创建临时结点/永久结点或序列结点)
     * @param path 结点路径
     * @param value 结点数据(传"",表示创建不带data数据的空结点)
     * @param isTemp  是否临时结点
     * @param isSequnce 是否序列结点
     * @return 返回结果  成功ZOK
     */
    int createNode(const std::string &path, const std::string &value, bool isTemp = false, bool isSequence = false);

    /**
     * 删除结点
     * @param path 结点路径
     * @param version  结点版本 -1表示不校验版本,其他表示验证版本，版本正确才能正确删除
     * @return 返回结果  成功ZOK
     */
    int deleteNode(const std::string &path, int version = IGNORE_VERSION);

    /**
     * 获取结点的子结点(like ls)
     * @param path 结点路径
     * @param childVec 子结点列表
     * @param getChildrenNodeHandler  传入自行定义的session handler
     * @param watcher 当defaultWatcher传false时,需要传
     * @return 返回结果  成功ZOK
     */
    int getClildren(const std::string &path, std::vector<std::string> &childrenVec,
                    GetChildrenWatcherHandler getChildrenNodeWatcherHandler = nullptr, void *context = nullptr);

    /**
     * 获取结点的子结点(like ls)
     * @param path 结点路径
     * @param childVec 子结点列表
     * @return 返回结果  成功ZOK
     */
    int getClildrenNoWatch(const std::string &path, std::vector<std::string> &childrenVec);


    /**
     * async api
     */

    /**
     * 异步方式创建结点(支持创建临时结点/永久结点或序列结点)
     * @param path 结点路径
     * @param value 结点数据(传"",表示创建不带data数据的空结点)
     * @param isTemp  是否临时结点
     * @param isSequnce 是否序列结点
     * @param handler 支持传递completion handler
     * @param context 上下文信息
     * @return 返回结果  成功ZOK,其他失败
     */
    int asyncCreateNode(const std::string &path, const std::string &value,
                        StringCompletionHandler handler = nullptr, void *context = nullptr,
                        bool isTemp = false, bool isSequence = false);
    /**
     * 异步方式删除结点
     * @param path 结点路径
     * @param version  结点版本 -1表示不校验版本,其他表示验证版本，版本正确才能正确删除
     * @param handler 支持传递completion handler
     * @param context 上下文信息
     * @return 返回结果  成功ZOK,其他失败
     */
    int asyncDeleteNode(const std::string &path, int version = IGNORE_VERSION,
                        VoidCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步获取结点数据
     * @param path 结点路径
     * @return 返回结果  成功ZOK,其他失败
     */
    int asyncGetNodeValue(const std::string &path, DataCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步获取结点数据(带监视器版本)
     * @param path 结点路径
     * @param value 结点数据
     * @param version 结点版本
     * @param watchHandler 监视处理器
     * @param watchContext 监视处理器上下文
     * @param completionHandler 异步完成处理器
     * @param completionContext 异步完成处理器上下文
     * @return 返回结果  成功ZOK,其他失败
     */
    int asyncGetNodeValueWithWatcher(const std::string &path,
                                     GetNodeWatcherHandler watchHandler = nullptr, void *watchContext = nullptr,
                                     DataCompletionHandler completionHandler = nullptr, void *completionContext = nullptr);
    /**
     * 异步设置结点信息
     * @param path 结点路径
     * @param value 结点数据
     * @param version 结点版本
     * @param handler 异步完成处理器
     * @param context 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncSetNodeValue(const std::string &path, const std::string &value, int version = IGNORE_VERSION,
                          StatCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步获取子节点信息
     * @param path 结点路径
     * @param handler 异步完成处理器
     * @param context 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncGetChildren(const std::string &path, StringsCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步获取子节点信息(带监视)
     * @param path 结点路径
     * @param watchHandler 监视处理器
     * @param watchContext 监视处理器上下文
     * @param completionHandler 异步完成处理器
     * @param completionContext 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncGetChildrenWithWatcher(const std::string &path,
                                    GetChildrenWatcherHandler watchHandler = nullptr, void *watchContext = nullptr,
                                    StringsCompletionHandler completionHandler = nullptr, void *completionContext = nullptr);

    /**
     * 异步获取子节点信息(支持带stat版本处理器)
     * @param path 结点路径
     * @param handler 异步完成处理器
     * @param context 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncGetChildren2(const std::string &path, StringsStatCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步获取子节点信息(带监视,支持带stat版本处理器)
     * @param path 结点路径
     * @param watchHandler 监视处理器
     * @param watchContext 监视处理器上下文
     * @param completionHandler 异步完成处理器
     * @param completionContext 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncGetChildren2WithWatcher(const std::string &path,
                                     GetChildrenWatcherHandler watchHandler = nullptr, void *watchContext = nullptr,
                                     StringsStatCompletionHandler completionHandler = nullptr, void *completionContext = nullptr);

    /**
     * 异步获取结点ACL信息
     * @param path 结点路径
     * @param handler 异步完成处理器
     * @param context 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncGetNodeACL(const std::string &path, AclCompletionHandler handler = nullptr, void *context = nullptr);

    /**
     * 异步设置结点ACL信息
     * @param path 结点路径
     * @param aclVector ACL信息
     * @param version 版本
     * @param handler 异步完成处理器
     * @param context 异步完成处理器上下文
     * @return 成功ZOK,其他失败
     */
    int asyncSetNodeACL(const std::string &path, ACL_vector *aclVector, int version = IGNORE_VERSION,
                        VoidCompletionHandler handler = nullptr, void *context = nullptr);
    /*
     *	zoo_async
     *	zoo_amulti 批处理，即原子性地一次提交多个ZK操作
     */


private:
    //watch handler
    static void sessionWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);
    static void existsNodeWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);
    static void getNodeValueWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);
    static void getChildrenWatcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

    //completion handler
    static void asyncCreateNodeHandler(int rc, const char *value, const void *data);
    static void asyncDeleteNodeHandler(int rc, const void *data);
    static void asyncGetNodeHandler(int rc, const char *value, int value_len, const struct Stat *stat, const void *data);
    static void asyncSetNodeHandler(int rc, const struct Stat *stat, const void *data);
    static void asyncGetChildrenHandler(int rc,const struct String_vector *strings, const void *data);
    static void asyncGetChildrenHandler2(int rc,const struct String_vector *strings, const struct Stat *stat,const void *data);
    static void asyncGetNodeACLHandler(int rc, struct ACL_vector *acl,struct Stat *stat, const void *data);
    static void asyncSetNodeACLHandler(int rc, const void *data);


private:
    void connectingSessionWatcher(int type, int state, const std::shared_ptr<ZookeeperClient> &zkClientPtr, void *context);


public:
    void SetConnectedWatcherHandler(ConnectedWatcherHandler hander);

private:
    ConnectedWatcherHandler m_connectedWatcherHandler;

private:
    zhandle_t *m_zkHandle = nullptr;
    std::atomic<int>  m_session_state;
    int m_session_timeout;
    std::string server_;

//    SessionWatcherHandler m_sessionWatcherHandler;

    std::mutex m_connect_mutex;
    std::condition_variable m_connect_cond;

//    std::boost::std::shared_ptr<spdlog::logger> logger_ = spdlog::get(getGlobalLogName());

};

class  zkLockGuard
{
 public:
  explicit zkLockGuard(std::shared_ptr<ZookeeperClient> zkClient,const std::string& nodePath,const std::string& value)
    : path(nodePath)
    , zk(zkClient)
  {
    zk->createNode(nodePath,value,true);
  }
    ~zkLockGuard()
    {
      zk->deleteNode(path);
    }
private:
    zkLockGuard(const zkLockGuard&);
    const zkLockGuard& operator =(const zkLockGuard&);
 private:
  std::string path;
  std::shared_ptr<ZookeeperClient> zk;
};

#endif