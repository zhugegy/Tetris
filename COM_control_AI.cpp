#include "stdafx.h"
#include "COM_control_AI.h"

#include "engine.h"
#include "COM_control_list.h"

static int initialize_tetris_space_score__COM_control_AI(
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X]);
static int simulate_rotation__COM_control_AI(Param *pstParam, int nRotateTime);
static int update_tetris_space_score__COM_control_AI(Param *pstParam,
  unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X],
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X]);
static int calculate_score__COM_control_AI(Param *pstParam, int nCoordX,
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X],
  int nPreviousHeight);
static int find_the_min_score__COM_control_AI(
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X], int *pnRotateTime,
  int *pnCoordX, int *pnCoordY, int *pnMinHeight, int nCurrentRotateTime);
static int calculate_height__COM_control_AI(
  unsigned char TetrisSpace[][TETRIS_PLAY_SPACE_X]);
/*
static int COM_control_wait_till__COM_control_AI(Param *pstParam, int nWaitTill,
  int *pnControlFlag, bool *pbIsSessionEnded, PlayerVSCOMControlFlag eFlag);*/

int get_the_best_rotate_and_coord__COM_control_AI(Param *pstParam,
  int *pnCoordiX, int *pnCoordiY, int *pnRotationTime)
{
  unsigned char TetrisSpaceTemplate[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  int TetrisSpaceScore_0[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  int TetrisSpaceScore_1[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  int TetrisSpaceScore_2[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  int TetrisSpaceScore_3[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  int nRotateTime = -1;
  int nCoordX = -1;
  int nCoordY = -1;
  int nMinHeight = COM_AI_TETRIS_SPACE_MIN_HEIGHT_DEFAULT;

  //复制当前游戏场地情况
  memcpy(TetrisSpaceTemplate, pstParam->TetrisPlaySpaceCOM,
    sizeof(TetrisSpaceTemplate) * sizeof(char));    //应该是_countof,这里有错，不过先不改，没影响

  //初始化得分器;对应的是4种旋转方式
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_0);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_1);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_2);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_3);

  //计算得分
  //不旋转，对应得分器0.
  simulate_rotation__COM_control_AI(pstParam, 0);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_0);

  //旋转1次，对应得分器1.
  simulate_rotation__COM_control_AI(pstParam, 1);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_1);

  //旋转2次，对应得分器2.
  simulate_rotation__COM_control_AI(pstParam, 2);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_2);

  //旋转3次，对应得分器3.
  simulate_rotation__COM_control_AI(pstParam, 3);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_3);

  //找到最小高度对应的旋转次数和坐标
  //不旋转，对应得分器0.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_0, &nRotateTime, 
    &nCoordX, &nCoordY, &nMinHeight, 0);

  //旋转1次，对应得分器1.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_1, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 1);

  //旋转2次，对应得分器2.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_2, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 2);

  //旋转3次，对应得分器3.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_3, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 3);
  
  *pnCoordiX = nCoordX;
  *pnCoordiY = nCoordY;
  *pnRotationTime = nRotateTime;

  return nMinHeight;
}

int translate_COM_cmd_to_action__COM_control_AI(Param *pstParam, int nCmd,
  int *pnControlFlag, bool *pbIsSessionEndedCOM)
{
  switch (nCmd)
  {
  case COM_CONTROL_DEFAULT:
    //什么都不做(其实还是往下移动一行)
    move_down_block__engine(pstParam, pnControlFlag, pbIsSessionEndedCOM,
      COM_CONTROL);
    break;
  case COM_CONTROL_ROTATE:
    //debug断点
    if (pstParam->pstFirstBlockElementCOM->nValue == 11)
    {
      printf("");
    }
    while (rotate_block__engine(pstParam, COM_CONTROL) != 1)
    {
      move_down_block__engine(pstParam, pnControlFlag, pbIsSessionEndedCOM,
        COM_CONTROL);
    }
    break;
  case COM_CONTROL_MOVE_LEFT:
    move_left_block__engine(pstParam, COM_CONTROL);
    break;
  case COM_CONTROL_MOVE_RIGHT:
    move_right_block__engine(pstParam, COM_CONTROL);
    break;
  case COM_CONTROL_MOVE_STRAIGT_DOWN:
    while (*pbIsSessionEndedCOM == false)
    {
      move_down_block__engine(pstParam, pnControlFlag, pbIsSessionEndedCOM, 
        COM_CONTROL);
    }
    break;
  /*case COM_CONTROL_WAIT_TILL_LINE_0:
    COM_control_wait_till__COM_control_AI(pstParam, 0, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_1:
    COM_control_wait_till__COM_control_AI(pstParam, 1, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_2:
    COM_control_wait_till__COM_control_AI(pstParam, 2, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_3:
    COM_control_wait_till__COM_control_AI(pstParam, 3, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_4:
    COM_control_wait_till__COM_control_AI(pstParam, 4, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_5:
    COM_control_wait_till__COM_control_AI(pstParam, 5, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_6:
    COM_control_wait_till__COM_control_AI(pstParam, 6, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_7:
    COM_control_wait_till__COM_control_AI(pstParam, 7, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_8:
    COM_control_wait_till__COM_control_AI(pstParam, 8, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_9:
    COM_control_wait_till__COM_control_AI(pstParam, 9, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_10:
    COM_control_wait_till__COM_control_AI(pstParam, 10, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_11:
    COM_control_wait_till__COM_control_AI(pstParam, 11, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_12:
    COM_control_wait_till__COM_control_AI(pstParam, 12, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_13:
    COM_control_wait_till__COM_control_AI(pstParam, 13, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_14:
    COM_control_wait_till__COM_control_AI(pstParam, 14, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_15:
    COM_control_wait_till__COM_control_AI(pstParam, 15, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_16:
    COM_control_wait_till__COM_control_AI(pstParam, 16, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_17:
    COM_control_wait_till__COM_control_AI(pstParam, 17, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_18:
    COM_control_wait_till__COM_control_AI(pstParam, 18, pnControlFlag,
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_19:
    COM_control_wait_till__COM_control_AI(pstParam, 19, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_20:
    COM_control_wait_till__COM_control_AI(pstParam, 20, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_21:
    COM_control_wait_till__COM_control_AI(pstParam, 21, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;
  case COM_CONTROL_WAIT_TILL_LINE_22:
    COM_control_wait_till__COM_control_AI(pstParam, 22, pnControlFlag, 
      pbIsSessionEndedCOM, COM_CONTROL);
    break;*/
  default:
    break;
  }

  return 0;
}

