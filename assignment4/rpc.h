/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_H_RPCGEN
#define _RPC_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


#define RPCPROG ((rpc_uint)0x20000824)
#define RPCVERS ((rpc_uint)1)

#ifdef __cplusplus
#define RPC_PROCESS ((rpc_uint)1)
extern "C" void * rpc_process_1(char *, CLIENT *);
extern "C" void * rpc_process_1_svc(char *, struct svc_req *);

#elif __STDC__
#define RPC_PROCESS ((rpc_uint)1)
extern  void * rpc_process_1(char *, CLIENT *);
extern  void * rpc_process_1_svc(char *, struct svc_req *);

#else /* Old Style C */
#define RPC_PROCESS ((rpc_uint)1)
extern  void * rpc_process_1();
extern  void * rpc_process_1_svc();
#endif /* Old Style C */

#endif /* !_RPC_H_RPCGEN */
