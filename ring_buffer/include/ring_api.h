#include <linux/list.h>
#include <linux/rwsem.h>
struct elem{
	int data;
};

struct node{
	struct elem;
	struct list_head list;
};

struct ringbuf{
	struct node *buf;
	int entries;
	struct node *front;
	struct node *rear;
	struct rw_semaphore;
};

int init_ringbuf(struct *ringbuf ring);
int clean_ringbuf(struct *ringbuf ring);
bool full(struct *ringbuf ring);
bool empty(struct *ringbuf ring);
int push(struct *ringbuf ring, struct elem e);
struct elem pop(struct *ringbuf ring);

/*call when full*/
int extend(struct *ringbuf ring, int extend_len);
/*push no lock*/
int __push(struct *ringbuf ring, struct elem e);
/*poo no lock*/
struct elem pop(struct *ringbuf ring);
