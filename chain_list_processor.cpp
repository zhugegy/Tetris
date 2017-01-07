#include "stdafx.h"
#include "chain_list_processor.h"

static int write_in_block_element_node__chain_list_processor(
  BlockElement *pstDest, BlockElement *pstSrc);

//�����Զ���ķ�����һ�����õ���š�
int find_avaiable_serial__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pTemp = NULL;
  int i = 0;

  //�����ʱ��δ���ַ������루�����ǿյģ�
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

  //�����������-1������ԡ�
  return -1;
}

//��Ŀǰ����������
int get_block_total_num__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pstTmp = NULL;

  for (pstTmp = pstParam->pstCustomizedBlockNodes; pstTmp->pNext; 
    pstTmp = pstTmp->pNext);

  return pstTmp->nSerial;
}

//�����������������
/*(�������洢��������Ԫ�صĵ�ַ�������Ժ��ֱ�Ӳ�����������ÿ�ζ���������,20161219
�������ʱ������ô�²�ģ��ǲ����ʼд��������ʱ���˼·��)*/
int build_fast_array__chain_list_processor(Param *pstParam)
{
  CustomizedBlock *pstTmp = NULL;

  for (pstTmp = pstParam->pstCustomizedBlockNodes; pstTmp; 
    pstTmp = pstTmp->pNext)
  {
    //fastarray��Ϣ��1��ʼ��������0
    pstParam->pstFastArray[pstTmp->nSerial] = pstTmp;
  }

  return 0;
}

//������ǰ�����BlockElement����(��ҷ���Ԫ������)
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

//������ǰ�����BlockElement����(���Է���Ԫ������)
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

//����ģ�嵽����������Ԫ��
static int write_in_block_element_node__chain_list_processor(
  BlockElement *pstDest, BlockElement *pstSrc)
{
  //20170101 ���ϼ�����
  pstDest->nSerial = pstSrc->nSerial;

  //�����޸�(�޹ؽ�Ҫ��)
  pstDest->nTailValue = pstSrc->nTailValue;

  pstDest->stCoord = pstSrc->stCoord;
  pstDest->nValue = pstSrc->nValue;
  pstDest->isCenter = pstSrc->isCenter;
  pstDest->pCenter = pstSrc->pCenter;
  pstDest->pNext = NULL;

  return 0;
}

//�ͷŵ�ǰ����Ԫ����Դ
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

//�ͷŵ�ǰ����Ԫ����Դ
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