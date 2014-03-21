#include "ring_api.h"

int init_ringbuf(struct ringbuf *ring)
{
	int i;
	struct rw_semaphore rwsem;
	struct ringnode *ring_head;
	ring_head = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
	INIT_LIST_HEAD(&ring_head->list);	
	for(i = 1; i < INIT_LENGTH; i++){
		struct ringnode *nd = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!IS_ERR(nd)){
			struct elem e;
			e.data = i;
			nd->e = e;
			list_add(&nd->list, &ring_head->list);
		} else {
			printk("Out of memeory!\n");
			return -1;
		}
	}

	init_rwsem(&rwsem);	
	ring->head = ring_head;
	ring->entries = INIT_LENGTH;
	ring->front = ring_head;
	ring->rear = ring_head;
	ring->rwsem = rwsem;
	return 0;	
}

int clean_ringbuf(struct ringbuf *ring)
{
	return 0;
}

bool full(struct ringbuf *ring)
{
	return 0;
}

bool empty(struct ringbuf *ring)
{
	return 0;
}

int push(struct ringbuf *ring, struct elem e)
{
	return 0;
}

struct elem pop(struct ringbuf *ring)
{
	struct elem e;
	e.data = 1;
	return e;
}

/*call when full*/
int extend(struct ringbuf *ring, int extend_len)
{
	return 0;
}

/*push no lock*/
int __push(struct ringbuf *ring, struct elem e)
{
	return 0;
}

/*pop no lock*/
struct elem __pop(struct ringbuf *ring)
{
	struct elem e;
	e.data = 1;
	return e;
}
