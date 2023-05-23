#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
static int prior_read(resmgr_context_t *ctp, io_read_t *msg,
                      RESMGR_OCB_T *ocb)
{
    static bool odd = true;
    int status = iofunc_read_verify(ctp, msg, ocb, NULL);
    if (status != EOK)
        return status;
    if (msg->i.xtype & _IO_XTYPE_MASK != _IO_XTYPE_NONE)
        return ENOSYS;
    if (odd)
    {
        struct sched_param param;
        sched_getparam(0, &param);
        static char rbuf[23];
        sprintf(rbuf, "%d %s\n", param.sched_curpriority,"Kalaganov Kirill I993");
        MsgReply(ctp->rcvid, strlen(rbuf) + 1, rbuf, strlen(rbuf) + 1);
    }
    else
        MsgReply(ctp->rcvid, 0, NULL, 0);
    odd = !odd;
    return _RESMGR_NOREPLY;
}

int main(int argc, char **argv)
{
    resmgr_attr_t resmgr_attr;
    dispatch_t *dpp;
    dispatch_context_t *ctp;

    int id;
    if ((dpp = dispatch_create()) == NULL)
        perror("allocate dispatch"), exit(EXIT_FAILURE);

    memset(&resmgr_attr, 0, sizeof resmgr_attr);
    resmgr_attr.nparts_max = 1;
    resmgr_attr.msg_max_size = 2048;

    static resmgr_connect_funcs_t connect_funcs;
    static resmgr_io_funcs_t io_funcs;
    iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs,
                     _RESMGR_IO_NFUNCS, &io_funcs);

    io_funcs.read = prior_read;
    static iofunc_attr_t attr;
    iofunc_attr_init(&attr, S_IFNAM | 0666, 0, 0);
    id = resmgr_attach(dpp, &resmgr_attr, "/dev/prior",
                       _FTYPE_ANY, 0, &connect_funcs,
                       &io_funcs, &attr);
    if (id == -1)
        perror("attach name"), exit(EXIT_FAILURE);
    ctp = dispatch_context_alloc(dpp);
    while (true)
    {
        if ((ctp = dispatch_block(ctp)) == NULL)
            perror("block error"), exit(EXIT_FAILURE);
        dispatch_handler(ctp);
    }
};
