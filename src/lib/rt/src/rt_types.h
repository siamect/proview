#ifndef rt_types_h
#define rt_types_h

/* co_types.h -- Enum and mask translations

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned int 	num;
	char		name[40];
	} types_sEnumElement;

typedef struct {
	pwr_tClassId		classid;
	char			name[40];
	types_sEnumElement	*elements;
	} types_sEnum;

int types_find_enum( pwr_tClassId classid, char *name, 
	types_sEnumElement **elem_p);
int types_translate_enum( pwr_tClassId classid, char *name, unsigned int num, 
	char *buf);
int types_find_mask( pwr_tClassId classid, char *name, 
		types_sEnumElement **elem_p);

#if defined __cplusplus
}
#endif
#endif

