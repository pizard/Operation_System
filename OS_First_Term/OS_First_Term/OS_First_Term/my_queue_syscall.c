#include <linux/kernel.h>
#include <linux/linkage.h>

int array[100];
int head = 0;
int tail = 0;

asmlinkage long sys_choi_enqueue(int n) {
	int check = 0;
	for (int a = head; a < tail; a++) {
		if (array[a] == n) {
			check = 1;
		}
	}
	if(check == 0){
		array[tail] = n;
		tail++;
	}
	printk(KERN_ALERT "enqueue success.");
	return 0;
}

asmlinkage long sys_choi_dequeue(void) {

	printk(KERN_ALERT "dequeue number: %d", array[head]);
	head++;
	return 0;

}
