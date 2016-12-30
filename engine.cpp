#include "stdafx.h"
#include "engine.h"

#include "interface.h"
#include "data_processor.h"
#include "COM_control_AI.h"
#include "COM_control_AI_1.h"

static int get_quadrant_type_for_rotation__engine(int nRelativeCoordX, 
                                                  int nRelativeCoordY);
static int get_the_align_coord__engine(int nRelativeCoordX, int nRelativeCoordY,
                               int nQudrantType,
                               int *pnAlignCoordX, int *pnAlignCoordY,
                               int *pnDistanceToAlignCoord);
static int get_relative_coord_after_rotation__engine(int nQudrantType,
                                                     int nDistance,
                                                     int nAlignCoordX, int nAlignCoordY, 
                                                     int *pnRelativeCoordX, int *pnRelativeCoordY);
static int clean_specific_line(Param *pstParam, int nLineNumber,
  PlayerVSCOMControlFlag eFlag);
static int decide_parameter_detail__engine(Param *pstParam,
  BlockElement **ppstBlockElement, unsigned char **ppchArrayAddress,
  int *pnCoordX, int *pnCoordY, PlayerVSCOMControlFlag eFlag);
static int get_dest_coord__engine(BlockElement *pstObj, int *pnDestX, int *pnDestY);


/*
//召唤新方块
int summon_new_block__engine(Param *pstParam, int *pnControlFlag)
{
  BlockElement *pstTmp = NULL;

  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //检测能否成功召唤（召唤失败就是游戏结束）
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] !=
      SPACE_VALUE_TYPE_A
      &&
      pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] !=
      SPACE_VALUE_TYPE_B)
    {
      *pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER;
    }

    //赋值
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;

    //画图
    print_element__interface(
      (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
      pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}*/



//召唤新方块
int summon_new_block__engine(Param *pstParam, int *pnControlFlag,
  PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  /*//debug 20161225 测试AI
  int nAIx = -1;
  int nAIy = -1;
  int nAIr = -1;
  int nResult = -1;

  //debug 20161225 测试AI
  if (eFlag == COM_CONTROL)
  {
  nResult = get_the_best_rotate_and_coord__COM_control_AI(pstParam, &nAIx, &nAIy, &nAIr);
  }*/

  //如果是电脑操控，把操作指令序列写入MessageContainer.
  if (eFlag == COM_CONTROL)
  {
    decide_control_list__COM_control_AI_1(pstParam);
  }

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //检测能否成功召唤（召唤失败就是游戏结束）
    if ( *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) != SPACE_VALUE_TYPE_A
      &&
      *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
        pstTmp->stCoord.nX) != SPACE_VALUE_TYPE_B)
    {
      if (eFlag == PLAYER_CONTROL)
      {
        //AI debug
        //*pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER;
      }
      if (eFlag == COM_CONTROL)
      {
        *pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM;
      }
    }

    //赋值
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) = pstTmp->nValue;
    
    //画图
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2, pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE, pstTmp->nValue);
  }

  return 0;
}

