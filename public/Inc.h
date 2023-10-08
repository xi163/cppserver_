#ifndef INCLUDE_INC_H
#define INCLUDE_INC_H

#include "Logger/src/utils/utils.h"

#include "IncMuduo.h"

#include <google/protobuf/message.h>

#include "zookeeperclient/zookeeperclient.h"
#include "zookeeperclient/zookeeperlocker.h"
#include "MongoDB/MongoDBOperation.h"
#include "RedisClient/RedisClient.h"
#include "RedisLock/redlock.h"
#include "TraceMsg/TraceMsg.h"

#define REDISCLIENT muduo::ThreadLocalSingleton<RedisClient>::instance()

#endif