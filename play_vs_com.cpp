#include "stdafx.h"
#include "play_vs_com.h"

#include "data_processor.h"
#include "interface.h"
int play_vs_com(Param *pstParam)
{
  //�������û���

  //�Ѷ���˹���鷽����Ϊ��ʼ״̬
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpace);

  //��ʼ����������˳�򡣡����ȡ���

  //��ӡ����
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpace, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  print_tetris_play_ground__interface(pstParam->TetrisPlaySpace, 
    (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 30 )* 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  getchar();

  return 0;
}