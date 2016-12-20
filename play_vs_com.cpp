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


  //��ʼ����������˳�򡣡����ȡ���

  //��ӡ����
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X * 2,
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y);

  //getchar();
  tetris_loop__main_loop(pstParam);

  return 0;
}