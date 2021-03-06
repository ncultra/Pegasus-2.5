//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
// Author: Mike Day (mdday@us.ibm.com) and Roger Kumpf (roger_kumpf@hp.com)
//
// Modified By: Keith Petley (keithp@veritas.com)
//
//%/////////////////////////////////////////////////////////////////////////////


#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>


typedef sem_t PEGASUS_SEMAPHORE_TYPE;
typedef pthread_t PEGASUS_THREAD_TYPE;
typedef pthread_mutex_t PEGASUS_MUTEX_TYPE;

typedef struct {
    sem_t sem;
    pthread_t owner;
} PEGASUS_SEM_HANDLE ;

typedef struct {
    pthread_mutex_t mut;
    pthread_mutexattr_t mutatt;
    pthread_t owner;
} PEGASUS_MUTEX_HANDLE ;

typedef PEGASUS_MUTEX_HANDLE  PEGASUS_CRIT_TYPE;

typedef void *PEGASUS_CLEANUP_HANDLE ;
typedef void *PEGASUS_THREAD_RETURN;

#define PEGASUS_THREAD_CDECL

typedef struct {
    pthread_t thid;
    pthread_attr_t thatt;
} PEGASUS_THREAD_HANDLE ;

//-----------------------------------------------------------------
/// Conditionals to support native or generic Conditional Semaphore
//-----------------------------------------------------------------

#define PEGASUS_CONDITIONAL_NATIVE

typedef pthread_cond_t PEGASUS_COND_TYPE;

typedef struct {
    pthread_cond_t cond;
    pthread_t owner;
} PEGASUS_COND_HANDLE;


//-----------------------------------------------------------------
/// Conditionals to support native or generic atomic variables
//-----------------------------------------------------------------

// #define PEGASUS_ATOMIC_INT_NATIVE


//-----------------------------------------------------------------
/// Conditionals to support native or generic read/write semaphores
//-----------------------------------------------------------------

#ifndef SUNOS_5_6
#define PEGASUS_READWRITE_NATIVE

typedef struct {
    pthread_rwlock_t rwlock;
    pthread_t owner;
} PEGASUS_RWLOCK_HANDLE;
#endif //SUNOS_5_6

PEGASUS_NAMESPACE_BEGIN
inline void pegasus_yield(void)
{
      sched_yield();
}


// pthreads cancellation calls 
inline void disable_cancel(void)
{
   pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
}

inline void enable_cancel(void)
{
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

inline void pegasus_sleep(int msec)
{
    struct timespec wait;
    wait.tv_sec = msec / 1000;
    wait.tv_nsec = (msec % 1000) * 1000000;
    nanosleep(&wait, NULL);
}

static inline int pegasus_gettimeofday(struct timeval *tv)
{
	return(gettimeofday(tv, NULL));
}

inline void exit_thread(PEGASUS_THREAD_RETURN rc)
{
  pthread_exit(rc);
}

inline PEGASUS_THREAD_TYPE pegasus_thread_self(void) 
{ 
   return(pthread_self());
}

// l10n start
typedef pthread_key_t PEGASUS_THREAD_KEY_TYPE;

inline Uint32 pegasus_key_create(PEGASUS_THREAD_KEY_TYPE * key)
{
	// Note: a destructor is not supported 
	// (because not supported on Windows (?))
	return pthread_key_create(key, NULL);
} 

inline Uint32 pegasus_key_delete(PEGASUS_THREAD_KEY_TYPE key)
{
	return pthread_key_delete(key);
} 

inline void * pegasus_get_thread_specific(PEGASUS_THREAD_KEY_TYPE key)
{
	return pthread_getspecific(key);
} 

inline Uint32 pegasus_set_thread_specific(PEGASUS_THREAD_KEY_TYPE key,
										 void * value)
{
	return pthread_setspecific(key, value);
} 
// l10n end

inline void destroy_thread(PEGASUS_THREAD_TYPE th, PEGASUS_THREAD_RETURN rc)
{
   pthread_cancel(th);
}


PEGASUS_NAMESPACE_END
