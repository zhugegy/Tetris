#pragma once

int get_the_best_rotate_and_coord__COM_control_AI(Param *pstParam,
  int *pnCoordiX, int *pnCoordiY, int *pnRotationTime);



#define COM_AI_TETRIS_SPACE_MAX_SCORE 200
#define COM_AI_TETRIS_SPACE_SCORE_OUT_REACH 199
#define COM_AI_TETRIS_SPACE_SCORE_CONTAIN_WALL 198
#define COM_AI_TETRIS_SPACE_SCORE_CONTAIN_SOLID_BLOCK 197
#define COM_AI_TETRIS_SPACE_SCORE_SOLID_BLOCK_ABOVE 196
#define COM_AI_TETRIS_SPACE_MIN_HEIGHT_DEFAULT \
  (COM_AI_TETRIS_SPACE_MAX_SCORE + 1)

/*
#define COM_AI_TETRIS_SPACE_MAX_SCORE 0
#define COM_AI_TETRIS_SPACE_SCORE_OUT_REACH 0
#define COM_AI_TETRIS_SPACE_SCORE_CONTAIN_WALL 0
#define COM_AI_TETRIS_SPACE_SCORE_CONTAIN_SOLID_BLOCK 0
#define COM_AI_TETRIS_SPACE_MIN_HEIGHT_DEFAULT 0*/