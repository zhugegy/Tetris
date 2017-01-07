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
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X]);
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

  //���Ƶ�ǰ��Ϸ�������
  memcpy(TetrisSpaceTemplate, pstParam->TetrisPlaySpaceCOM,
    sizeof(TetrisSpaceTemplate) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��

  //��ʼ���÷���;��Ӧ����4����ת��ʽ
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_0);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_1);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_2);
  initialize_tetris_space_score__COM_control_AI(TetrisSpaceScore_3);

  //����÷�
  //����ת����Ӧ�÷���0.
  simulate_rotation__COM_control_AI(pstParam, 0);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_0);

  //��ת1�Σ���Ӧ�÷���1.
  simulate_rotation__COM_control_AI(pstParam, 1);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_1);

  //��ת2�Σ���Ӧ�÷���2.
  simulate_rotation__COM_control_AI(pstParam, 2);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_2);

  //��ת3�Σ���Ӧ�÷���3.
  simulate_rotation__COM_control_AI(pstParam, 3);
  update_tetris_space_score__COM_control_AI(pstParam, TetrisSpaceTemplate, TetrisSpaceScore_3);

  //�ҵ���С�߶ȶ�Ӧ����ת����������
  //����ת����Ӧ�÷���0.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_0, &nRotateTime, 
    &nCoordX, &nCoordY, &nMinHeight, 0);

  //��ת1�Σ���Ӧ�÷���1.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_1, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 1);

  //��ת2�Σ���Ӧ�÷���2.
  find_the_min_score__COM_control_AI(TetrisSpaceScore_2, &nRotateTime,
    &nCoordX, &nCoordY, &nMinHeight, 2);

  //��ת3�Σ���Ӧ�÷���3.
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
    //ʲô������(��ʵ���������ƶ�һ��)
    move_down_block__engine(pstParam, pnControlFlag, pbIsSessionEndedCOM,
      COM_CONTROL);
    break;
  case COM_CONTROL_ROTATE:
    //debug�ϵ�
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

//��ʼ���÷�����
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

  //�Ȱ�����Ԫ�ص�stCoordAItmp��ֵ(��Ϊԭֵ)
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
  int nPreHeight = 0;

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

  nPreHeight = calculate_height__COM_control_AI(TetrisSpaceTemplate);

  nShiftDistanceX = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nX - nCoordX;
  nShiftDistanceY = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nY - nCoordY;

  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;
    
    //���������Խ��
    if (nCurrentCoordXtmp <= 0 || nCurrentCoordXtmp >= TETRIS_PLAY_SPACE_X - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //���������Խ��
    if (nCurrentCoordYtmp < 0 || nCurrentCoordYtmp >= TETRIS_PLAY_SPACE_Y - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //�����ǽ
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] == WALL_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_WALL;
    }

    //����ǹ̻���
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] == 
      SOLID_BLOCK_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_SOLID_BLOCK;
    }

    //����յ������з��飨Ҳ�����޷����䵽���λ�ã�
    for (i = nCurrentCoordYtmp - 1; i >= 0; i--)
    {
      if (TetrisSpaceTemplate[i][nCurrentCoordXtmp] == SOLID_BLOCK_VALUE)
      {
        return COM_AI_TETRIS_SPACE_SCORE_SOLID_BLOCK_ABOVE;
      }
    }
  }

  //�����Υ�棬����ո��Ϸ����³ͷ��������յ������пո�ĳͷ���
  memcpy(TetrisSpaceWorkPlace, TetrisSpaceTemplate,
    sizeof(TetrisSpaceWorkPlace) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��

  //����λ��������ֵ��TetrisSpaceWorkPlace����
  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;

    TetrisSpaceWorkPlace[nCurrentCoordYtmp][nCurrentCoordXtmp] = SOLID_BLOCK_VALUE;
  }

  //������λ����������пո�ĳͷ�
  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;

    for (i = nCurrentCoordYtmp + 1; 
      i < nCurrentCoordYtmp + AI_STRATEGY_PUNISHMENT_DEEPTH; i++)
    {
      if (i < TETRIS_PLAY_SPACE_Y - 1 &&
        TetrisSpaceWorkPlace[i][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
      {
        nPulishment += AI_STRATEGY_PUNISHMENT_VALUE;
      }
    }
  }

  nCenterPointHeight = TETRIS_PLAY_SPACE_Y - nCoordY - 1;
  nHeightDistance = nCenterPointHeight - nPreHeight;

  //���ѹ���ע�ͣ�������������У���ôÿ��һ��pulishment -100����������
  //20170101����ǳ����Ļ��������о�ֱ�����У�nValue == 11 ���жϲ��Ͻ�������ʱ���ã�
  //if (pstParam->pstFirstBlockElementCOM->nValue == 11)
  //20170101 ���޸�Ϊ nSerial == 1
  //��Ч�ʸ��ߵĽǶȿ��ǣ����жϿ���ǰ�ƣ���Ŀǰ����û����
  if (pstParam->pstFirstBlockElementCOM->nSerial == 1)
  {
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
          return COM_AI_TETRIS_SPACE_SCORE_ABLE_TO_CLEAN;
        }
      }
    }
  }

  return nHeightDistance * AI_STRATEGY_HEIGHT_DISTANCE_FACTOR + 
    nCenterPointHeight * AI_STRATEGY_CENTER_POINT_HEIGHT_FACTOR + 
    nPulishment;
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

