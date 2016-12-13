#pragma once
//本文件记录了所有的宏。

/*定义俄罗斯方块类型对应的数值上下限，最低51，最高80，代表可以有30个不同的方块类
型*/
/*
#define BLOCK_VALUE_MIN 51
#define BLOCK_VALUE_MAX 80*/

#define FILE_LOCATION_LINK_INFO "C:\\blockinfo.txt"

#define WALL_VALUE 100    //墙的数值
#define SOLID_BLOCK_VALUE 110    //固化的方块数值

//为了让玩家更好地识别纵向直线，相邻直线之间颜色略有差别（类似足球越位草坪）
#define SPACE_VALUE_TYPE_A 52    //空格数值:TYPE_A
/*#define SPACE_VALUE_TYPE_B 51    //空格数值：TYPE_B*/
#define SPACE_VALUE_TYPE_B 52

//判断空格的类型，X坐标奇数就是TYPE_A,偶数就是TYPE_B
#define SPACE_TYPE(x) x % 2 == 0 ? SPACE_VALUE_TYPE_B : SPACE_VALUE_TYPE_A 



#define MAX_STRING_LENGTH 1024    //字符串最大长度，多用于初始化临时字符串数组

//界面相关参数
#define PROGRAM_TITLE "俄罗斯方块"
#define PROGRAM_VERSION "Version 0.01"

//方块种类数量（默认的俄罗斯方块是7种）
#define MAX_BLOCK_NUM 100

//control flag 其实可以用enum
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING 1
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_QUIT 0
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_SAVE 99

#define CONTROL_FLAG_MAIN_LOOP_RUNNING 1
#define CONTROL_FLAG_MAIN_LOOP_GAME_OVER 0
#define CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER 11 

//背景颜色
#define INTERFACE_BASIC_BACKGROUND "30"    //蓝底（黑字）
#define INTERFACE_PLAY_BACKGROUND "0F"    //黑底(白字)

//方块形状及其颜色（颜色也同时应用于字体打印，同时也作为方块的数值用于存储）
#define INTERFACE_BLOCK_FIGURE "■"
#define INTERFACE_BLOCK_COLOR_BLACK_WHITE 0x0F    //黑底白色
#define INTERFACE_BLOCK_COLOR_BLACK_CYAN 0x0B    //黑底青色
#define INTERFACE_BLOCK_COLOR_BLACK_BLUE 0x09    //黑底蓝色
#define INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW 0x06    //黑底暗黄色
#define INTERFACE_BLOCK_COLOR_BLACK_YELLOW 0x0E    //黑底黄色
#define INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE 0x05    //黑底暗紫色
#define INTERFACE_BLOCK_COLOR_BLACK_PURPLE 0x0D    //黑底紫色
#define INTERFACE_BLOCK_COLOR_BLACK_RED 0x0C    //黑底红色


//空地形状及其颜色
#define INTERFACE_SPACE_FIGURE "  "
#define INTERFACE_SPACE_COLOR_TYPE_A 0xFF    //空格纯白底纯白
#define INTERFACE_SPACE_COLOR_TYPE_B 0x88    //空格灰色底灰色

//固化物颜色（固定的方块）
#define INTERFACE_SOLID_BLOCK_COLOR 0x13

//墙形状及其颜色
#define INTERFACE_WALL_FIGURE "■"
#define INTERFACE_WALL_COLOR 0x02    //黑底浅绿

//窗口在屏幕上的坐标，同时也用于窗口缓冲区大小、窗口大小（除以10）
#define INTERFACE_WINDOW_COORD_X0_BASIC 0
#define INTERFACE_WINDOW_COORD_Y0_BASIC 0
#define INTERFACE_WINDOW_COORD_X1_BASIC 800
#define INTERFACE_WINDOW_COORD_Y1_BASIC 270

#define INTERFACE_WINDOW_COORD_X1_WIDE 1200

#define TETRIS_PLAY_SPACE_X 14    //12 + 2（墙） 列
#define TETRIS_PLAY_SPACE_Y 23    //23行

#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y 6    //给高度为6的自定义方块空间
#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_X TETRIS_PLAY_SPACE_X    //自定义方块空间宽度

//玩家solo俄罗斯方块阵列打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y 2

//玩家solo俄罗斯方块NextBlock打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//自定义俄罗斯方块阵列打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y 2

//自定义俄罗斯方块说明打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_X \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X + 16
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_Y 2

//自定义俄罗斯方块方块出现频率打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 3
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y + 8

//自定义俄罗斯方块方块出现具体频率打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_X \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X + 9
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_Y \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y

//const变量是C++语法
/*与常量字符数组szCustomizeBlocksIntroColor是一一对应的关系*/
const char szCustomizeBlocksIntro[16][MAX_STRING_LENGTH] = {
  "按键选色后，鼠标左键操作：",/*[0]*/
  "",/*[1]*/
  "【A】：青色",/*[2]*/
  "【S】：蓝色",/*[3]*/
  "【D】：暗黄色",/*[4]*/
  "【F】：黄色",/*[5]*/
  "【G】：暗紫色",/*[6]*/
  "【H】：紫色",/*[7]*/
  "【J】：红色",/*[8]*/
  "【E】：擦除",/*[9]*/
  "",/*[10]*/
  "鼠标滚轮：调节出现频率",/*[11]*/
  "",/*[12]*/
  "【Q】：保存",/*[13]*/
  "",/*[14]*/
  "【ESC】：回主界面"/*[15]*/
};

//const变量是C++语法
/*与常量字符串数组szCustomizeBlocksIntro是一一对应的关系*/
const unsigned short szCustomizeBlocksIntroColor[16] = {
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[0]*//*黑底白字*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[1]*/
  INTERFACE_BLOCK_COLOR_BLACK_CYAN,/*[2]*//*黑底青色*/
  INTERFACE_BLOCK_COLOR_BLACK_BLUE,/*[3]*//*黑底蓝色*/
  INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW,/*[4]*//*黑底暗黄色*/
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[5]*//*黑底黄色*/
  INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE,/*[6]*//*黑底暗紫色*/
  INTERFACE_BLOCK_COLOR_BLACK_PURPLE,/*[7]*//*黑底紫色*/
  INTERFACE_BLOCK_COLOR_BLACK_RED,/*[8]*//*黑底红色*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[9]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[10]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[11]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[12]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[13]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[14]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[15]*/
};

#define INTERFACE_CUSTOMIZE_BLOCKS_FREQUENCY_STRING "方块出现频率："

//const变量是C++语法
/*与常量字符串数组szCustomizeBlocksFrequencyColor是一一对应的关系*/
const char szCustomizeBlocksFrequency[6][MAX_STRING_LENGTH] = {
  "缺省",/*[0]*/
  "极低",/*[1]*/
  "低  ",/*[2]*/
  "中等",/*[3]*/
  "高  ",/*[4]*/
  "极高",/*[5]*/
};

//const变量是C++语法
/*与常量字符串数组szCustomizeBlocksFrequency是一一对应的关系*/
const unsigned short szCustomizeBlocksFrequencyColor[6] = {
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[0]*//*黑底白字*/
  INTERFACE_BLOCK_COLOR_BLACK_BLUE,/*[1]*//*黑底蓝色*/
  INTERFACE_BLOCK_COLOR_BLACK_CYAN,/*[2]*//*黑底青色*/
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[3]*//*黑底黄色*/
  INTERFACE_BLOCK_COLOR_BLACK_PURPLE,/*[4]*//*黑底紫色*/
  INTERFACE_BLOCK_COLOR_BLACK_RED,/*[5]*//*黑底红色*/
};