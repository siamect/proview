#ifndef glow_utils_h
#define glow_utils_h

#if defined __cplusplus
  extern "C" {
#endif

void glow_qsort(
  void *basearg, 
  size_t nel, 
  size_t width, 
  int (* compar)(const void*, const void*)
);

#if defined __cplusplus
  }
#endif

#endif
