#include "Philosophers.h"

long get_time(t_time t_code)
{
    struct timeval tv;

    if(gettimeofday(&tv, NULL))
        exit_err("time day failed\n");
    if(t_code == SECOND)
        return(tv.tv_sec + (tv.tv_usec / 1e6));
    else if(t_code == MILLISECOND)
        return((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
    else if(t_code == MICROSECOND)
        return((tv.tv_sec * 1e6) + tv.tv_usec);
    else
        exit_err("wrong input in gettime\n");
    return(0);
}