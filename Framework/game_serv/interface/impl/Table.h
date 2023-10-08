#ifndef INCLUDE_TABLE_H
#define INCLUDE_TABLE_H

#include "public/gameConst.h"
#include "public/gameStruct.h"

#include "Packet/Packet.h"
#include "ITableContext.h"
#include "ITable.h"
#include "ITableDelegate.h"
#include "IPlayer.h"
#include "Player.h"
//#include "IReplayRecord.h"

class CTable : public ITable/*, public IReplayRecord*/ {
public:
	CTable(muduo::net::EventLoop* loop, ITableContext* tableContext);
	virtual ~CTable();
	virtual void Reset();
	virtual void ResetInLoop(bool& ok);
	bool send(
		std::shared_ptr<IPlayer> const& player,
		uint8_t const* msg, size_t len,
		uint8_t mainId,
		uint8_t subId, bool v = false, int flag = 0);
	bool send(
		std::shared_ptr<IPlayer> const& player,
		uint8_t const* msg, size_t len,
		uint8_t subId, bool v = false, int flag = 0);
	bool send(
		std::shared_ptr<IPlayer> const& player,
		::google::protobuf::Message* msg,
		uint8_t mainId,
		uint8_t subId, bool v = false, int flag = 0);
	bool send(
		std::shared_ptr<IPlayer> const& player,
		::google::protobuf::Message* msg,
		uint8_t subId, bool v = false, int flag = 0);
	virtual void Init(std::shared_ptr<ITableDelegate>& tableDelegate, TableState& tableState, tagGameRoomInfo* roomInfo);
	virtual uint16_t GetTableId();
	virtual void GetTableInfo(TableState& tableState);
	virtual void GetPlayers(std::vector<std::shared_ptr<CPlayer>>& items);
	virtual void GetPlayersInLoop(std::vector<std::shared_ptr<CPlayer>>& items, bool& ok);
	virtual bool Full();
	virtual void FullInLoop(bool& rc, bool& ok);
	virtual muduo::net::EventLoop* GetLoop();
	virtual void assertThisThread();
	virtual std::string const& ServId();
	virtual std::string NewRoundId();
	virtual std::string GetRoundId();
	virtual bool DismissGame();
	virtual bool ConcludeGame(uint8_t gameStatus);
	virtual int64_t CalculateRevenue(int64_t score);
	virtual std::shared_ptr<IPlayer> GetChairPlayer(uint16_t chairId);
	virtual std::shared_ptr<IPlayer> GetPlayer(int64_t userId);
	virtual bool ExistUser(uint16_t chairId);
	virtual void SetGameStatus(uint8_t status = GAME_STATUS_FREE);
	virtual uint8_t GetGameStatus();
	virtual std::string StrGameStatus();
	virtual void SetUserTrustee(uint16_t chairId, bool trustee);
	virtual bool GetUserTrustee(uint16_t chairId);
	virtual void SetUserReady(uint16_t chairId);
	/// <summary>
	/// 点击离开按钮
	/// </summary>
	/// <param name="player"></param>
	/// <param name="sendToSelf"></param>
	/// <returns></returns>
	virtual bool OnUserLeft(std::shared_ptr<CPlayer> const& player, bool sendToSelf = true);
	/// <summary>
	/// 关闭页面
	/// </summary>
	/// <param name="player"></param>
	/// <returns></returns>
	virtual bool OnUserOffline(std::shared_ptr<CPlayer> const& player);
	virtual bool CanJoinTable(std::shared_ptr<CPlayer> const& player);
	virtual bool CanJoinTable(std::shared_ptr<CPlayer> const& player, uint16_t excludeId/* = INVALID_TABLE*/);
	virtual void CanJoinTableInLoop(std::shared_ptr<CPlayer> const& player, uint16_t excludeId, bool& rc, bool& ok);
	virtual bool CanLeftTable(int64_t userId);
	virtual size_t GetPlayerCount();
	virtual size_t GetPlayerCount(bool includeRobot);
	virtual size_t GetRobotPlayerCount();
	virtual size_t GetRealPlayerCount();
	virtual void GetPlayerCount(size_t& realCount, size_t& robotCount);
	virtual size_t GetMaxPlayerCount();
	virtual tagGameRoomInfo* GetRoomInfo();
	virtual bool IsRobot(uint16_t chairId);
	virtual bool IsOfficial(uint16_t chairId);
	virtual bool OnGameEvent(uint16_t chairId, uint8_t subId, uint8_t const* data, size_t len);
	virtual void OnStartGame();
	virtual bool IsGameStarted() { return status_ >= GAME_STATUS_START && status_ < GAME_STATUS_END; }
	virtual bool CheckGameStart();
	virtual bool RoomSitChair(std::shared_ptr<CPlayer> const& player, packet::internal_prev_header_t const* pre_header, packet::header_t const* header);
	virtual bool OnUserEnterAction(std::shared_ptr<CPlayer> const& player, packet::internal_prev_header_t const* pre_header, packet::header_t const* header);
	virtual void SendUserSitdownFinish(std::shared_ptr<CPlayer> const& player, packet::internal_prev_header_t const* pre_header, packet::header_t const* header);
	virtual bool OnUserStandup(std::shared_ptr<CPlayer> const& player, bool sendState = true, bool sendToSelf = true, uint8_t sendErrCode = 0);
	virtual bool SendTableData(uint16_t chairId, uint8_t subId, uint8_t const* data, size_t len);
	virtual bool SendTableData(uint16_t chairId, uint8_t subId, ::google::protobuf::Message* msg);
	virtual bool SendUserData(std::shared_ptr<IPlayer> const& player, uint8_t subId, uint8_t const* data, size_t len);
	virtual bool SendGameMessage(uint16_t chairId, std::string const& msg, uint8_t msgType, int64_t score = 0);
	virtual void ClearTableUser(uint16_t chairId = INVALID_CHAIR, bool sendState = true, bool sendToSelf = true, uint8_t sendErrCode = 0);
	virtual void BroadcastUserInfoToOther(std::shared_ptr<IPlayer> const& player);
	virtual void SendAllOtherUserInfoToUser(std::shared_ptr<IPlayer> const& player);
	virtual void SendOtherUserInfoToUser(std::shared_ptr<IPlayer> const& player, tagUserInfo& userInfo);
	virtual void BroadcastUserScore(std::shared_ptr<IPlayer> const& player);
	virtual void BroadcastUserStatus(std::shared_ptr<IPlayer> const& player, bool sendToSelf = true);
	virtual bool WriteUserScore(tagScoreInfo* scoreInfo, size_t count, std::string& strRound);
	virtual bool WriteSpecialUserScore(tagSpecialScoreInfo* scoreInfo, size_t count, std::string& strRound);
	virtual int UpdateStorageScore(int64_t changeStockScore);
	virtual bool GetStorageScore(tagStorageInfo& storageInfo);
	bool WriteGameChangeStorage(int64_t changeStockScore);
	static bool ReadStorageScore(tagGameRoomInfo* roomInfo);
	void KickUser(std::shared_ptr<IPlayer> const& player, int32_t kickType = KICK_GS | KICK_CLOSEONLY);
	bool SetOnlineInfo(int64_t userId);
	bool DelOnlineInfo(int64_t userId);
	virtual void RefreshRechargeScore(std::shared_ptr<IPlayer> const& player);
	virtual int64_t CalculateAgentRevenue(uint16_t chairId, int64_t revenue);
	virtual bool UpdateUserScoreToDB(int64_t userId, tagScoreInfo* pScoreInfo);
	virtual bool UpdateUserScoreToDB(int64_t userId, tagSpecialScoreInfo* pScoreInfo);
	bool AddUserGameInfoToDB(UserBaseInfo& userBaseInfo, tagScoreInfo* scoreInfo, std::string& strRoundId, size_t userCount, bool bAndroidUser = false);
	bool AddUserGameInfoToDB(tagSpecialScoreInfo* scoreInfo, std::string& strRoundId, size_t userCount, bool bAndroidUser = false);
	bool AddScoreChangeRecordToDB(UserBaseInfo& userBaseInfo, int64_t sourceScore, int64_t addScore, int64_t targetScore);
	bool AddScoreChangeRecordToDB(tagSpecialScoreInfo* scoreInfo);
	bool AddUserGameLogToDB(UserBaseInfo& userBaseInfo, tagScoreInfo* scoreInfo, std::string& strRoundId);
	bool AddUserGameLogToDB(tagSpecialScoreInfo* scoreInfo, std::string& strRoundId);
	virtual bool SaveReplay(tagGameReplay& replay);
	virtual bool SaveReplayRecord(tagGameRecPlayback& replay);
	bool SaveReplayDetailJson(tagGameReplay& replay);
	bool SaveReplayDetailBlob(tagGameReplay& replay);
public:
	STD::Weight weight_;
	uint8_t status_;
	TableState tableState_;
	tagGameRoomInfo* roomInfo_;
	ITableContext* tableContext_;
	std::vector<std::shared_ptr<CPlayer>> items_;//items_[chairId]
	std::shared_ptr<ITableDelegate> tableDelegate_;
	muduo::net::EventLoop* loop_;//桌子逻辑线程/定时器
public:
	static std::atomic_llong curStorage_; //系统当前库存
	static double lowStorage_;//系统最小库存，系统输分不得低于库存下限，否则赢分
	static double highStorage_;//系统最大库存，系统赢分不得大于库存上限，否则输分
	static double secondLowStorage_;
	static double secondHighStorage_;
	static int sysKillAllRatio_; //系统通杀率
	static int sysReduceRatio_;//系统库存衰减
	static int sysChangeCardRatio_;//系统换牌率
};

#endif