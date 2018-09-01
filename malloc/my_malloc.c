#include<stdio.h>
#include<string.h>

char buffer[16000];
char *gptr = &buffer[0];

struct header
{
	char *next;		//pointer to the next free location
	int size;					//size of the block
	char alloc_status;			//'a' if allocated, 'f' if free
}s;

int maxsize = sizeof(buffer) - 2*sizeof(struct header);   //currently available maximum block size

char* my_malloc(int n)
{
  if(n > maxsize)						//enough space is not available
  	return NULL;
  else
  {
    memcpy(&s,gptr,sizeof(s));			//copy header of free location to s
  	char *lptr = gptr;			      	//iterator to find best fit block
  	int minsize = sizeof(buffer);
  	char *minsizeptr = gptr;	    	//points to best fit block

    struct header temp;           		//temporary structure object

    do {
      /*after this loop,
      minsize has the size of the best fit block
      and minsizeptr points to it
      */
      memcpy(&temp,lptr,sizeof(struct header));

      if((temp.size >= n + 2*sizeof(struct header)) && (temp.size < minsize))
      {
        minsize = temp.size;
        minsizeptr = lptr;
      }
      // printf("PRINTING TEMP->NEXT = %d\n",temp.next);
      // if(temp.next == 0)
        // printf("LPTR = %d\n",lptr);
      lptr = temp.next;
    } while(lptr != gptr);
    memcpy(&temp,minsizeptr,sizeof(struct header));

    /*Make the next header*/
    lptr = (minsizeptr + n + sizeof(struct header));   //lptr now points to the remaning part of the free location
    gptr = lptr;                                      //update GPTR

    /*if it was the ONLY existing free block, then it will point to itself*/
    if(temp.next == minsizeptr)
        s.next = lptr;
    else
        s.next = temp.next;

    s.size = temp.size - n - 2*sizeof(struct header);
    s.alloc_status = 'f';
    memcpy(lptr,&s,sizeof(struct header));

    /*Update Max Size*/
    if(temp.size == maxsize)
    {
      maxsize = maxsize - n - 2*sizeof(struct header);
      lptr = temp.next;
      do
      {
        memcpy(&s,lptr,sizeof(struct header));
        if(s.size > maxsize)
          maxsize = s.size;
        lptr = s.next;
      }while(lptr != minsizeptr);
    }

    /*Update the current header*/
    temp.next = NULL;
    temp.size = n;
    temp.alloc_status = 'a';
    memcpy(minsizeptr,&temp,sizeof(struct header));

    return minsizeptr + sizeof(struct header);
  }
}

void my_free(char *x)
{
  gptr = x - sizeof(struct header);
  struct header temp;
  // char *iteratePtr;
  memcpy(&s,gptr,sizeof(struct header));        //s now points to the block being freed
  s.alloc_status = 'f';                         //mark current block as 'free'

  char *lptr = gptr;
  lptr=lptr + s.size + sizeof(struct header);
  memcpy(&temp,lptr,sizeof(struct header));

  //this loop finds the first free block after x
  while(temp.alloc_status != 'f')
  {
    lptr = lptr + temp.size + sizeof(struct header);
    memcpy(&temp,lptr,sizeof(struct header));
  }
  //now lptr points to the first free block

  char *lptr2 = lptr;
  memcpy(&temp,lptr2,sizeof(struct header));

  while(temp.next != lptr)
  {
    printf("%d\n",lptr2);
    if(temp.next == NULL)
    {
      printf("FAIL\n");

    }
    lptr2 = temp.next;
    memcpy(&temp,lptr2,sizeof(struct header));
  }
  temp.next = gptr;
  memcpy(lptr2,&temp,sizeof(struct header));
  s.next = lptr;
  memcpy(gptr,&s,sizeof(struct header));

  //if LOWER block and block to be freed are consecutive, merge them
  if(gptr + s.size + sizeof(struct header) == lptr)           //check if next immediate block is free, in order to merge
  {
    s.size = s.size + temp.size + sizeof(struct header);
    s.next = temp.next;
    memcpy(gptr,&s,sizeof(struct header));
  }

  //if upper block and block to be freed are consecutive, merge them
  if(lptr2 + temp.size + sizeof(struct header) == gptr)
  {
    temp.size = temp.size + s.size + sizeof(struct header);
    temp.next = s.next;
    gptr = lptr2;
    memcpy(gptr,&temp,sizeof(struct header));
  }

}

int main()
{
  s.next = buffer;
  s.size = sizeof(buffer) - 2*sizeof(s);
  s.alloc_status = 'f';
  memcpy(gptr,&s,sizeof(s));

  printf("GPTR = %d\n",gptr);
  char *p = my_malloc(4000);
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
  char *s = my_malloc(2000);
  printf("S = %d\n",s);
  printf("GPTR = %d\n",gptr);
  // char *t = my_malloc(2000);
  // printf("%d\n",t);
  // my_free(s);
  // my_free(t);
  // my_free(p);
  my_free(r);
  printf("GPTR = %d\n",gptr);
  return 0;
}
