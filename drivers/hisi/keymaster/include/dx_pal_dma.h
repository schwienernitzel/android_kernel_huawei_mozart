/*******************************************************************  
* (c) Copyright 2011-2012 Discretix Technologies Ltd.              *
* This software is protected by copyright, international           *
* treaties and patents.                                            *
* Use of this Software as part of or with the Discretix CryptoCell *
* or Packet Engine products is governed by the products'           *
* commercial end user license agreement ("EULA").                  *
* It is possible that copies of this Software might be distributed *
* under some type of GNU General Public License ("GPL").           *
* Notwithstanding any such distribution under the terms of GPL,    *
* GPL does not govern the use of this Software as part of or with  *
* the Discretix CryptoCell or Packet Engine products, for which a  *
* EULA is required.                                                *
* If the product's EULA allows any copy or reproduction of this    *
* Software, then such copy or reproduction must include this       *
* Copyright Notice as well as any other notices provided           *
* thereunder.                                                      *
********************************************************************/



#ifndef _DX_PAL_DMA_H
#define _DX_PAL_DMA_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "dx_pal_types.h"
#include "dx_pal_dma_plat.h"

typedef uint32_t DX_PAL_DmaAddr_t;
typedef void * DX_PAL_DmaBufferHandle; 

typedef enum {
	DX_PAL_DMA_DIR_NONE = 0,	
	DX_PAL_DMA_DIR_TO_DEVICE = 1,	
	DX_PAL_DMA_DIR_FROM_DEVICE = 2,
	DX_PAL_DMA_DIR_BI_DIRECTION = 3,
	DX_PAL_DMA_DIR_MAX,
	DX_PAL_DMA_DIR_RESERVE32 = 0x7FFFFFFF
}DX_PAL_DmaBufferDirection_t;


// user buffer scatter info
typedef struct {
	DX_PAL_DmaAddr_t	blockPhysAddr;
	uint32_t		blockSize;
}DX_PAL_DmaBlockInfo_t;


#ifdef BIG__ENDIAN
#define  SET_WORD_LE(val) cpu_to_le32(val)
#else
#define  SET_WORD_LE
#endif

/** 
 * @brief   Maps a given buffer of any type. Returns the list of DMA-able blocks that the buffer maps to. 
 *         
 * @param[in] pDataBuffer -  Address of the buffer to map
 * @param[in] buffSize - Buffer size in bytes
 * @param[in] copyDirection - Copy direction of the buffer. Can be TO_DEVICE, FROM_DEVICE or BI_DIRECTION
 * @param[in/out] numOfBlocks - maximum numOfBlocks to fill, as output the actual number
 * @param[out] pDmaBlockList - List of DMA-able blocks that the buffer maps to 
 * @param[out] dmaBuffHandle - A handle to the mapped buffer private resources
 *
 * @return Returns a non-zero value in case of failure
 */ 	
uint32_t DX_PAL_DmaBufferMap(uint8_t                	  *pDataBuffer,
			     uint32_t                     buffSize,
			     DX_PAL_DmaBufferDirection_t  copyDirection,
			     uint32_t                     *pNumOfBlocks,
			     DX_PAL_DmaBlockInfo_t        *pDmaBlockList,
			     DX_PAL_DmaBufferHandle       *dmaBuffHandle);
 

/** 
 * @brief   Unmaps a given buffer, and frees its associated resources, if exist
 *         
 * @param[in] pDataBuffer -  Address of the buffer to map
 * @param[in] buffSize - Buffer size in bytes
 * @param[in] copyDirection - Copy direction of the buffer. Can be TO_DEVICE, FROM_DEVICE or BI_DIRECTION
 * @param[in] numOfBlocks - Number of DMA-able blocks that the buffer maps to
 * @param[in] pDmaBlockList - List of DMA-able blocks that the buffer maps to 
 * @param[in] dmaBuffHandle - A handle to the mapped buffer private resources
 *
 * @return Returns a non-zero value in case of failure
 */ 	
uint32_t DX_PAL_DmaBufferUnmap(uint8_t                	  *pDataBuffer,
			     uint32_t                     buffSize,
			     DX_PAL_DmaBufferDirection_t  copyDirection,
			     uint32_t                     numOfBlocks,
			     DX_PAL_DmaBlockInfo_t        *pDmaBlockList,
			     DX_PAL_DmaBufferHandle       dmaBuffHandle);
   
   
/** 
 * @brief   Allocates a DMA-contiguous buffer, and returns both its physical and virtual addresses
 *         
 *
 * @param[in] buffSize - Buffer size in bytes
 * @param[out] ppVirtBuffAddr - Virtual address of the allocated buffer
 *
 * @return Returns a non-zero value in case of failure
 */ 	
uint32_t DX_PAL_DmaContigBufferAllocate(uint32_t          buffSize,
					uint8_t           **ppVirtBuffAddr);



/** 
 * @brief   free resources previuosly allocated by DX_PAL_DmaContigBufferAllocate 
 *         
 *
 * @param[in] buffSize - buffer size in Bytes
 * @param[in] pVirtBuffAddr - virtual address of the buffer to free
 *
 * @return success/fail
 */ 	
uint32_t DX_PAL_DmaContigBufferFree(uint32_t          buffSize,
				    uint8_t           *pVirtBuffAddr);

    
    
/** 
 * @brief   Returns TRUE if the buffer is guaranteed to be a single contiguous DMA block, and FALSE otherwise.
 *
 *         
 * @param[in] pDataBuffer - User buffer address
 * @param[in] buffSize - User buffer size
 *
 * @return Returns TRUE if the buffer is guaranteed to be a single contiguous DMA block, and FALSE otherwise.
 */ 	
uint32_t DX_PAL_IsDmaBufferContiguous(uint8_t                	  *pDataBuffer,
				      uint32_t                    buffSize);
 
    
#ifdef __cplusplus
}
#endif

#endif


