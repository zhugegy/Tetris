#include "stdafx.h"
#include "play_vs_com.h"

#include "data_processor.h"
#include "interface.h"
int play_vs_com(Param *pstParam)
{
  //重新设置环境

  //把俄罗斯方块方阵置为开始状态
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpace);

  //初始化。。生成顺序。。。等。。

  //打印背景
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpace, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  print_tetris_play_ground__interface(pstParam->TetrisPlaySpace, 
    (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 30 )* 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  getchar();

  return 0;
}