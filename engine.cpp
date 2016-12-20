#include "stdafx.h"
#include "engine.h"

#include "interface.h"
#include "data_processor.h"

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
static int get_dest_coord__engine(BlockElement *pstObj, int *pnDestX, int *pnDestY);
static int clean_specific_line(Param *pstParam, int nLineNumber,
  PlayerVSCOMControlFlag eFlag);
static int decide_parameter_detail__engine(Param *pstParam,
  BlockElement **ppstBlockElement, unsigned char **ppchArrayAddress,
  int *pnCoordX, int *pnCoordY, PlayerVSCOMControlFlag eFlag);

/*
//�ٻ��·���
int summon_new_block__engine(Param *pstParam, int *pnControlFlag)
{
  BlockElement *pstTmp = NULL;

  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //����ܷ�ɹ��ٻ����ٻ�ʧ�ܾ�����Ϸ������
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] !=
      SPACE_VALUE_TYPE_A
      &&
      pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] !=
      SPACE_VALUE_TYPE_B)
    {
      *pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER;
    }

    //��ֵ
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;

    //��ͼ
    print_element__interface(
      (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
      pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}*/



//�ٻ��·���
int summon_new_block__engine(Param *pstParam, int *pnControlFlag,
  PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //����ܷ�ɹ��ٻ����ٻ�ʧ�ܾ�����Ϸ������
    if ( *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) != SPACE_VALUE_TYPE_A
      &&
      *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X +
        pstTmp->stCoord.nX) != SPACE_VALUE_TYPE_B)
    {
      if (eFlag == PLAYER_CONTROL)
      {
        *pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER;
      }
      if (eFlag == COM_CONTROL)
      {
        *pnControlFlag = CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM;
      }
    }

    //��ֵ
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) = pstTmp->nValue;
    
    //��ͼ
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2, pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE, pstTmp->nValue);
  }

  return 0;
}

