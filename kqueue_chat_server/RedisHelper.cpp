#include "RedisHelper.h"
#include <hiredis/hiredis.h>  // hiredis 라이브러리

RedisHelper::RedisHelper() {
    // Redis 서버에 연결
    context = redisConnect("127.0.0.1", 6379);
    if (context == NULL || context->err) {
        std::cerr << "Connection error: " << context->errstr << std::endl;
        exit(1);
    }
    std::cout << "Connected to Redis" << std::endl;
}

RedisHelper::~RedisHelper() {
    // Redis 연결 해제
    if (context) {
        redisFree(context);
        std::cout << "Disconnected from Redis" << std::endl;
    }
}

bool RedisHelper::set(std::string name, std::string chat) {
    // Redis에 데이터 저장 (SET key value)
    redisReply* reply = (redisReply*)redisCommand(context, "SET %s %s", name.c_str(), chat.c_str());
    if (reply == NULL) {
        std::cerr << "SET command failed" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

std::string RedisHelper::get(std::string name) {
    // Redis에서 데이터 가져오기 (GET key)
    redisReply* reply = (redisReply*)redisCommand(context, "GET %s", name.c_str());
    if (reply == NULL || reply->type == REDIS_REPLY_NIL) {
        std::cerr << "GET command failed or key not found" << std::endl;
        return "";
    }
    std::string value = reply->str;
    freeReplyObject(reply);
    return value;
}