//方块下落(玩家专用。。暂时不兼容com,因为链表和数组都不一样（虽然格式一样）)
//20161220现在兼容COM了
int move_down_block__engine(Param *pstParam, int *pnControlFlag,
   bool *pbIsSessionEnded, PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  bool bLocalNewSessionFlag = false;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

   //检测能否成功向下移动
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //任何一个元素下面是墙或者已固化的方块（value >= 100），就不能移动了
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY + 1][pstTmp->stCoord.nX] 
        >= 100)*/
    if (*(chTmpArrayAddress + (pstTmp->stCoord.nY + 1) * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) >= 100)
    {
      //判断是否已经游戏结束，游戏结束则没有必要再刷新新的方块
      //pnControlFlag的逻辑有点混乱，不过现在能正常运行，先不改
      /*if (*pnControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER &&
        *pnControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM)*/
      //20161221改了逻辑
      if (*pnControlFlag == CONTROL_FLAG_MAIN_LOOP_RUNNING)
      {
        bLocalNewSessionFlag = true;
        
        *pbIsSessionEnded = true;
      }
    }
  }

  //如果是new session，那么固化目前的所有方块,固化以后就退出函数
  if (bLocalNewSessionFlag == true)
  {
    for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
    {
      /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] = 
        SOLID_BLOCK_VALUE;*/
      *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
        pstTmp->stCoord.nX) = SOLID_BLOCK_VALUE;
    }

    //20161018由于消行的原因，固化后颜色需要固定（统一）
    for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
    {
      print_element__interface((pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY, INTERFACE_BLOCK_FIGURE,
        INTERFACE_SOLID_BLOCK_COLOR);
    }

    return 0;
  }

  //给尾巴赋值,为以后的工作做准备
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //尾巴就是它上面的值
    if (pstTmp->stCoord.nY == 0)
    {
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX);
    }
    else
    {
     /* pstTmp->nTailValue = 
        pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY - 1][pstTmp->stCoord.nX];*/
      pstTmp->nTailValue = 
        *(chTmpArrayAddress + (pstTmp->stCoord.nY - 1) * TETRIS_PLAY_SPACE_X + 
          pstTmp->stCoord.nX);
    }

    //如果上面是固化物，那就当成空白看（一个容易遗漏的BUG）
    if (pstTmp->nTailValue == SOLID_BLOCK_VALUE)
    {
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX);
    }

  }


  //赋值1.方块元素坐标下移
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nY += 1;
  }

  //赋值2.背景数组数值的改变
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //元素所在位置赋值自身value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) = pstTmp->nValue;

    //元素上面赋值尾巴的value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY - 1][pstTmp->stCoord.nX] =
      pstTmp->nTailValue;*/
    *(chTmpArrayAddress + (pstTmp->stCoord.nY - 1) * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) = pstTmp->nTailValue;
  }

  //画图
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //画上面的（尾巴）
    switch (pstTmp->nTailValue)
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY - 1 + nCoordY,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_A);
      break;
    /*case SPACE_VALUE_TYPE_B:
      print_element__interface(
        (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY - 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_B);
      break;*/
    case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
    case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_RED:
      print_element__interface(
        (pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY - 1 + nCoordY,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nTailValue);
      break;
    default:
      printf("error debug");
      break;
    }

    //画现在的
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2,
      pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//方块左移(玩家专用。。暂时不兼容com,因为链表和数组都不一样（虽然格式一样）)
//20161221现在支持COM
int move_left_block__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  //检测能否成功向左移动
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //任何一个元素左面是墙或者已固化的方块（value >= 100），就不能移动了
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] 
    >= 100)*/
    if (*(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
        pstTmp->stCoord.nX - 1) >= 100)
    {
      //返回，什么也不做
      return 0;
    }
  }

  //给尾巴赋值,为以后的工作做准备
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //尾巴就是它右面的值
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] >=
      100)*/
    if (*(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX + 1) >= 100)
    {
      //如果右边紧靠墙或者固化物
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX + 1);
    }
    else
    {
      /*pstTmp->nTailValue = 
        pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1];*/
      pstTmp->nTailValue = 
        *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
          pstTmp->stCoord.nX + 1);
    }
  }

  //赋值1.方块元素坐标左移
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nX -= 1;
  }

  //赋值2.背景数组数值的改变
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //元素所在位置赋值自身value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) = pstTmp->nValue;


    //元素右面赋值尾巴的value
    //两种颜色空地数值互换
    //SPACE_VALUE_TYPE_A 和 SPACE_VALUE_TYPE_B 其实是一样的。
    //他们原本是不一样，但是因为显示不方便的原因，改成了一样的。
    if (pstTmp->nTailValue == SPACE_VALUE_TYPE_A)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_B;
    }
    else if (pstTmp->nTailValue == SPACE_VALUE_TYPE_B)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_A;
    }

    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] =
      pstTmp->nTailValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX + 1) = pstTmp->nTailValue;
  }

  //画图
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //画右面的（尾巴）
    switch (pstTmp->nTailValue)
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX + 1 + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_A);
      break;
    /*case SPACE_VALUE_TYPE_B:
      print_element__interface(
        (pstTmp->stCoord.nX + 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_B);
      break;*/
    case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
    case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_RED:
      print_element__interface(
        (pstTmp->stCoord.nX + 1 + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nTailValue);
      break;
    default:
      printf("error debug");
      break;
    }

    //画现在的
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2,
      pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//方块右移(玩家专用。。暂时不兼容com,因为链表和数组都不一样（虽然格式一样）)