//��������(���ר�á�����ʱ������com,��Ϊ��������鶼��һ������Ȼ��ʽһ����)
//20161220���ڼ���COM��
int move_down_block__engine(Param *pstParam, int *pnControlFlag,
   bool *pbIsSessionEndedPlayer, bool *pbIsSessionEndedCOM,
   PlayerVSCOMControlFlag eFlag)
{
  BlockElement *pstTmp = NULL;

  bool bLocalNewSessionFlag = false;

  BlockElement *pstTmpExtral = NULL;
  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

   //����ܷ�ɹ������ƶ�
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //�κ�һ��Ԫ��������ǽ�����ѹ̻��ķ��飨value >= 100�����Ͳ����ƶ���
    /*if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY + 1][pstTmp->stCoord.nX] 
        >= 100)*/
    if (*(chTmpArrayAddress + (pstTmp->stCoord.nY + 1) * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) >= 100)
    {
      //�ж��Ƿ��Ѿ���Ϸ��������Ϸ������û�б�Ҫ��ˢ���µķ���
      if (*pnControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER &&
        *pnControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM)
      {
        bLocalNewSessionFlag = true;
        if (eFlag == PLAYER_CONTROL)
        {
          *pbIsSessionEndedPlayer = true;
        }
        if (eFlag == COM_CONTROL)
        {
          *pbIsSessionEndedCOM = true;
        }
      }
    }
  }

  //�����new session����ô�̻�Ŀǰ�����з���,�̻��Ժ���˳�����
  if (bLocalNewSessionFlag == true)
  {
    for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
    {
      /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] = 
        SOLID_BLOCK_VALUE;*/
      *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
        pstTmp->stCoord.nX) = SOLID_BLOCK_VALUE;
    }

    //20161018�������е�ԭ�򣬹̻�����ɫ��Ҫ�̶���ͳһ��
    for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
    {
      print_element__interface((pstTmp->stCoord.nX + nCoordX) * 2,
        pstTmp->stCoord.nY + nCoordY, INTERFACE_BLOCK_FIGURE,
        INTERFACE_SOLID_BLOCK_COLOR);
    }

    return 0;
  }

  //��β�͸�ֵ,Ϊ�Ժ�Ĺ�����׼��
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //β�;����������ֵ
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

    //��������ǹ̻���Ǿ͵��ɿհ׿���һ��������©��BUG��
    if (pstTmp->nTailValue == SOLID_BLOCK_VALUE)
    {
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX);
    }

  }


  //��ֵ1.����Ԫ����������
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nY += 1;
  }

  //��ֵ2.����������ֵ�ĸı�
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //Ԫ������λ�ø�ֵ����value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;*/
    *(chTmpArrayAddress + pstTmp->stCoord.nY * TETRIS_PLAY_SPACE_X + 
      pstTmp->stCoord.nX) = pstTmp->nValue;

    //Ԫ�����渳ֵβ�͵�value
    /*pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY - 1][pstTmp->stCoord.nX] =
      pstTmp->nTailValue;*/
    *(chTmpArrayAddress + (pstTmp->stCoord.nY - 1) * TETRIS_PLAY_SPACE_X +
      pstTmp->stCoord.nX) = pstTmp->nTailValue;
  }

  //��ͼ
  for (pstTmp = pstTmpExtral; pstTmp; pstTmp = pstTmp->pNext)
  {
    //������ģ�β�ͣ�
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

    //�����ڵ�
    print_element__interface(
      (pstTmp->stCoord.nX + nCoordX) * 2,
      pstTmp->stCoord.nY + nCoordY,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//��������(���ר�á�����ʱ������com,��Ϊ��������鶼��һ������Ȼ��ʽһ����)
int move_left_block_player__engine(Param *pstParam)
{
  BlockElement *pstTmp = NULL;

  //����ܷ�ɹ������ƶ�
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //�κ�һ��Ԫ��������ǽ�����ѹ̻��ķ��飨value >= 100�����Ͳ����ƶ���
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] 
    >= 100)
    {
      //���أ�ʲôҲ����
      return 0;
    }
  }

  //��β�͸�ֵ,Ϊ�Ժ�Ĺ�����׼��
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //β�;����������ֵ
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] >=
      100)
    {
      //����ұ߽���ǽ���߹̻���
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX + 1);
    }
    else
    {
      pstTmp->nTailValue = 
        pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1];
    }
  }

  //��ֵ1.����Ԫ����������
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nX -= 1;
  }

  //��ֵ2.����������ֵ�ĸı�
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //Ԫ������λ�ø�ֵ����value
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;

    //Ԫ�����渳ֵβ�͵�value
    //������ɫ�յ���ֵ����
    if (pstTmp->nTailValue == SPACE_VALUE_TYPE_A)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_B;
    }
    else if (pstTmp->nTailValue == SPACE_VALUE_TYPE_B)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_A;
    }

    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] =
      pstTmp->nTailValue;
  }

  //��ͼ
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //������ģ�β�ͣ�
    switch (pstTmp->nTailValue)
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX + 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
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
        (pstTmp->stCoord.nX + 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nTailValue);
      break;
    default:
      printf("error debug");
      break;
    }

    //�����ڵ�
    print_element__interface(
      (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
      pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//��������(���ר�á�����ʱ������com,��Ϊ��������鶼��һ������Ȼ��ʽһ����)
//��ʵҲ�򵥣�����Ӹ��ж�Player����COM�Ĳ�����Ȼ��pstParam->pstFirstBlockElement ��
//pstParam->TetrisPlaySpace switchcase ������ֵ���������Ȳ�Ū������������ֱ�Ӳ�����
int move_right_block_player__engine(Param *pstParam)
{
  BlockElement *pstTmp = NULL;

  //����ܷ�ɹ������ƶ�
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //�κ�һ��Ԫ��������ǽ�����ѹ̻��ķ��飨value >= 100�����Ͳ����ƶ���
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX + 1] 
    >= 100)
    {
      //���أ�ʲôҲ����
      return 0;
    }
  }

  //��β�͸�ֵ,Ϊ�Ժ�Ĺ�����׼��
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //β�;����������ֵ
    if (pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] >=
      100)
    {
      //�����߽���ǽ���߹̻���
      pstTmp->nTailValue = SPACE_TYPE(pstTmp->stCoord.nX - 1);
    }
    else
    {
      pstTmp->nTailValue = 
        pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1];
    }
  }

  //��ֵ1.����Ԫ����������
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    pstTmp->stCoord.nX += 1;
  }

  //��ֵ2.����������ֵ�ĸı�
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //Ԫ������λ�ø�ֵ����value
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      pstTmp->nValue;

    //Ԫ�����渳ֵβ�͵�value
    //������ɫ�յ���ֵ����
    if (pstTmp->nTailValue == SPACE_VALUE_TYPE_A)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_B;
    }
    else if (pstTmp->nTailValue == SPACE_VALUE_TYPE_B)
    {
      pstTmp->nTailValue = SPACE_VALUE_TYPE_A;
    }

    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX - 1] =
      pstTmp->nTailValue;
  }

  //��ͼ
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //������ģ�β�ͣ�
    switch (pstTmp->nTailValue)
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX - 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
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
        (pstTmp->stCoord.nX - 1 + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nTailValue);
      break;
    default:
      printf("error debug");
      break;
    }

    //�����ڵ�
    print_element__interface(
      (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
      pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
      INTERFACE_BLOCK_FIGURE,
      pstTmp->nValue);
  }

  return 0;
}

