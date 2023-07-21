#include "RpcService.h"

#include "RpcClients.h"

namespace Rpc {
	
	GameGateService::GameGateService(const ClientConn& conn)
		: stub_(muduo::get_pointer(conn.get<2>())) {
		muduo::net::TcpConnectionPtr c(conn.get<1>().lock());
		conn.get<2>()->setConnection(c);
	}
	
	void GameGateService::GetGameGate(
		const ::ProxyServer::Message::GameGateReq* req,
		::ProxyServer::Message::GameGateRsp* rsp) {
		_LOG_WARN(req->DebugString().c_str());
		stub_.GetGameGate(NULL, req, rsp, ::google::protobuf::NewCallback(this, &GameGateService::Done, rsp));
	}

	void GameGateService::Done(::ProxyServer::Message::GameGateRsp* rsp) {
		_LOG_WARN(rsp->DebugString().c_str());
	}
}