#ifndef INCLUDE_GAME_API_H
#define INCLUDE_GAME_API_H

#include "public/Inc.h"
#include "public/gameConst.h"
#include "public/gameStruct.h"
#include "Packet/Packet.h"

#include "Entities.h"

#include "rpc/client/RpcClients.h"
#include "rpc/client/RpcContainer.h"

#include "rpc/server/RpcService.h"

#include "public/errorCode.h"
#include "public/gameConst.h"
#include "public/gameStruct.h"

#include "IPLocator/IPLocator.h"

class ApiServ : public boost::noncopyable {
public:
	typedef std::function<
		void(const muduo::net::TcpConnectionPtr&, BufferPtr const&)> CmdCallback;
	typedef std::map<uint32_t, CmdCallback> CmdCallbacks;
	ApiServ(muduo::net::EventLoop* loop,
		const muduo::net::InetAddress& listenAddr,
		const muduo::net::InetAddress& listenAddrRpc,
		const muduo::net::InetAddress& listenAddrHttp,
		std::string const& cert_path, std::string const& private_key_path,
		std::string const& client_ca_cert_file_path = "",
		std::string const& client_ca_cert_dir_path = "");
	~ApiServ();
	void Quit();
	void registerHandlers();
	bool InitZookeeper(std::string const& ipaddr);
	void onZookeeperConnected();
	void onGateWatcher(
		int type, int state,
		const std::shared_ptr<ZookeeperClient>& zkClientPtr,
		const std::string& path, void* context);
	void registerZookeeper();
	bool InitRedisCluster(std::string const& ipaddr, std::string const& passwd);
	bool InitMongoDB(std::string const& url);
	void threadInit();
	bool InitServer();
	void Start(int numThreads, int numWorkerThreads, int maxSize);
private:
	bool onCondition(const muduo::net::InetAddress& peerAddr, muduo::net::InetRegion& peerRegion);
	void onConnection(const muduo::net::TcpConnectionPtr& conn);
	bool onVerify(muduo::net::http::IRequest const* request);
	void onConnected(
		const muduo::net::TcpConnectionPtr& conn,
		std::string const& ipaddr);
	void onMessage(
		const muduo::net::TcpConnectionPtr& conn,
		muduo::net::Buffer* buf, int msgType,
		muduo::Timestamp receiveTime);
	void asyncClientHandler(
		const muduo::net::WeakTcpConnectionPtr& weakConn,
		BufferPtr const& buf,
		muduo::Timestamp receiveTime);
	void asyncOfflineHandler(Context& entryContext);
	void refreshBlackList();
	bool refreshBlackListSync();
	bool refreshBlackListInLoop();
private:
	bool onHttpCondition(const muduo::net::InetAddress& peerAddr, muduo::net::InetRegion& peerRegion);
	void onHttpConnection(const muduo::net::TcpConnectionPtr& conn);
	void onHttpMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);
	void asyncHttpHandler(
		const muduo::net::WeakTcpConnectionPtr& weakConn,
		BufferPtr const& buf,
		muduo::Timestamp receiveTime);
	void onHttpWriteComplete(const muduo::net::TcpConnectionPtr& conn);
	void processHttpRequest(
		const muduo::net::HttpRequest& req,
		muduo::net::HttpResponse& rsp,
		const muduo::net::TcpConnectionPtr& conn,
		BufferPtr const& buf,
		muduo::Timestamp receiveTime);
	//std::string onProcess(std::string const& reqStr, muduo::Timestamp receiveTime, int& code, std::string& errMsg, boost::property_tree::ptree& latest, int& testTPS);
	//int execute(int opType, std::string const& account, double score, std::string const& orderId, std::string& errmsg, boost::property_tree::ptree& latest, int& testTPS);
	
	//刷新所有agent_info信息
	//1.web后台更新代理通知刷新
	//2.游戏启动刷新一次
	//3.redis广播通知刷新一次
	bool refreshAgentInfo();
	
	//刷新所有IP访问白名单信息
	//1.web后台更新白名单通知刷新
	//2.游戏启动刷新一次
	//3.redis广播通知刷新一次
	void refreshWhiteList();
	
	//同步刷新IP访问白名单
	bool refreshWhiteListSync();
	bool refreshWhiteListInLoop();
	
	//请求挂维护/恢复服务 status=0挂维护 status=1恢复服务
	bool repairServer(containTy servTy, std::string const& servname, std::string const& name, int status, std::string& rspdata);
	bool repairServer(std::string const& queryStr, std::string& rspdata);
	void repairServerNotify(std::string const& msg, std::string& rspdata);
private:
	void cmd_on_user_login(
		const muduo::net::TcpConnectionPtr& conn, BufferPtr const& buf);
public:
	std::shared_ptr<ZookeeperClient> zkclient_;
	std::string nodePath_, nodeValue_, invalidNodePath_;
	//redis订阅/发布
	std::shared_ptr<RedisClient> redisClient_;
	std::string redisIpaddr_;
	std::string redisPasswd_;

	std::vector<std::string> redlockVec_;

	std::string mongoDBUrl_;
public:
	int ttlUserLock_, ttlAgentLock_;
	int ttlExpired_;

	std::map<int32_t, agent_info_t> agent_info_;
	mutable boost::shared_mutex agent_info_mutex_;
	bool isdecrypt_;


	muduo::AtomicInt32 numConnected_[kMaxNodeTy];
	muduo::AtomicInt64 numTotalReq_;
	muduo::AtomicInt64 numTotalBadReq_;

	//map[session] = weakConn
	STR::Entities entities_;
	//map[userid] = weakConn
	INT::Entities sessions_;

	int interval_ = 1;
	int idleTimeout_;
	int maxConnections_;

	CmdCallbacks handlers_;
	std::string internetIp_, server_ipport_, httpserver_ipport_;
	std::string proto_ = "ws://";
	std::string http_proto_ = "http://";
	std::string path_http_order_ = "/opt/order";
	std::string path_handshake_;
	rpc::server::Service rpcservice_;
	muduo::net::TcpServer server_;
	//muduo::net::TcpServer tcpserver_;
	muduo::net::RpcServer rpcserver_;
	muduo::net::TcpServer httpserver_;
	
	rpc::Connector gateRpcClients_;
	rpc::Container rpcClients_[rpc::kMaxRpcTy];
	
	muduo::AtomicInt32 nextPool_;
	std::hash<std::string> hash_session_;
	std::vector<Buckets> bucketsPool_;
	std::vector<std::shared_ptr<muduo::ThreadPool>> threadPool_;
	std::shared_ptr<muduo::net::EventLoopThread> thisTimer_;
	
	//管理员挂维护/恢复服务
	volatile long server_state_;
	std::map<in_addr_t, eApiVisit> admin_list_;
	
	eApiCtrl whiteListControl_;
	std::map<in_addr_t, eApiVisit> white_list_;
	mutable boost::shared_mutex white_list_mutex_;
	
	eApiCtrl blackListControl_;
	std::map<in_addr_t, eApiVisit> black_list_;
	mutable boost::shared_mutex black_list_mutex_;
	
	CIPLocator ipLocator_;
	bool tracemsg_ = 0;
	bool verify_ = true;
	bool et_ = false;

#ifdef _STAT_ORDER_QPS_
	//性能测试指标 间隔输出时间(s)
	int deltaTime_;
#endif
};

#endif