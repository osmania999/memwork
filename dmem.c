#include "simrts.h"

static BOOL
dmem_assign_task(task_t *task)
{
	mem_type_t	mem_types_try[] = { MEMTYPE_DRAM, MEMTYPE_NVRAM };
	int	i;

	task->idx_cpufreq = 1;
	for (i = 0; i < 2; i++) {
		if (assign_mem(task, mem_types_try[i]))
			return TRUE;
	}
	return FALSE;
}

static BOOL
dmem_reassign_task(task_t *task)
{
	mem_type_t	mem_types_try[] = { MEMTYPE_NVRAM, MEMTYPE_DRAM };
	int	i;

	revoke_mem(task);

	for (i = 0; i < 2; i++) {
		if (assign_mem(task, mem_types_try[i])) {
			calc_task_det(task);
			if (is_schedulable(task))
				return TRUE;
			revert_task_det(task);
		}
	}
	return FALSE;
}

policy_t	policy_dmem = {
	"dmem",
	dmem_assign_task,
	dmem_reassign_task
};
