#ifndef xtt_jop_h
#define xtt_jop_h

/* xtt_jop.h -- Handling of java processe in xtt.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

class Jop {
  public:
    Jop(
	void *op_parent_ctx);
    ~Jop();

    void 		*parent_ctx;
    qcom_sQid		qcom_qid;
    qcom_sQid		jop_qcom_qid;
    void 		(*command_cb)( void *, char *);

    void	scan();
    int		create_que();
    void 	que_free( char *data);
    int 	que_get( char **data);
    int 	que_put( char *data);
    void	set_jop_qid( int qix);
    int 	command( char *command);
    void	close();
    void	create_process();
};

#if defined __cplusplus
}
#endif
#endif

