struct ring_node{
	struct ring_node *next;
	struct ring_node *priv;
	void *data;
};
struct ring_buf{
	void *buf;
	int entries;
	spinlock_t lock;


};