//其实也简单，后面加个判断Player还是COM的参数，然后pstParam->pstFirstBlockElement 和
//pstParam->TetrisPlaySpace switchcase 独立赋值给变量，先不弄，或者这两个直接参数给
//20161221现在已兼容COM
int move_right_block__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  //检测能否成功向右移动
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //任何一个元素右面是墙或者已固化的方块（value >= 100），就不能移动了
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] 
    >= 100)*/
    if (*(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX + 1) >= 100)
    {
      //返回，什么也不做
      return 0;
    }
  }

  //给尾巴赋值,为以后的工作做准备
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //尾巴就是它左面的值
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] >=
      100)*/
    if (*(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX - 1) >= 100)
    {
      //如果左边紧靠墙或者固化物
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX - 1);
    }
    else
    {
      /*pstTmp->nTailValue = 
        pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1];*/
      pstTmp->nTailValue =
        *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
          pstTmp->stCoord.nX - 1);
    }
  }

  //赋值1.方块元素坐标右移
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nX += 1;
  }

  //赋值2.背景数组数值的改变
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //元素所在位置赋值自身value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) = pstTmp->nValue;

    //元素左面赋值尾巴的value
    //两种颜色空地数值互换
    if (pstTmp->nTailValue == SPACE_VALUE_TYPE_A)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_B;
    }
    else if (pstTmp->nTailValue == SPACE_VALUE_TYPE_B)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_A;
    }

    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] =
      pstTmp->nTailValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX - 1) = pstTmp->nTailValue;
  }

  //画图
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //画左面的（尾巴）
    switch (pstTmp->nTailValue)
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX - 1 + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_A);
      break;
    /*case SPACE_VALUE_TYPE_B:
      print_element__interface(
        (pstTmp->stCoord.nX - 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_B);
      break;*/
    case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
    case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_RED:
      print_element__interface(
        (pstTmp->stCoord.nX - 1 + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nTailValue);
      break;
    default:
      printf("error debug");
      break;
    }

    //画现在的
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2,
      pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//方块旋转
int rotate_block__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  int nDestX = 0;
  int nDestY = 0;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  //检测能否旋转
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //求旋转后的目标位置
    get_dest_coord__engine(pstTmp, &nDestX, &nDestY);

    if (nDestX <= 0 || nDestX >= TETRIS_PLAY_SPACE_X - 1)
    {
      return 0;
    }
    if (nDestY < 0 || nDestY >= TETRIS_PLAY_SPACE_Y - 1)
    {
      return 0;
    }
    //如果目标地点是墙，或者固化物
    /*if (pstParam->TetrisPlaySpacePlayer[nDestY][nDestX] >= 100)*/
    if (*(chTmpArrayAddress + nDestY * TETRIS_PLAY_SPACE_X + nDestX) >= 100)
    {
      return 0;
    }
  }

  
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //赋值1.背景数组数值的改变元素所在位置赋值空地
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      SPACE_TYPE(pstTmp->stCoord.nX);*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) = SPACE_TYPE(pstTmp->stCoord.nX);
    //画图1.元素所在位置画空地
    switch (SPACE_TYPE(pstTmp->stCoord.nX))
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_A);
      break;
    /*case SPACE_VALUE_TYPE_B:
      print_element__interface(
        (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_SPACE_FIGURE,
        INTERFACE_SPACE_COLOR_TYPE_B);
      break;*/
    default:
      break;
    }
  }

  //赋值：改变方块元素坐标
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    get_dest_coord__engine(pstTmp, &nDestX, &nDestY);
    pstTmp->stCoord.nX = nDestX;
    pstTmp->stCoord.nY = nDestY;
  }

  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //赋值2.背景数组数值的改变元素所在位置赋值当前元素的值
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] = 
      pstTmp->nValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) = pstTmp->nValue;
    //画图2.元素所在位置画空地
    //20161221应该是画方块，估计是当时的笔误（复制过来忘了改）
    switch (pstTmp->nValue)
    {
    case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
    case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
    case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
    case INTERFACE_BLOCK_COLOR_BLACK_RED:
      print_element__interface(
        (pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nValue);
      break;
    
    default:
      break;
    }
  }

  return 1;
}

