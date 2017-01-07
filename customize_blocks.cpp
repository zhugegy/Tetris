//此文件负责自定义方块相关操作

#include "stdafx.h"
#include "customize_blocks.h"

#include "data_processor.h"
#include "interface.h"
#include "chain_list_processor.h"
#include "file_operator.h"

static int handle_keyboard_input__customize_blocks(KEY_EVENT_RECORD kerTmp,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char *pchCurrentChoice, int *pnControlFlag);
static int handle_mouse_click_input__customize_blocks(MOUSE_EVENT_RECORD merTmp, 
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char chCurrentChoice);
static int edit_blocks__customize_blocks(Param *pstParam,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int refresh_tetris_space__customize_blocks(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);
static int print_block_frequency__customize_interface(int nFrequency);
static int handle_mouse_wheel_input__customize_blocks(MOUSE_EVENT_RECORD merTmp,
                                                      int *pnFrequency);
static int save_customize_info__customize_interface(
  Param *pstParam, int nFrequency,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X]);

int customize_blocks(Param *pstParam)
{
  //打印说明
  print_string_array__interface(
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_X * 2,
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_Y,
    (char (*)[MAX_STRING_LENGTH]) szCustomizeBlocksIntro, 
    _countof(szCustomizeBlocksIntro),
    (unsigned short *) szCustomizeBlocksIntroColor);

  //打印“方块出现频率”这一行字
  print_element__interface(
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X * 2,
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y, 
    INTERFACE_CUSTOMIZE_BLOCKS_FREQUENCY_STRING, 
    INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //打印当前频率（中等）
  print_block_frequency__customize_interface(
    CUSTOMIZED_BLOCK_FREQUENCY_NORMAL);
  
  edit_blocks__customize_blocks(pstParam, pstParam->TetrisCustomizeBlocksSpace);

  return 0;
}

static int edit_blocks__customize_blocks(Param *pstParam,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //标记当前的用户选择（通过键盘按键来设置）
  unsigned char chCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_CYAN;
  //程序控制，被置为0的时候跳出编辑方块的循环
  int nControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING;    
  //记录方块出现频率
  int nFrequency = CUSTOMIZED_BLOCK_FREQUENCY_NORMAL;
  unsigned int k = 0;    //用户输入(事件)计数

  refresh_tetris_space__customize_blocks(chTetris);

  //获取输出句柄
  //HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

  //定义接收输入事件的数组和实际接收到的量
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  //设置允许鼠标输入
  SetConsoleMode(hStdIn, ENABLE_MOUSE_INPUT);

  while (nControlFlag != CONTROL_FLAG_CUSTOMIZE_BLOCKS_QUIT)
  {

    //代表用户保存了方块信息
    if (nControlFlag == CONTROL_FLAG_CUSTOMIZE_BLOCKS_SAVE)    
    {
      //处理用户自定义的方块，并保存到文件
      save_customize_info__customize_interface(
        pstParam, nFrequency, chTetris);

      //重置方块画图板
      refresh_tetris_space__customize_blocks(chTetris);
      nFrequency = CUSTOMIZED_BLOCK_FREQUENCY_NORMAL;
      //打印当前频率（中等）
      print_block_frequency__customize_interface(
        CUSTOMIZED_BLOCK_FREQUENCY_NORMAL);

      nControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING;
    }

    //死循环来获取鼠标和键盘事件，没有新的输入事件就继续死循环
    if (!ReadConsoleInput(hStdIn, irBuf, 128, &nNumRead))
    {
      continue;
    }

    //遍历事件
    for (k = 0; k < nNumRead; k++)
    {
      //根据事件种类（键盘按键、鼠标按键）决定处理方式
      switch (irBuf[k].EventType)
      {
      case KEY_EVENT:
        if (irBuf[k].Event.KeyEvent.bKeyDown == 1)
        {
          handle_keyboard_input__customize_blocks(irBuf[k].Event.KeyEvent,
            chTetris, &chCurrentChoice, &nControlFlag);
        }
        break;

      case MOUSE_EVENT:
        //左键按下的情况(画图)
        if (irBuf[k].Event.MouseEvent.dwButtonState == 
          FROM_LEFT_1ST_BUTTON_PRESSED)
        {
          handle_mouse_click_input__customize_blocks(irBuf[k].Event.MouseEvent,
            chTetris, chCurrentChoice);
        }
        else if (irBuf[k].Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
        {
          handle_mouse_wheel_input__customize_blocks(irBuf[k].Event.MouseEvent,
            &nFrequency);
        }
        break;

      default:
        break;
      }
    }

  }

  pstParam->eStageFlag = STAGE_MAIN_MENU;

  return 0;
}

static int save_customize_info__customize_interface(
  Param *pstParam, int nFrequency,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //待会要构建的俄罗斯方块结构体
  CustomizedBlock stBlockToBeSaved;
  //用于计算自定义方块中心点的临时数组
  unsigned char chTetrisCustomizeBlocksSpaceTmp[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X] = {0};
  ElementCoord pstCenterPoint;

  //1.拷贝序列号到结构体
  //==没有错误检查，错误情况返回-1，先不弄了
  stBlockToBeSaved.nSerial = 
    find_avaiable_serial__chain_list_processor(pstParam);

  //2.3.拷贝中心点坐标到结构体
  //向左上方优化方块
  optimize_tetris_customize_blocks_space__data_processor(chTetris);
  //拷贝当前自定义方块信息到临时数组，用于计算中心点
  memcpy(chTetrisCustomizeBlocksSpaceTmp, chTetris, 
    sizeof(chTetrisCustomizeBlocksSpaceTmp) * sizeof(char));    //应该是_countof,这里有错，不过先不改，没影响
  //计算中心点：给每个方块赋值权重
  align_weight_to_customized_block_element__data_processor(
    chTetrisCustomizeBlocksSpaceTmp);
  //计算中心点：比较权重，得到中心点（权重最小的）
  get_customized_block_center_point__data_processor(&pstCenterPoint,
    chTetrisCustomizeBlocksSpaceTmp);
  //给结构体赋值
  stBlockToBeSaved.nCenterX = pstCenterPoint.nX;
  stBlockToBeSaved.nCenterY = pstCenterPoint.nY;

  //4.拷贝出现频率到结构体
  stBlockToBeSaved.nFrequency = nFrequency;

  //5.拷贝数组原始数据（方块颜色和坐标）到结构体
  memcpy(stBlockToBeSaved.TetrisCustomizeBlocksSpace, chTetris,
    sizeof(stBlockToBeSaved.TetrisCustomizeBlocksSpace) * sizeof(char));    //应该是_countof,这里有错，不过先不改，没影响

  //把结构体存入文件
  write_customized_block_into_file__file_operator(&stBlockToBeSaved);

  //把结构体存入（插入）链表
  file_operator_insert_block_node_into_chain(pstParam, &stBlockToBeSaved);

  return 0;
}

static int print_block_frequency__customize_interface(int nFrequency)
{
  print_element__interface(
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_X * 2,
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_Y,
    szCustomizeBlocksFrequency[nFrequency],
    szCustomizeBlocksFrequencyColor[nFrequency]);

  return 0;
}

static int refresh_tetris_space__customize_blocks(
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //把俄罗斯方块方阵置为开始状态
  set_tetris_customize_blocks_space_to_start__data_processor(chTetris);
  //optimize_tetris_customize_blocks_space__data_processor(chTetris);
  //打印俄罗斯方块方阵（小型）
  print_tetris_customize_blocks_ground__interface(
    chTetris, 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y);

  return 0;
}

//键盘输入处理
static int handle_keyboard_input__customize_blocks(KEY_EVENT_RECORD kerTmp,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char *pchCurrentChoice, int *pnControlFlag)
{
  switch (kerTmp.wVirtualKeyCode)
  {
  /*case 'a':*/
  case 'A':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_CYAN;
    break;
  case 'S':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_BLUE;
    break;
  case 'D':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW;
    break;
  case 'F':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_YELLOW;
    break;
  case 'G':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE;
    break;
  case 'H':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_PURPLE;
    break;
  case 'J':
    *pchCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_RED;
    break;
  case 'E':
    *pchCurrentChoice = INTERFACE_SPACE_COLOR_TYPE_A;
    break;
    //==1 debug
  case '1':
  case VK_NUMPAD1:
    optimize_tetris_customize_blocks_space__data_processor(chTetris);
    break;
  case 'Q':
    //通知上层函数保存当前方块信息
    *pnControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_SAVE;
    break;
  case VK_ESCAPE:
    *pnControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_QUIT;
    break;
  default:
    break;
  }
  return 0;
}

//处理鼠标滚轮事件
static int handle_mouse_wheel_input__customize_blocks(MOUSE_EVENT_RECORD merTmp,
                                                      int *pnFrequency)
{
  //鼠标滚轮前滚，增加频率
  if ((int) merTmp.dwButtonState > 0)
  {
    if (*pnFrequency < CUSTOMIZED_BLOCK_FREQUENCY_VERY_HIGH)
    {
      (*pnFrequency)++;
      print_block_frequency__customize_interface(*pnFrequency);
    }
  }

  //鼠标滚轮后滚，减少频率
  if ((int) merTmp.dwButtonState < 0)
  {
    if (*pnFrequency > CUSTOMIZED_BLOCK_FREQUENCY_VERY_LOW)
    {
      (*pnFrequency)--;
      print_block_frequency__customize_interface(*pnFrequency);
    }
  }

  return 0;
}

static int handle_mouse_click_input__customize_blocks(MOUSE_EVENT_RECORD merTmp, 
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X], 
  unsigned char chCurrentChoice)
{
  //边界判断，如果鼠标坐标不在许可范围内，忽略这次点击
  if (
    merTmp.dwMousePosition.X < 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2 + 2/*左侧墙*/
    ||
    merTmp.dwMousePosition.Y < 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y 
    ||
    merTmp.dwMousePosition.X >=
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2 + 2/*左侧墙*/ +
    (TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 2) * 2/*中间的空白总长*/
    ||
    merTmp.dwMousePosition.Y >=
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y +
    TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y
    )
  {
    return 0;
  }

  //横坐标奇数处理
  if (merTmp.dwMousePosition.X % 2 != 0)
  {
    merTmp.dwMousePosition.X -= 1;
  }

  //根据键盘选择不同进行不同的操作
  switch (chCurrentChoice)
  {
  //如果是打印方块block:
  case INTERFACE_BLOCK_COLOR_BLACK_CYAN:
  case INTERFACE_BLOCK_COLOR_BLACK_BLUE:
  case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW:
  case INTERFACE_BLOCK_COLOR_BLACK_YELLOW:
  case INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE:
  case INTERFACE_BLOCK_COLOR_BLACK_PURPLE:
  case INTERFACE_BLOCK_COLOR_BLACK_RED:
      print_block__interface(merTmp.dwMousePosition, chCurrentChoice);
      change_tetris_customize_blocks_space__data_processor(chTetris, 
        chCurrentChoice, 
        (merTmp.dwMousePosition.X - 
        INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2) / 2, 
        merTmp.dwMousePosition.Y - 
        INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y);
    break;
  //如果是擦除（也就是打印空白space）：
  case INTERFACE_SPACE_COLOR_TYPE_A:
    print_space__interface(merTmp.dwMousePosition, chCurrentChoice);
    change_tetris_customize_blocks_space__data_processor(chTetris, 
      SPACE_VALUE_TYPE_A, 
      (merTmp.dwMousePosition.X - 
      INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2) / 2, 
      merTmp.dwMousePosition.Y - 
      INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y);
    break;
  //如果用户进入此界面，但是什么都没选就用鼠标点：
  case 0:
    //什么也不做
    break;
  default:
    printf("ERROR:handle_mouse_input__customize_blocks DEBUG");
    break;
  }

  return 0;
}