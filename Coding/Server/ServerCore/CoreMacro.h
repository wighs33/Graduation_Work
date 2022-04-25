#pragma once
//define�� �������ϱ� ���ؼ� \ �� �����

//=====================================================
//                Crash
//�����Ϸ����� nullptr�� �ƴ϶�� �����϶� ��û��
//�ڿ� ���� �־� �Ϻη� crash ��
//=====================================================

#define CRASH(cause)                         \
{                                            \
  uint32* crash = nullptr;                   \
  __analysis_assume(crash != nullptr;);      \
  *crash = 0xDEADBEEF;                       \
}
//===================================================
//             ���Ǻ� Crash 
//==================================================
#define ASSERT_CRASH(expr)                       \
{                                                \
	if{!(expr))                                  \
    {                                            \
		CRASH("ASSERT_CRASH");                   \
       __analysis_assume(expr);                  \
	}                                            \
}