# <b>完善堆栈内存管理</b>

malloc函数用于程序运行时动态从堆中申请内存,但malloc仅仅是堆内存接口,能够动态分配内存,底层必然有一套完善的内存管理系统在支撑。因此实现malloc的前提是先把底层系统搭建起来。

## malloc底层原理

系统分配的内存都是以4Kb大小的页框为单位的，当我们仅需要几十字节,几百字节这样的小内存块时,显然无法直接满足,为此必须实现一种小内存块的管理,可以满足任意内存大小的分配,这就是malloc的任务。

arena是很多开源项目都会使用到的内存管理概念,将一大块内存划分为多个小内存块,每个小内存块之间互不干扰,可以分别管理,这些小内存块就称为arena。

arena是由一大块内存划分成无数小内存块的内存仓库,系统只能分配4Kb粒度的内存页框的情况下,arena的一大块内存就是申请到的4Kb粒度大小的内存,根据请求的内存量大小,arena的大小也许是一个页框,也许是多个页框,随后再将它们平均拆分为多个小内存块。至于小内存块应该多大,可以有多种不同的规格大小,那就有多种不同规格的arena,比如一种arena中全是16字节大小的内存块,故它只响应16字节以内的内存分配,另一种arena中全是32字节大小的内存块,故它只响应32字节以内的内存分配。我们平时使用的malloc申请内存时,操作系统返回的地址其实就是某个内存块的起始地址,操作系统会根据malloc申请内存的大小来选择不同规格的内存块。因此为支持多种容量内存块的分配,我们需要建立多种不同容量内存块的arena。

arena是个提供内存分配的数据结构,它分为两部分,一部分是元信息,用来描述自己内存池中空闲内存块的数量,其中包括内存块描述符指针,通过它可以间接获知本arena所包含内存块的规格大小,此部分占用空间是固定的,约12字节。另一部分就是内存池区域,这里面有无数的内存块,此部分占用大量的arena空间。整个arena就像一个仓库,元信息就像库房管理员,内存块相当于库物品。

内存块描述符中有block_size为规格大小,还有free_list队列来指向这一规格还空闲可用的arena汇总,所以在分配时必须进过此处free_list挑选,最终所分配的内存块必定属于free_list队列中的某个arena。

尽管arena用小内存块来满足小内存量的分配,但实际上,arena为内存分配提供了统一的入口,无论申请的内存量是多大,都可以用同一个arena来分配内存。小内存块的容量虽然有几种规格,但毕竟是为了满足小内存量的分配,最大内存块容量不会超过1024字节,如果申请的内存量较大,超过1024字节,单独的一个小内存块无法满足时,就需要将多个较大的小内存块组合,但这样却极大的影响了性能,所以对于大内存请求时也会创建一个arena,但不会再拆分,而是将它直接分配出去,此类arena没用对应的内存块描述符,元信息中内存块描述符指针为空。

总结,在内存管理中,arena为任意大小内存的分配提供了统一接口,它既支持1024字节以下的小块内存分配,又支持大于1024字节以上的大块内存,malloc实际上就是通过arena申请这些内存块。arena是个内存仓库,并不是直接对外提供内存分配,只有内存块描述符才对外提供内存块,内存块描述符将同类arena中空闲的内存块汇聚到一起,作为某一规格内存块的分配入口。

## 底层初始化

memory.h
```
#define DESC_CNT 7   // 内存块描述符种类个数  16/32/64/128/256/512/1024字节

/* 内存块描述符 */
struct memory_block_desc {
   int block_size;		 // 内存块大小
   int blocks_number;	 // arena中可容纳此memory_block的数量.
   struct list free_block;	 // 目前可用的memory_block链表
};

/* 内存块 */
struct memory_block {
   struct list free_block;
};
```

memory.c
```
/* 内存仓库arena元信息 */
struct arena {
   struct memory_block_desc* desc;	 // 此arena关联的memory_block_desc
   int cnt;
   bool large;  // true:申请超过1024字节,cnt为页框数; false:不过1024,cnt表示空闲memory_block数量
};

/* 为malloc做准备 */
void block_desc_init(struct memory_block_desc* desc_array) {
   int desc_idx, block_size = 16;

   /* 初始化每个memory_block_desc描述符 */
   for (desc_idx = 0; desc_idx < DESC_CNT; desc_idx++) {
      desc_array[desc_idx].block_size = block_size;

      /* 初始化arena中的内存块数量 */
      desc_array[desc_idx].blocks_per_arena = (PG_SIZE - sizeof(struct arena)) / block_size;

      list_init(&desc_array[desc_idx].free_list);

      block_size *= 2;         // 更新为下一个规格内存块
   }
}

/* 内存管理部分初始化入口 */
void memory_init() {
   put_str("mem_init start\n");
   unsigned int mem_bytes_total = (*(uint32_t*)(0xb00));
   mem_pool_init(mem_bytes_total);	  // 初始化内存池
/* 初始化mem_block_desc数组descs,为malloc做准备 */
   block_desc_init(k_block_descs);
   put_str("mem_init done\n");
}

```
