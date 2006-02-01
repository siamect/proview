
/* 
 * Proview   $Id: rt_mq.c,v 1.4 2006-02-01 09:12:17 claes Exp $
 * Copyright (C) GARRET.
 *
 * Distribution terms
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the Software), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHOR OF THIS SOFTWARE BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*-< MQUEUE.C >-----------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Message queue implementation                                     *        */
/*------------------------------------------------------------------*--------*/

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <alloca.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <sys/msg.h>

#include <sys/mman.h>

#ifndef MSGMAX
#define MSGMNI   128   /* <= 1K */     /* max # of msg queue identifiers */
#define MSGMAX  4056   /* <= 4056 */   /* max size of message (bytes) */
#endif

#include "rt_mq.h"

#define IMPLEMENT_MQ_NOTIFY 1 // support asynchronouse message receiving

mqd_t mq_open(const char *name, int oflag, ...)
{
    key_t key = IPC_PRIVATE;
    int msgid, fd = -1;
    int mode = 0;
    mqd_t q;

    if (name != NULL) { 
        fd = open(name, O_RDWR|O_CREAT, 0777);
	if (fd < 0) {
	    return (mqd_t)-1;
	}
	key = ftok(name, 0);
	if (key < 0) {
	    return (mqd_t)-1;
	}
    }
    if (oflag & O_CREAT) { 
	va_list ap;
	va_start(ap, oflag);
	mode = va_arg(ap, mode_t)|IPC_CREAT;
	va_end(ap);
    }
    if (oflag & O_EXCL) { 
	mode |= IPC_EXCL;
    }
    msgid = msgget(key, mode);
    if (msgid < 0) { 
	return (mqd_t)-1;
    }
    q = (mqd_t)malloc(sizeof(struct mq_descriptor));
    q->msgid = msgid;
    q->fd = fd;
    q->flags = 0;
#if IMPLEMENT_MQ_NOTIFY
    ftruncate(fd, sizeof(struct mq_notification_request));
    q->req = (struct mq_notification_request*)
	mmap(NULL, sizeof(struct mq_notification_request),
	     PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if (q->req == (struct mq_notification_request*)-1) { 
	q->req = NULL;
    } else { 
	q->req->pid = 0;
    }
#else 
    close(fd);
    q->fd = -1;
    q->req = NULL;
#endif		
    return q;
}  
    
int mq_close(mqd_t mqdes)
{
    if (mqdes->req != NULL) { 
	munmap((void*)mqdes->req, sizeof(struct mq_notification_request));
    }
    if (mqdes->fd >= 0) { 
	close(mqdes->fd);
    }
    free(mqdes);
    return 0;
}

int mq_unlink(const char *name)
{
    int key, msgid;
    key = ftok(name, 0);
    if (key < 0) {
	return -1;
    }
    msgid = msgget(key, 0);
    if (msgid < 0) { 
	return -1;
    }
    unlink(name);
    return msgctl(msgid, IPC_RMID, 0);    
}

int mq_notify(mqd_t mqdes, const struct sigevent *notification)
{
    if (mqdes->req == NULL) { 
	errno = ENOENT;
	return -1;
    } 
    if (notification == NULL || notification->sigev_notify == SIGEV_NONE) { 
	mqdes->req->pid = 0;
    } else if (notification->sigev_notify == SIGEV_SIGNAL) { 
	if (mqdes->req->pid != 0) { 
	    errno = EBUSY;
	    return -1;
	}
	mqdes->req->pid = getpid();
	mqdes->req->signo = notification->sigev_signo;
    } else { 
	errno = EINVAL;
	return -1;
    }
    return 0;
}



int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
	    unsigned int msg_prio)
{
    struct msgbuf* buf;
    int rc;
    if (msg_prio > PWR_MQ_PRIO_MAX) { 
	errno = EINVAL;
	return -1;
    }
    buf = (struct msgbuf*)alloca(msg_len + sizeof(long));
    buf->mtype = PWR_MQ_PRIO_MAX - msg_prio;
    memcpy(buf->mtext, msg_ptr, msg_len);
    rc = msgsnd(mqdes->msgid, buf, msg_len, mqdes->flags);
    if (rc == 0 && mqdes->req != NULL && mqdes->req->pid != 0) { 
	kill(mqdes->req->pid, mqdes->req->signo);
	mqdes->req->pid = 0;
    }
    return 0;
}


ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, 
		   unsigned int *msg_prio)
{
    struct msgbuf* buf = (struct msgbuf*)alloca(msg_len + sizeof(long));
    ssize_t rc = msgrcv(mqdes->msgid, buf, msg_len,-PWR_MQ_PRIO_MAX, mqdes->flags);
    if (msg_prio != NULL && rc >= 0) { 
	*msg_prio = PWR_MQ_PRIO_MAX - buf->mtype;
    }
    if (rc > 0) { 
	memcpy(msg_ptr, buf->mtext, rc);
    }
    return rc;
}


int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat)
{
    struct msqid_ds buf;
    if (mqstat == NULL) { 
	errno = EINVAL;
	return -1;
    }
    if (msgctl(mqdes->msgid, IPC_STAT, &buf) < 0) { 
	return -1;
    }
    mqstat->mq_maxmsg = MSGMNI;	    /* max # of messages allowed in MQ */
    mqstat->mq_msgsize = MSGMAX;     /* max size of any one message */
    mqstat->mq_flags = (mqdes->flags & IPC_NOWAIT) ? O_NONBLOCK : 0; 
    mqstat->mq_curmsgs = buf.msg_qnum; /* # of messages currently on mq */
    return 0;
}


int mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat,
	       struct mq_attr *omqstat)
{
    long oflags = mqdes->flags;
    if (mqstat != NULL) { 
	mqdes->flags = (mqstat->mq_flags & O_NONBLOCK) ? IPC_NOWAIT : 0;
    } 
    if (omqstat != NULL) { 
	struct msqid_ds buf;
	if (msgctl(mqdes->msgid, IPC_STAT, &buf) < 0) { 
	    return -1;
	}
	omqstat->mq_maxmsg = MSGMNI;  
	omqstat->mq_msgsize = MSGMAX; 
	omqstat->mq_flags = (oflags & IPC_NOWAIT) ? O_NONBLOCK : 0; 
	omqstat->mq_curmsgs = buf.msg_qnum; 
    }
    return 0;
}
