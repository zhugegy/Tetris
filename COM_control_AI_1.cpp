#include "stdafx.h"
#include "COM_control_AI_1.h"

#include "COM_control_list.h"
#include "COM_control_AI.h"
#include "engine.h"

//本文件指定电脑的控制策略。
//初级策略：高度最低原则。寻找可旋转点-》等待到达可旋转点-》旋转-》左右位移-》迅速落位。
//改进后：寻找可旋转点-》等待旋转完成-》左右位移-》（迅速落位）。
//此AI略显粗糙，仅作为测试，或者作为最低级电脑AI使用。

//static int find_the_first_line_for_rotation__COM_control_AI_1(Param *pstParam);
//static int translate_first_line_to_cmd__COM_control_AI_1(int nLineNum);


int decide_control_list__COM_control_AI_1(Param *pstParam)
{
  int i = 0;
  //int nCmd;

  int nDestCoordX = 0;
  int nDestCoordY = 0;
  int nRotateTimes = 0;

  //int nRotateLine = -1;

  int nCurrentCenterCoordX = 0;
/*
  //初始化现在的ControlMsg
  for (i = 0; i < MESSAGE_CONTAINER_MAX_NUM; i++)
  {
    BuildingControlMsg[i] = COM_CONTROL_DEFAULT;
  }*/

  //重置现存的控制信息
  pstParam->COMControlMsg.reset_message_list();

  if (pstParam->pstFirstBlockElementCOM->nValue == 11)
  {
    //debug
    printf("");
  }

  //获取旋转次数和中心点的最终位置（最优情况）
  get_the_best_rotate_and_coord__COM_control_AI(pstParam, 
    &nDestCoordX, &nDestCoordY, &nRotateTimes);

  //获取可以进行旋转动作的那一行
  //nRotateLine = find_the_first_line_for_rotation__COM_control_AI_1(pstParam);



  //把等待行数（等待旋转）写入指令
  //nCmd = translate_first_line_to_cmd__COM_control_AI_1(nRotateLine);
  //pstParam->COMControlMsg.write_message(nCmd);

  //把旋转次数写入指令
  for (i = 0; i < nRotateTimes; i++)
  {
    pstParam->COMControlMsg.write_message(COM_CONTROL_ROTATE);
  }

  //把左右移动写入指令
  nCurrentCenterCoordX = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nX;
  if (nCurrentCenterCoordX >= nDestCoordX)
  {
    //如果中心点初始位置在最终位置的右边
    while (nCurrentCenterCoordX > nDestCoordX)
    {
      pstParam->COMControlMsg.write_message(COM_CONTROL_MOVE_LEFT);
      nCurrentCenterCoordX--;
    }
  }
  else
  {
    //如果中心点初始位置在最终位置的左边
    while (nCurrentCenterCoordX < nDestCoordX)
    {
      pstParam->COMControlMsg.write_message(COM_CONTROL_MOVE_RIGHT);
      nCurrentCenterCoordX++;
    }
  }

  //最后写入直接往下指令
  pstParam->COMControlMsg.write_message(COM_CONTROL_MOVE_STRAIGT_DOWN);

  return 0;
}

/*
static int find_the_first_line_for_rotation__COM_control_AI_1(Param *pstParam)
{
  BlockElement *pstTmp = NULL;
  BlockElement *pstCenter = NULL;

  int nCoordAfterRotationX = 0;
  int nCoordAfterRotationY = 0;


  //先把所有元素的stCoordAItmp赋值(赋为原值)
  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoordAItmp.nX = pstTmp->stCoord.nX;
    pstTmp->stCoordAItmp.nY = pstTmp->stCoord.nY;
  }

  pstCenter = pstParam->pstFirstBlockElementCOM->pCenter;

  //当中心点的Y坐标小于最下面一行（底面的墙）的Y坐标
  while (pstCenter->stCoordAItmp.nY < TETRIS_PLAY_SPACE_Y - 1)
  {
    for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
    {
      get_dest_coord_for_COM_control_AI__engine(pstTmp, 
        &nCoordAfterRotationX, &nCoordAfterRotationY);

      if (nCoordAfterRotationX <=0 ||
          nCoordAfterRotationX >= TETRIS_PLAY_SPACE_X -1 ||
          nCoordAfterRotationY < 0 ||
          nCoordAfterRotationY >= TETRIS_PLAY_SPACE_Y - 1 ||
          pstParam->TetrisPlaySpaceCOM[nCoordAfterRotationY][nCoordAfterRotationX] == WALL_VALUE ||
          pstParam->TetrisPlaySpaceCOM[nCoordAfterRotationY][nCoordAfterRotationX] == SOLID_BLOCK_VALUE)
      {
        //如果旋转不合法
        break;
      }
      else
      {
        //如果旋转合法
        continue;
      }
    }

    if (pstTmp == NULL)
    {
      //证明这个方块的所有element都可以合法旋转
      pstTmp = pstParam->pstFirstBlockElementCOM;
      return pstTmp->pCenter->stCoordAItmp.nY;
    }
    else
    {
      //证明方框在此处旋转不合法，必须下移一格再尝试
      for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
      {
        pstTmp->stCoordAItmp.nY++;
      }

    }

  }

  return -1;
}*/

/*
static int translate_first_line_to_cmd__COM_control_AI_1(int nLineNum)
{
  switch (nLineNum)
  {
  case 0:
    return COM_CONTROL_WAIT_TILL_LINE_0;
    break;
  case 1:
    return COM_CONTROL_WAIT_TILL_LINE_1;
    break;
  case 2:
    return COM_CONTROL_WAIT_TILL_LINE_2;
    break;
  case 3:
    return COM_CONTROL_WAIT_TILL_LINE_3;
    break;
  case 4:
    return COM_CONTROL_WAIT_TILL_LINE_4;
    break;
  case 5:
    return COM_CONTROL_WAIT_TILL_LINE_5;
    break;
  case 6:
    return COM_CONTROL_WAIT_TILL_LINE_6;
    break;
  case 7:
    return COM_CONTROL_WAIT_TILL_LINE_7;
    break;
  case 8:
    return COM_CONTROL_WAIT_TILL_LINE_8;
    break;
  case 9:
    return COM_CONTROL_WAIT_TILL_LINE_9;
    break;
  case 10:
    return COM_CONTROL_WAIT_TILL_LINE_10;
    break;
  case 11:
    return COM_CONTROL_WAIT_TILL_LINE_11;
    break;
  case 12:
    return COM_CONTROL_WAIT_TILL_LINE_12;
    break;
  case 13:
    return COM_CONTROL_WAIT_TILL_LINE_13;
    break;
  case 14:
    return COM_CONTROL_WAIT_TILL_LINE_14;
    break;
  case 15:
    return COM_CONTROL_WAIT_TILL_LINE_15;
    break;
  case 16:
    return COM_CONTROL_WAIT_TILL_LINE_16;
    break;
  case 17:
    return COM_CONTROL_WAIT_TILL_LINE_17;
    break;
  case 18:
    return COM_CONTROL_WAIT_TILL_LINE_18;
    break;
  case 19:
    return COM_CONTROL_WAIT_TILL_LINE_19;
    break;
  case 20:
    return COM_CONTROL_WAIT_TILL_LINE_20;
    break;
  case 21:
    return COM_CONTROL_WAIT_TILL_LINE_21;
    break;
  case 22:
    return COM_CONTROL_WAIT_TILL_LINE_22;
    break;
  default:
    //return COM_CONTROL_DEFAULT;
    return COM_CONTROL_WAIT_TILL_LINE_2;
    break;
  }

  return 0;
}*/