#ifndef INCLUDE_RPCSERVICE_H
#define INCLUDE_RPCSERVICE_H

#include "proto/ProxyServer.Message.pb.h"

#include "public/Inc.h"

#include "RpcClients.h"

namespace Rpc {
	
	typedef ::ProxyServer::Message::RpcService::Stub Stub;
	
	class GameGateService {
	public:
		GameGateService(const ClientConn& conn);
		
		virtual void GetGameGate(
			const ::ProxyServer::Message::GameGateReq* req,
			::ProxyServer::Message::GameGateRsp* rsp);

		void Done(::ProxyServer::Message::GameGateRsp* rsp);
	private:
		Stub stub_;
	};
}

#endif