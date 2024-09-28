#pragma once
#include <iostream>
#include <string>
#include <hiredis/hiredis.h>

class RedisHelper {
public:
    static RedisHelper& getInstance() {
        static RedisHelper instance;  // 싱글톤 인스턴스
        return instance;
    }

    bool set(std::string name, std::string chat);   // 데이터 저장
    std::string get(std::string name);              // 데이터 조회

private:
    RedisHelper();  // 생성자
    ~RedisHelper(); // 소멸자

    redisContext* context;  // Redis 연결을 위한 컨텍스트

    // 싱글톤이므로 복사 및 대입 금지
    RedisHelper(const RedisHelper&) = delete;
    RedisHelper& operator=(const RedisHelper&) = delete;
};


