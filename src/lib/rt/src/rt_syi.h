/* rt_syi.h -- System information

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   This module gives information about the system.  */

char		*syi_BootDisk (pwr_tStatus*, char*, int);
int		syi_Busid (pwr_tStatus*);
char		*syi_Ethernet (pwr_tStatus*, char*, int);
char		*syi_HostName (pwr_tStatus*, char*, int);
char		*syi_HostSpec (pwr_tStatus*, char*, int);
char		*syi_NodeName (pwr_tStatus*, char*, int);
char		*syi_NodeSpec (pwr_tStatus*, char*, int);
pwr_tBoolean	syi_LocalBoot (pwr_tStatus*);
