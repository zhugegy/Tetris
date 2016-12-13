//此文件负责界面相关操作

#include "stdafx.h"
#include "interface.h"

static int change_window_size__interface(int nX0, int nY0, int nX1, int nY1);

//改变界面风格：色彩，窗口大小
int change_interface_style__interface(StageList eStage)
{
  char szBuffer[MAX_STRING_LENGTH] = {0};

  //更改色彩
  strcpy_s(szBuffer, _countof(szBuffer), "color ");

  switch (eStage)
  {
  case STAGE_MAIN_MENU:
  case STAGE_HIGH_SCORES:
  case STAGE_QUIT:
    strcat_s(szBuffer, _countof(szBuffer), INTERFACE_BASIC_BACKGROUND);
    break;

  case STAGE_CUSTOMIZE_BLOCKS:
  case STAGE_PLAY_SOLO:
  case STAGE_PLAY_VS_COM:
    strcat_s(szBuffer, _countof(szBuffer), INTERFACE_PLAY_BACKGROUND);
    break;

  default:
    strcat_s(szBuffer, _countof(szBuffer), INTERFACE_BASIC_BACKGROUND);
    break;
  }
  system(szBuffer);

  //改变窗口大小（等一系列操作）
  switch (eStage)
  {
  case STAGE_MAIN_MENU:
  case STAGE_PLAY_SOLO:
  case STAGE_HIGH_SCORES:
  case STAGE_CUSTOMIZE_BLOCKS:
  case STAGE_QUIT:
    change_window_size__interface(INTERFACE_WINDOW_COORD_X0_BASIC,
                                  INTERFACE_WINDOW_COORD_Y0_BASIC,
                                  INTERFACE_WINDOW_COORD_X1_BASIC,
                                  INTERFACE_WINDOW_COORD_Y1_BASIC);
    break;
  case STAGE_PLAY_VS_COM:
    change_window_size__interface(INTERFACE_WINDOW_COORD_X0_BASIC,
      INTERFACE_WINDOW_COORD_Y0_BASIC,
      INTERFACE_WINDOW_COORD_X1_WIDE,
      INTERFACE_WINDOW_COORD_Y1_BASIC);
    break;

  default:
    change_window_size__interface(INTERFACE_WINDOW_COORD_X0_BASIC,
                                  INTERFACE_WINDOW_COORD_Y0_BASIC,
                                  INTERFACE_WINDOW_COORD_X1_BASIC,
                                  INTERFACE_WINDOW_COORD_Y1_BASIC);
    break;
  }


  return 0;
}

//改变程序标题
int change_program_title__interface()
{
  char szBuffer[MAX_STRING_LENGTH] = {0};

  strcpy_s(szBuffer, _countof(szBuffer), "title ");
  strcat_s(szBuffer, _countof(szBuffer), PROGRAM_TITLE);

  system(szBuffer);

  return 0;
}

//隐藏光标
int hide_cursor__interface()
{
  CONSOLE_CURSOR_INFO CurInfo;
  
  CurInfo.bVisible = false;
  CurInfo.dwSize = 1;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);

  return 0;
}

/*打印俄罗斯方块场地(仅用于开场的打印，游戏过程中为了防止闪屏，更改哪里就打印哪
里，不是全部刷新)*/
int print_tetris_play_ground__interface(
  unsigned char chTetris[][TETRIS_PLAY_SPACE_X], 
  int nAnchorPointX, int nAnchorPointY)
{
  COORD cPos;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;
  int i = 0;
  int j = 0;

  
  cPos.X = nAnchorPointX;
  cPos.Y = nAnchorPointY;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;


  for (i = 0; i < TETRIS_PLAY_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      switch (chTetris[i][j])
      {
      case SPACE_VALUE_TYPE_A:
        SetConsoleTextAttribute(hConsole, INTERFACE_SPACE_COLOR_TYPE_A);
        printf(INTERFACE_SPACE_FIGURE);
        break;
      /*case SPACE_VALUE_TYPE_B:
        SetConsoleTextAttribute(hConsole, INTERFACE_SPACE_COLOR_TYPE_B);
        printf(INTERFACE_SPACE_FIGURE);
        break;*/
      case WALL_VALUE:
        SetConsoleTextAttribute(hConsole, INTERFACE_WALL_COLOR);
        printf(INTERFACE_WALL_FIGURE);
        break;
      default:
        printf("error DEBUG: print_tetris_play_ground__interface");
        break;
      }
    }

    cPos.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cPos);
  }

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);
  
  return 0;
}

