#pragma once
//本文件记录了所有的宏。

/*定义俄罗斯方块类型对应的数值上下限，最低51，最高80，代表可以有30个不同的方块类
型*/
/*
#define BLOCK_VALUE_MIN 51
#define BLOCK_VALUE_MAX 80*/

#define FILE_LOCATION_LINK_INFO "game_data//block_info.txt"
#define FILE_LOCATION_LINK_INFO_BACKUP_FILE "game_data//backup//block_info_backup.txt"

#define WALL_VALUE 100    //墙的数值
#define SOLID_BLOCK_VALUE 110    //固化的方块数值

//为了让玩家更好地识别纵向直线，相邻直线之间颜色略有差别（类似足球越位草坪）
//20161231补充备注之前的改变：足球越位草坪的设计已经被取消了。现在空地一律是52。
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
#define CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER 20
#define CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM 21

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
#define INTERFACE_BLOCK_COLOR_CYAN_BLACK 0x30    //青底黑色
#define INTERFACE_BLOCK_COLOR_BLUE_BLACK 0x90    //蓝底黑色
#define INTERFACE_BLOCK_COLOR_WHITE_BLACK 0xF0    //白底黑色
#define INTERFACE_BLOCK_COLOR_CYAN_WHITE 0x3F    //青底黑色
#define INTERFACE_BLOCK_COLOR_CYAN_YELLOW 0x3E    //青底黄色



//空地形状及其颜色
#define INTERFACE_SPACE_FIGURE "  "
#define INTERFACE_SPACE_COLOR_TYPE_A 0xFF    //空格纯白底纯白
#define INTERFACE_SPACE_COLOR_TYPE_B 0x88    //空格灰色底灰色

//固化物颜色（固定的方块）
#define INTERFACE_SOLID_BLOCK_COLOR 0x13  //一种很好看的蓝色

//墙形状及其颜色
#define INTERFACE_WALL_FIGURE "■"
#define INTERFACE_WALL_COLOR 0x02    //黑底浅绿

//窗口在屏幕上的坐标，同时也用于窗口缓冲区大小、窗口大小（除以10）
#define INTERFACE_WINDOW_COORD_X0_BASIC 0
#define INTERFACE_WINDOW_COORD_Y0_BASIC 0
//#define INTERFACE_WINDOW_COORD_X1_BASIC 800
#define INTERFACE_WINDOW_COORD_X1_BASIC 1320    //暂时这么用着
//270 maxever 470 修改此数值用于调试 别忘了同时修改 TETRIS_PLAY_SPACE_Y
#define INTERFACE_WINDOW_COORD_Y1_BASIC 330 

//WIDE用于宽窗口（人机对战，区别于单人游戏时的BASIC）
#define INTERFACE_WINDOW_COORD_X1_WIDE 1320

#define TETRIS_PLAY_SPACE_X 14    //12 + 2（墙） 列
//23行  maxever 43 修改此数值用于调试 别忘了同时修改 INTERFACE_WINDOW_COORD_Y1_BASIC
#define TETRIS_PLAY_SPACE_Y 29    

#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y 6    //给高度为6的自定义方块空间
#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_X TETRIS_PLAY_SPACE_X    //自定义方块空间宽度

//菜单打印锚定点坐标（左上角）
#define INTERFACE_MENU_ANCHOR_POINT_X 22    //12
#define INTERFACE_MENU_ANCHOR_POINT_Y 4

//菜单指示器锚定点坐标（左上角）
#define INTERFACE_MENU_POINTER_ANCHOR_POINT_X \
  (INTERFACE_MENU_ANCHOR_POINT_X + 2)
#define INTERFACE_MENU_POINTER_ANCHOR_POINT_Y \
  (INTERFACE_MENU_ANCHOR_POINT_Y + 3)

//玩家solo俄罗斯方块阵列打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y 2

//玩家solo俄罗斯方块NextBlock打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//玩家solo得分信息打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y + 10)

//玩家solo具体得分打印锚定点坐标（左上角）
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_EXACT_SCORING_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X + 7)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_EXACT_SCORING_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y

//玩家VS电脑，电脑的俄罗斯方块阵列列打印锚定点坐标（左上角)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 32)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//玩家VS电脑，俄罗斯方块NextBlock打印锚定点坐标（左上角）
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y

//玩家VS电脑，得分信息打印锚定点坐标（左上角）
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y + 10)

//玩家VS电脑，具体得分打印锚定点坐标（左上角）
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_EXACT_SCORING_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X + 7)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_EXACT_SCORING_Y \
  INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y

//自定义俄罗斯方块阵列打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y 2

//自定义俄罗斯方块说明打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_X \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_Y 2

//自定义俄罗斯方块方块出现频率打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 3)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y + 8)

//自定义俄罗斯方块方块出现具体频率打印锚定点坐标（左上角）
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_X \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X + 9)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_Y \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y

//const变量是C++语法
/*与常量字符数组szMenuStringColor是一一对应的关系*/
const char szMenuString[11][MAX_STRING_LENGTH] = {
  " 俄罗斯方块(可自定义方块) ",/*[0]*/
  "",/*[1]*/
  "",/*[2]*/
  "      单人游戏",/*[3]*/
  "      人机对战",/*[4]*/
  "      自定义方块",/*[5]*/
  "      退出游戏",/*[6]*/
  "",/*[7]*/
  "",/*[8]*/
  "控制电脑速度：小键盘1和2;控制电脑是否快速下落：小键盘3。",/*[9]*/
  "游戏中按ESC键返回主菜单。",/*[10]*/
};

//菜单字符串开头的无效选项：标题，空行,尾部的说明
#define VOID_SLOT_IN_MENU 7

//const变量是C++语法
/*与常量字符串数组szMenuString是一一对应的关系*/
const unsigned short szMenuStringColor[11] = {
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[0]*//*黑底黄字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[1]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[2]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[3]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[4]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[5]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[6]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[7]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[8]*//*青底黑字*/
  INTERFACE_BLOCK_COLOR_CYAN_YELLOW,/*[9]*//*青底黄字*/
  INTERFACE_BLOCK_COLOR_CYAN_YELLOW,/*[10]*//*青底黄字*/
};

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

#define INTERFACE_SCORING_STRING "已消除行数："


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

//电脑操作速度表数值个数，现在是10级变速（第0个元素（等级0）不用）
#define MAX_COM_SPEED_LIST_NUM 11

//const变量是C++语法
/*电脑操作速度（难度），10级。数值是电脑读取指令列表的时间间隔。每次新的方块出现时，指令
列表由AI生成。*/
const int nCOMSpeed[MAX_COM_SPEED_LIST_NUM] = {
  500,  //等级0：缺省
  500,  //等级1
  300,  //等级2
  200,  //3
  100,  //4
  50,   //5
  20,   //6
  10,   //7
  5,    //8
  2,    //9
  1     //10
};

//AI策略参数：调整AI策略可以直接修改下面参数的数值
//AI是否会使用快速下落:1会，0不会
#define AI_STRATEGY_STRAIGHT_DOWN 0

//落位后下方有空格会惩罚，检测深度：默认3
#define AI_STRATEGY_PUNISHMENT_DEEPTH 3

//落位后下方有空格会惩罚，惩罚数值：默认40
#define AI_STRATEGY_PUNISHMENT_VALUE 40

//落位后，整体高度差的权重：默认5
#define AI_STRATEGY_HEIGHT_DISTANCE_FACTOR 5

//落位后，当前方块的中心点高度权重：默认5
#define AI_STRATEGY_CENTER_POINT_HEIGHT_FACTOR 5

