/* rt_hd.h
   */
#ifndef rt_hd_h
#define rt_hd_h

#define HD_CQ_QUOTA		    50	/* Queue quota for Client queue */
#define HD_AQ_QUOTA		    50	/* Queue quota for Adm    queue */
#define HD_SQ_QUOTA		    50	/* Queue quota for Store  queue */

#define HD_SHORT_STR                41
#define HD_LONG_STR                161
#define HD_DATA_SIZE		     4	/* Lagringsformatet i byte */
#define HD_TABLES		     4	/* Antal tabeller för datalagring */
#define HD_COLUMNS		  1500	/* Max antal data kolumner */

#define HD_S_CLIENTS		    20
#define HD_A_CLIENTS		     5

#define HD_MAINT_COUNT            8640	/* Anger ant cykler mellan underhåll */
#define HD_CYCLE_TIME               10	/* Anger rs_hd_adm:s tid mellan exekv */
#define HD_VALID_FACT             50.0	/* Anger antal giltiga sample i % för att status på medelvärde = _OK */
#define HD_DELETE_FACT               2	/* Anger hur många lagringar mellan varje radering */
#define HD_MAX_INTERVAL             15	/* Anger max tid mellan exekvering */

#define HD_MEAN_TYPE                 1	/* Definierar typen för medelvärde */
#define HD_ACTIVE                    1	/* Def att punkten aktiv */
#define HD_NOT_ACTIVE                0	/* Def att punkten inte aktiv */
#define HD_NOT_USED                 -1	/* Def att punkten inte används */

#define HD_OK                        1	/* Status är OK */
#define HD_FAULT                    -1	/* Status är inte OK */

#endif
