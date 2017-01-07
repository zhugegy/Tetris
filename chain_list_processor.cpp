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
/*(快捷数组存储了链表中元素的地址，方便以后的直接操作，而不用每次都遍历链表,20161219
看代码的时候是这么猜测的，记不清最开始写这个代码的时候的思路了)*/
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

//构建当前方块的BlockElement链表(玩家方块元素链表)
int insert_player_block_element_node_into_chain__chain_list_processor(
  Param *pstParam, BlockElement *pstBlockElementTemplate)
{
  BlockElement *pstTmp;

  if (pstParam->pstFirstBlockElementPlayer == NULL)
  {
    pstParam->pstFirstBlockElementPlayer = 
      (BlockElement *) malloc(sizeof(BlockElement));

    if (pstParam->pstFirstBlockElementPlayer == NULL)
    {
      return ERROR_BLOCK_ELEMENT_NODE_MALLOC;
    }

    write_in_block_element_node__chain_list_processor(
      pstParam->pstFirstBlockElementPlayer, pstBlockElementTemplate);

  }
  else
  {
    for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp->pNext; 
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

//构建当前方块的BlockElement链表(电脑方块元素链表)
int insert_COM_block_element_node_into_chain__chain_list_processor(
  Param *pstParam, BlockElement *pstBlockElementTemplate)
{
  BlockElement *pstTmp;

  if (pstParam->pstFirstBlockElementCOM == NULL)
  {
    pstParam->pstFirstBlockElementCOM =
      (BlockElement *) malloc(sizeof(BlockElement));

    if (pstParam->pstFirstBlockElementCOM == NULL)
    {
      return ERROR_BLOCK_ELEMENT_NODE_MALLOC;
    }

    write_in_block_element_node__chain_list_processor(
      pstParam->pstFirstBlockElementCOM, pstBlockElementTemplate);

  }
  else
  {
    for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp->pNext;
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
  //20170101 见上级函数
  pstDest->nSerial = pstSrc->nSerial;

  //附加修改(无关紧要的)
  pstDest->nTailValue = pstSrc->nTailValue;

  pstDest->stCoord = pstSrc->stCoord;
  pstDest->nValue = pstSrc->nValue;
  pstDest->isCenter = pstSrc->isCenter;
  pstDest->pCenter = pstSrc->pCenter;
  pstDest->pNext = NULL;

  return 0;
}

//释放当前方块元素资源
int free_current_block_element__chain_list_processor(Param *pstParam, 
	PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;
  BlockElement *pstNext = NULL;

  if (eFlag == PLAYER_CONTROL)
  {
	  pstTmp = pstParam->pstFirstBlockElementPlayer;
  }
  if (eFlag == COM_CONTROL)
  {
	  pstTmp = pstParam->pstFirstBlockElementCOM;
  }

  while (pstTmp)
  {
    pstNext = pstTmp->pNext;

    free((void *) pstTmp);

    pstTmp = pstNext;
  }

  if (eFlag == PLAYER_CONTROL)
  {
	  pstParam->pstFirstBlockElementPlayer = NULL;
  }
  if (eFlag == COM_CONTROL)
  {
	  pstParam->pstFirstBlockElementCOM = NULL;
  }

  return 0;
}

//释放当前方块元素资源
int free_block_info__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pstTmp = NULL;
  CustomizedBlock *pstNext = NULL;

  pstTmp = pstParam->pstCustomizedBlockNodes;

  while (pstTmp)
  {
    pstNext = pstTmp->pNext;

    free((void *)pstTmp);

    pstTmp = pstNext;
  }

  pstParam->pstCustomizedBlockNodes = NULL;

  return 0;
}