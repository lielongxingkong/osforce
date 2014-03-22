#include "ring_api.h"

int init_ringbuf(struct ringbuf *ring)
{
	int i;
	struct ringnode *head;
	head = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
	if(!head)
		return -1;
	memset(head, 0, sizeof(struct ringnode));
	head->e.data = 0;
	INIT_LIST_HEAD(&head->list);	
	for(i = 1; i < INIT_LEN; i++){
		struct ringnode *ptr;
		ptr = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!ptr)
			return -1;
		memset(ptr, 0, sizeof(struct ringnode));
		ptr->e.data = i;
		list_add(&ptr->list, &head->list);
	}

	init_rwsem(&ring->rwsem);	
	ring->entries = INIT_LEN;
	ring->head = &head->list;
	ring->front = ring->head;
	ring->rear = ring->head;
	return 0;	
}

int cleanup_ringbuf(struct ringbuf *ring)
{
	struct ringnode *next, *ptr;
	list_for_each_entry_safe(ptr, next, ring->head, list) {
		list_del(&ptr->list);
		kfree(ptr);
	}
	return 0;
}

bool full(struct ringbuf *ring)
{
	if(ring->rear->next == ring->front)
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
	int i;
	for(i = 0; i < extend_len; i++){
		struct ringnode *ptr;
		ptr = (struct ringnode*)kmalloc(sizeof(struct ringnode), GFP_KERNEL);
		if(!ptr)
			return -1;
		memset(ptr, 0, sizeof(struct ringnode));
		ptr->e.data = i + ring->entries;
		list_add(&ptr->list, ring->head);
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
	entry->e.data = e->data;	
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
	e->data = entry->e.data;	
	ring->front = ring->front->next;
	return 0;
}
