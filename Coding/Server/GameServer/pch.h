#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif

#include "CorePch.h"
#include "Enum.h"

const int RANGE = 10000;
const int BONFIRE_RANGE = 1700;

const int Tornado_id = 100;

struct timer_ev {

    int this_id;
    int target_id;
    chrono::system_clock::time_point   start_t;
    EVENT_TYPE order;
    constexpr bool operator < (const timer_ev& _Left) const
    {
        return (start_t > _Left.start_t);
    }
};

void error_display(int err_no);