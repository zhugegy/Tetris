//���ļ�����������ز���

#include "stdafx.h"
#include "data_processor.h"

#include "chain_list_processor.h"

/*
static int is_the_top_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);*/
static int shift_tetris_customize_blocks_space_to_top__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
/*
static int is_the_left_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);*/
static int shift_tetris_customize_blocks_space_to_left__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int is_this_column_empty__data_processor(int nColumn,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int is_this_line_empty__data_processor(int nLine,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int get_customized_block_height_data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int get_customized_block_length_data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int shift_tetris_customize_blocks_space_to_right__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int shift_block_to_mid__data_processor(
  CustomizedBlock *pstCustomizedBlocks);

//��ʼ������˹���鷽��(��Ϸʱ)
int set_tetris_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      //��ȫ����Ϊ�ո�
      chTetris[i][j] = SPACE_TYPE(j);

      //���ŵ�2��ǽ
      if ((j == 0) || (j == TETRIS_PLAY_SPACE_X - 1))
      {
        chTetris[i][j] = WALL_VALUE;
      }

      //�ײ���ǽ
      if (i == TETRIS_PLAY_SPACE_Y - 1)
      {
        chTetris[i][j] = WALL_VALUE;
      }
    }
  }

  return 0;
}

//��ʼ������˹���鷽��(�Զ��巽��)
int set_tetris_customize_blocks_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      //��ȫ����Ϊ�ո�,�Զ���ģʽ�����ֿո��Ǵ��׻��ǻҰ�
      chTetris[i][j] = SPACE_VALUE_TYPE_A;

      //���ŵ�2��ǽ
      if ((j == 0) || (j == TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1))
      {
        chTetris[i][j] = WALL_VALUE;
      }

      //�Զ���ģʽû�еײ���ǽ
    }
  }

  return 0;
}

//�ı����˹���鷽���Ԫ����ֵ(�Զ��巽��)
int change_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char chValue, int nX, int nY)
{
  chTetris[nY][nX] = chValue;

  return 0;
}

//�������Ż�����˹���鷽��(�Զ��巽��)
int optimize_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //���������ƶ�����������
  while (is_this_line_empty__data_processor(0, chTetris) == 1)
  {
    shift_tetris_customize_blocks_space_to_top__data_processor(chTetris);
  }

  //���������ƶ�����������
  while (is_this_column_empty__data_processor(1, chTetris) == 1)
  {
    shift_tetris_customize_blocks_space_to_left__data_processor(chTetris);
  }

  return 0;
}

/*
//�鿴����˹���鷽��(�Զ��巽��)����һ���ǲ��ǿ�
static int is_the_top_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; i++)
  {
    if (chTetris[0][i] != SPACE_VALUE_TYPE_A)
    {
      //����зǿ�Ԫ�أ�����0
      return 0;
    }
  }

  return 1;
}*/

static int is_this_line_empty__data_processor(int nLine,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; i++)
  {
    if (chTetris[nLine][i] != SPACE_VALUE_TYPE_A)
    {
      //����зǿ�Ԫ�أ�����0
      return 0;
    }
  }

  return 1;
}

/*
//�鿴����˹���鷽���Զ��巽�飩����һ���ǲ��ǿ�
static int is_the_left_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    if (chTetris[i][1] != SPACE_VALUE_TYPE_A)
    {
      //����зǿ�Ԫ�أ�����0
      return 0;
    }
  }

  return 1;
}*/

//�鿴����˹���鷽��ĳһ���ǲ��ǿ�
static int is_this_column_empty__data_processor(int nColumn,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    if (chTetris[i][nColumn] != SPACE_VALUE_TYPE_A)
    {
      //����зǿ�Ԫ�أ�����0
      return 0;
    }
  }

  return 1;
}

//����˹���鷽���Զ��巽�飩��������һ�У����һ�в�0���������ߵ�ǽ��
static int shift_tetris_customize_blocks_space_to_top__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;
  
  //�������£����������һ�У���ÿһ�ж�����������һ��
  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y - 1; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      chTetris[i][j] = chTetris[i + 1][j];
    }
  }

  //���һ�е�����ֵ
  chTetris[i][0] = WALL_VALUE;
  
  for (j = 1; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; j++)
  {
    chTetris[i][j] = SPACE_VALUE_TYPE_A;
  }

  chTetris[i][j] = WALL_VALUE;

  return 0;
}

