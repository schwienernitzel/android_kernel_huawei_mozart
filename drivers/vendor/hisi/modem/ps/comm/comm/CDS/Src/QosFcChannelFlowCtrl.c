


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 头文件包含
*****************************************************************************/

#include "CDS.h"
#include "QosFcCommon.h"
#include "QosFcChannelFlowCtrl.h"
#include "QosFcOm.h"
#include "FcCdsInterface.h"
#include "CdsMsgProc.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_QOS_FC_CHANNEL_FLOWCTRL_C
/*lint +e767*/




/*****************************************************************************
  2 外部函数声明
*****************************************************************************/

/******************************************************************************
   3 私有定义
******************************************************************************/

/******************************************************************************
   4 全局变量定义
******************************************************************************/
QOS_FC_CHANNEL_ENTITY_STRU g_stQosFcChannelEntity[FC_MAX_RAB_NUM];

/******************************************************************************
   5 函数实现
******************************************************************************/

VOS_VOID QosFcChannelInit(VOS_VOID)
{
    VOS_UINT32 i=0;
    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        g_stQosFcChannelEntity[i].ucRabId = FC_INVALID_RABID;
        g_stQosFcChannelEntity[i].enQci = QCI_TYPE_BUTT;
        g_stQosFcChannelEntity[i].enChannelState = QOS_FC_CHANNEL_STATE_BUTT;
    }

    return;
}


