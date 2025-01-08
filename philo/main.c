#include "Philosophers.h"

int check_valid_args(char **av)
{
    int i;
	int f;
	size_t check;

	f = 1;
	while(av[f])
	{	
		check = 0;
    	i = 0;
		while(av[f][i])
		{
			if(av[f][i] >= '0' && av[f][i] <= '9')
				check++;
			i++;
		}
		if(ft_strlen(av[f]) != check)
			return (1);
		f++;
	}
	return (0);
}
void philo_struct_int(t_philo *philo, char **av)
{	
	int i;
	int holder;

	i = 1;
	philo = malloc(sizeof(t_philo));
	if(!philo)
		perror("Malloc Fail");
	while(av[i])
	{	
		holder = ft_atoi(av[i]);
		printf("holder: %d\n",holder);
		if(i == 1)
			philo->num_of_philos = holder;
		else if(i == 2)
			philo->time_to_die = holder;
		else if(i == 3)
			philo->time_to_eat = holder;
		else if(i == 4)
			philo->time_to_sleep = holder;
		else if(i == 5)
			philo->num_times_to_eat = holder;
		i++;
	}
}
//check av if valid arguments
// convert av to numbers
/* make a struct with these values
Number of philos
time a philo will die if he does not eat
time for philo to eat
time for philo to sleep
number of times philo need to eat vefore ending the program
*/
int main(int ac, char **av)
{   
	t_philo *philo;

    if(ac == 5 || ac == 6)
    {
        printf("welcome to philo\n");
		if(!check_valid_args(av))
		{
			philo_struct_int(philo, av);
		}
    }
    else
        printf("enter valid arguments :)\n");
}