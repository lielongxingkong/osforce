#include "ring_api.h"

int init_ringbuf(struct ringbuf *ring, int len)
{
	int i;
	struct ringnode *ptr;
	INIT_LIST_HEAD(&ring->head);	
	for(i = 0; i < len; i++){
		ptr = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!ptr)
			return -1;
		memset(ptr, 0, sizeof(struct ringnode));
		ptr->seq = i;
		list_add_tail(&ptr->list, &ring->head);
	}

	init_rwsem(&ring->rwsem);	
	ring->entries = 0;
	ring->len = len;
	ring->front = ring->head.next;
	ring->rear = ring->front;
	return 0;	
}

int cleanup_ringbuf(struct ringbuf *ring)
{
	struct ringnode *next, *ptr;
	list_for_each_entry_safe(ptr, next, &ring->head, list) {
		list_del(&ptr->list);
		kfree(ptr);
	}
	return 0;
}

bool full(struct ringbuf *ring)
{
	if(ring->entries == ring->len)
		printk("full!\n");
	if(ring->entries == ring->len)
		return 1;
	else
		return 0;
}

/* front->next == rear */
bool empty(struct ringbuf *ring)
{
	if(ring->entries == 0)
		printk("empty!\n");
	if(ring->entries == 0)
		return 1;
	else
		return 0;
}

/* rear->next == front */
int push(struct ringbuf *ring, struct elem *e)
{
	return 0;
}

int pop(struct ringbuf *ring, struct elem *e)
{
	return 0;
}

/*call when full*/
int extend(struct ringbuf *ring, int extend_len)
{
	int i, orig_len;
	orig_len = ring->len;
	for(i = 0; i < extend_len; i++){
		struct ringnode *ptr;
		ptr = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!ptr)
			return -1;
		memset(ptr, 0, sizeof(struct ringnode));
		ptr->seq = i + orig_len;
		list_add_tail(&ptr->list, &ring->head);
		ring->len += 1;
	}
	return 0;
}

/*push no lock*/
int __push(struct ringbuf *ring, struct elem *e)
{
	struct ringnode *entry;
	entry = list_entry(ring->rear, struct ringnode, list);
	memcpy(&entry->e, e, sizeof(struct elem));	
	ring->entries += 1;
	if(full(ring)){
		if(extend(ring, EXTEND_LEN))
			return -1;
	}
	ring->rear = ring->rear->next;
	return 0;
}

/*pop no lock*/
int __pop(struct ringbuf *ring, struct elem *e)
{
	struct ringnode *entry;
	if(empty(ring))
		return -1;
	entry = list_entry(ring->front, struct ringnode, list);
	memcpy(e, &entry->e, sizeof(struct elem));	
	if(ring->front->next == &ring->head)
		ring->front = ring->front->next->next;
	else
		ring->front = ring->front->next;
	ring->entries -= 1;
	return 0;
}