//����˹���鷽���Զ��巽�飩��������һ�У�����һ�в�0���������ߵ�ǽ��;-)
static int shift_tetris_customize_blocks_space_to_left__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  //�������ң���������һ�к����ұߵ�ǽ�����һ�У���ÿһ�ж������ұߵ���һ��
  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 - 1; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
    {
      chTetris[j][i] = chTetris[j][i + 1];
    }
  }

  //���ұߵ�ǽ�����һ����Ϊ��
  for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
  {
    chTetris[j][i] = SPACE_VALUE_TYPE_A;
  }

  return 0;
}

//����˹���鷽���Զ��巽�飩��������һ�У�����һ�в�0���������ߵ�ǽ��;-)
static int shift_tetris_customize_blocks_space_to_right__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  //�������ң���������һ�к����ұߵ�ǽ�����һ�У���ÿһ�ж������ұߵ���һ��
  //�������󣨲��������һ�к������ǽ���ұ�һ�У���ÿһ�ж�������ߵ���һ��
  for (i = TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 -1 ; i > 1; i--)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
    {
      chTetris[j][i] = chTetris[j][i - 1];
    }
  }

  //����ߵ�ǽ���ұ�һ����Ϊ��
  for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
  {
    chTetris[j][i] = SPACE_VALUE_TYPE_A;
  }

  return 0;
}

//�����Զ��巽��ĳ���(ֻ�������������Ż�֮�����״)
static int get_customized_block_length_data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; i++)
  {
    if (is_this_column_empty__data_processor(i, chTetris) == 1)
    {
      return i - 1;
    }
  }

  //return TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 - 1;
  return i - 1 - 1;
}

//�����Զ��巽��ĸ߶�
static int get_customized_block_height_data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  
  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    if (is_this_line_empty__data_processor(i, chTetris) == 1)
    {
      return i - 0;
    }
  }

  //return TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y - 1;
  return i - 0 - 1;
}

//����ÿ�����������Ȩ�ؼ������ĵ�Ĳ���֮һ
int align_weight_to_customized_block_element__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  int nLength = get_customized_block_length_data_processor(chTetris);
  int nHeight = get_customized_block_height_data_processor(chTetris);

  float fCenterPointX = ((float) nLength) / 2;
  float fCenterPointY = ((float) nHeight) / 2;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      if (chTetris[i][j] != WALL_VALUE && chTetris[i][j] != SPACE_VALUE_TYPE_A)
      {
        //���ֵ(12-6)^2 + (6-3)^2 = 36 + 9 = 45 ���ᳬ��unsigned char��Χ��
        //���þ���ֵ������Ϊ���������ֵ��ȡֵ��Χ
        chTetris[i][j] = (unsigned char) 
                         ((i - fCenterPointY) * (i - fCenterPointY) 
                           + 
                         (j - fCenterPointX) * (j - fCenterPointX));
      }
    }
  }

  return 0;
}

//����Զ��巽������ĵ����꣨pstCoord��
int get_customized_block_center_point__data_processor(ElementCoord *pstCoord, 
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //��ʼ��
  unsigned char chWeight = 66;    //49 + 16 ���ֵ + 1�����������ֵ45����
  pstCoord->nX = 1;
  pstCoord->nY = 0;
  int i = 0;
  int j = 0;

  //�����ѭ�����������������˹�����е����ĵ����
  for (i = TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y - 1; i >= 0 ; i--)
  {
    for (j = TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 - 1; j >= 1 ; j--)
    {
      if (chTetris[i][j] != SPACE_VALUE_TYPE_A &&
          chTetris[i][j] != WALL_VALUE && /*��ʵ��������ǽ�����������ֹ����*/
          chTetris[i][j] < chWeight)
      {
        chWeight = chTetris[i][j];
        pstCoord->nX = j;
        pstCoord->nY = i;
      }
    }
  }

  return 0;
}

//�����һ�����飨��ţ�
int get_a_random_block(Param *pstParam, int nCurrentBlock)
{
  int nSerial = 0;
  int nChance = 0;

  while (true)
  {
    nSerial = (rand() % pstParam->nBlockNum) + 1;
    nChance = rand() % 5;    /*0 - 4*/

    if (nSerial != nCurrentBlock /*��������������ͬ�ķ���*/ && 
      nChance < pstParam->pstFastArray[nSerial]->nFrequency)
    {
      return nSerial;
    }
    else
    {
      continue;
    }
  }

  return 0;
}

