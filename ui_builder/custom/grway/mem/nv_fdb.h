#ifndef __NV_FDB_H__
#define __NV_FDB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/gw_def.h"
#include "flashdb.h"

struct fdb_kvdb *nv_fdb_get_kvdb(void);
struct fdb_tsdb *nv_fdb_get_tsdb(void);
int nv_fdb_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __NV_FDB_H__ */