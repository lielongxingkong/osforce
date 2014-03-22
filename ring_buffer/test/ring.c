#include <ring_api.h>

void print(struct ringbuf ring)
{
	struct ringnode *entry;
	printk("[DEBUG start] ---------------\n");
	list_for_each_entry(entry, ring.head, list){
		printk("[DEBUG] %d\n", entry->e.data);
	}
	printk("[DEBUG end] ----------------\n");
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

void test(struct ringbuf ring)
{
	init_ringbuf(&ring);
	print(ring);
	print_circle(ring, 2);

	cleanup_ringbuf(&ring);
	print(ring);
}
