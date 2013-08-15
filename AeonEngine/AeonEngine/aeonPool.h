//
//  aeonPool.h
//  AeonEngine
//
//  Created by Isaac Turner on 7/27/13.
//  Copyright (c) 2013 Isaac Turner. All rights reserved.
//

#ifndef AeonEngine_aeonPool_h
#define AeonEngine_aeonPool_h

// All things that live in pools should derive from this
struct PooledResource
{
    bool isAlive = false;
};

typedef unsigned int aeon_Handle;
#define POOL_MAX 512

//------------------------------------
// Pool Class
//------------------------------------

template <class T>
class aeonPool
{
private:
    static unsigned int numPools;
    unsigned int poolKey;
public:
    T storage[POOL_MAX];
    
    aeonPool();
    ~aeonPool();
    unsigned int unmangleHandle(aeon_Handle h);
    aeon_Handle mangleHandle(unsigned int i);
    T* getElement(aeon_Handle h);
    void killElement(aeon_Handle h);
    unsigned int getNextEmptySlot();
    void drain();
};

//------------------------------------
// Methods
//------------------------------------

#define MANGLE_KEY_BASE 0x41656f6e

template <class T>
aeonPool<T>::aeonPool()
{
    poolKey = MANGLE_KEY_BASE + (POOL_MAX * numPools);
    numPools++;
    for(int i = 0; i < POOL_MAX; i++)
    {
        storage[i].isAlive = false;
    }
}

template <class T>
aeonPool<T>::~aeonPool()
{
    numPools--;
}

template <class T>
unsigned int aeonPool<T>::unmangleHandle(aeon_Handle h)
{
    return h ^ poolKey;
}

template <class T>
aeon_Handle aeonPool<T>::mangleHandle(unsigned int i)
{
    return i ^ poolKey;
}

template <class T>
T* aeonPool<T>::getElement(aeon_Handle h)
{
    T* pr = &storage[unmangleHandle(h)];
    assert(pr->isAlive);
    return pr;
}

template <class T>
void aeonPool<T>::killElement(aeon_Handle h)
{
    PooledResource* pr = ((PooledResource*)&storage[unmangleHandle(h)]);
    assert(pr->isAlive);
    pr->isAlive = false;
}

template <class T>
unsigned int aeonPool<T>::getNextEmptySlot()
{
    unsigned int emptySlot = 0;
    for(int i = 0; i < POOL_MAX; i++)
    {
        if(!storage[i].isAlive)
        {
            emptySlot = i;
            break;
        }
        assert(i < POOL_MAX - 1); // ran out of pool!
    }
    
    return emptySlot;
}

template <class T>
void aeonPool<T>::drain()
{
    for(int i = 0; i < POOL_MAX; i++)
    {
        storage[i].isAlive = false;
    }
}

template <class T>
unsigned int aeonPool<T>::numPools = 0;

#endif
