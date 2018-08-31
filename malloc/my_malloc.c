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

int maxsize = sizeof(buffer) - 2*sizeof(struct header); //initial available size

char* my_malloc(int n)
{
  if(n > maxsize)				//enough space is not available
  	return 0;
  else
  {
    memcpy(&s,gptr,sizeof(s));
  	char *lptr = gptr;			//iterator to find best fit block
  	int minsize = s.size;
  	char *minsizeptr = gptr;	//points to best fit block

    struct header temp;       //temporary structure object

    do {
      /*after this loop,
      minsize has the size of the best fit block
      and minsizeptr points to it
      */
      memcpy(&temp,lptr,sizeof(struct header));

      if((temp.size > n + 2*sizeof(struct header)) && (temp.size < minsize))
      {
        minsize = temp.size;
        minsizeptr = lptr;
      }

      lptr = temp.next;
    } while(lptr != gptr);

    /*Make the next header*/
    lptr = (minsizeptr + n + sizeof(struct header));   //lptr now points to the remaning part of the free location
    // if(lptr > buffer + sizeof(buffer))
    // {
    //   /*
    //   point to the first free location
    //   from the beginning
    //   */
    //   lptr = buffer;
    //   struct header temp2;
    //   memcpy(&temp2,lptr,sizeof(struct header));
    //   while(temp2.alloc_status == 'a')
    //   {
    //     lptr = lptr + temp2.size + sizeof(struct header);
    //     memcpy(&temp2,lptr,sizeof(struct header));
    //   }
    // }

    //if it was the ONLY existing free block, then it will point to itself.
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
      maxsize = maxsize - n - sizeof(struct header);
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
  gptr = x;
  struct header temp;
  char *iteratePtr;
  memcpy(&s,gptr,sizeof(struct header));        //s now points to the block being freed
  s.alloc_status = 'f';                         //mark current block as 'free'

  char *lptr = gptr;
  lptr=lptr + s.size;
  char *lptr2;

  memcpy(&temp,lptr,sizeof(struct header));

  // for merging below consecutive block
  if(temp.alloc_status == 'f')                  //check if next immediate block is free, in order to merge
  {
    s.size = s.size + temp.size + sizeof(struct header);
    s.next = temp.next;
  }

  //this loop finds the first free block after x
  while(temp.alloc_status != 'f')
  {
    lptr = lptr + temp.size + sizeof(struct header);
    memcpy(&temp,lptr,sizeof(struct header));
  }
  //now lptr points to the first free block

  lptr2 = lptr;
  memcpy(&temp,lptr2,sizeof(struct header))

  while(temp.next != lptr)
  {
    lptr2 = temp.next;
    memcpy(&temp,lptr2,sizeof(struct header));
  }
  temp.next = gptr;
  s.next = lptr;

  if(lptr2 + temp.size + sizeof(struct header) == gptr)                  //
  {
    temp.size = temp.size + s.size + sizeof(struct header);
    temp.next = s.next;
    gptr = lptr2;
  }

}

int main()
{
  s.next = buffer;
  s.size = sizeof(buffer) - 2*sizeof(s);
  s.alloc_status = 'f';
  memcpy(gptr,&s,sizeof(s));

  // char *p = my_malloc(4000);
  // printf("%d\n",p);
  // char *q = my_malloc(1000);
  // printf("%d\n",q);
  // char *r = my_malloc(3000);
  // printf("%d\n",r);
  // my_free(r);
  // char *s = my_malloc(2000);
  // printf("%d\n",s);
  // char *t = my_malloc(2000);
  // printf("%d\n",t);
  // my_free(t);
  // my_free(s);
  // my_free(q);
  // my_free(p);

  return 0;
}
