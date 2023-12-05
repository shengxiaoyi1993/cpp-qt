#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include "../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h"
#include "../../../common_lib/sskj_lib/com_client/include/com_client_export.h"
#include "../../../common_lib/sskj_lib/com_server/include/com_server_export.h"
/// 用于com模块字符串与具体数据结构的的转换



namespace ClientApi {

// 使用该模块前调用，只需调用一次
int initHttpEnv();


// 结束使用该模块后调用，只需调用一次
int uintHTTPEnv();





};




#endif // CLIENTAPI_H
