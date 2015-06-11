struct cs1550_sem
{
   int value;
   struct task_struct * queue[26];
   int inQueue;
   int id;
};

void add(struct cs1550_sem* semmy, struct task_struct * added)
{
	if (semmy->inQueue < 0 || semmy->inQueue > 26)	//if it's some garbage value
	{
		semmy->inQueue = 0;								//let it be 0
	}
	semmy->queue[semmy->inQueue] = added;	//place at end of the queue
	//printk("\t\tADDED %d AT INDEX %d\n", added->pid, semmy->inQueue);
	//printk("\t\tHERE'S WHAT'S THERE: %d\n", semmy->queue[semmy->inQueue]->pid);
	semmy->inQueue += 1;						//end of the queue will be one further back
	//printk("\t\tNEW INDEX IS %d\n", semmy->inQueue);
}

struct task_struct * take(struct cs1550_sem* semmy)
{
	struct task_struct * removed;
	int i;
	if (semmy->inQueue < 0 || semmy->inQueue > 26)		//if it's some garbage value
	{
		semmy->inQueue = 0;								//let it be 0
	}
	removed = semmy->queue[0];			//get what's at the front
	//printk("\t\tTOOK FROM INDEX 0: %d\n", removed->pid);
	i = 0;
	while (i <= (semmy->inQueue - 2))			//shift everything forward
	{
		semmy->queue[i] = semmy->queue[i + 1];
		//printk("\t\tINDEX %d: %d\n", i, semmy->queue[i]->pid);
		i += 1;
	}
	semmy->inQueue -= 1;						//end of the queue will be one further up
	//printk("\t\tNOW %d THINGS IN QUEUE\n", semmy->inQueue);
	
	return removed;								//return what we removed
}

DEFINE_SPINLOCK(sem_lock);

asmlinkage long sys_cs1550_down(struct cs1550_sem *sem)
{
        struct task_struct *curr = current;
        spin_lock(&sem_lock);
		//if (sem->id == 0)
			//printk("\tMUTEX DOWN\n");
		//else if (sem->id == 1)
			//printk("\tEMPTY DOWN\n");
		//else if (sem->id == 2)
			//printk("\tFULL DOWN\n");
        sem->value -= 1;
		//printk("\tVALUE IS NOW %d\n", sem->value);
        if (sem->value < 0)
        {
                //printk("\tSYS DOWN SAYS < 0\n");
                //printk("\tPID IS %d\n", curr->pid);
				add(sem, curr);						//enqueues the current process
				//printk("\tADDED %d TO THE QUEUE\n", curr->pid);
				set_current_state(TASK_INTERRUPTIBLE);	//mark the current task as not ready
				schedule();								//invoke the scheduler
        }

        spin_unlock(&sem_lock);
        return 0;
}

asmlinkage long sys_cs1550_up(struct cs1550_sem *sem)
{
		struct task_struct *curr;
        spin_lock(&sem_lock);
		//if (sem->id == 0)
			//printk("\tMUTEX UP\n");
		//else if (sem->id == 1)
			//printk("\tEMPTY UP\n");
		//else if (sem->id == 2)
			//printk("\tFULL UP\n");
        sem->value += 1;
		//printk("\tVALUE IS NOW %d\n", sem->value);
        if (sem->value <= 0)
        {
                //printk("\tSYS UP SAYS <= 0\n");
				curr = take(sem);					//get a waiting process from the queue
				//printk("\tGOT %d FROM THE QUEUE\n", curr->pid);
				wake_up_process(curr);				//wake it up
        }
        spin_unlock(&sem_lock);
        return 0;
}
