main ()
{

  union {
    int i;
    struct {
      char  c0;
      char  c1;
      char  c2;
      char  c3;
    } c;
    struct {
      unsigned int b0 : 4;
      unsigned int b1 : 4;
      unsigned int b2 : 8;
      unsigned int b3 : 8;
      unsigned int b4 : 8;
    } b;
    char vc[4];
    float f;
  } a, b;

  a.c.c0 = 1;
  a.c.c1 = 2;
  a.c.c2 = 3;
  a.c.c3 = 4;
    
  b.i = 123456789;
}
