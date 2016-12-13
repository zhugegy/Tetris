#include "stdafx.h"
#include "file_operator.h"

static int write_in_block_node(CustomizedBlock *psnTarget, 
                               CustomizedBlock *psnTemplate);

//向链表信息文件写入（字符写入）
int write_customized_block_into_file__file_operator(CustomizedBlock *pstTmp)
{
  int i = 0;
  int j = 0;

  FILE *pFile = fopen(FILE_LOCATION_LINK_INFO, "r+");

  fseek(pFile, 0, SEEK_END);

  fprintf(pFile, "%d %d %d %d ", 
    pstTmp->nSerial, 
    pstTmp->nCenterX, 
    pstTmp->nCenterY, 
    pstTmp->nFrequency);

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      fprintf(pFile, "%d ", pstTmp->TetrisCustomizeBlocksSpace[i][j]);
    }
  }

  fflush(pFile);

  fclose(pFile);

  return 0;
}

//从链表信息文件载入链表信息
/*int load_block_list__file_operator(CustomizedBlock *pstFirstNode)*/
/*20161017 本来是传第一个指针，结果有BUG，调试之后发现原来得传Param *pstParam，
但是还是不100%知道为什么。形参/实参的问题，大体能明白*/
/*同样的情况还有它的子函数file_operator_insert_block_node_into_chain的参数*/
int load_block_list__file_operator(Param *pstParam)
{
  CustomizedBlock stTemplateBlock;
  FILE *pFile = fopen(FILE_LOCATION_LINK_INFO, "r+");
  int i = 0;
  int j = 0;

  stTemplateBlock.pNext = NULL;
  while (!feof(pFile))
  {
    fscanf(pFile, "%d %d %d %d ", 
      &stTemplateBlock.nSerial, 
      &stTemplateBlock.nCenterX, 
      &stTemplateBlock.nCenterY, 
      &stTemplateBlock.nFrequency);

    for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
    {
      for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
      {
        fscanf(pFile, "%d ", 
          &(stTemplateBlock.TetrisCustomizeBlocksSpace[i][j]));
      }
    }

    file_operator_insert_block_node_into_chain(pstParam, &stTemplateBlock);
  }

  fclose(pFile);

  return 0;
}

int file_operator_insert_block_node_into_chain(
  Param *pstParam, CustomizedBlock *pstTemplateBlock)
{
  CustomizedBlock *pstTmp;

  if (pstParam->pstCustomizedBlockNodes == NULL)
  {
    pstParam->pstCustomizedBlockNodes = 
      (CustomizedBlock *) malloc(sizeof(CustomizedBlock));

    if (pstParam->pstCustomizedBlockNodes == NULL)
    {
      return ERROR_CUSTOMIZE_BLOCK_NODE_MALLOC;
    }

    write_in_block_node(pstParam->pstCustomizedBlockNodes, pstTemplateBlock);
  }
  else
  {
    for (pstTmp = pstParam->pstCustomizedBlockNodes; pstTmp->pNext; 
      pstTmp = pstTmp->pNext);

      pstTmp->pNext = (CustomizedBlock *) malloc(sizeof(CustomizedBlock));

    if (pstTmp->pNext == NULL)
    {
      return ERROR_CUSTOMIZE_BLOCK_NODE_MALLOC;
    }

    pstTmp = pstTmp->pNext;

    write_in_block_node(pstTmp, pstTemplateBlock);
  }

  return 0;
}

//向BlockNode填入信息。
static int write_in_block_node(CustomizedBlock *psnTarget, 
                        CustomizedBlock *psnTemplate)
{
  psnTarget->nSerial = psnTemplate->nSerial;
  psnTarget->nCenterX = psnTemplate->nCenterX;
  psnTarget->nCenterY = psnTemplate->nCenterY;
  psnTarget->nFrequency = psnTemplate->nFrequency;

  memcpy(psnTarget->TetrisCustomizeBlocksSpace, 
    psnTemplate->TetrisCustomizeBlocksSpace,
    sizeof(psnTarget->TetrisCustomizeBlocksSpace) * sizeof(char));

  psnTarget->pNext = NULL;

  return 0;
}