#include <linux/kernel.h>
#include <linux/linkage.h>


int faster_PID;
int slower_PID;

asmlinkage long sys_benchmark_F(int pid) {  // 318

	faster_PID = pid;

	return 0;
};
EXPORT_SYMBOL(faster_PID);


asmlinkage long	sys_benchmark_S(int pid) {	// 319

	slower_PID = pid;

	
	return 0;
};
EXPORT_SYMBOL(slower_PID);
