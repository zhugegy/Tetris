#include "stdafx.h"
#include "COM_control_AI_1.h"

#include "COM_control_list.h"
#include "COM_control_AI.h"
#include "engine.h"

//���ļ�ָ�����ԵĿ��Ʋ��ԡ�
//�������ԣ��߶����ԭ��Ѱ�ҿ���ת��-���ȴ��������ת��-����ת-������λ��-��Ѹ����λ��
//�Ľ���Ѱ�ҿ���ת��-���ȴ���ת���-������λ��-����Ѹ����λ����
//��AI���Դֲڣ�����Ϊ���ԣ�������Ϊ��ͼ�����AIʹ�á�

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
  //��ʼ�����ڵ�ControlMsg
  for (i = 0; i < MESSAGE_CONTAINER_MAX_NUM; i++)
  {
    BuildingControlMsg[i] = COM_CONTROL_DEFAULT;
  }*/

  //�����ִ�Ŀ�����Ϣ
  pstParam->COMControlMsg.reset_message_list();

  if (pstParam->pstFirstBlockElementCOM->nValue == 11)
  {
    //debug
    printf("");
  }

  //��ȡ��ת���������ĵ������λ�ã����������
  get_the_best_rotate_and_coord__COM_control_AI(pstParam, 
    &nDestCoordX, &nDestCoordY, &nRotateTimes);

  //��ȡ���Խ�����ת��������һ��
  //nRotateLine = find_the_first_line_for_rotation__COM_control_AI_1(pstParam);



  //�ѵȴ��������ȴ���ת��д��ָ��
  //nCmd = translate_first_line_to_cmd__COM_control_AI_1(nRotateLine);
  //pstParam->COMControlMsg.write_message(nCmd);

  //����ת����д��ָ��
  for (i = 0; i < nRotateTimes; i++)
  {
    pstParam->COMControlMsg.write_message(COM_CONTROL_ROTATE);
  }

  //�������ƶ�д��ָ��
  nCurrentCenterCoordX = pstParam->pstFirstBlockElementCOM->pCenter->stCoord.nX;
  if (nCurrentCenterCoordX >= nDestCoordX)
  {
    //������ĵ��ʼλ��������λ�õ��ұ�
    while (nCurrentCenterCoordX > nDestCoordX)
    {
      pstParam->COMControlMsg.write_message(COM_CONTROL_MOVE_LEFT);
      nCurrentCenterCoordX--;
    }
  }
  else
  {
    //������ĵ��ʼλ��������λ�õ����
    while (nCurrentCenterCoordX < nDestCoordX)
    {
      pstParam->COMControlMsg.write_message(COM_CONTROL_MOVE_RIGHT);
      nCurrentCenterCoordX++;
    }
  }

  //���д��ֱ������ָ��
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


  //�Ȱ�����Ԫ�ص�stCoordAItmp��ֵ(��Ϊԭֵ)
  for (pstTmp = pstParam->pstFirstBlockElementCOM; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoordAItmp.nX = pstTmp->stCoord.nX;
    pstTmp->stCoordAItmp.nY = pstTmp->stCoord.nY;
  }

  pstCenter = pstParam->pstFirstBlockElementCOM->pCenter;

  //�����ĵ��Y����С��������һ�У������ǽ����Y����
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
        //�����ת���Ϸ�
        break;
      }
      else
      {
        //�����ת�Ϸ�
        continue;
      }
    }

    if (pstTmp == NULL)
    {
      //֤��������������element�����ԺϷ���ת
      pstTmp = pstParam->pstFirstBlockElementCOM;
      return pstTmp->pCenter->stCoordAItmp.nY;
    }
    else
    {
      //֤�������ڴ˴���ת���Ϸ�����������һ���ٳ���
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