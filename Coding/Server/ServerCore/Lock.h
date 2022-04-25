#pragma once
#include "Types.h"

/*----------------
    RW SpinLock
-----------------*/

/*--------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)
---------------------------------------------*/

//write lock�� ���� ���¿��� read lock�� ������ ����
// write lock�� ���� ���¿��� �ٽ� write�� ���� ���� ����
//�� read lock�� ���� ���¿��� write lock�� ������ �ȵ�

class Lock
{
    enum : uint32                        //32��Ʈ ������ 16��Ʈ�� ���� ���� 
    { 
        ACQUIRE_TIMEOUT_TICK = 10000,    //�ִ� tick
        MAX_SPIN_COUNT = 5000,           //�ִ� spinlock ī��Ʈ
        WRITE_THREAD_MASK = 0xFFFF'0000, //write �ϴ� ������ id    
        READ_COUNT_MASK = 0x0000'FFFF,   //read ī��Ʈ
        EMPTY_FLAG = 0x0000'0000
    };

public:
    void WriteLock();
    void WriteUnlock();
    void ReadLock();
    void ReadUnlock();

private:
    Atomic<uint32> _lockFlag = EMPTY_FLAG;
    uint16 _writeCount = 0;
};

/*----------------
    LockGuards
-----------------*/

class ReadLockGuard
{
public:
    ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
    ~ReadLockGuard() { _lock.ReadUnlock(); }

private:
    Lock& _lock;
};

class WriteLockGuard
{
public:
    WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
    ~WriteLockGuard() { _lock.WriteUnlock(); }

private:
    Lock& _lock;
};