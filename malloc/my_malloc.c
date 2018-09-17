#include<stdio.h>
#include<string.h>

char buffer[16000];
struct header *gptr = (struct header *)buffer;

struct header
{
	struct header *next;		//pointer to the next free location
	int size;					      //size of the block
	char alloc_status;			//'a' if allocated, 'f' if free
};

int maxsize = sizeof(buffer) - 2*sizeof(struct header);   //currently available maximum block size

void find_best_fit_block(struct header **minsizeptr,int requested_size)
{
  struct header *lptr = gptr;
  int minsize = sizeof(buffer);

  do {
    /*after this loop,
    minsize has the size of the best fit block
    and minsizeptr points to it
    */

    if((lptr->size >= requested_size + 2*sizeof(struct header)) && (lptr->size < minsize))
    {
      minsize = lptr->size;
      *minsizeptr = lptr;
    }

    lptr = lptr->next;
  } while(lptr != gptr);

}

void set_above_header(struct header *minsizeptr,int requested_size)
{
  struct header *lptr = minsizeptr;

  while(lptr->next != minsizeptr)
  {
    lptr = lptr->next;
  }

  //if it wasn't pointing to itself (i.e. it wasn't the only free block)
  if(lptr != minsizeptr)
  {
    lptr->next = (struct header *)((char *)minsizeptr + sizeof(struct header) + requested_size);
  }
}

void set_next_header(struct header *minsizeptr,int requested_size)
{
  struct header *lptr;
  //lptr now points to the remaning part of the free location
  lptr = (struct header *)((char *)minsizeptr + sizeof(struct header) + requested_size);
  gptr = lptr;                                                  //update GPTR

  /*if it was the ONLY existing free block, then it will point to itself*/
  if(minsizeptr->next == minsizeptr)
      lptr->next = lptr;
  else
      lptr->next = minsizeptr->next;

  lptr->size = minsizeptr->size - requested_size - 2*sizeof(struct header);
  lptr->alloc_status = 'f';
}

void update_max_size(struct header *minsizeptr,int requested_size)
{
  maxsize = maxsize - requested_size - 2*sizeof(struct header);
  struct header *lptr = (struct header *)((char *)minsizeptr + sizeof(struct header) + requested_size);

  struct header *temp = lptr;
  do
  {
    if(lptr->size > maxsize)
      maxsize = lptr->size;

    lptr = lptr->next;
  }while(lptr != temp);

}

char* my_malloc(int requested_size)
{
  if(requested_size > maxsize)						//enough space is not available
  	return NULL;

  struct header *minsizeptr = gptr;     //points to the best fit block

  /*find the best fit block*/
  // printf("\nFINDING THE BEST FIT BLOCK\n");
  find_best_fit_block(&minsizeptr,requested_size);

  /*change the next pointer of the free block above the best fit block*/
  // printf("\nSET ABOVE HEADER\n");
  set_above_header(minsizeptr,requested_size);

  /*Make the next header*/
  // printf("\nSET NEXT HEADER\n");
  set_next_header(minsizeptr,requested_size);

  /*Update Max Size*/
  // printf("\nUPDATING MAX SIZE\n");
  if(minsizeptr->size == maxsize)
  {
    update_max_size(minsizeptr,requested_size);
  }

  /*Update the current header*/
  // printf("\nUPDATE CURRENT HEADER\n");
  minsizeptr->next = NULL;
  minsizeptr->size = requested_size;
  minsizeptr->alloc_status = 'a';

  return (char *)minsizeptr + sizeof(struct header);      //necessary to convert it to (char *) before adding
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
  struct header *temp = (struct header *)buffer;

  temp->size = sizeof(buffer) - 2*sizeof(struct header);
  temp->next = temp;
  temp->alloc_status = 'f';

  // printf("temp->size = %d\ntemp->next = %d\ntemp->alloc_status = %c",temp->size,temp->next,temp->alloc_status);
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
