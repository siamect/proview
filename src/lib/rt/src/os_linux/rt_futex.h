/* rt_futex.h */

#ifndef rt_futex_h
#define rt_futex_h

#ifdef __cplusplus
extern "C" { 
#endif
    int futex_wait(int *futex, int val);
    
    int futex_timed_wait(int *futex, int val, const struct timespec * timespec);
    
    int futex_wake(int *futex,int nr);

#ifdef __cplusplus
} 
#endif

#endif
