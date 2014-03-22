#include <ring_api.h>

void print(struct ringbuf ring)
{
	struct ringnode *entry;
	int len = 0;
	printk("[DEBUG start] len %d ---------------\n", len);
	list_for_each_entry(entry, ring.head, list){
		len++;
		printk("[DEBUG] %d\n", entry->e.data);
	}
	printk("[DEBUG end] %d ----------------\n", len);
}

void print_circle(struct ringbuf ring, int times)
{
	int i;
	struct list_head *ptr;
	ptr = ring.head;
	for(i = 0; i < times * INIT_LEN; i++){
		struct ringnode *entry;
		entry = list_entry(ptr, struct ringnode, list);
		printk("[DEBUG circle]%d:%d\n", i, entry->e.data);
		ptr = ptr->next;
	}
}

void test_extend(struct ringbuf *ring, int push_times)
{
	int i, ret;
	for(i = 0; i < push_times; i++){
		struct elem e;
		e.data = i + 80000;
		ret = push(ring, &e);
	}

}

void test(struct ringbuf ring)
{
	init_ringbuf(&ring, INIT_LEN);
	print(ring);
	//print_circle(ring, 2);
	test_extend(&ring, 10);
	print(ring);

	cleanup_ringbuf(&ring);
	print(ring);
}
