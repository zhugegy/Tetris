#include "stdafx.h"
#include "COM_control_AI.h"

#include "engine.h"
#include "COM_control_list.h"

static int initialize_tetris_space_score__COM_control_AI(
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X]);
static int simulate_rotation__COM_control_AI(Param *pstParam, int nRotateTime);
static int update_tetris_space_score__COM_control_AI(Param *pstParam,
  unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X],
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X]);
static int calculate_score__COM_control_AI(Param *pstParam, int nCoordX,
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X]);
static int find_the_min_height__COM_control_AI(
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X], int *pnRotateTime,
  int *pnCoordX, int *pnCoordY, int *pnMinHeight, int nCurrentRotateTime);

int get_the_best_rotate_and_coord__COM_control_AI(Param *pstParam,
  int *pnCoordiX, int *pnCoordiY, int *pnRotationTime)
{
  unsigned char TetrisSpaceTemplate[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  unsigned char TetrisSpaceScore_0[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  unsigned char TetrisSpaceScore_1[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  unsigned char TetrisSpaceScore_2[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];
  unsigned char TetrisSpaceScore_3[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

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
  find_the_min_height__COM_control_AI(TetrisSpaceScore_0, &nRotateTime, 
    &nCoordX, &nCoordY, &nMinHeight, 0);

  //旋转1次，对应得分器1.
  find_the_min_height__COM_control_AI(TetrisSpaceScore_1, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 1);

  //旋转2次，对应得分器2.
  find_the_min_height__COM_control_AI(TetrisSpaceScore_2, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 2);

  //旋转3次，对应得分器3.
  find_the_min_height__COM_control_AI(TetrisSpaceScore_3, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 3);
  
  *pnCoordiX = nCoordX;
  *pnCoordiY = nCoordY;
  *pnRotationTime = nRotateTime;

  return nMinHeight;
}

static int find_the_min_height__COM_control_AI(
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X], int *pnRotateTime, 
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
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X])
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
  unsigned char TetrisSpaceScore[][TETRIS_PLAY_SPACE_X])
{
  int i = 0;
  int j = 0;

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      TetrisSpaceScore[i][j] = 
        calculate_score__COM_control_AI(pstParam, j, i, TetrisSpaceTemplate);
    }
  }

  return 0;
}

static int calculate_score__COM_control_AI(Param *pstParam, int nCoordX, 
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X])
{
  BlockElement *pstTmp = NULL;

  int i = 0;
  int j = 0;

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

  //如果可以消除行，那么直接返回0，消行优先
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
        return 0;
      }
    }
  }

  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 1; j < TETRIS_PLAY_SPACE_X - 1; j++)
    {
      if (
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
      else if (
        i != 0 &&
        TetrisSpaceWorkPlace[i - 1][j] == SOLID_BLOCK_VALUE &&
        TetrisSpaceWorkPlace[i][j] == SPACE_VALUE_TYPE_A
        )
      {
        nPulishment += 4;
      }
    }
  }

  

  if (TETRIS_PLAY_SPACE_Y - nCoordY - 1 + nPulishment > 255)
  {
    return 255;
  }

  //返回最终得分：中心点的高度+惩罚
  return TETRIS_PLAY_SPACE_Y - nCoordY - 1 + nPulishment;
}

