#include<stdio.h>
#include<string.h>

char buffer[16000];
struct header *gptr = (struct header *)buffer;

struct header
{
	struct header *next;		//pointer to the next free location
  struct header *prev;    //pointer to the previous free location
  struct header *above;   //pointer to the block immediately above itself
	int size;					      //size of the block
	char alloc_status;			//'a' if allocated, 'f' if free
};

/*
  This function returns pointer to the best fit block
*/
struct header* find_best_fit_block(int requested_size)
{
  struct header *lptr = gptr;

  while(lptr->size >= requested_size + 2*(sizeof(struct header)) && lptr->next != NULL)
  {
    lptr = lptr->next;
  }

  if(lptr->next == NULL)    //if it is the last block
    return lptr;
  else
    return lptr->prev;
}

struct header* change_headers(struct header *best_fit_block_pointer,int requested_size)
{
  if(best_fit_block_pointer->next != NULL)
  {
    best_fit_block_pointer->next->prev = best_fit_block_pointer->prev;
  }
  if(best_fit_block_pointer->prev != NULL)
  {
    best_fit_block_pointer->prev->next = best_fit_block_pointer->next;
  }

  //this points to the remaining part of the block
  struct header *lptr = best_fit_block_pointer + requested_size;
  lptr->size = best_fit_block_pointer->size - requested_size - sizeof(struct header);
  lptr->above = best_fit_block_pointer;
  lptr->prev = NULL;
  lptr->next = NULL;
  lptr->alloc_status = 'f';

  best_fit_block_pointer->size = requested_size;
  best_fit_block_pointer->next = NULL;
  best_fit_block_pointer->prev = NULL;
  best_fit_block_pointer->alloc_status = 'a';

  return lptr;
}

void insert_into_list(struct header *remaining_free_block)
{
  struct header *lptr = gptr;
  while(lptr->size > remaining_free_block->size && lptr->next != NULL)
    lptr = lptr->next;

  if(lptr->next == NULL)
  {
    lptr->next = remaining_free_block;
    remaining_free_block->prev = lptr;
  }
  else
  {
    remaining_free_block->prev = lptr->prev;
    remaining_free_block->next = lptr;

    if(lptr->prev != NULL)
    {
      lptr->prev->next = remaining_free_block;
    }
    lptr->prev = remaining_free_block;
  }
}

char* my_malloc(int requested_size)
{
  if(requested_size > gptr->size)						//enough space is not available
  	return NULL;

  /*find the best fit block*/
  struct header *best_fit_block_pointer = find_best_fit_block(requested_size);
  best_fit_block_pointer->alloc_status = 'a';

  /* change all pointers*/
  struct header *remaining_free_block = change_headers(best_fit_block_pointer,requested_size);

  /*Insert it into the linked list of free blocks into its appropriate position*/
  insert_into_list(remaining_free_block);

  return (char *)best_fit_block_pointer + sizeof(struct header);      //necessary to convert it to (char *) before adding
}

void get_next_free_block(struct header **next_free_pointer)
{
  *next_free_pointer = gptr;

  *next_free_pointer=(struct header *)((char *)(*next_free_pointer) + gptr->size + sizeof(struct header));

  //this loop finds the first free block after x
  while((*next_free_pointer)->alloc_status != 'f')
  {
    *next_free_pointer = (struct header *)((char *)(*next_free_pointer) + (*next_free_pointer)->size + sizeof(struct header));
  }
}

void get_above_free_block(struct header **above_pointer,struct header *next_free_pointer)
{
  while((*above_pointer)->next != next_free_pointer)
  {
    *above_pointer = (*above_pointer)->next;
  }
  (*above_pointer)->next = gptr;
  gptr->next = next_free_pointer;
}

void merge_lower_block(struct header* next_free_pointer)
{
  if((char *)gptr + gptr->size + sizeof(struct header) == (char *)next_free_pointer)           //check if next immediate block is free, in order to merge
  {
    gptr->size = gptr->size + next_free_pointer->size + sizeof(struct header);
    gptr->next = next_free_pointer->next;
  }
}

void merge_upper_block(struct header* above_pointer)
{
  if((char *)above_pointer + above_pointer->size + sizeof(struct header) == (char *)gptr)
  {
    above_pointer->size = above_pointer->size + gptr->size + sizeof(struct header);
    above_pointer->next = gptr->next;
    gptr = above_pointer;
  }
}

void my_free(char *x)
{
  gptr = (struct header *)(x - sizeof(struct header));
  struct header temp;

  gptr->alloc_status = 'f';

  //pointer to find the next free block
  struct header *next_free_pointer;

  /*get the next free block */
  get_next_free_block(&next_free_pointer);

  struct header *above_pointer = next_free_pointer;
  /* get the free block above the block to be freed */
  get_above_free_block(&above_pointer,next_free_pointer);

  //if LOWER block and block to be freed are consecutive, merge them
  merge_lower_block(next_free_pointer);

  //if upper block and block to be freed are consecutive, merge them
  merge_upper_block(above_pointer);

}

void my_malloc_init()
{
  gptr->size = sizeof(buffer) - 2*sizeof(struct header);
  gptr->next = NULL;
  gptr->prev = NULL;
  gptr->above = NULL;
  gptr->alloc_status = 'f';
}

int main()
{
  my_malloc_init();

  printf("GPTR = %d\n",gptr);
  printf("SIZE OF HEADER = %d\n",sizeof(struct header));

  char *p = (char *)my_malloc(4000);
  printf("P = %d\n",p);
  printf("GPTR = %d\n",gptr);

  char *q = my_malloc(1000);
  printf("Q = %d\n",q);
  printf("GPTR = %d\n",gptr);

  char *r = my_malloc(3000);
  printf("R = %d\n",r);
  printf("GPTR = %d\n",gptr);

  my_free(q);
  printf("GPTR = %d\n",gptr);
  printf("GPTR->size = %d\n",gptr->size);
  struct header *t = gptr;
  printf("Q->next = %d\n",t->next);
  t = t->next;
  printf("S->next = %d\n",t->next);

  char *s = my_malloc(2000);
  printf("S = %d\n",s);
  printf("GPTR = %d\n",gptr);
  //t = s - sizeof(struct header);
  //t = t + t->size + sizeof(struct header);
  t = gptr;
  printf("T->next = %d\n",t->next);
  t = t->next;
  printf("Q->next = %d\n",t->next);

  char *u = my_malloc(2000);
  printf("U = %d\n",u);
  printf("GPTR = %d\n",gptr);

  my_free(s);
  printf("GPTR = %d\n",gptr);
  printf("GPTR->size = %d\n",gptr->size);
  // my_free(u);
  // my_free(p);
  my_free(r);
  printf("GPTR = %d\n",gptr);
  t = gptr;
  printf("T->SIZE = %d",t->size);
  return 0;
}
