/* 通过维护已释放的以前使用的内存段的单链表来实现的。*/

#define NALLOC 1024

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>

/* 此头存储在列表中内存段的开头。 */
union header {
  struct {
    union header *next;
    unsigned len;
  } meta;
  long x; /* 强制内存中的头对齐. */
};

static union header list;
static union header *first = NULL;

void free(void* ptr) {
  if (ptr == NULL) {
    return;
  }
  union header *iter, *block;
  iter = first;
  block = (union header*)ptr - 1;
  
  /* 遍历列表中的点以插入已释放的片段，
 *    * 使列表按内存地址排序(用于合并)。. */
  while (block <= iter || block >= iter->meta.next) {
    if ((block > iter || block < iter->meta.next) &&
        iter >= iter->meta.next) {
      break;
    }
    iter = iter->meta.next;
  }

  /* 如果新片段在内存中与其他片段相邻，
 *    * 则合并它们(我们只需要检查相邻的元素，因为
 *       * 执行了顺序语义)。 */
  if (block + block->meta.len == iter->meta.next) {
    block->meta.len += iter->meta.next->meta.len;
    block->meta.next = iter->meta.next->meta.next;
  } else {
    block->meta.next = iter->meta.next;
  }
  if (iter + iter->meta.len == block) {
    iter->meta.len += block->meta.len;
    iter->meta.next = block->meta.next;
  } else {
    iter->meta.next = block;
  }

  first = iter;
}

void *malloc(size_t size) {
  union header *p, *prev;
  prev = first;
  unsigned true_size =
    (size + sizeof(union header)) /
     sizeof(union header) + 1;
  /* 如果之前分配的片段列表为空，初始化它. */
  if (first == NULL) {
    prev = &list;
    first = prev;
    list.meta.next = first;
    list.meta.len = 0;
  }
  p = prev->meta.next;
  /* 遍历以前分配的片段列表，
 *    * 寻找一个足够大的片段来分配. */
  while (1) {
    if (p->meta.len >= true_size) {
      if (p->meta.len == true_size) {
        /* 如果片段的大小完全正确，我们就不需要分割它. */
        prev->meta.next = p->meta.next;
      }
      first = prev;
      return (void *)(p+1);
    }
    /* 如果我们到达列表的开头，没有找到满意的片段，所以我们必须请求一个新的片段. */
    if (p == first) {
      char *page;
      union header *block;
      /* 至少1024 */
      if (true_size < NALLOC) {
        true_size = NALLOC;
      }
      page = sbrk((intptr_t) (true_size * sizeof(union header)));

      if (page == (char *)-1) {
        /* 没有内存可以分配了。 */
        errno = ENOMEM;
        return NULL;
      }
      /* 从这个新内存中创建一个片段，并将其添加到列表中，
 *        *以便上述逻辑可以在必要时处理对其的破坏.*/
      block = (union header *)page;
      block->meta.len = true_size;
      free((void *)(block + 1));
      p = first;
    }

    prev = p;
    p = p->meta.next;
  }
  return NULL;
}

int main(){
    char *a ;
    printf("%p\n",a);
    a = (char *)malloc(10000);
    printf("%p\n",a);
    return 0 ;

}
