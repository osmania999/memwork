#include "simrts.h"

double	power_consumed_cpu_active;
double	power_consumed_mem_active;
double	power_consumed_cpu_idle;
double	power_consumed_mem_idle;

void
calc_idle_power_consumed_task(task_t *task, unsigned idle)
{
	power_consumed_cpu_idle += (idle * cpufreqs[task->idx_cpufreq - 1].power_idle);
	power_consumed_mem_idle += (idle * mems[task->mem_type - 1].power_idle);
}

void
calc_idle_power_consumed(unsigned idle)
{
	struct list_head	*lp;

	if (idle == 0)
		return;
	list_for_each (lp, &tasks) {
		task_t	*task = list_entry(lp, task_t, list_sched);

		calc_idle_power_consumed_task(task, idle);
	}
}

void
calc_active_power_consumed(task_t *task, unsigned ret)
{
	power_consumed_cpu_active += (ret * cpufreqs[task->idx_cpufreq - 1].power_active);
	power_consumed_mem_active += (ret * mems[task->mem_type - 1].power_active);

	/* currently task is excluded from the list */
	ASSERT(list_empty(&task->list_sched));
	calc_idle_power_consumed(ret);
}
