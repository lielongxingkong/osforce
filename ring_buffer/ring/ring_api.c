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
		ptr->e.data = i;
		list_add_tail(&ptr->list, &ring->head);
	}

	init_rwsem(&ring->rwsem);	
	ring->entries = len;
	ring->front = ring->head.next;
	ring->rear = ring->front->next;
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
	if(ring->rear == ring->front)
		return 1;
	else
		return 0;
}

/* front->next == rear */
bool empty(struct ringbuf *ring)
{
	if(ring->front->next == ring->rear)
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
	int i;
	for(i = 0; i < extend_len; i++){
		struct ringnode *ptr;
		ptr = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!ptr)
			return -1;
		memset(ptr, 0, sizeof(struct ringnode));
		ptr->e.data = i + ring->entries;
		list_add(&ptr->list, &ring->head);
		ring->entries++;
	}
	return 0;
}

/*push no lock*/
int __push(struct ringbuf *ring, struct elem *e)
{
	struct ringnode *entry;
	if(full(ring))
		if(extend(ring, EXTEND_LEN))
			return -1;
	entry = list_entry(ring->rear->next, struct ringnode, list);
	memcpy(&entry->e, e, sizeof(struct elem));	
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
	ring->front = ring->front->next;
	return 0;
}
