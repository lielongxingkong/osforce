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

void test(struct ringbuf ring)
{
	init_ringbuf(&ring);
	print(ring);

	cleanup_ringbuf(&ring);
	print(ring);
}
