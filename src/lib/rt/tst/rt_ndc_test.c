/* rt_ndc.c -- Network data conversion.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Vax f-float.  */

union vax_f {
  int	i;
  struct {
    unsigned int    f22_16	:  7;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
    unsigned int    f15_0	: 16;
  } b;
};

/* IEEE single.  */

union i3e_s {
  int	i;
  struct {
    unsigned int    f22_0	: 23;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
  } b;
  struct {
    unsigned int    f22_7	: 16;
    unsigned int    f6_0	:  7;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
  } v;
};

#define VAX_F_BIAS    0x81
#define I3E_S_BIAS    0x7f
#define VAX_D_BIAS    0x81
#define VAX_G_BIAS    0x401
#define I3E_D_BIAS    0x3ff

#if defined(__vms)
# define IBYTE0(s) (((*(int *)s) >> 0x18) & 0x000000ff) 
# define IBYTE1(s) (((*(int *)s) >> 0x08) & 0x0000ff00) 
# define IBYTE2(s) (((*(int *)s) << 0x08) & 0x00ff0000) 
# define IBYTE3(s) (((*(int *)s) << 0x18) & 0xff000000) 

# define CONVERT_INT(t, s)\
  (*(int *)t = (IBYTE0(s) | IBYTE1(s) | IBYTE2(s) | IBYTE3(s)))

# define SBYTE0(s) (((*(short *)s) >> 0x08) & 0x00ff) 
# define SBYTE1(s) (((*(short *)s) << 0x08) & 0xff00) 

# define CONVERT_SHORT(t, s)\
  (*(short *)t = (SBYTE0(s) | SBYTE1(s)))

# define CONVERT_BOOL(t, s) (*(int *)t = (0 != *(int *)s))
#else
# define CONVERT_INT(t, s) (*(int *)t = *(int *)s)
# define CONVERT_SHORT(t, s) (*(short *)t = *(short *)s)
# define CONVERT_BOOL(t, s) (*(int *)t = (0 != *(int *)s))
#endif



static char*
print_bin (
  int *i
)
{
  static char buff[80];
  int b;

  buff[0] = '\0';

#if defined (__vms)
  for (b = 0; b < 32; b++) {
    strcat(buff, ((*i & (1<<(31-b))) ? "1" : "0"));
    if ((b + 1) % 8 == 0) strcat(buff, " ");
  }
#else
  for (b = 0; b < 32; b++) {
    strcat(buff, ((*i & (1<<b)) ? "1" : "0"));
    if ((b + 1) % 8 == 0) strcat(buff, " ");
  }
#endif

  return buff;
}

/* .  */