/*
static int COM_control_wait_till__COM_control_AI(Param *pstParam, int nWaitTill,
  int *pnControlFlag, bool *pbIsSessionEnded, PlayerVSCOMControlFlag eFlag)
{
  move_down_block__engine(pstParam, pnControlFlag, pbIsSessionEnded,
    COM_CONTROL);

  if (pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nY < nWaitTill)
  {
    pstParam->COMControlMsg.backwards_reading_postion();
  }

  return 0;
}*/

static int find_the_min_score__COM_control_AI(
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X], int *pnRotateTime, 
  int *pnCoordX, int *pnCoordY, int *pnMinHeight, int nCurrentRotateTime)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      if (TetrisSpaceScore[i][j] < *pnMinHeight)
      {
        *pnMinHeight = TetrisSpaceScore[i][j];

        *pnRotateTime = nCurrentRotateTime;
        *pnCoordX = j;
        *pnCoordY = i;
      }
    }
  }

  return 0;
}

//初始化得分器。
static int initialize_tetris_space_score__COM_control_AI(
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      TetrisSpaceScore[i][j] = COM_AI_TETRIS_SPACE_MAX_SCORE;
    }
  }

  return 0;
}

static int simulate_rotation__COM_control_AI(Param *pstParam, int nRotateTime)
{
  int i = 0;

  BlockElement *pstTmp = NULL;
  int nTmpCoordX = 0;
  int nTmpCoordY = 0;

  //先把所有元素的stCoordAItmp赋值(赋为原值)
  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoordAItmp.nX = pstTmp->stCoord.nX;
    pstTmp->stCoordAItmp.nY = pstTmp->stCoord.nY;
  }

  for (i = 0; i < nRotateTime; i++)
  {
    for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
    {
      get_dest_coord_for_COM_control_AI__engine(pstTmp, &nTmpCoordX, &nTmpCoordY);
      pstTmp->stCoordAItmp.nX = nTmpCoordX;
      pstTmp->stCoordAItmp.nY = nTmpCoordY;
    }
  }

  return 0;
}

static int update_tetris_space_score__COM_control_AI(Param *pstParam, 
  unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X],
  int TetrisSpaceScore[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  int nPreviousHeight = 0;

  nPreviousHeight = calculate_height__COM_control_AI(TetrisSpaceTemplate);

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      TetrisSpaceScore[i][j] = 
        calculate_score__COM_control_AI(pstParam, j, i, TetrisSpaceTemplate,
          nPreviousHeight);
    }
  }

  return 0;
}

