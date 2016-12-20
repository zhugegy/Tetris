#include "stdafx.h"
#include "play_vs_com.h"

#include "data_processor.h"
#include "interface.h"
#include "main_loop.h"

int play_vs_com(Param *pstParam)
{
  //重新设置环境

  //把俄罗斯方块方阵置为开始状态(玩家)
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpacePlayer);

  //把俄罗斯方块方阵置为开始状态(电脑)
  set_tetris_space_to_start__data_processor(pstParam->TetrisPlaySpaceCOM);


  //初始化。。生成顺序。。。等。。

  //打印背景
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