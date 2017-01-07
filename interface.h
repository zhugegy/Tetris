#pragma once

#include <string.h>
#include <windows.h>

int change_program_title__interface();
int hide_cursor__interface();
int change_interface_style__interface(StageList eStage);
int print_tetris_play_ground__interface(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X], 
  int nAnchorPointX, int nAnchorPointY);
int print_tetris_customize_blocks_ground__interface(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  int nAnchorPointX, int nAnchorPointY);
int print_string_array__interface(int nAnchorPointX, int nAnchorPointY,
  char szArray[][MAX_STRING_LENGTH], int nArrayCount,
  unsigned short StringArrayColor[]);
int print_block__interface(COORD cPos, unsigned char chColor);
int print_space__interface(COORD cPos, unsigned char chColor);
int print_element__interface(int nCoordX, int nCoordY, 
                             const char *pSrc = INTERFACE_SPACE_FIGURE,
                             int nColor = INTERFACE_SPACE_COLOR_TYPE_A);
int print_score__interface(int nScore, PlayerVSCOMControlFlag eFlag);