static int calculate_score__COM_control_AI(Param *pstParam, int nCoordX, 
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X],
  int nPreHeight)
{
  BlockElement *pstTmp = NULL;

  int i = 0;
  int j = 0;

  int nCurrentHeight = 0;
  int nHeightDistance = 0;
  int nCenterPointHeight = 0;

  int nShiftDistanceX = 0;
  int nShiftDistanceY = 0;

  int nCurrentCoordXtmp = 0;
  int nCurrentCoordYtmp = 0;

  unsigned char TetrisSpaceWorkPlace[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  int nPulishment = 0;

  nShiftDistanceX = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nX - nCoordX;
  nShiftDistanceY = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nY - nCoordY;

  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;
    
    //如果横坐标越界
    if (nCurrentCoordXtmp <= 0 || nCurrentCoordXtmp >= TETRIS_PLAY_SPACE_X - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //如果纵坐标越界
    if (nCurrentCoordYtmp < 0 || nCurrentCoordYtmp >= TETRIS_PLAY_SPACE_Y - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //如果是墙
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] == WALL_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_WALL;
    }

    //如果是固化物
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] == 
      SOLID_BLOCK_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_SOLID_BLOCK;
    }

    for (i = nCurrentCoordYtmp - 1; i >= 0; i--)
    {
      if (TetrisSpaceTemplate[i][nCurrentCoordXtmp] == SOLID_BLOCK_VALUE)
      {
        return COM_AI_TETRIS_SPACE_SCORE_SOLID_BLOCK_ABOVE;
      }
    }

  }

  /*//如果不违规，返回中心点的高度
  return TETRIS_PLAY_SPACE_Y - nCoordY - 1;*/
  

  //如果不违规，计算空格上方被堵惩罚
  memcpy(TetrisSpaceWorkPlace, TetrisSpaceTemplate,
    sizeof(TetrisSpaceWorkPlace) * sizeof(char));    //应该是_countof,这里有错，不过先不改，没影响

  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;

    TetrisSpaceWorkPlace[nCurrentCoordYtmp][nCurrentCoordXtmp] = SOLID_BLOCK_VALUE;
  }

  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;

    for (i = nCurrentCoordYtmp + 1; i < nCurrentCoordYtmp + 3; i++)
    {
      if (i < TETRIS_PLAY_SPACE_Y - 1 &&
        TetrisSpaceWorkPlace[i][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
      {
        nPulishment += 30;
      }
    }
    
   /* if (nCurrentCoordYtmp + 2 < TETRIS_PLAY_SPACE_Y &&
      TetrisSpaceWorkPlace[nCurrentCoordYtmp + 2][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
      return COM_AI_TETRIS_SPACE_SCORE_SAPCE_BELOW;
    }
    if (nCurrentCoordYtmp + 1 < TETRIS_PLAY_SPACE_Y &&
      TetrisSpaceWorkPlace[nCurrentCoordYtmp + 1][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
      nPulishment += 30;
      //return COM_AI_TETRIS_SPACE_SCORE_SAPCE_BELOW;
    }*/
  }

  //nCurrentHeight = calculate_height__COM_control_AI(TetrisSpaceWorkPlace);
  nCenterPointHeight = TETRIS_PLAY_SPACE_Y - nCoordY - 1;
  nCurrentHeight = nCenterPointHeight;
  nHeightDistance = nCurrentHeight - nPreHeight;

  //如果可以消除行，那么每消一行pulishment -100，消行优先
  for (i = TETRIS_PLAY_SPACE_Y - 1 - 1; i >= 0; i--)
  {
    for (j = 1; j <= TETRIS_PLAY_SPACE_X - 1 - 1; j++)
    {
      if (TetrisSpaceWorkPlace[i][j] != SOLID_BLOCK_VALUE)
      {
        break;
      }
      if (j == TETRIS_PLAY_SPACE_X - 1 - 1)
      {
        //nPulishment -= 2000;
        return -2000;
      }
    }
  }

  /*for (i = nCoordY - 2; i < nCoordY + 3; i++)  for (i = nCoordY - 2; i < nCoordY + 3; i++)
  {
    for (j = nCoordX - 1; j < nCoordX + 2; j++)
    {
  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {      if (
        i != 0 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j - 1] >= WALL_VALUE &&
        TetrisSpaceWorkPlace[i][j + 1] >= WALL_VALUE &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A)
      {
        nPulishment += 8;
      }
      else if (
        i != 0 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j - 1] == SPACE_VALUE_TYPE_A &&
        TetrisSpaceWorkPlace[i][j + 1] >= WALL_VALUE &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A
        )
      {
        nPulishment += 6;
      }
      else if (
        i != 0 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j - 1] >= WALL_VALUE &&
        TetrisSpaceWorkPlace[i][j + 1] == SPACE_VALUE_TYPE_A &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A
        )
      {
        nPulishment += 6;
      }
      else       if (
        i - 1 >= 0 &&
        i + 1 <= TETRIS_PLAY_SPACE_Y - 1 &&
        j >= 1 &&
        j <= TETRIS_PLAY_SPACE_X - 1 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A &&
        TetrisSpaceWorkPlace[i + 1][j] == SPACE_VALUE_TYPE_A
        )
      {
        nPulishment += 8;
      }
      else if (
        i - 1 >= 0 &&
        i + 1 <= TETRIS_PLAY_SPACE_Y - 1 &&
        j >= 1 &&
        j <= TETRIS_PLAY_SPACE_X - 1 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A
        )
      {
        nPulishment += 4;
      }
    }
  }*/

  

  /*if (nHeightDistance + nCenterPointHeight + nPulishment > 255)
  {
    return 255;
  }

  if (nHeightDistance + nCenterPointHeight + nPulishment < 0)
  {
    return 0;
  }*/

  //返回最终得分：中心点的高度+惩罚
  //返回最终得分：高度差+惩罚

  return nHeightDistance * 3 + nCenterPointHeight * 3 + nPulishment;
}

static int calculate_height__COM_control_AI(
  unsigned char TetrisSpace[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      if (TetrisSpace[i][j] == SOLID_BLOCK_VALUE)
      {
        return TETRIS_PLAY_SPACE_Y - i - 1;
      }
    }
  }

  return 0;
}