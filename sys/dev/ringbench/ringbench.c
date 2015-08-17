#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/bus.h>
#include <sys/libkern.h>


typedef struct {
	void *(*ring_alloc) (int size);
	void *(*ring_free) (int size);
	int (*ring_enqueue) (void *ring, void *buf);
	int (*ring_batch_enqueue) (void *ring, void *ents[], int count, int *qcount);
	int (*ring_dequeue) (void *ring, void *ents[], int count);
	int (*ring_trylock) (void *ring);
	void (*ring_lock) (void *ring);
	int (*ring_unlock) (void *ring);
} ring_ops;

static void *rb_br_alloc(int size);
static void *rb_br_aligned_alloc(int size);
static int rb_br_enqueue(void *ring, void *buf);
static int rb_br_batch_enqueue(void *ring, void *ents[], int count, int *qcount);
static int rb_br_dequeue(void *ring, void *ents[], int count);


static void *rb_brsc_alloc(int size);
static void *rb_brsc_aligned_alloc(int size);
static int rb_brsc_enqueue(void *ring, void *buf);
static int rb_brsc_batch_enqueue(void *ring, void *ents[], int count, int *qcount);
static int rb_brsc_dequeue(void *ring, void *ents[], int count);


static ring_ops br_ops;
static ring_ops br_aligned_ops;
static ring_ops brsc_ops;
static ring_ops brsc_aligned_ops;
static ring_ops mpr_ops;
static ring_ops ifq_ops;

static void *
rb_br_alloc(int size);
{
	struct mtx *br_mtx;

	br_mtx = malloc(sizeof(struct mtx), M_RINGBENCH, M_ZERO|M_WAITOK);
	mtx_init(br_mtx, MTX_DEF);

	return (buf_ring_alloc(size, M_RINGBENCH, M_WAITOK, br_mtx));
}

static void *
rb_br_aligned_alloc(int size);
{
	struct mtx *br_mtx;

	br_mtx = malloc(sizeof(struct mtx), M_RINGBENCH, M_ZERO|M_WAITOK);
	mtx_init(br_mtx, MTX_DEF);

	return (buf_ring_aligned_alloc(size, M_RINGBENCH, M_WAITOK, br_mtx));
}

static int
rb_br_enqueue(void *ring, void *buf)
{
	struct buf_ring *br = ring;
	int rc;
	
	rc = buf_ring_enqueue(br, buf);
	/* do the racy handoff here */
	return (rc);
}

static void *
rb_brsc_alloc(int size)
{

	return (buf_ring_sc_alloc(size, M_RINGBENCH, M_WAITOK, 0));
}

static void *
rb_brsc_aligned_alloc(int size)
{

	return (buf_ring_sc_alloc(size, M_RINGBENCH, M_WAITOK, BR_FLAGS_ALIGNED));
}

static int
rb_brsc_enqueue(void *ring, void *buf)
{
	struct buf_ring *br = ring;
	int rc;
	
	return (buf_ring_sc_enqueue(br, buf));
}

static uint32_t
mp_drain(struct mp_ring *r, uint32_t cidx, uint32_t pidx)
{
	/* XXX */
}

static uint32_t
mp_can_drain(struct mp_ring *r)
{

	/* XXX */
}

static void *
rb_mp_ring_alloc(int size)
{
	struct mp_ring *ring;
	
	if (mp_ring_alloc(&ring, size, NULL, mp_drain, mp_can_drain, M_RINGBENCH, M_WAITOK))
		return (NULL);
	return (ring);
}

static int
rb_mpr_enqueue(void *ring, void *buf)
{
	struct mp_ring *br = ring;
	int rc;
	
	rc = mp_ring_enqueue(br, &buf, 1, 0));

}



static void *
rb_ifq_alloc(int size)
{
	struct ifqueue *ifq;

	ifq = malloc(sizeof(struct ifqueue), M_RINGBENCH, M_WAITOK|M_ZERO);
	mtx_init(&ifq->ifq_mtx, MTX_DEF);
	IFQ_SET_MAXLEN(ifq, size);
	return (ifq);
}

static int
rinchbench_mod_init(void)
{

	return (0);
}

static int
ringbench_module_event_handler(module_t mod, int what, void *arg)
{
        int err;

        switch (what) {
        case MOD_LOAD:
			if ((err = ringbench_mod_init()) != 0) {
				printf("ringbench initialization failure\n");
				return (err);
			}
                break;
        case MOD_UNLOAD:
                return (0);
        default:
                return (EOPNOTSUPP);
        }
        printf("ringbench loaded\n");
        return (0);
}

static moduledata_t ringbench_moduledata = {
        "ringbench",
        ringbench_module_event_handler,
        NULL
};

DECLARE_MODULE(ringbench, ringbench_moduledata, SI_SUB_KLD, SI_ORDER_ANY);