static int get_dest_coord__engine(BlockElement *pstObj, int *pnDestX, int *pnDestY)
{
  //先把整体移动到0,0坐标系，这两个变量用来求得移动需要的位移
  int nShiftDistanceX = 0;
  int nShiftDistanceY = 0;
  //此点在0,0坐标系里的坐标
  int nRelativeCoordX = 0;
  int nRelativeCoordY = 0;
  //此点的象限类型
  int nQuadrantType = 0;
  //此点的旋转基准点
  int nAlignCoordX = 0;
  int nAlignCoordY = 0;
  int nDistanceToAlignCoord = 0;

  //求移动到0,0坐标系需要的整体位移
  nShiftDistanceX = 0 - pstObj->pCenter->stCoord.nX;
  nShiftDistanceY = 0 - pstObj->pCenter->stCoord.nY;

  //求得此点在0,0坐标系里的坐标
  nRelativeCoordX = pstObj->stCoord.nX + nShiftDistanceX;
  nRelativeCoordY = pstObj->stCoord.nY + nShiftDistanceY;

  //求得此点的象限类型
  nQuadrantType = get_quadrant_type_for_rotation__engine(nRelativeCoordX, 
    nRelativeCoordY);

  //求得此点的旋转基准点
  get_the_align_coord__engine(nRelativeCoordX, nRelativeCoordY, nQuadrantType,
    &nAlignCoordX, &nAlignCoordY, &nDistanceToAlignCoord);

  //开始旋转，求得此点旋转后的0,0坐标
  get_relative_coord_after_rotation__engine(nQuadrantType, nDistanceToAlignCoord,
    nAlignCoordX, nAlignCoordY, &nRelativeCoordX, &nRelativeCoordY);

  //根据现在的0,0坐标，反推出目的地坐标
  *pnDestX = pstObj->pCenter->stCoord.nX - nRelativeCoordX;
  *pnDestY = pstObj->pCenter->stCoord.nY - nRelativeCoordY;

  return 0;
}

int get_dest_coord_for_COM_control_AI__engine(BlockElement *pstObj, 
  int *pnDestX, int *pnDestY)
{
  //先把整体移动到0,0坐标系，这两个变量用来求得移动需要的位移
  int nShiftDistanceX = 0;
  int nShiftDistanceY = 0;
  //此点在0,0坐标系里的坐标
  int nRelativeCoordX = 0;
  int nRelativeCoordY = 0;
  //此点的象限类型
  int nQuadrantType = 0;
  //此点的旋转基准点
  int nAlignCoordX = 0;
  int nAlignCoordY = 0;
  int nDistanceToAlignCoord = 0;

  //求移动到0,0坐标系需要的整体位移
  nShiftDistanceX = 0 - pstObj->pCenter->stCoordAItmp.nX;
  nShiftDistanceY = 0 - pstObj->pCenter->stCoordAItmp.nY;

  //求得此点在0,0坐标系里的坐标
  nRelativeCoordX = pstObj->stCoordAItmp.nX + nShiftDistanceX;
  nRelativeCoordY = pstObj->stCoordAItmp.nY + nShiftDistanceY;

  //求得此点的象限类型
  nQuadrantType = get_quadrant_type_for_rotation__engine(nRelativeCoordX,
    nRelativeCoordY);

  //求得此点的旋转基准点
  get_the_align_coord__engine(nRelativeCoordX, nRelativeCoordY, nQuadrantType,
    &nAlignCoordX, &nAlignCoordY, &nDistanceToAlignCoord);

  //开始旋转，求得此点旋转后的0,0坐标
  get_relative_coord_after_rotation__engine(nQuadrantType, nDistanceToAlignCoord,
    nAlignCoordX, nAlignCoordY, &nRelativeCoordX, &nRelativeCoordY);

  //根据现在的0,0坐标，反推出目的地坐标
  *pnDestX = pstObj->pCenter->stCoordAItmp.nX - nRelativeCoordX;
  *pnDestY = pstObj->pCenter->stCoordAItmp.nY - nRelativeCoordY;

  return 0;
}