VOS_VOID QosFc_SendStopChannel2AFC(VOS_UINT32 ulIndex)
{
    CDS_FC_STOP_CHANNEL_IND_STRU    *pstStopChannelInd;
    QOS_FC_CHANNEL_ENTITY_STRU      *pstQosFcChannelEntity;
    MODEM_ID_ENUM_UINT16             enModemId;

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter QosFc_SendStopChannel2AFC ");

    if(FC_MAX_RAB_NUM <= ulIndex)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC : Error Index: ",ulIndex);
        return;
    }

    pstQosFcChannelEntity = &g_stQosFcChannelEntity[ulIndex];
    if(QosFc_IsInValidRabId(FC_MASK_RABID & pstQosFcChannelEntity->ucRabId))
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC : Error RabId: ",pstQosFcChannelEntity->ucRabId);
        return ;
    }

    enModemId = pstQosFcChannelEntity->ucRabId >> FC_MODEMID_SHIFT;
    if(MODEM_ID_BUTT <= enModemId)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC : Error ModemId:", enModemId);
        return;
    }

    /*申请OSA消息内存*/
    pstStopChannelInd = (VOS_VOID *)PS_ALLOC_MSG_ALL_CHECK(UEPS_PID_CDS, sizeof(CDS_FC_STOP_CHANNEL_IND_STRU));
    if (VOS_NULL_PTR == pstStopChannelInd)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC : Error to Alloc Msg Mem");
        return ;
    }

    /*填写消息内容*/
    CDS_CFG_MSG_HDR(pstStopChannelInd, ACPU_PID_FC);
    pstStopChannelInd->enMsgName = ID_CDS_FC_STOP_CHANNEL_IND;
    pstStopChannelInd->ucRabId = FC_MASK_RABID & pstQosFcChannelEntity->ucRabId;
    pstStopChannelInd->enModemId = enModemId;

    /*发送消息*/
    if (VOS_OK != PS_SND_MSG_ALL_CHECK(UEPS_PID_CDS, pstStopChannelInd))
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC : Send Msg Fail");
        return;
    }

    FC_DBG_CHANNEL_FC_STAT(enModemId, (FC_MASK_RABID & pstQosFcChannelEntity->ucRabId), 1);
    CDS_WARNING_LOG1(UEPS_PID_CDS,"QosFc_SendStopChannel2AFC: Send Stop Msg Succ:", pstQosFcChannelEntity->ucRabId);

    return;
}
VOS_VOID QosFc_SendStartChannel2AFC(VOS_UINT32 ulIndex)
{
    CDS_FC_START_CHANNEL_IND_STRU   *pstStartChannelInd;
    QOS_FC_CHANNEL_ENTITY_STRU      *pstQosFcChannelEntity;
    MODEM_ID_ENUM_UINT16             enModemId;

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter QosFc_SendStartChannel2AFC ");

    if(FC_MAX_RAB_NUM <= ulIndex)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC : Error Index: ",ulIndex);
        return;
    }

    pstQosFcChannelEntity = &g_stQosFcChannelEntity[ulIndex];
    if(QosFc_IsInValidRabId(FC_MASK_RABID & pstQosFcChannelEntity->ucRabId))
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC : Error RabId: ",pstQosFcChannelEntity->ucRabId);
        return;
    }

    enModemId = pstQosFcChannelEntity->ucRabId >> FC_MODEMID_SHIFT;
    if(MODEM_ID_BUTT <= enModemId)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC : Error ModemId:", enModemId);
        return;
    }

    /*申请OSA消息内存*/
    pstStartChannelInd = (VOS_VOID *)PS_ALLOC_MSG_ALL_CHECK(UEPS_PID_CDS, sizeof(CDS_FC_START_CHANNEL_IND_STRU));
    if (VOS_NULL_PTR == pstStartChannelInd)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC : Error to Alloc Msg Mem");
        return ;
    }

    /*填写消息内容*/
    CDS_CFG_MSG_HDR(pstStartChannelInd, ACPU_PID_FC);
    pstStartChannelInd->enMsgName = ID_CDS_FC_START_CHANNEL_IND;
    pstStartChannelInd->ucRabId = FC_MASK_RABID & pstQosFcChannelEntity->ucRabId;
    pstStartChannelInd->enModemId = enModemId;

    /*发送消息*/
    if (VOS_OK != PS_SND_MSG_ALL_CHECK(UEPS_PID_CDS, pstStartChannelInd))
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC : Send Msg Fail");
        return;
    }

    FC_DBG_CHANNEL_RESUME_STAT(enModemId, (FC_MASK_RABID & pstQosFcChannelEntity->ucRabId), 1);
    CDS_WARNING_LOG1(UEPS_PID_CDS,"QosFc_SendStartChannel2AFC: Send Start Msg Succ:", pstQosFcChannelEntity->ucRabId);

    return;
}
VOS_VOID QosFc_ChannelControl(VOS_UINT8 ucRabId)
{
    VOS_UINT32  i;

    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        if(ucRabId == g_stQosFcChannelEntity[i].ucRabId)
        {
            /* 找到对应通道位置 */
            break;
        }
    }
    if(FC_MAX_RAB_NUM == i)
    {
        /* 出错保护 */
        CDS_ERROR_LOG1(UEPS_PID_CDS, "QosFc_ChannelControl : Not find the channel: ", ucRabId);
        return;
    }

    if(QOS_FC_CHANNEL_STATE_FLOWCTRL != g_stQosFcChannelEntity[i].enChannelState)
    {
        CDS_WARNING_LOG2(UEPS_PID_CDS, "QosFc_ChannelControl : RabId: Pre State:", ucRabId, g_stQosFcChannelEntity[i].enChannelState );
        /* 置通道为流控状态 */
        g_stQosFcChannelEntity[i].enChannelState = QOS_FC_CHANNEL_STATE_FLOWCTRL;

        QosFc_SendStopChannel2AFC(i);
    }

    return;
}
VOS_VOID QosFc_ChannelRestore(VOS_UINT8 ucRabId)
{
    VOS_UINT32  i;

    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        if(ucRabId == g_stQosFcChannelEntity[i].ucRabId)
        {
            /* 找到对应通道位置 */
            break;
        }
    }
    if(FC_MAX_RAB_NUM == i)
    {
        /* 出错保护 */
        CDS_ERROR_LOG1(UEPS_PID_CDS, "QosFc_ChannelRestore : Not find the channel: ", ucRabId);
        return;
    }

    if(QOS_FC_CHANNEL_STATE_NORMAL != g_stQosFcChannelEntity[i].enChannelState)
    {
        CDS_WARNING_LOG2(UEPS_PID_CDS, "QosFc_ChannelRestore : RabId: Pre State:", ucRabId, g_stQosFcChannelEntity[i].enChannelState );
        /* 恢复通道数传 */
        g_stQosFcChannelEntity[i].enChannelState = QOS_FC_CHANNEL_STATE_NORMAL;

        QosFc_SendStartChannel2AFC(i);
    }

    return;
}
VOS_VOID QosFc_UpdateChannelQos(VOS_UINT8 ucRabId, QCI_TYPE_ENUM_UINT8 enQci)
{
    VOS_UINT32  i,j;

    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        if((ucRabId == g_stQosFcChannelEntity[i].ucRabId)
            ||(FC_INVALID_RABID == g_stQosFcChannelEntity[i].ucRabId))
        {
            /* 找到对应通道位置 */
            break;
        }
    }

    if(FC_MAX_RAB_NUM == i)
    {
        /* 出错保护 */
        CDS_ERROR_LOG(UEPS_PID_CDS, "QosFc_UpdateChannelQos : too many channel created!");
        return;
    }

    if(QCI_TYPE_BUTT != enQci)
    {
        if(FC_INVALID_RABID == g_stQosFcChannelEntity[i].ucRabId)
        {
            /* 新建通道 */
            g_stQosFcChannelEntity[i].ucRabId = ucRabId;
            g_stQosFcChannelEntity[i].enQci = enQci;
            g_stQosFcChannelEntity[i].enChannelState = QOS_FC_CHANNEL_STATE_NORMAL;
            CDS_WARNING_LOG2(UEPS_PID_CDS, "QosFc_UpdateChannelQos : Create Channel: Qci:", ucRabId, enQci);
        }
        else
        {
            CDS_WARNING_LOG3(UEPS_PID_CDS, "QosFc_UpdateChannelQos : Update Channel: Qci: -> :", ucRabId, g_stQosFcChannelEntity[i].enQci, enQci);
            /* 更新QCI */
            g_stQosFcChannelEntity[i].enQci = enQci;
        }

        /* 给A核流控模块发消息更新通道QCI */
        /* 暂由AT更新通道QCI，此处不发消息 */
    }
    else
    {
        /* 给A核流控模块发消息删除通道 */
        /* 暂由AT负责，此处不发消息 */

        /* 通道删除时恢复通道状态，保护设计 */
        QosFc_ChannelRestore(ucRabId);

        CDS_WARNING_LOG2(UEPS_PID_CDS, "QosFc_UpdateChannelQos : Delete Channel: Qci:", g_stQosFcChannelEntity[i].ucRabId, g_stQosFcChannelEntity[i].enQci);

        /* 删除通道 */
        for(j = i; j < FC_MAX_RAB_NUM-1; j++)
        {
            if(FC_INVALID_RABID == g_stQosFcChannelEntity[j+1].ucRabId)
            {
                break;
            }
            VOS_MemCpy(&g_stQosFcChannelEntity[j], &g_stQosFcChannelEntity[j+1], sizeof(QOS_FC_CHANNEL_ENTITY_STRU));
        }

        g_stQosFcChannelEntity[j].ucRabId = FC_INVALID_RABID;
        g_stQosFcChannelEntity[j].enQci = QCI_TYPE_BUTT;
        g_stQosFcChannelEntity[j].enChannelState = QOS_FC_CHANNEL_STATE_BUTT;

    }

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



