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
  //printf("Finding best fit\n");   //testing purpose
  struct header *lptr = gptr;

  while(lptr->size >= requested_size + 2*(sizeof(struct header)) && lptr->next != NULL)
  {
    printf("looping\n");        //testing purpose
    lptr = lptr->next;
  }
  
  if(lptr->next == NULL && lptr->size > requested_size)    //if it is the last block
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
  struct header *lptr = (char*)best_fit_block_pointer + requested_size + sizeof(struct header);
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

void insert_into_list(struct header *free_block)
{
  struct header *lptr = gptr;

  if(gptr == NULL)          //free list is empty
  {
    //printf("gptr was null\n");      //testing purpose
    gptr = free_block;
    //printf("GPTR = %d\n",gptr);     //testing purpose
    return;
  }
  if(free_block->size>gptr->size)   //theblock to be inserted is bigger than the biggest block
  {
    free_block->next=gptr;
    gptr->prev=free_block;
    gptr=free_block;
    return ;
  }
  while(lptr->size > free_block->size && lptr->next != NULL)
    lptr = lptr->next;

  if(lptr->next == NULL && lptr->size > free_block->size)
  {
    lptr->next = free_block;
    free_block->prev = lptr;
  }
  else
  {
    free_block->prev = lptr->prev;
    free_block->next = lptr;

    if(lptr->prev != NULL)
    {
      lptr->prev->next = free_block;
    }
    lptr->prev = free_block;
  }
}

char* my_malloc(int requested_size)
{
  if(requested_size > gptr->size)						//enough space is not available
  	return NULL;

  //printf("HELLO\n");          //testing purpose

  /*find the best fit block*/
  struct header *best_fit_block_pointer = find_best_fit_block(requested_size);

  if(best_fit_block_pointer == gptr)
    gptr = gptr->next;

  best_fit_block_pointer->alloc_status = 'a';

  /* change all pointers*/
  struct header *remaining_free_block = change_headers(best_fit_block_pointer,requested_size);

  /*Insert it into the linked list of free blocks into its appropriate position*/
  insert_into_list(remaining_free_block);


  return (char *)best_fit_block_pointer + sizeof(struct header);      //necessary to convert it to (char *) before adding
}

void get_next_block(struct header **next_block_pointer,struct header *lptr)
{
  *next_block_pointer = lptr;

  *next_free_pointer=(struct header *)((char *)(*next_free_pointer) + lptr->size + sizeof(struct header));
}


void merge_lower_block(struct header* next_pointer, struct header* lptr)
{
  lptr->size += next_pointer->size + sizeof(struct  header);
}

void merge_upper_block(struct header* above_pointer,struct header** lptr)
{
  above_pointer->size +=  (*lptr)->size + sizeof(struct  header);
  *lptr = above_pointer;
}

void delete_from_list(struct header* block_deleted_pointer)
{
  if(block_deleted_pointer == gptr)
    gptr = gptr->next;
  
  else
  {
      if(block_deleted_pointer->next!=NULL)
      {
        block_deleted_pointer->next->prev=block_deleted_pointer->prev;
      }
      if(block_deleted_pointer->prev!=NULL)
      {
        block_deleted_pointer->prev->next=block_deleted_pointer->next;
      }
      block_deleted_pointer->prev =NULL;
      block_deleted_pointer->next =NULL;
  }
}
void my_free(char *x)
{
  struct header *lptr = (struct header *)(x - sizeof(struct header));
  
  lptr->alloc_status = 'f';

  //pointer to find the next block
  struct header *next_pointer;

  /*get the next block */
  get_next_block(&next_pointer,lptr);

  struct header *above_pointer = lptr->above;
  /* get the free block above the block to be freed */

  if(next_pointer < (struct header*)(buffer + sizeof(buffer)) && next_pointer->alloc_status=='f')
  {
     delete_from_list(next_pointer); 
      merge_lower_block(next_pointer,lptr);
      struct header* merged_block_consecutive_next = lptr+lptr->size+ sizeof(struct header);

      printf("lptr->size = %d\n",lptr->size);
     if(merged_block_consecutive_next < (struct header*)(buffer + sizeof(buffer)))
         merged_block_consecutive_next->above = lptr;
   }
  
  if(above_pointer != NULL && above_pointer->alloc_status=='f')
  {
    delete_from_list(above_pointer);
    merge_upper_block(above_pointer,&lptr);
  
    struct header* merged_block_consecutive_next = lptr+lptr->size+ sizeof(struct header);

      if(merged_block_consecutive_next < (struct header*)(buffer + sizeof(buffer)))
        merged_block_consecutive_next->above = lptr;
  }
  insert_into_list(lptr);

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
   printf("Starting GPTR = %d\n",gptr);
    printf("Starting GPTR = %d\n",gptr);
     printf("Starting GPTR = %d\n",gptr);
      printf("Starting GPTR = %d\n",gptr);
  printf("Starting GPTR = %d\n",gptr);
  printf("Starting GPTR->size = %d\n",gptr->size);
  printf("SIZE OF HEADER = %d\n",sizeof(struct header));

  char *p = (char *)my_malloc(368);
  printf("P = %d\n",p);
  printf("GPTR = %d\n",gptr);

  char *q = my_malloc(768);
  printf("Q = %d\n",q);
  printf("GPTR = %d\n",gptr);

  char *r = my_malloc(1268);
  printf("R = %d\n",r);
  printf("GPTR = %d\n",gptr);

  my_free(p);
  printf("GPTR = %d\n",gptr);
  printf("GPTR->size = %d\n",gptr->size);

  my_free(r);
  printf("GPTR = %d\n",gptr);
  printf("GPTR->size = %d\n",gptr->size);

  my_free(q);
  printf("GPTR = %d\n",gptr);
  printf("GPTR->size = %d\n",gptr->size);

  // struct header *t = gptr;
  // printf("Q->next = %d\n",t->next);
  // t = t->next;
  // printf("S->next = %d\n",t->next);

  // char *s = my_malloc(2000);
  // printf("S = %d\n",s);
  // printf("GPTR = %d\n",gptr);
  // //t = s - sizeof(struct header);
  // //t = t + t->size + sizeof(struct header);
  // t = gptr;
  // printf("T->next = %d\n",t->next);
  // t = t->next;
  // printf("Q->next = %d\n",t->next);

  // char *u = my_malloc(2000);
  // printf("U = %d\n",u);
  // printf("GPTR = %d\n",gptr);

  // my_free(s);
  // printf("GPTR = %d\n",gptr);
  // printf("GPTR->size = %d\n",gptr->size);
  // my_free(u);
  // my_free(p);
  // my_free(r);
  // printf("GPTR = %d\n",gptr);
  // printf("GPTR->size = %d\n",gptr->size);
  // t = gptr;
  // printf("T->SIZE = %d",t->size);
  return 0;
}