//求得一个点的象限类型(enum BlockRotationQuadrant)
static int get_quadrant_type_for_rotation__engine(int nRelativeCoordX, 
                                                  int nRelativeCoordY)
{
  int nQuadrantType = 0;

  if (nRelativeCoordX < 0 && nRelativeCoordY >= 0)
  {
    nQuadrantType = TOP_LEFT_WITH_NEGATIVE_X_0_Y;
  }

  if (nRelativeCoordX >= 0 && nRelativeCoordY > 0)
  {
    nQuadrantType = TOP_RIGHT_WITH_0_X_POSTIVE_Y;
  }

  if (nRelativeCoordX > 0 && nRelativeCoordY <= 0)
  {
    nQuadrantType = BOTTOM_RIGHT_WITH_POSTIVE_X_0_Y;
  }

  if (nRelativeCoordX <=0 && nRelativeCoordY < 0)
  {
    nQuadrantType = BOTTOM_LEFT_WITH_0_X_NEGATIVE_Y;
  }

  return nQuadrantType;
}

//求得一个点的旋转基准点
static int get_the_align_coord__engine(int nRelativeCoordX, int nRelativeCoordY,
                               int nQudrantType,
                               int *pnAlignCoordX, int *pnAlignCoordY,
                               int *pnDistanceToAlignCoord)
{
  switch (nQudrantType)
  {
  case TOP_LEFT_WITH_NEGATIVE_X_0_Y:
    *pnAlignCoordX = nRelativeCoordX;
    *pnAlignCoordY = 0;
    *pnDistanceToAlignCoord = nRelativeCoordY;
    break;
  case TOP_RIGHT_WITH_0_X_POSTIVE_Y:
    *pnAlignCoordX = 0;
    *pnAlignCoordY = nRelativeCoordY;
    *pnDistanceToAlignCoord = nRelativeCoordX;
    break;
  case BOTTOM_RIGHT_WITH_POSTIVE_X_0_Y:
    *pnAlignCoordX = nRelativeCoordX;
    *pnAlignCoordY = 0;
    *pnDistanceToAlignCoord = nRelativeCoordY * (-1);
    break;
  case BOTTOM_LEFT_WITH_0_X_NEGATIVE_Y:
    *pnAlignCoordX = 0;
    *pnAlignCoordY = nRelativeCoordY;
    *pnDistanceToAlignCoord = nRelativeCoordX * (-1);
    break;
  default:
    break;
  }

  return 0;
}

static int get_relative_coord_after_rotation__engine(int nQudrantType,
                                                     int nDistance,
                                   int nAlignCoordX, int nAlignCoordY, 
                                   int *pnRelativeCoordX, int *pnRelativeCoordY)
{
  //旋转后的基准点坐标
  int nCurrentAlignCoordX = 0;
  int nCurrentAlignCoordY = 0;

  switch (nQudrantType)
  {
  case TOP_LEFT_WITH_NEGATIVE_X_0_Y:
    nCurrentAlignCoordX = 0;
    nCurrentAlignCoordY = nAlignCoordX * (-1);
    
    *pnRelativeCoordX = nCurrentAlignCoordX + nDistance;
    *pnRelativeCoordY = nCurrentAlignCoordY;

    break;

  case TOP_RIGHT_WITH_0_X_POSTIVE_Y:
    nCurrentAlignCoordX = nAlignCoordY;
    nCurrentAlignCoordY = 0;

    *pnRelativeCoordX = nCurrentAlignCoordX;
    *pnRelativeCoordY = nCurrentAlignCoordY - nDistance;

    break;

  case BOTTOM_RIGHT_WITH_POSTIVE_X_0_Y:
    nCurrentAlignCoordX = 0;
    nCurrentAlignCoordY = nAlignCoordX * (-1);

    *pnRelativeCoordX = nCurrentAlignCoordX - nDistance;
    *pnRelativeCoordY = nCurrentAlignCoordY;

    break;

  case BOTTOM_LEFT_WITH_0_X_NEGATIVE_Y:
    nCurrentAlignCoordX = nAlignCoordY;
    nCurrentAlignCoordY = 0;

    *pnRelativeCoordX = nCurrentAlignCoordX;
    *pnRelativeCoordY = nCurrentAlignCoordY + nDistance;

    break;
  default:
    break;
  }
  return 0;
}

