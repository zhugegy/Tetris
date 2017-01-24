//此文件负责主菜单逻辑

#include "stdafx.h"
#include "main_menu.h"

#include <conio.h>

#include "interface.h"

static int erase_menu_pointer__main_menu(int option);
static int print_menu_pointer__main_menu(int nOption);


//主菜单函数，功能是根据用户输入更改stMainParam的eStageFlag，从而控制程序流程
int main_menu(Param *pstParam)
{
  unsigned int k = 0;    //用户输入(事件)计数
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);    //获取输入句柄
  //定义接收输入事件的数组和实际接收到的量
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  int nControlFlag = 1;

  int nOptionCount = 0;
  int nCurrentOption = 0;

  //打印菜单
  print_string_array__interface(
    INTERFACE_MENU_ANCHOR_POINT_X * 2,
    INTERFACE_MENU_ANCHOR_POINT_Y,
    (char(*)[MAX_STRING_LENGTH]) szMenuString,
    _countof(szMenuString),
    (unsigned short *)szMenuStringColor);

  nOptionCount = _countof(szMenuString) - VOID_SLOT_IN_MENU;
  nCurrentOption = 1;

  //打印指示标记
  print_element__interface(INTERFACE_MENU_POINTER_ANCHOR_POINT_X * 2,
    INTERFACE_MENU_POINTER_ANCHOR_POINT_Y,
    INTERFACE_BLOCK_FIGURE, INTERFACE_BLOCK_COLOR_CYAN_WHITE);

  while (nControlFlag == 1)
  {
    if (_kbhit())
    {
      ReadConsoleInput(hStdIn, irBuf, 128, &nNumRead);
      //遍历事件
      for (k = 0; k < nNumRead; k++)
      {
        if (irBuf[k].Event.KeyEvent.bKeyDown == 1)
        {
          //根据按键决定处理方式
          switch (irBuf[k].Event.KeyEvent.wVirtualKeyCode)
          {
          case VK_UP:
            erase_menu_pointer__main_menu(nCurrentOption);
            nCurrentOption--;
            if (nCurrentOption < 1)
            {
              nCurrentOption = nOptionCount;
            }
            print_menu_pointer__main_menu(nCurrentOption);
            break;
          case VK_DOWN:
            erase_menu_pointer__main_menu(nCurrentOption);
            nCurrentOption++;
            if (nCurrentOption > nOptionCount)
            {
              nCurrentOption = 1;
            }
            print_menu_pointer__main_menu(nCurrentOption);
            break;
          case VK_RETURN:
          case VK_SPACE:
            //跳出while循环，不再接受键盘输入
            nControlFlag = 0;
            break;
          default:
            break;
          }
        }
      }
    }
  }

  //根据用户选项，改变flag
  switch (nCurrentOption)
  {
  case 1:
    pstParam->eStageFlag = STAGE_PLAY_SOLO;
    break;
  case 2:
    pstParam->eStageFlag = STAGE_PLAY_VS_COM;
    break;
  case 3:
    pstParam->eStageFlag = STAGE_CUSTOMIZE_BLOCKS;
    break;
  case 4:
    pstParam->eStageFlag = STAGE_QUIT;
    break;
  default:
    break;
  }

  return 0;
}

static int erase_menu_pointer__main_menu(int nOption)
{
  int i = 0;

  int nDistance = 0;

  for (i = 1; i < nOption; i++)
  {
    nDistance++;
  }

  print_element__interface(INTERFACE_MENU_POINTER_ANCHOR_POINT_X * 2,
    INTERFACE_MENU_POINTER_ANCHOR_POINT_Y + nDistance,
    INTERFACE_SPACE_FIGURE, INTERFACE_BLOCK_COLOR_CYAN_WHITE);

  return 0;
}

static int print_menu_pointer__main_menu(int nOption)
{
  int i = 0;

  int nDistance = 0;

  for (i = 1; i < nOption; i++)
  {
    nDistance++;
  }

  print_element__interface(INTERFACE_MENU_POINTER_ANCHOR_POINT_X * 2,
    INTERFACE_MENU_POINTER_ANCHOR_POINT_Y + nDistance,
    INTERFACE_BLOCK_FIGURE, INTERFACE_BLOCK_COLOR_CYAN_WHITE);

  return 0;
}