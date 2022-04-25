#pragma once

#define OUT


/*---------------
	  Lock
---------------*/

//���� ������ ����Ҷ�
#define USE_MANY_LOCKS(count)	Lock _locks[count];
//���� �ϳ��� ����Ҷ�
#define USE_LOCK				USE_MANY_LOCKS(1)
// ������� read lock�� ���� ���̳�
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0)



//define�� �������ϱ� ���ؼ� \ �� �����

//=====================================================
//                Crash
//�����Ϸ����� nullptr�� �ƴ϶�� �����϶� ��û��
//�ڿ� ���� �־� �Ϻη� crash ��
//=====================================================

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

//===================================================
//             ���Ǻ� Crash 
//==================================================

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}