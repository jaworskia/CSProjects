//Adam Jaworski
//CS 1550
//Project 2
//October 5, 2014

struct cs1550_sem 					//we declare the struct here as well, so the syscalls know what a semaphore is
{
   int value;						//the value of the semaphore
   struct task_struct * queue[52];	//queue of processes, implemented as an array
   
   //important to note: queue can only hold 52 processes
   //but this ought to be sufficient, so long as the producers and consumers don't extend beyond 'Z'
   
   int inQueue;						//the number of items in the queue
   int id;							//used for debugging purposes
};

void add(struct cs1550_sem* semmy, struct task_struct * added)	//adds added to semmy's queue
{
	if (semmy->inQueue < 0 || semmy->inQueue > 26)	//if it's some garbage value
	{
		semmy->inQueue = 0;								//let it be 0
	}
	semmy->queue[semmy->inQueue] = added;	//place at end of the queue
	semmy->inQueue += 1;						//end of the queue will be one further back
}

struct task_struct * take(struct cs1550_sem* semmy)		//dequeues a process from semmy's queue
{
	struct task_struct * removed;						//what we'll be returning
	int i;
	if (semmy->inQueue < 0 || semmy->inQueue > 26)		//if it's some garbage value
	{
		semmy->inQueue = 0;								//let it be 0
	}
	removed = semmy->queue[0];			//get what's at the front
	i = 0;
	while (i <= (semmy->inQueue - 2))			//shift everything forward
	{
		semmy->queue[i] = semmy->queue[i + 1];
		i += 1;
	}
	semmy->inQueue -= 1;						//end of the queue will be one further up
	
	return removed;								//return what we removed
}

DEFINE_SPINLOCK(sem_lock);					//defines the spin lock

asmlinkage long sys_cs1550_down(struct cs1550_sem *sem)		//the down syscall implementation
{
        struct task_struct *curr = current;					//gets the current process
        spin_lock(&sem_lock);								//acquires lock
        sem->value -= 1;									//decrement the value of the passed semaphore
        if (sem->value < 0)							//it it is now less than 0
        {
				add(sem, curr);						//enqueues the current process
				set_current_state(TASK_INTERRUPTIBLE);	//mark the current task as not ready
				schedule();								//invoke the scheduler
        }

        spin_unlock(&sem_lock);								//releases lock
        return 0;											//just returns 0
}

asmlinkage long sys_cs1550_up(struct cs1550_sem *sem)		//the up syscall implementation
{
		struct task_struct *curr;			//declares a task_struct
        spin_lock(&sem_lock);				//acquires lock
        sem->value += 1;					//increment the value of the passed semaphore
        if (sem->value <= 0)				//if it's still less than or equal to 0 (means there are still processes waiting in the queue)
        {
				curr = take(sem);					//get a waiting process from the queue
				wake_up_process(curr);				//wake it up
        }
        spin_unlock(&sem_lock);				//releases lock
        return 0;							//just returns 0
}
