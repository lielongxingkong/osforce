#include <linux/list.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/rwsem.h>
#include <linux/semaphore.h>
#define INIT_LENGTH	1000

struct elem{
	int data;
};

struct ringnode{
	struct elem e;
	struct list_head list;
};

struct ringbuf{
	struct ringnode *head;
	int entries;
	struct ringnode *front;
	struct ringnode *rear;
	struct rw_semaphore rwsem;
};

int init_ringbuf(struct ringbuf *ring);
int clean_ringbuf(struct ringbuf *ring);
bool full(struct ringbuf *ring);
bool empty(struct ringbuf *ring);
int push(struct ringbuf *ring, struct elem e);
struct elem pop(struct ringbuf *ring);

/*call when full*/
int extend(struct ringbuf *ring, int extend_len);
/*push no lock*/
int __push(struct ringbuf *ring, struct elem e);
/*poo no lock*/
struct elem pop(struct ringbuf *ring);
