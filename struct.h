#pragma once
//此文件记录了所有的结构体

//舞台控制
enum StageList{
  STAGE_MAIN_MENU, 
  STAGE_PLAY_SOLO, 
  STAGE_PLAY_VS_COM, 
  STAGE_HIGH_SCORES,
  STAGE_CUSTOMIZE_BLOCKS,
  STAGE_QUIT
};

//自定义方块出现频率
enum CustomBlockFrequency {
  CUSTOMIZED_BLOCK_FREQUENCY_VERY_LOW = 1,
  CUSTOMIZED_BLOCK_FREQUENCY_LOW,
  CUSTOMIZED_BLOCK_FREQUENCY_NORMAL,
  CUSTOMIZED_BLOCK_FREQUENCY_HIGH,
  CUSTOMIZED_BLOCK_FREQUENCY_VERY_HIGH
};

//自定义人机对战控制标示符
enum PlayerVSCOMControlFlag {
	PLAYER_CONTROL = 1,
	COM_CONTROL
};

//备忘：CUSTOMIZE_BLOCK别忘了释放（Param->pstCustomizedBlockNodes）
enum ErrorMessages{
  ERROR_CUSTOMIZE_BLOCK_NODE_MALLOC,
  ERROR_BLOCK_ELEMENT_NODE_MALLOC
};

enum BlockRotationQuadrant
{
  TOP_LEFT_WITH_NEGATIVE_X_0_Y = 1,
  TOP_RIGHT_WITH_0_X_POSTIVE_Y,
  BOTTOM_RIGHT_WITH_POSTIVE_X_0_Y,
  BOTTOM_LEFT_WITH_0_X_NEGATIVE_Y
};

//自定义方块
typedef struct CustomizedBlock
{
  int nSerial;
  int nCenterX;
  int nCenterY;
  int nFrequency;
  unsigned char TetrisCustomizeBlocksSpace[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X];

  struct CustomizedBlock *pNext;
}CustomizedBlock;

//自定义坐标
typedef struct ElementCoord
{
  int nX;
  int nY;
}ElementCoord;

//俄罗斯方块每一块的结构体
typedef struct BlockElement
{
  ElementCoord stCoord;
  int nValue;    //数值
  int nTailValue;    //尾部数值，用于移动时的临时存储
  char isCenter;

  BlockElement *pCenter;
  BlockElement *pNext;
}BlockElement;

//重要参数
typedef struct Param
{
  StageList eStageFlag;

  //游戏时的方块方阵空间(玩家)
  unsigned char TetrisPlaySpacePlayer[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  //游戏时的方块方阵空间(电脑)
  unsigned char TetrisPlaySpaceCOM[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  //自定义模式的方块方阵空间
  unsigned char TetrisCustomizeBlocksSpace[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X];

  //所有方块的链表
  CustomizedBlock *pstCustomizedBlockNodes;

  //存储当前方块总数
  int nBlockNum;

  //存储方块地址的快捷数组
  CustomizedBlock *pstFastArray[MAX_BLOCK_NUM];

  //存储当前方块中所有元素的链表（用于游戏中的上下左右操作）(玩家)
  BlockElement *pstFirstBlockElementPlayer;

  //存储当前方块中所有元素的链表（用于游戏中的上下左右操作）(电脑)
  BlockElement *pstFirstBlockElementCOM;

}Param;