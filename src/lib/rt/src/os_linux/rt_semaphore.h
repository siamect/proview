/* rt_sect.h */

/*-< SEMAPHORE.H >--------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Semaphore interface                                              *        */
/*------------------------------------------------------------------*--------*/

#ifndef rt_semaphore_h
#define rt_semaphore_h

#ifdef __cplusplus
extern "C" { 
#endif


    typedef struct {
	long  semkey;
	int   semid;
	int   initialized;
    } sem_t;

    
    // Not POSIX. The caller generates the key
    int posix_sem_init_shared(sem_t *sem, int key, unsigned int value);
    
    int posix_sem_init(sem_t *sem, int pshared, unsigned int value);

    sem_t *posix_sem_open(const char *name, int oflag, ...);

    int posix_sem_post(sem_t *sem);

    int posix_sem_getvalue(sem_t *sem, int *sval);

    int posix_sem_wait(sem_t *sem);

    int posix_sem_trywait(sem_t *sem);

    int posix_sem_unlink(const char *name);

    int posix_sem_close(sem_t *sem);

    int posix_sem_destroy(sem_t *sem);


#ifdef __cplusplus
} 
#endif

#endif
