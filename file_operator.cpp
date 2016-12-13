#include "stdafx.h"
#include "file_operator.h"

static int write_in_block_node(CustomizedBlock *psnTarget, 
                               CustomizedBlock *psnTemplate);

//��������Ϣ�ļ�д�루�ַ�д�룩
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

//��������Ϣ�ļ�����������Ϣ
/*int load_block_list__file_operator(CustomizedBlock *pstFirstNode)*/
/*20161017 �����Ǵ���һ��ָ�룬�����BUG������֮����ԭ���ô�Param *pstParam��
���ǻ��ǲ�100%֪��Ϊʲô���β�/ʵ�ε����⣬����������*/
/*ͬ����������������Ӻ���file_operator_insert_block_node_into_chain�Ĳ���*/
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

//��BlockNode������Ϣ��
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