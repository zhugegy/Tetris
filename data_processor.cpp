//此文件负责数据相关操作

#include "stdafx.h"
#include "data_processor.h"

#include "chain_list_processor.h"
#include "file_operator.h"

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
static int pre_process_blocks__data_processor(Param *pstParam);

//初始化俄罗斯方块方阵(游戏时)
int set_tetris_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      //先全部置为空格
      chTetris[i][j] = SPACE_TYPE(j);

      //竖着的2个墙
      if ((j == 0) || (j == TETRIS_PLAY_SPACE_X - 1))
      {
        chTetris[i][j] = WALL_VALUE;
      }

      //底部的墙
      if (i == TETRIS_PLAY_SPACE_Y - 1)
      {
        chTetris[i][j] = WALL_VALUE;
      }
    }
  }

  return 0;
}

//初始化俄罗斯方块方阵(自定义方块)
int set_tetris_customize_blocks_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      //先全部置为空格,自定义模式不区分空格是纯白还是灰白
      chTetris[i][j] = SPACE_VALUE_TYPE_A;

      //竖着的2个墙
      if ((j == 0) || (j == TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1))
      {
        chTetris[i][j] = WALL_VALUE;
      }

      //自定义模式没有底部的墙
    }
  }

  return 0;
}

//改变俄罗斯方块方阵的元素数值(自定义方块)
int change_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char chValue, int nX, int nY)
{
  chTetris[nY][nX] = chValue;

  return 0;
}

//向左上优化俄罗斯方块方阵(自定义方块)
int optimize_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //整体向上移动，挤掉空行
  while (is_this_line_empty__data_processor(0, chTetris) == 1)
  {
    shift_tetris_customize_blocks_space_to_top__data_processor(chTetris);
  }

  //整体向左移动，挤掉空行
  while (is_this_column_empty__data_processor(1, chTetris) == 1)
  {
    shift_tetris_customize_blocks_space_to_left__data_processor(chTetris);
  }

  return 0;
}

/*
//查看俄罗斯方块方阵(自定义方块)最上一行是不是空
static int is_the_top_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; i++)
  {
    if (chTetris[0][i] != SPACE_VALUE_TYPE_A)
    {
      //如果有非空元素，返回0
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
      //如果有非空元素，返回0
      return 0;
    }
  }

  return 1;
}

/*
//查看俄罗斯方块方阵（自定义方块）最左一列是不是空
static int is_the_left_line_empty__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    if (chTetris[i][1] != SPACE_VALUE_TYPE_A)
    {
      //如果有非空元素，返回0
      return 0;
    }
  }

  return 1;
}*/

//查看俄罗斯方块方阵某一列是不是空
static int is_this_column_empty__data_processor(int nColumn,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;

  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    if (chTetris[i][nColumn] != SPACE_VALUE_TYPE_A)
    {
      //如果有非空元素，返回0
      return 0;
    }
  }

  return 1;
}

//俄罗斯方块方阵（自定义方块）整体上移一行，最底一行补0（除了两边的墙）
static int shift_tetris_customize_blocks_space_to_top__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;
  
  //从上往下（不包括最后一行），每一行都等于它的下一行
  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y - 1; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X; j++)
    {
      chTetris[i][j] = chTetris[i + 1][j];
    }
  }

  //最后一行单独赋值
  chTetris[i][0] = WALL_VALUE;
  
  for (j = 1; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1; j++)
  {
    chTetris[i][j] = SPACE_VALUE_TYPE_A;
  }

  chTetris[i][j] = WALL_VALUE;

  return 0;
}

//俄罗斯方块方阵（自定义方块）整体左移一行，最右一行补0（除了两边的墙）;-)
static int shift_tetris_customize_blocks_space_to_left__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  //从左往右（不包括第一列和最右边的墙的左边一列），每一列都等于右边的那一列
  for (i = 1; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 - 1; i++)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
    {
      chTetris[j][i] = chTetris[j][i + 1];
    }
  }

  //最右边的墙的左边一列置为空
  for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
  {
    chTetris[j][i] = SPACE_VALUE_TYPE_A;
  }

  return 0;
}

//俄罗斯方块方阵（自定义方块）整体左移一行，最右一行补0（除了两边的墙）;-)
static int shift_tetris_customize_blocks_space_to_right__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  int i = 0;
  int j = 0;

  //从左往右（不包括第一列和最右边的墙的左边一列），每一列都等于右边的那一列
  //从右往左（不包括最后一列和最左边墙的右边一列），每一列都等于左边的那一列
  for (i = TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 -1 ; i > 1; i--)
  {
    for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
    {
      chTetris[j][i] = chTetris[j][i - 1];
    }
  }

  //最左边的墙的右边一列置为空
  for (j = 0; j < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; j++)
  {
    chTetris[j][i] = SPACE_VALUE_TYPE_A;
  }

  return 0;
}

//计算自定义方块的长度(只能用于向左上优化之后的形状)
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

//计算自定义方块的高度
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