//获得最下方的一行可以消除的行号
int get_the_line_at_bottom_to_clean__data_processor(Param *pstParam,
  PlayerVSCOMControlFlag eFlag)
{
  int i = 0;
  int j = 0;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  for (i = TETRIS_PLAY_SPACE_Y - 1 - 1; i >= 0; i--)
  {
    for (j = 1; j <= TETRIS_PLAY_SPACE_X - 1 - 1; j++)
    {
      if (*(chTmpArrayAddress + i * TETRIS_PLAY_SPACE_X + j) !=
        SOLID_BLOCK_VALUE)
      {
        break;
      }
      if (j == TETRIS_PLAY_SPACE_X - 1 - 1)
      {
        return i;
      }
    }
  }

  return -1;
}

int clean_line__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag)
{
  int nLineNumber = -1;

  //检测是否有可以消除的行
  nLineNumber = get_the_line_at_bottom_to_clean__data_processor(pstParam, 
    eFlag);

  while (nLineNumber != -1)
  {
    clean_specific_line(pstParam, nLineNumber, eFlag);
	//可以在此处添加得分相关代码
    nLineNumber = get_the_line_at_bottom_to_clean__data_processor(pstParam, 
      eFlag);
  }

  return 0;
}

static int clean_specific_line(Param *pstParam, int nLineNumber, 
  PlayerVSCOMControlFlag eFlag)
{
  int i = 0;
  int j = 0;

  //在此函数，这个变量没用，定义它是为了配合下面的decide_parameter_detail__engine函数的参数需求
  BlockElement *pstTmpExtral = NULL;  

  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  //被删除的行以上的行向下平移
  for (i = nLineNumber; i >= 1; i--)
  {
    for (j = 1; j < TETRIS_PLAY_SPACE_X - 1; j++)
    {
      //赋值
      //pstParam->TetrisPlaySpacePlayer[i][j] = pstParam->TetrisPlaySpacePlayer[i - 1][j];
      *(chTmpArrayAddress + i * TETRIS_PLAY_SPACE_X + j) = 
        *(chTmpArrayAddress + (i - 1) * TETRIS_PLAY_SPACE_X + j);

      //画图
      switch (*(chTmpArrayAddress + i * TETRIS_PLAY_SPACE_X + j))
      {
      case SOLID_BLOCK_VALUE:
        print_element__interface((j + nCoordX) * 2, i + nCoordY,
          INTERFACE_BLOCK_FIGURE, INTERFACE_SOLID_BLOCK_COLOR);
        break;
      case SPACE_VALUE_TYPE_A:
        print_element__interface((j + nCoordX) * 2, i + nCoordY,
          INTERFACE_SPACE_FIGURE, INTERFACE_SPACE_COLOR_TYPE_A);
        break;
      default:
        break;
      }
    }
  }

  //最上一行变成空白
  for (j = 1; j < TETRIS_PLAY_SPACE_X - 1; j++)
  {
    //赋值
    //pstParam->TetrisPlaySpacePlayer[0][j] = SPACE_VALUE_TYPE_A;
    *(chTmpArrayAddress + 0 * TETRIS_PLAY_SPACE_X + j) = SPACE_VALUE_TYPE_A;

    //画图
    print_element__interface((j + nCoordX) * 2, 0 + nCoordY,
      INTERFACE_SPACE_FIGURE, INTERFACE_SPACE_COLOR_TYPE_A);
  }

  return 0;
}

//根据是玩家还是电脑，来决定参数具体是什么
static int decide_parameter_detail__engine(Param *pstParam,
  BlockElement **ppstBlockElement, unsigned char **ppchArrayAddress,
  int *pnCoordX, int *pnCoordY, PlayerVSCOMControlFlag eFlag)
{
  switch (eFlag)
  {
  case PLAYER_CONTROL:
    *ppstBlockElement = pstParam->pstFirstBlockElementPlayer;
    *ppchArrayAddress = (unsigned char *)pstParam->TetrisPlaySpacePlayer;
    *pnCoordX = INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X;
    *pnCoordY = INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y;
    break;
  case COM_CONTROL:
    *ppstBlockElement = pstParam->pstFirstBlockElementCOM;
    *ppchArrayAddress = (unsigned char *)pstParam->TetrisPlaySpaceCOM;
    *pnCoordX = INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X;
    *pnCoordY = INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y;
    break;
  default:
    break;
  }

  return 0;
}