#include "stdafx.h"
#include "play_vs_com.h"

#include "data_processor.h"
#include "interface.h"
#include "main_loop.h"

int play_vs_com(Param *pstParam)
{
  //�������û���

  //�Ѷ���˹���鷽����Ϊ��ʼ״̬(���)
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpacePlayer);

  //�Ѷ���˹���鷽����Ϊ��ʼ״̬(����)
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpaceCOM);


  //���뷽����Ϣ
  reload_block_info(pstParam);

  //��ӡ����

  //A.��Ҳ���
  //��ӡ��ҳ��أ���һ����������Ĵ�ӡ����Ϸ�в��Ϸ��������Դ˴�����ӡ��
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  //��ӡ����������������һ����
  print_element__interface(
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X * 2,
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y,
    INTERFACE_SCORING_STRING, INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //B.���Բ���
  //��ӡ���Գ��أ���һ����������Ĵ�ӡ����Ϸ�в��Ϸ��������Դ˴�����ӡ��
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X * 2,
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y);

  //��ӡ����������������һ����
  print_element__interface(
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X * 2,
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y,
    INTERFACE_SCORING_STRING, INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //getchar();
  tetris_loop__main_loop(pstParam);

  return 0;
}