/*打印俄罗斯方块自定义场地*/
int print_tetris_customize_blocks_ground__interface(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  int nAnchorPointX, int nAnchorPointY)
{
  COORD cPos;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;
  int i = 0;
  int j = 0;


  cPos.X = nAnchorPointX;
  cPos.Y = nAnchorPointY;
  SetConsoleCursorPosition(hConsole,cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;


  for (i = 0; i < TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y; i++)
  {
    for (j = 0; j < TETRIS_PLAY_SPACE_X; j++)
    {
      switch (chTetris[i][j])
      {
      case SPACE_VALUE_TYPE_A:
      //case SPACE_VALUE_TYPE_B: 自定义模式只有TYPE_A类型的空地
        SetConsoleTextAttribute(hConsole, INTERFACE_SPACE_COLOR_TYPE_A);
        printf(INTERFACE_SPACE_FIGURE);
        break;
      case WALL_VALUE:
        SetConsoleTextAttribute(hConsole, INTERFACE_WALL_COLOR);
        printf(INTERFACE_WALL_FIGURE);
        break;
        /*下面的cases用于打印NextBlock信息*/
      case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
      case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
      case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
      case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
      case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
      case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
      case INTERFACE_BLOCK_COLOR_BLACK_RED:
        SetConsoleTextAttribute(hConsole, chTetris[i][j]);
        printf(INTERFACE_BLOCK_FIGURE);
        break;
      default:
        printf("error DEBUG: print_tetris_play_ground__interface");
        break;
      }
    }

    cPos.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cPos);
  }

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);

  return 0;
}

//打印元素默认：打印空格,颜色白底纯白色（这里用了C++语法）
int print_element__interface(int nCoordX, int nCoordY, 
                             const char *pSrc, int nColor)
{
  COORD cPos;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;
  int i = 0;

  cPos.X = nCoordX;
  cPos.Y = nCoordY;
  SetConsoleCursorPosition(hConsole,cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;

  SetConsoleTextAttribute(hConsole, nColor);

  printf("%s", pSrc);

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);

  return 0;
}

//打印自定义方块界面说明信息
int print_tetris_customize_intro__interface(int nAnchorPointX, 
                                            int nAnchorPointY)
{
  COORD cPos;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;
  int i = 0;

  cPos.X = nAnchorPointX;
  cPos.Y = nAnchorPointY;
  SetConsoleCursorPosition(hConsole,cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;

  for (i = 0; i < _countof(szCustomizeBlocksIntro); i++)
  {
    SetConsoleTextAttribute(hConsole, szCustomizeBlocksIntroColor[i]);
    printf(szCustomizeBlocksIntro[i]);

    cPos.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cPos);
  }

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);

  return 0;
}

//改变窗口大小（等一系列操作）
static int change_window_size__interface(int nX0, int nY0, int nX1, int nY1)
{
  HWND hWindow = GetConsoleWindow();
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiRawMap;
  SMALL_RECT srRawMap = {nX0, nY0, nX1 / 10, nY1 / 10};
  
  GetConsoleScreenBufferInfo(hStdOut, &csbiRawMap);

  //移动窗口
  MoveWindow(hWindow, nX0, nY0, nX1, nY1, TRUE);

  //设置缓冲区大小
  csbiRawMap.dwSize.X = nX1 / 10;
  csbiRawMap.dwSize.Y = nY1 / 10;
  SetConsoleScreenBufferSize(hStdOut, csbiRawMap.dwSize);

  //设置窗口大小
  //SetConsoleWindowInfo(hStdOut, true, &srRawMap);

  //最大化窗口
  ShowWindow(hWindow, SW_MAXIMIZE);

  return 0;
}

int print_block__interface(COORD cPos, unsigned char chColor)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;

  SetConsoleCursorPosition(hConsole,cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;

  SetConsoleTextAttribute(hConsole, chColor);
  printf(INTERFACE_BLOCK_FIGURE);

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);

  return 0;
}

//这个函数多余了，其实可以用print_element__interface函数代替，但是太懒，没弄
//就这样吧先，不影响使用
int print_space__interface(COORD cPos, unsigned char chColor)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
  WORD wSavedAttributes;

  SetConsoleCursorPosition(hConsole,cPos);

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &csbiConsoleInfo);
  wSavedAttributes = csbiConsoleInfo.wAttributes;

  SetConsoleTextAttribute(hConsole, chColor);
  printf(INTERFACE_SPACE_FIGURE);

  /* Restore original attributes */
  SetConsoleTextAttribute(hConsole, wSavedAttributes);

  return 0;
}