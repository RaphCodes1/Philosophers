#include "Philosophers.h"

void mutex_err(int status, t_operation opcode)
{
	if(status == 0)
		return ;
	if(EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		ft_putstr_fd("The value specified by mutex is invalid.\n", 2);
	else if(EINVAL == status && INIT == opcode)
		ft_putstr_fd("The value specified by attr is invalid.\n", 2);
	else if(EDEADLK == status)
	{
		ft_putstr_fd("A deadlock would occur if the thread ", 2);
		ft_putstr_fd("blocked waiting for mutex.\n", 2);
	}
	else if(EPERM == status)
		ft_putstr_fd("The current thread does not hold a lock on mutex\n", 2);
	else if(ENOMEM == status)
	{
		ft_putstr_fd("The process cannot allocate ", 2);
		ft_putstr_fd("enough memory to create another mutex.\n", 2);

	}
	else if(EBUSY == status)
		ft_putstr_fd("Mutex is locked\n", 2);
}

void mutex_handle(pthread_mutex_t *mutex, t_operation opcode)
{
	if(opcode == INIT)
		mutex_err(pthread_mutex_init(mutex, NULL), INIT);
	else if(opcode == LOCK)
		mutex_err(pthread_mutex_lock(mutex), LOCK);
	else if(opcode == UNLOCK)
		mutex_err(pthread_mutex_unlock(mutex), UNLOCK);
	else if(opcode == DESTROY)
		mutex_err(pthread_mutex_destroy(mutex), DESTROY);
	else
		printf("wrong opcode for mutex_handling\n");
}

void thread_err(int status, t_operation opcode)
{
	if(status == 0)
		return ;
	if(EAGAIN == status)
		ft_putstr_fd("No resources to create another thread\n.", 2);
	else if(EINVAL == status && opcode == CREATE)
		ft_putstr_fd("The caller does not have appropiate permission\n.", 2);
	else if(EINVAL == status && (opcode == JOIN || opcode == DETATCH))
		ft_putstr_fd("The value specified by thread is not joinable\n", 2);
	else if(ESRCH == status)
	{
		ft_putstr_fd("No thread could be found corresponding to that", 2);
		ft_putstr_fd("specified by the given thread ID, thread.\n", 2);
	}
	else if(EDEADLK == status)
	{
		ft_putstr_fd("A deadlock was detected or the value", 2);
		ft_putstr_fd("of thread specifies the calling thread.\n", 2);
	}
}

void thread_handle(pthread_t *thread, void *(*func)(void *), 
	void *data, t_operation opcode)
{
	if(opcode == CREATE)
		thread_err(pthread_create(thread, NULL, func, data), CREATE);
	else if(opcode == JOIN)
		thread_err(pthread_join(*thread, NULL), JOIN);
	else if(opcode == DETATCH)
		thread_err(pthread_detach(*thread), DETATCH);
	else
		printf("wrong opcode for pthread_handling\n");
}