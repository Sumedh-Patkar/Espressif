#include<stdio.h>

char buf[16000];
char *gptr = &buf[0];

char* my_malloc(int n)
{
  if((gptr + n) < (buf + 16000))
  {
    gptr += n;
    return gptr - n;
  }
  else
    return 0;
}
void my_free(char *x)
{
  gptr = x;
  x = NULL;
}

int main()
{
  char *p = my_malloc(4000);
  printf("%d\n",p);
  char *q = my_malloc(1000);
  printf("%d\n",q);
  char *r = my_malloc(3000);
  printf("%d\n",r);
  my_free(r);
  char *s = my_malloc(2000);
  printf("%d\n",s);
  char *t = my_malloc(2000);
  printf("%d\n",t);
  my_free(t);
  my_free(s);
  my_free(q);
  my_free(p);
  return 0;
}
