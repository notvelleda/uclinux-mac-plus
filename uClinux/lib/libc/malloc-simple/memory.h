

struct memblk
  {
    int mb_magic;
    struct memblk *mb_next;
    struct memblk *mb_prev;
    size_t mb_len;
    struct poolblk *mb_pool;
  };


struct poolblk
  {
    struct poolblk *pb_next;
    struct memblk *pb_freelist;
  };

extern struct poolblk *_poollist;

#define POOLSIZE 8*1024

#define MEM_MAGIC_FREE 0x46524545
#define MEM_MAGIC_USED 0x55534544

