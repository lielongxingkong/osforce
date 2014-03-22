#include <ring_api.h>

void print(struct ringbuf *ring)
{
	struct ringnode *entry;
	int len = 0;
	printk("[DEBUG start] len %d ---------------\n", len);
	list_for_each_entry(entry, &ring->head, list){
		len++;
		printk("[DEBUG] [%d]%d\n", entry->seq, entry->e.data);
	}
	printk("[DEBUG end] %d ----------------\n", len);
}

void print_circle(struct ringbuf *ring, int times)
{
	int i;
	struct list_head *ptr;
	ptr = ring->head.next;
	for(i = 0; i < times * (ring->len); i++){
		struct ringnode *entry;
		entry = list_entry(ptr, struct ringnode, list);
		printk("[DEBUG circle]%d [%ld]  [%d]%d\n", i, (long)ptr, entry->seq, entry->e.data);
		ptr = ptr->next;
	}
}

#define	FRONT	0
#define	REAR	1

int get_p_seq(struct ringbuf *ring, int type)
{
	struct ringnode *ptr;
	if(type == FRONT)
		ptr = list_entry(ring->front, struct ringnode, list);
	else
		ptr = list_entry(ring->rear, struct ringnode, list);
	return ptr->seq;
}

void test_push_extend(struct ringbuf *ring, int push_times)
{
	int i, ret;
	int f, r;
	for(i = 0; i < push_times; i++){
		struct elem e;
		e.data = i + 80000;
		ret = __push(ring, &e);
		f = get_p_seq(ring, FRONT);
		r = get_p_seq(ring, REAR);
		printk("i: %d, entries:%d, front: %d, rear: %d\n", i, ring->entries, f, r);
	}

}

void test_push(struct ringbuf *ring)
{
	int i, ret;
	for(i = 0; i < INIT_LEN; i++){
		struct elem e;
		e.data = i + 80000;
		ret = __push(ring, &e);
	}

}

void test_pop(struct ringbuf *ring){
	int i, ret;
	for(i = 0; i < INIT_LEN + 3; i++){
		struct elem e;
		ret = __pop(ring, &e);
		if(ret)
			printk("[POP] empty\n");
		else
			printk("[POP] %d\n", e.data);
	}
}

void test_extend(struct ringbuf *ring){
	extend(ring, EXTEND_LEN);
}

void test(struct ringbuf *ring)
{
	//int f, r;
	init_ringbuf(ring, INIT_LEN);
	//f = get_p_seq(ring, FRONT);
	//r = get_p_seq(ring, REAR);
	//print(ring);
	//printk("[INIT] front: %d, rear: %d\n", f, r);
	//printk("ringbuf len:%d, entries:%d\n", ring->len, ring->entries);
	//test_extend(ring);
	//test_push_extend(ring, 15);
	test_push(ring);
	test_pop(ring);
	print(ring);
	printk("ringbuf len:%d, entries:%d\n", ring->len, ring->entries);
	//print_circle(ring, 2);
	cleanup_ringbuf(ring);
}
