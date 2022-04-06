#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif

#include "CorePch.h"
#include "Enum.h"

const int RANGE = 1000;

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