//������ת
int rotate_block_player__engine(Param *pstParam)
{
  BlockElement *pstTmp = NULL;

  int nDestX = 0;
  int nDestY = 0;

  //����ܷ���ת
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //����ת���Ŀ��λ��
    get_dest_coord__engine(pstTmp, &nDestX, &nDestY);

    if (nDestX <= 0 || nDestX >= TETRIS_PLAY_SPACE_X - 1)
    {
      return 0;
    }
    if (nDestY < 0 || nDestY >= TETRIS_PLAY_SPACE_Y - 1)
    {
      return 0;
    }
    //���Ŀ��ص���ǽ�����߹̻���
    if (pstParam->TetrisPlaySpacePlayer[nDestY][nDestX] >= 100)
    {
      return 0;
    }
  }

  
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //��ֵ1.����������ֵ�ĸı�Ԫ������λ�ø�ֵ�յ�
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] =
      SPACE_TYPE(pstTmp->stCoord.nX);
    //��ͼ1.Ԫ������λ�û��յ�
    switch (SPACE_TYPE(pstTmp->stCoord.nX))
    {
    case SPACE_VALUE_TYPE_A:
      print_element__interface(
        (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
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

  //��ֵ���ı䷽��Ԫ������
  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    get_dest_coord__engine(pstTmp, &nDestX, &nDestY);
    pstTmp->stCoord.nX = nDestX;
    pstTmp->stCoord.nY = nDestY;
  }

  for (pstTmp = pstParam->pstFirstBlockElementPlayer; pstTmp; pstTmp = pstTmp->pNext)
  {
    //��ֵ2.����������ֵ�ĸı�Ԫ������λ�ø�ֵ��ǰԪ�ص�ֵ
    pstParam->TetrisPlaySpacePlayer[pstTmp->stCoord.nY][pstTmp->stCoord.nX] = 
      pstTmp->nValue;
    //��ͼ2.Ԫ������λ�û��յ�
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
        (pstTmp->stCoord.nX + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X) * 2,
        pstTmp->stCoord.nY + INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y,
        INTERFACE_BLOCK_FIGURE,
        pstTmp->nValue);
      break;
    
    default:
      break;
    }
  }

  return 0;
}