//�ѵ�ǰ���������Ԫ�ؼ�������
//�������Ӧ��(���)����chain_list_processor.cpp���Ȳ�����
int build_block_element_chain__data_processor(Param *pstParam, 
                   int nCurrentBlock, PlayerVSCOMControlFlag eFlag)
{
  unsigned char TetrisCustomizeBlocksSpaceTmp[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X] = {0};
  BlockElement stBlockElementTemplate;
  int i = 0;
  int j = 0;

  //pNext�����������ֵ��pCenter������ĺ�����ֵ
  stBlockElementTemplate.pNext = NULL;
  stBlockElementTemplate.pCenter = NULL;
  stBlockElementTemplate.nTailValue = 0;

  memcpy(TetrisCustomizeBlocksSpaceTmp, 
    pstParam->pstFastArray[nCurrentBlock]->TetrisCustomizeBlocksSpace,
    sizeof(TetrisCustomizeBlocksSpaceTmp) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      if (TetrisCustomizeBlocksSpaceTmp[i][j] == WALL_VALUE ||
        TetrisCustomizeBlocksSpaceTmp[i][j] == SPACE_VALUE_TYPE_A)
      {
        continue;
      }

      stBlockElementTemplate.stCoord.nX = j;
      stBlockElementTemplate.stCoord.nY = i;
      stBlockElementTemplate.nValue = TetrisCustomizeBlocksSpaceTmp[i][j];

      if (j == pstParam->pstFastArray[nCurrentBlock]->nCenterX &&
          i == pstParam->pstFastArray[nCurrentBlock]->nCenterY)
      {
        stBlockElementTemplate.isCenter = 1;
      }
      else
      {
        stBlockElementTemplate.isCenter = 0;
      }

      //����Ԫ�ز�������,������ҵģ����ߵ��Ե�����
      if (eFlag == PLAYER_CONTROL)
      {
        insert_player_block_element_node_into_chain__chain_list_processor(
          pstParam, &stBlockElementTemplate);
      }
      if (eFlag == COM_CONTROL)
      {
        insert_COM_block_element_node_into_chain__chain_list_processor(
          pstParam, &stBlockElementTemplate);
      }
    }
  }

  return 0;
}

//��������Ԫ����������ɺ��ٴδ���������ÿ��Ԫ�ص�pCenter��ֵ
//�������Ӧ��(���)����chain_list_processor.cpp���Ȳ�����
int after_process_block_element_chain__data_processor(Param *pstParam,
  PlayerVSCOMControlFlag eFlag)
{
  //CenterBlockElement�����ã�������תʱҪ��
  BlockElement *pstCenterBlockElement = NULL;

  BlockElement *pstTmp = NULL;

  /*�˴��Եö��࣬����Ϊ�˱�����⣬�ඨ��һ�Ρ�����Ϊ������player��COM��һ��������*/
  BlockElement *pstTmpExtral = NULL;  

  if (eFlag == PLAYER_CONTROL)
  {
    pstTmpExtral = pstParam->pstFirstBlockElementPlayer;
  }
  if (eFlag == COM_CONTROL)
  {
    pstTmpExtral = pstParam->pstFirstBlockElementCOM;
  }

  //��λ���ĵ�ĵ�ַ
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    if (pstTmp->isCenter == 1)
    {
      pstCenterBlockElement = pstTmp;
      break;
    }
  }

  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->pCenter = pstCenterBlockElement;
  }

  return 0;
}

//Ԥ�������з��飨�ѷ���ĳ���λ�����°��źã����м���ã�
int pre_process_blocks__data_processor(Param *pstParam)
{
  int i = 0;

  for (i = 1; i <= pstParam->nBlockNum; i++)
  {
    //�ѷ���ĳ���λ�����°��źã����м����
    shift_block_to_mid__data_processor(pstParam->pstFastArray[i]);
  }

  return 0;
}

static int shift_block_to_mid__data_processor(
  CustomizedBlock *pstCustomizedBlocks)
{
  int nLength = 0;
  int nShiftTime = 0;

  //�õ�����ĳ���
  nLength = get_customized_block_length_data_processor(
    pstCustomizedBlocks->TetrisCustomizeBlocksSpace);

  //���ݳ��ȣ�������������ƶ��ٴ�
  /*11,12 0
    9, 10 1
    7, 8  2
    5, 6  3
    3, 4  4
    1, 2  5*/
  nShiftTime = (TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 2 - nLength) / 2;
  pstCustomizedBlocks->nCenterX += nShiftTime;

  while (nShiftTime != 0)
  {
    shift_tetris_customize_blocks_space_to_right__data_processor(
      pstCustomizedBlocks->TetrisCustomizeBlocksSpace);
    nShiftTime--;
  }

  return 0;
}