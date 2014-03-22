#include <linux/list.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/rwsem.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#define INIT_LEN	5
#define EXTEND_LEN	5

struct elem{
	int data;
};

struct ringnode{
	int seq;
	struct elem e;
	struct list_head list;
};

struct ringbuf{
	int len;
	int entries;
	struct list_head head;
	struct list_head *front; //head of queue, point to first one
	struct list_head *rear;  //tail of queue, point to last one's next position
	struct rw_semaphore rwsem;
};

int init_ringbuf(struct ringbuf *ring, int len);
int cleanup_ringbuf(struct ringbuf *ring);
bool full(struct ringbuf *ring);
bool empty(struct ringbuf *ring);
int push(struct ringbuf *ring, struct elem *e);
int pop(struct ringbuf *ring, struct elem *e);

/*call when full*/
int extend(struct ringbuf *ring, int extend_len);
/*push no lock*/
int __push(struct ringbuf *ring, struct elem *e);
/*pop no lock*/
int __pop(struct ringbuf *ring, struct elem *e);
void test(struct ringbuf *ring);