static int
encode_sfloat (
  int	count,
  char	*tp,
  char	*sp,
  int	*size
)
{

  int			i;
  union vax_f		*vp;
  union i3e_s		i3e;

  for (i = count; i > 0; i--) {
    vp = ((union vax_f *)sp);
    
    if (vp->b.f22_16 == 0x7f && vp->b.exp == 0xff && vp->b.f15_0 == 0xffff) {  /* High value.  */
      i3e.i = 0, i3e.b.exp = 0xff;
    } else if (vp->b.f22_16 == 0 && vp->b.exp == 0 && vp->b.f15_0 == 0) {  /* Low value.  */
      i3e.i = 0;
    } else {
      i3e.b.exp	  = vp->b.exp - VAX_F_BIAS + I3E_S_BIAS;
      i3e.v.f22_7 = vp->b.f15_0;
      i3e.v.f6_0  = vp->b.f22_16;
    }

    i3e.b.sign = vp->b.sign;

    CONVERT_INT(tp, &i3e.i);
    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return 1;
}    

/* .  */

static int
decode_sfloat (
  int	count,
  char	*tp,
  char	*sp,
  int	*size
)
{

  int			i;
  union vax_f		*vp;
  union i3e_s		i3e;

  for (i = count; i > 0; i--) {
    vp = ((union vax_f *)tp);
    CONVERT_INT(&i3e.i, sp);
    
    if (i3e.b.f22_0 == 0x0 && i3e.b.exp == 0xff) {  /* High value.  */
      vp->b.f22_16  = 0x7f;
      vp->b.exp	    = 0xff;
      vp->b.f15_0   = 0xffff;
    } else if (i3e.b.f22_0 == 0x0 && i3e.b.exp == 0x00) {  /* Low value.  */
      vp->i = 0;
    } else {
      vp->b.exp	    = i3e.b.exp - I3E_S_BIAS + VAX_F_BIAS;
      vp->b.f22_16  = i3e.v.f6_0;
      vp->b.f15_0   = i3e.v.f22_7;
    }

    vp->b.sign = i3e.b.sign;

    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return 1;
}    

typedef struct {
  int		b_int_v[10];
  float		a_float_v[100];
  int		a_int;
  short int	a_short_v[5];
  int		a_int_v[5];
  char		a_char;
  float		a_float;
} sTest;

main ()
{
  sTest		src;
  sTest		trg;
  float		a = 1234567890.123456;
  float		b = 1.1;
  int		i;
  int		size;
  FILE		*fp;

  float		av[9];
  float		bv[9];
  av[0] = 0.19550715;
  av[1] = 1.9550715;
  av[2] = 19.550715;
  av[3] = 195.50715;
  av[4] = 1955.0715;
  av[5] = 19550.715;
  av[6] = 195507.15;
  av[7] = 1955071.5;
  av[8] = 19550715.;
  
  printf("a1 ..............:% 22.10f %12e %s\n", av[0], av[0], print_bin((int *)&av[0]));
  printf("a2 ..............:% 22.10f %12e %s\n", av[1], av[1], print_bin((int *)&av[1]));
  printf("a3 ..............:% 22.10f %12e %s\n", av[2], av[2], print_bin((int *)&av[2]));
  printf("a4 ..............:% 22.10f %12e %s\n", av[3], av[3], print_bin((int *)&av[3]));
  printf("a5 ..............:% 22.10f %12e %s\n", av[4], av[4], print_bin((int *)&av[4]));
  printf("a6 ..............:% 22.10f %12e %s\n", av[5], av[5], print_bin((int *)&av[5]));
  printf("a7 ..............:% 22.10f %12e %s\n", av[6], av[6], print_bin((int *)&av[6]));
  printf("a8 ..............:% 22.10f %12e %s\n", av[7], av[7], print_bin((int *)&av[7]));
  printf("a9 ..............:% 22.10f %12e %s\n", av[8], av[8], print_bin((int *)&av[8]));

#if !defined(__vms)

  for (i = 0; i < 10; i++) {
    src.b_int_v[i] = i*i * 2300000;
    printf("a_int_v[%3.3d] ..:% 12.12d %s\n", i, src.b_int_v[i], print_bin((int *)&src.b_int_v[i]));
  }

  for (i = 0; i < 100; i++) {
    a /= b;
    src.a_float_v[i] = a;
    printf("a_float_v[%3.3d] :% 22.10f %12e %s\n", i, a, a, print_bin((int *)&a));
  }        

  src.a_int = 1507153392;
  printf("a_int .........:% 12.12d %s\n", src.a_int, print_bin((int *)&src.a_int));

  for (i = 0; i < 5; i++) {
    src.a_short_v[i] = i*i * 32000;
    printf("a_short_v[%3.3d] :% 12.12d\n", i, src.a_short_v[i]);
  }

  for (i = 0; i < 5; i++) {
    src.a_int_v[i] = i*i * 23000000;
    printf("a_int_v[%3.3d] ..:% 12.12d %s\n", i, src.a_int_v[i], print_bin((int *)&src.a_int_v[i]));
  }

  src.a_char = 'L';
  printf("a_char ........: %c\n", src.a_char);

  src.a_float = 550715.3392;
  printf("a_float .......:% 22.10f %18e %s\n", src.a_float, src.a_float, print_bin((int *)&src.a_float));


  fp = fopen("/view/pwr_x2_3_1/vobs/pwr_src/src/lib/rt/tst/test_vms.dat", "wb");
  if (fp == NULL) exit(0);

  size = fwrite(av, sizeof(av), 1, fp);
  size = fwrite(&src, sizeof(src), 1, fp);

  fclose(fp);

  exit(1);  
#else
  fp = fopen("/pwr_root/src/lib/rt/tst/test_vms.dat", "rb");
  if (fp == NULL) exit(0);

  size = fread(av, sizeof(av), 1, fp);
  printf("\nRead %d bytes\n\n", size);

  printf("a1 ..............:% 22.10f %12e %s\n", av[0], av[0], print_bin((int *)&av[0]));
  printf("a2 ..............:% 22.10f %12e %s\n", av[1], av[1], print_bin((int *)&av[1]));
  printf("a3 ..............:% 22.10f %12e %s\n", av[2], av[2], print_bin((int *)&av[2]));
  printf("a4 ..............:% 22.10f %12e %s\n", av[3], av[3], print_bin((int *)&av[3]));
  printf("a5 ..............:% 22.10f %12e %s\n", av[4], av[4], print_bin((int *)&av[4]));
  printf("a6 ..............:% 22.10f %12e %s\n", av[5], av[5], print_bin((int *)&av[5]));
  printf("a7 ..............:% 22.10f %12e %s\n", av[6], av[6], print_bin((int *)&av[6]));
  printf("a8 ..............:% 22.10f %12e %s\n", av[7], av[7], print_bin((int *)&av[7]));
  printf("a9 ..............:% 22.10f %12e %s\n", av[8], av[8], print_bin((int *)&av[8]));

  size = 9 * sizeof(float);
  decode_sfloat(9, (char *)bv, (char *)&av, &size);
  printf("\ndeocde\n\n", size);

  printf("a1 ..............:% 22.10f %12e %s\n", bv[0], bv[0], print_bin((int *)&bv[0]));
  printf("a2 ..............:% 22.10f %12e %s\n", bv[1], bv[1], print_bin((int *)&bv[1]));
  printf("a3 ..............:% 22.10f %12e %s\n", bv[2], bv[2], print_bin((int *)&bv[2]));
  printf("a4 ..............:% 22.10f %12e %s\n", bv[3], bv[3], print_bin((int *)&bv[3]));
  printf("a5 ..............:% 22.10f %12e %s\n", bv[4], bv[4], print_bin((int *)&bv[4]));
  printf("a6 ..............:% 22.10f %12e %s\n", bv[5], bv[5], print_bin((int *)&bv[5]));
  printf("a7 ..............:% 22.10f %12e %s\n", bv[6], bv[6], print_bin((int *)&bv[6]));
  printf("a8 ..............:% 22.10f %12e %s\n", bv[7], bv[7], print_bin((int *)&bv[7]));
  printf("a9 ..............:% 22.10f %12e %s\n", bv[8], bv[8], print_bin((int *)&bv[8]));

  size = fread(&src, sizeof(src), 1, fp);
  printf("\nRead %d bytes\n\n", size);

  for (i = 0; i < 10; i++)
    CONVERT_INT(&trg.b_int_v[i], &src.b_int_v[i]);

  size = sizeof(trg.a_float_v);
  decode_sfloat(100, (char *) trg.a_float_v, (char *) src.a_float_v, &size);

  CONVERT_INT(&trg.a_int, &src.a_int);

  for (i = 0; i < 5; i++)
    CONVERT_SHORT(&trg.a_short_v[i], &src.a_short_v[i]);

  for (i = 0; i < 5; i++)
    CONVERT_INT(&trg.a_int_v[i], &src.a_int_v[i]);

  trg.a_char = src.a_char;

  size = sizeof(trg.a_float);
  decode_sfloat(1, (char *) &trg.a_float, (char *) &src.a_float, &size);

  for (i = 0; i < 10; i++) {
    printf("b_int_v[%3.3d] ..:% 12.12d %s\n", i, trg.b_int_v[i], print_bin(&trg.b_int_v[i]));
  }

  for (i = 0; i < 100; i++) {
    printf("a_float_v[%3.3d] :% 22.10f %15e %s\n", i, trg.a_float_v[i], trg.a_float_v[i], print_bin((int *)&trg.a_float_v[i]));
  }        

  printf("a_int .........:% 12.12d %s\n", trg.a_int, print_bin(&trg.a_int));

  for (i = 0; i < 5; i++) {
    printf("a_short_v[%3.3d] :% 12.12d\n", i, trg.a_short_v[i]);
  }

  for (i = 0; i < 5; i++) {
    printf("a_int_v[%3.3d] ..:% 12.12d %s\n", i, trg.a_int_v[i], print_bin(&trg.a_int_v[i]));
  }

  printf("a_char ........: %c\n", trg.a_char);

  printf("a_float .......:% 22.10f %15e %s\n", trg.a_float, trg.a_float, print_bin((int *)&trg.a_float));

  fclose(fp);

  exit(1);  

#endif
}