static int get_dest_coord__engine(BlockElement *pstObj, int *pnDestX, int *pnDestY)
{
  //�Ȱ������ƶ���0,0����ϵ��������������������ƶ���Ҫ��λ��
  int nShiftDistanceX = 0;
  int nShiftDistanceY = 0;
  //�˵���0,0����ϵ�������
  int nRelativeCoordX = 0;
  int nRelativeCoordY = 0;
  //�˵����������
  int nQuadrantType = 0;
  //�˵����ת��׼��
  int nAlignCoordX = 0;
  int nAlignCoordY = 0;
  int nDistanceToAlignCoord = 0;

  //���ƶ���0,0����ϵ��Ҫ������λ��
  nShiftDistanceX = 0 - pstObj->pCenter->stCoord.nX;
  nShiftDistanceY = 0 - pstObj->pCenter->stCoord.nY;

  //��ô˵���0,0����ϵ�������
  nRelativeCoordX = pstObj->stCoord.nX + nShiftDistanceX;
  nRelativeCoordY = pstObj->stCoord.nY + nShiftDistanceY;

  //��ô˵����������
  nQuadrantType = get_quadrant_type_for_rotation__engine(nRelativeCoordX, 
    nRelativeCoordY);

  //��ô˵����ת��׼��
  get_the_align_coord__engine(nRelativeCoordX, nRelativeCoordY, nQuadrantType,
    &nAlignCoordX, &nAlignCoordY, &nDistanceToAlignCoord);

  //��ʼ��ת����ô˵���ת���0,0����
  get_relative_coord_after_rotation__engine(nQuadrantType, nDistanceToAlignCoord,
    nAlignCoordX, nAlignCoordY, &nRelativeCoordX, &nRelativeCoordY);

  //�������ڵ�0,0���꣬���Ƴ�Ŀ�ĵ�����
  *pnDestX = pstObj->pCenter->stCoord.nX - nRelativeCoordX;
  *pnDestY = pstObj->pCenter->stCoord.nY - nRelativeCoordY;

  return 0;
}

//���һ�������������(enum BlockRotationQuadrant)
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

//���һ�������ת��׼��
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
  //��ת��Ļ�׼������
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

//������·���һ�п����������к�
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

  //����Ƿ��п�����������
  nLineNumber = get_the_line_at_bottom_to_clean__data_processor(pstParam, eFlag);

  while (nLineNumber != -1)
  {
    clean_specific_line(pstParam, nLineNumber, eFlag);
	//�����ڴ˴���ӵ÷���ش���
    nLineNumber = get_the_line_at_bottom_to_clean__data_processor(pstParam, eFlag);
  }

  return 0;
}

static int clean_specific_line(Param *pstParam, int nLineNumber, 
  PlayerVSCOMControlFlag eFlag)
{
  int i = 0;
  int j = 0;

  //�ڴ˺������������û�ã���������Ϊ����������decide_parameter_detail__engine�����Ĳ�������
  BlockElement *pstTmpExtral = NULL;  

  unsigned char *chTmpArrayAddress = NULL;
  int nCoordX = 0;
  int nCoordY = 0;

  decide_parameter_detail__engine(pstParam, &pstTmpExtral, &chTmpArrayAddress,
    &nCoordX, &nCoordY, eFlag);

  //��ɾ���������ϵ�������ƽ��
  for (i = nLineNumber; i >= 1; i--)
  {
    for (j = 1; j < TETRIS_PLAY_SPACE_X - 1; j++)
    {
      //��ֵ
      //pstParam->TetrisPlaySpacePlayer[i][j] = pstParam->TetrisPlaySpacePlayer[i - 1][j];
      *(chTmpArrayAddress + i * TETRIS_PLAY_SPACE_X + j) = 
        *(chTmpArrayAddress + (i - 1) * TETRIS_PLAY_SPACE_X + j);

      //��ͼ
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

  //����һ�б�ɿհ�
  for (j = 1; j < TETRIS_PLAY_SPACE_X - 1; j++)
  {
    //��ֵ
    //pstParam->TetrisPlaySpacePlayer[0][j] = SPACE_VALUE_TYPE_A;
    *(chTmpArrayAddress + 0 * TETRIS_PLAY_SPACE_X + j) = SPACE_VALUE_TYPE_A;

    //��ͼ
    print_element__interface((j + nCoordX) * 2, 0 + nCoordY,
      INTERFACE_SPACE_FIGURE, INTERFACE_SPACE_COLOR_TYPE_A);
  }

  return 0;
}

//��������һ��ǵ��ԣ�����������������ʲô
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