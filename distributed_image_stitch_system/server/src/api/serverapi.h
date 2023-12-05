#ifndef SERVERAPI_H
#define SERVERAPI_H

#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/com_client/include/com_client_export.h"
#include "../../../common_lib/sskj_lib/com_server/include/com_server_export.h"

/// 用于com模块字符串与具体数据结构的的转换
/// 由于没有状态，因此写成命名空间的方式

namespace ServerApi {
int getClientStatus(string v_client,ds_df_diss::ClientStatus_JE& v_state);



};


#endif // SERVERAPI_H