//计算每个方块的中心权重计算中心点的步骤之一
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
        //最大值(12-6)^2 + (6-3)^2 = 36 + 9 = 45 不会超过unsigned char范围；
        //不用绝对值，是因为想扩大这个值的取值范围
        chTetris[i][j] = (unsigned char) 
                         ((i - fCenterPointY) * (i - fCenterPointY) 
                           + 
                         (j - fCenterPointX) * (j - fCenterPointX));
      }
    }
  }

  return 0;
}

//求得自定义方块的中心点坐标（pstCoord）
int get_customized_block_center_point__data_processor(ElementCoord *pstCoord, 
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //初始化
  unsigned char chWeight = 66;    //49 + 16 最大值 + 1（比理论最大值45更大）
  pstCoord->nX = 1;
  pstCoord->nY = 0;
  int i = 0;
  int j = 0;

  //这里的循环倒着来，更像俄罗斯方块中的中心点分配
  for (i = TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y - 1; i >= 0 ; i--)
  {
    for (j = TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 1 - 1; j >= 1 ; j--)
    {
      if (chTetris[i][j] != SPACE_VALUE_TYPE_A &&
          chTetris[i][j] != WALL_VALUE && /*其实不可能是墙，加在这里防止意外*/
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

//随机出一个方块（序号）
int get_a_random_block(Param *pstParam, int nCurrentBlock)
{
  int nSerial = 0;
  int nChance = 0;

  while (true)
  {
    nSerial = (rand() % pstParam->nBlockNum) + 1;
    nChance = rand() % 5;    /*0 - 4*/

    if (nSerial != nCurrentBlock /*不能连续两个相同的方块*/ && 
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

//把当前方块的所有元素加入链表
//这个函数应该(最好)属于chain_list_processor.cpp，先不改了
int build_block_element_chain__data_processor(Param *pstParam, 
                   int nCurrentBlock, PlayerVSCOMControlFlag eFlag)
{
  unsigned char TetrisCustomizeBlocksSpaceTmp[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X] = {0};
  BlockElement stBlockElementTemplate;
  int i = 0;
  int j = 0;

  //20170101添加nSerial相关赋值，方便COM AI的判断
  stBlockElementTemplate.nSerial =
    pstParam->pstFastArray[nCurrentBlock]->nSerial;

  //pNext由链表操作赋值，pCenter由另外的函数赋值
  stBlockElementTemplate.pNext = NULL;
  stBlockElementTemplate.pCenter = NULL;

  stBlockElementTemplate.nTailValue = 0;

  memcpy(TetrisCustomizeBlocksSpaceTmp, 
    pstParam->pstFastArray[nCurrentBlock]->TetrisCustomizeBlocksSpace,
    sizeof(TetrisCustomizeBlocksSpaceTmp) * sizeof(char));    //应该是_countof,这里有错，不过先不改，没影响

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

      //将此元素插入链表,插入玩家的，或者电脑的链表
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

//方块所有元素链表构建完成后，再次处理它，给每个元素的pCenter赋值
//这个函数应该(最好)属于chain_list_processor.cpp，先不改了
int after_process_block_element_chain__data_processor(Param *pstParam,
  PlayerVSCOMControlFlag eFlag)
{
  //CenterBlockElement的作用：计算旋转时要用
  BlockElement *pstCenterBlockElement = NULL;

  BlockElement *pstTmp = NULL;

  /*此处显得多余，但是为了避免误解，多定义一次。这是为了整合player和COM到一个函数。*/
  BlockElement *pstTmpExtral = NULL;  

  if (eFlag == PLAYER_CONTROL)
  {
    pstTmpExtral = pstParam->pstFirstBlockElementPlayer;
  }
  if (eFlag == COM_CONTROL)
  {
    pstTmpExtral = pstParam->pstFirstBlockElementCOM;
  }

  //定位中心点的地址
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

int reload_block_info(Param *pstParam)
{
  //先释放旧的方块信息链表
  if (pstParam->pstCustomizedBlockNodes != NULL)
  {
    free_block_info__chain_list_processor(pstParam);
  }

  //从文件载入方块信息链表
  load_block_list__file_operator(pstParam);

  //把方块总数存储
  pstParam->nBlockNum = get_block_total_num__chain_list_processor(pstParam);

  //方块链表存入快捷数组
  build_fast_array__chain_list_processor(pstParam);

  //预处理所有方块（把方块的出现位置重新安排好，靠中间放置）
  pre_process_blocks__data_processor(pstParam);

  return 0;
}

//预处理所有方块（把方块的出现位置重新安排好，靠中间放置）
static int pre_process_blocks__data_processor(Param *pstParam)
{
  int i = 0;

  for (i = 1; i <= pstParam->nBlockNum; i++)
  {
    //把方块的出现位置重新安排好，靠中间放置
    shift_block_to_mid__data_processor(pstParam->pstFastArray[i]);
  }

  return 0;
}


static int shift_block_to_mid__data_processor(
  CustomizedBlock *pstCustomizedBlocks)
{
  int nLength = 0;
  int nShiftTime = 0;

  //得到方块的长度
  nLength = get_customized_block_length_data_processor(
    pstCustomizedBlocks->TetrisCustomizeBlocksSpace);

  //根据长度，计算出改向右移多少次
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