//���ݣ�AI������ʷ
/*
static int calculate_score__COM_control_AI(Param *pstParam, int nCoordX,
  int nCoordY, unsigned char TetrisSpaceTemplate[][TETRIS_PLAY_SPACE_X])
{
  BlockElement *pstTmp = NULL;
  int nPreHeight = 0;

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

  nPreHeight = calculate_height__COM_control_AI(TetrisSpaceTemplate);

  nShiftDistanceX = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nX - nCoordX;
  nShiftDistanceY = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nY - nCoordY;

  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    nCurrentCoordXtmp = pstTmp->stCoordAItmp.nX - nShiftDistanceX;
    nCurrentCoordYtmp = pstTmp->stCoordAItmp.nY - nShiftDistanceY;

    //���������Խ��
    if (nCurrentCoordXtmp <= 0 || nCurrentCoordXtmp >= TETRIS_PLAY_SPACE_X - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //���������Խ��
    if (nCurrentCoordYtmp < 0 || nCurrentCoordYtmp >= TETRIS_PLAY_SPACE_Y - 1)
    {
      return COM_AI_TETRIS_SPACE_SCORE_OUT_REACH;
    }

    //�����ǽ
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] == WALL_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_WALL;
    }

    //����ǹ̻���
    if (TetrisSpaceTemplate[nCurrentCoordYtmp][nCurrentCoordXtmp] ==
      SOLID_BLOCK_VALUE)
    {
      return COM_AI_TETRIS_SPACE_SCORE_CONTAIN_SOLID_BLOCK;
    }

    //����յ������з��飨Ҳ�����޷����䵽���λ�ã�
    for (i = nCurrentCoordYtmp - 1; i >= 0; i--)
    {
      if (TetrisSpaceTemplate[i][nCurrentCoordXtmp] == SOLID_BLOCK_VALUE)
      {
        return COM_AI_TETRIS_SPACE_SCORE_SOLID_BLOCK_ABOVE;
      }
    }

  }

  / * //�����Υ�棬�������ĵ�ĸ߶�
  return TETRIS_PLAY_SPACE_Y - nCoordY - 1;* /


  //�����Υ�棬����ո��Ϸ����³ͷ��������յ������пո�ĳͷ���
  memcpy(TetrisSpaceWorkPlace, TetrisSpaceTemplate,
    sizeof(TetrisSpaceWorkPlace) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��

                                                     //����λ��������ֵ��TetrisSpaceWorkPlace����
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
        //nPulishment += 20 * (i - nCurrentCoordYtmp);
        nPulishment += 40;
        //nPulishment += 20 + (i - nCurrentCoordYtmp) * 30 + (TETRIS_PLAY_SPACE_Y - i) * 40;
      }
    }

    //���������A
    / *for (i = nCurrentCoordYtmp + 1; i < nCurrentCoordYtmp + 4; i++)
    {
    if (i < TETRIS_PLAY_SPACE_Y - 1 &&
    TetrisSpaceWorkPlace[i][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
    //nPulishment += 32;// *(i - nCurrentCoordYtmp);
    //A
    nPulishment += 8 * (TETRIS_PLAY_SPACE_Y - i) +  20 * (i - nCurrentCoordYtmp);
    }
    }* /

    / *for (i = nCurrentCoordYtmp + 1; i < TETRIS_PLAY_SPACE_Y - 1; i++)
    {
    if (TetrisSpaceWorkPlace[i][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
    nPulishment += 3 * (TETRIS_PLAY_SPACE_Y - i);
    }
    }* /

    / * if (nCurrentCoordYtmp + 2 < TETRIS_PLAY_SPACE_Y &&
    TetrisSpaceWorkPlace[nCurrentCoordYtmp + 2][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
    return COM_AI_TETRIS_SPACE_SCORE_SAPCE_BELOW;
    }
    if (nCurrentCoordYtmp + 1 < TETRIS_PLAY_SPACE_Y &&
    TetrisSpaceWorkPlace[nCurrentCoordYtmp + 1][nCurrentCoordXtmp] == SPACE_VALUE_TYPE_A)
    {
    nPulishment += 30;
    //return COM_AI_TETRIS_SPACE_SCORE_SAPCE_BELOW;
    }* /
  }

  //nCurrentHeight = calculate_height__COM_control_AI(TetrisSpaceWorkPlace);
  nCenterPointHeight = TETRIS_PLAY_SPACE_Y - nCoordY - 1;
  //nCurrentHeight = nCenterPointHeight;
  //nHeightDistance = nCurrentHeight - nPreHeight;
  nHeightDistance = nCenterPointHeight - nPreHeight;

  //������������У���ôÿ��һ��pulishment -100����������
  //20170101����������У������ǳ����Ļ���ֱ�����У�nValue == 11 ���жϲ��Ͻ�������ʱ���ã�
  //if (pstParam->pstFirstBlockElementCOM->nValue == 11)
  //20170101 ���޸�Ϊ nSerial == 1
  if (pstParam->pstFirstBlockElementCOM->nSerial == 1)
  {
    printf("");   //debug
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
  }


  / *for (i = nCoordY - 2; i < nCoordY + 3; i++)  for (i = nCoordY - 2; i < nCoordY + 3; i++)
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
  }* /



  / *if (nHeightDistance + nCenterPointHeight + nPulishment > 255)
  {
  return 255;
  }

  if (nHeightDistance + nCenterPointHeight + nPulishment < 0)
  {
  return 0;
  }* /

  //�������յ÷֣����ĵ�ĸ߶�+�ͷ�
  //�������յ÷֣��߶Ȳ�+�ͷ�

  //���������A

  / * if (pstParam->pstFirstBlockElementCOM->nValue == 11)
  {
  nPulishment = 0;
  }* /

  / *return nHeightDistance * 4 + nCenterPointHeight * 6 + nPulishment;* /
  return nHeightDistance * 5 + nCenterPointHeight * 5 + nPulishment;
}*/