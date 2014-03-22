#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <ring_api.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhao Zhenlong");
MODULE_DESCRIPTION("Ring buffer module.");

#define MAX_THREADS	10

static struct ringbuf ring;

static unsigned long reader_bitmap;

static void set_reader_number(int reader){
	reader_bitmap = 0;
	while(reader){
		reader_bitmap |= (1 << --reader);
	}
}

struct task_struct *threads[MAX_THREADS];
static DEFINE_MUTEX(mut);

static void reader_do(void){
	return;
}

static void writer_do(void){
	mutex_lock(&mut);
	mutex_unlock(&mut);
}

static int thread_do(void *data){
	long i = (long)data;
	int reader = (reader_bitmap & (1 << i));

	printk("run..., %ld %s\n", i, reader? "reader" : "writer");
	while(!kthread_should_stop()){
		if(reader)
			reader_do();
		else 
			writer_do();
		msleep(10);
	}
	return 0;
}

static int create_threads(void){
	int i;
	for(i = 0; i < MAX_THREADS; i++){
		struct task_struct *thread;
		thread = kthread_run(thread_do, (void *)(long)i, "thread-%d", i);
		if(IS_ERR(thread))
			return -1;
		threads[i] = thread;
	}
	return 0;
}

static int cleanup_threads(void){
	int i;
	for(i = 0; i < MAX_THREADS; i++){
		kthread_stop(threads[i]);	
	}
	return 0;
}

static void test(void)
{
	struct ringnode *entry;
	printk("[DEBUG start] ---------------\n");
	list_for_each_entry(entry, &(ring.head), list){
		printk("[DEBUG] %d\n", entry->e.data);
	}
	printk("[DEBUG end] ----------------\n");
}

static __init int minit(void){
	printk("call %s\n", __FUNCTION__); 
	set_reader_number(0);
	if(create_threads())
		goto err;
	init_ringbuf(&ring);
	test();
	return 0;
err:
	cleanup_threads();
	return -1;
}

static __exit void mexit(void){
	printk("call %s\n", __FUNCTION__);
	cleanup_ringbuf(&ring);
	test();
	cleanup_threads();
}
module_init(minit);
module_exit(mexit);
