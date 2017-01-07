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


  //载入方块信息
  reload_block_info(pstParam);

  //打印背景

  //A.玩家部分
  //打印玩家场地（下一个方块区域的打印在游戏中不断发生，所以此处不打印）
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y);

  //打印“已消除行数”这一行字
  print_element__interface(
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X * 2,
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y,
    INTERFACE_SCORING_STRING, INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //B.电脑部分
  //打印电脑场地（下一个方块区域的打印在游戏中不断发生，所以此处不打印）
  print_tetris_play_ground__interface(pstParam->TetrisPlaySpacePlayer, 
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X * 2,
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y);

  //打印“已消除行数”这一行字
  print_element__interface(
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X * 2,
    INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y,
    INTERFACE_SCORING_STRING, INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //getchar();
  tetris_loop__main_loop(pstParam);

  return 0;
}