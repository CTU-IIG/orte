/*
 *  $Id: rwlock.h,v 0.0.0.1             2003/12/19 
 *
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 */

#ifndef _RWLOCK_H
#define _RWLOCK_H

#define PTW32_RWLOCK_MAGIC 0xfacade2
#define PTHREAD_RWLOCK_INITIALIZER ((pthread_rwlock_t) -1)

typedef struct pthread_rwlock_t_ *pthread_rwlock_t;
typedef struct pthread_rwlockattr_t_ *pthread_rwlockattr_t;
struct pthread_rwlock_t_ {
  pthread_mutex_t   mtxExclusiveAccess;
  pthread_mutex_t   mtxSharedAccessCompleted;
  pthread_cond_t    cndSharedAccessCompleted;
  int               nSharedAccessCount;
  int               nExclusiveAccessCount;
  int               nCompletedSharedAccessCount;
  int               nMagic;
};
	      
struct pthread_rwlockattr_t_ {
  int               pshared;
};
extern void ptw32_rwlock_cancelwrwait(void * arg);

extern int
ptw32_rwlock_check_need_init(pthread_rwlock_t *rwlock);

extern int pthread_rwlock_init(pthread_rwlock_t *lock,
 			       const pthread_rwlockattr_t *attr);

extern int pthread_rwlock_destroy(pthread_rwlock_t *lock);

extern int pthread_rwlock_tryrdlock(pthread_rwlock_t *);

extern int pthread_rwlock_trywrlock(pthread_rwlock_t *);

extern int pthread_rwlock_rdlock(pthread_rwlock_t *lock);

extern int pthread_rwlock_timedrdlock(pthread_rwlock_t *lock,
  			              const struct timespec *abstime);

extern int pthread_rwlock_wrlock(pthread_rwlock_t *lock);

extern int pthread_rwlock_timedwrlock(pthread_rwlock_t *lock,
				      const struct timespec *abstime);

extern int pthread_rwlock_unlock(pthread_rwlock_t *lock);

extern int pthread_rwlockattr_init(pthread_rwlockattr_t * attr);

extern int pthread_rwlockattr_destroy(pthread_rwlockattr_t * attr);

extern int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t * attr,
					  int *pshared);

extern int pthread_rwlockattr_setpshared (pthread_rwlockattr_t * attr,
					  int pshared);


#endif /* _RWLOCK_H */
