#include "stdafx.h"
#include "chain_list_processor.h"

static int write_in_block_element_node__chain_list_processor(
  BlockElement *pstDest, BlockElement *pstSrc);

//给新自定义的方块找一个可用的序号。
int find_avaiable_serial__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pTemp = NULL;
  int i = 0;

  //如果此时尚未有字符串加入（链表是空的）
  if (pstParam->pstCustomizedBlockNodes == NULL) 
  {
    return 1;
  }

  for (i = 2; ; i++)
  {
    for (pTemp = pstParam->pstCustomizedBlockNodes; pTemp/*->psnNext*/; 
      pTemp = pTemp->pNext)
    {
      if (i == pTemp->nSerial)
      {
        break;
      }
    }

    if (pTemp == NULL)
    {
      return i;
    }
  }

  //如果出错，返回-1方便调试。
  return -1;
}

//求目前方块总数量
int get_block_total_num__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pstTmp = NULL;

  for (pstTmp = pstParam->pstCustomizedBlockNodes; pstTmp->pNext; 
    pstTmp = pstTmp->pNext);

  return pstTmp->nSerial;
}

//方块链表存入快捷数组
int build_fast_array__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pstTmp = NULL;

  for (pstTmp = pstParam->pstCustomizedBlockNodes; pstTmp; 
    pstTmp = pstTmp->pNext)
  {
    //fastarray信息从1开始，而不是0
    pstParam->pstFastArray[pstTmp->nSerial] = pstTmp;
  }

  return 0;
}

//构建当前方块的BlockElement链表
int insert_block_element_node_into_chain__chain_list_processor(
  Param *pstParam, BlockElement *pstBlockElementTemplate)
{
  BlockElement *pstTmp;

  if (pstParam->pstFirstBlockElement == NULL)
  {
    pstParam->pstFirstBlockElement = 
      (BlockElement *) malloc(sizeof(BlockElement));

    if (pstParam->pstFirstBlockElement == NULL)
    {
      return ERROR_BLOCK_ELEMENT_NODE_MALLOC;
    }

    write_in_block_element_node__chain_list_processor(
      pstParam->pstFirstBlockElement, pstBlockElementTemplate);

  }
  else
  {
    for (pstTmp = pstParam->pstFirstBlockElement; pstTmp->pNext; 
      pstTmp = pstTmp->pNext);

      pstTmp->pNext = (BlockElement *) malloc(sizeof(BlockElement));

    if (pstTmp->pNext == NULL)
    {
      return ERROR_BLOCK_ELEMENT_NODE_MALLOC;
    }

    pstTmp = pstTmp->pNext;

    write_in_block_element_node__chain_list_processor(pstTmp,
      pstBlockElementTemplate);
  }

  return 0;
}

//复制模板到真正的链表元素
static int write_in_block_element_node__chain_list_processor(
  BlockElement *pstDest, BlockElement *pstSrc)
{
  pstDest->stCoord = pstSrc->stCoord;
  pstDest->nValue = pstSrc->nValue;
  pstDest->isCenter = pstSrc->isCenter;
  pstDest->pCenter = pstSrc->pCenter;
  pstDest->pNext = NULL;

  return 0;
}

//释放当前方块元素资源
int free_current_block_element__chain_list_processor(Param *pstParam)
{
  BlockElement *pstTmp = NULL;
  BlockElement *pstNext = NULL;

  pstTmp = pstParam->pstFirstBlockElement;

  while (pstTmp)
  {
    pstNext = pstTmp->pNext;

    free((void *) pstTmp);

    pstTmp = pstNext;
  }

  pstParam->pstFirstBlockElement = NULL;

  return 0;
}