#pragma once



int summon_new_block__engine(Param *pstParam, int *pnControlFlag,
  PlayerVSCOMControlFlag eFlag);
int move_down_block__engine(Param *pstParam, int *pnControlFlag,
  bool *pbIsSessionEndedPlayer, bool *pbIsSessionEndedCOM,
  PlayerVSCOMControlFlag eFlag);
int move_left_block__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag);
int move_right_block__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag);
int rotate_block_player__engine(Param *pstParam);
int clean_line__engine(Param *pstParam, PlayerVSCOMControlFlag eFlag);
int get_the_line_at_bottom_to_clean__data_processor(Param *pstParam,
  PlayerVSCOMControlFlag eFlag);