#include "stdafx.h"
#include "play_solo.h"

#include "data_processor.h"
#include "interface.h"
#include "main_loop.h"

int play_solo(Param *pstParam)
{
  //�Ѷ���˹���鷽����Ϊ��ʼ״̬
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpacePlayer);

  //��ʼ����������˳�򡣡����ȡ���

  //��ӡ����
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  //�������ѭ��
  tetris_loop__main_loop(pstParam);
  

  return 0;
}