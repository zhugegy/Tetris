#pragma once

int set_tetris_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X]);
int set_tetris_customize_blocks_space_to_start__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
int change_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char chValue, int nX, int nY);
int optimize_tetris_customize_blocks_space__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
int align_weight_to_customized_block_element__data_processor(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
int get_customized_block_center_point__data_processor(ElementCoord *pstCoord, 
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
int get_a_random_block(Param *pstParam);
int build_block_element_chain__data_processor(Param *pstParam, 
                                              int nCurrentBlock);
int pre_process_blocks__data_processor(Param *pstParam);
int after_process_block_element_chain__data_processor(Param *pstParam);
int get_the_line_at_bottom_to_clean__data_processor(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X]);