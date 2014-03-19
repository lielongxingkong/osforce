#include <linux/slab.h>
#include <ring_api.h>


int init_ringbuf(struct ringbuf *ring)
{
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
