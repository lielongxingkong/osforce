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

int init_ringbuf(struct *ringbuf);
int clean_ringbuf(struct *ringbuf);
bool full(struct *ringbuf);
bool empty(struct *ringbuf);
int push(struct *ringbuf, struct elem);
struct elem pop(struct *ringbuf);

/*call when full*/
int extend(struct *ringbuf, int extend_len);
/*push no lock*/
int __push(struct *ringbuf, struct elem);
/*pop no lock*/
struct elem __pop(struct *ringbuf);
