//���ļ������Զ��巽����ز���

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
  //��ӡ˵��
  print_string_array__interface(
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_X * 2,
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_Y,
    (char (*)[MAX_STRING_LENGTH]) szCustomizeBlocksIntro, 
    _countof(szCustomizeBlocksIntro),
    (unsigned short *) szCustomizeBlocksIntroColor);

  //��ӡ���������Ƶ�ʡ���һ����
  print_element__interface(
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X * 2,
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y, 
    INTERFACE_CUSTOMIZE_BLOCKS_FREQUENCY_STRING, 
    INTERFACE_BLOCK_COLOR_BLACK_WHITE);

  //��ӡ��ǰƵ�ʣ��еȣ�
  print_block_frequency__customize_interface(
    CUSTOMIZED_BLOCK_FREQUENCY_NORMAL);
  
  edit_blocks__customize_blocks(pstParam, pstParam->TetrisCustomizeBlocksSpace);

  return 0;
}

static int edit_blocks__customize_blocks(Param *pstParam,
  unsigned char chTetris[][TETRIS_CUSTOMIZE_BLOCKS_SPACE_X])
{
  //��ǵ�ǰ���û�ѡ��ͨ�����̰��������ã�
  unsigned char chCurrentChoice = INTERFACE_BLOCK_COLOR_BLACK_CYAN;
  //������ƣ�����Ϊ0��ʱ�������༭�����ѭ��
  int nControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING;    
  //��¼�������Ƶ��
  int nFrequency = CUSTOMIZED_BLOCK_FREQUENCY_NORMAL;
  unsigned int k = 0;    //�û�����(�¼�)����

  refresh_tetris_space__customize_blocks(chTetris);

  //��ȡ������
  //HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

  //������������¼��������ʵ�ʽ��յ�����
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  //���������������
  SetConsoleMode(hStdIn, ENABLE_MOUSE_INPUT);

  while (nControlFlag != CONTROL_FLAG_CUSTOMIZE_BLOCKS_QUIT)
  {

    //�����û������˷�����Ϣ
    if (nControlFlag == CONTROL_FLAG_CUSTOMIZE_BLOCKS_SAVE)    
    {
      //�����û��Զ���ķ��飬�����浽�ļ�
      save_customize_info__customize_interface(
        pstParam, nFrequency, chTetris);

      //���÷��黭ͼ��
      refresh_tetris_space__customize_blocks(chTetris);
      nFrequency = CUSTOMIZED_BLOCK_FREQUENCY_NORMAL;
      //��ӡ��ǰƵ�ʣ��еȣ�
      print_block_frequency__customize_interface(
        CUSTOMIZED_BLOCK_FREQUENCY_NORMAL);

      nControlFlag = CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING;
    }

    //��ѭ������ȡ���ͼ����¼���û���µ������¼��ͼ�����ѭ��
    if (!ReadConsoleInput(hStdIn, irBuf, 128, &nNumRead))
    {
      continue;
    }

    //�����¼�
    for (k = 0; k < nNumRead; k++)
    {
      //�����¼����ࣨ���̰�������갴������������ʽ
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
        //������µ����(��ͼ)
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
  //����Ҫ�����Ķ���˹����ṹ��
  CustomizedBlock stBlockToBeSaved;
  //���ڼ����Զ��巽�����ĵ����ʱ����
  unsigned char chTetrisCustomizeBlocksSpaceTmp[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X] = {0};
  ElementCoord pstCenterPoint;

  //1.�������кŵ��ṹ��
  //==û�д����飬�����������-1���Ȳ�Ū��
  stBlockToBeSaved.nSerial = 
    find_avaiable_serial__chain_list_processor(pstParam);

  //2.3.�������ĵ����굽�ṹ��
  //�����Ϸ��Ż�����
  optimize_tetris_customize_blocks_space__data_processor(chTetris);
  //������ǰ�Զ��巽����Ϣ����ʱ���飬���ڼ������ĵ�
  memcpy(chTetrisCustomizeBlocksSpaceTmp, chTetris, 
    sizeof(chTetrisCustomizeBlocksSpaceTmp) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��
  //�������ĵ㣺��ÿ�����鸳ֵȨ��
  align_weight_to_customized_block_element__data_processor(
    chTetrisCustomizeBlocksSpaceTmp);
  //�������ĵ㣺�Ƚ�Ȩ�أ��õ����ĵ㣨Ȩ����С�ģ�
  get_customized_block_center_point__data_processor(&pstCenterPoint,
    chTetrisCustomizeBlocksSpaceTmp);
  //���ṹ�帳ֵ
  stBlockToBeSaved.nCenterX = pstCenterPoint.nX;
  stBlockToBeSaved.nCenterY = pstCenterPoint.nY;

  //4.��������Ƶ�ʵ��ṹ��
  stBlockToBeSaved.nFrequency = nFrequency;

  //5.��������ԭʼ���ݣ�������ɫ�����꣩���ṹ��
  memcpy(stBlockToBeSaved.TetrisCustomizeBlocksSpace, chTetris,
    sizeof(stBlockToBeSaved.TetrisCustomizeBlocksSpace) * sizeof(char));    //Ӧ����_countof,�����д������Ȳ��ģ�ûӰ��

  //�ѽṹ������ļ�
  write_customized_block_into_file__file_operator(&stBlockToBeSaved);

  //�ѽṹ����루���룩����
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
  //�Ѷ���˹���鷽����Ϊ��ʼ״̬
  set_tetris_customize_blocks_space_to_start__data_processor(chTetris);
  //optimize_tetris_customize_blocks_space__data_processor(chTetris);
  //��ӡ����˹���鷽��С�ͣ�
  print_tetris_customize_blocks_ground__interface(
    chTetris, 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2, 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y);

  return 0;
}

//�������봦��
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
    //֪ͨ�ϲ㺯�����浱ǰ������Ϣ
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

//�����������¼�
static int handle_mouse_wheel_input__customize_blocks(MOUSE_EVENT_RECORD merTmp,
                                                      int *pnFrequency)
{
  //������ǰ��������Ƶ��
  if ((int) merTmp.dwButtonState > 0)
  {
    if (*pnFrequency < CUSTOMIZED_BLOCK_FREQUENCY_VERY_HIGH)
    {
      (*pnFrequency)++;
      print_block_frequency__customize_interface(*pnFrequency);
    }
  }

  //�����ֺ��������Ƶ��
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
  //�߽��жϣ����������겻����ɷ�Χ�ڣ�������ε��
  if (
    merTmp.dwMousePosition.X < 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2 + 2/*���ǽ*/
    ||
    merTmp.dwMousePosition.Y < 
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y 
    ||
    merTmp.dwMousePosition.X >=
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2 + 2/*���ǽ*/ +
    (TETRIS_CUSTOMIZE_BLOCKS_SPACE_X - 2) * 2/*�м�Ŀհ��ܳ�*/
    ||
    merTmp.dwMousePosition.Y >=
    INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y +
    TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y
    )
  {
    return 0;
  }

  //��������������
  if (merTmp.dwMousePosition.X % 2 != 0)
  {
    merTmp.dwMousePosition.X -= 1;
  }

  //���ݼ���ѡ��ͬ���в�ͬ�Ĳ���
  switch (chCurrentChoice)
  {
  //����Ǵ�ӡ����block:
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
  //����ǲ�����Ҳ���Ǵ�ӡ�հ�space����
  case INTERFACE_SPACE_COLOR_TYPE_A:
    print_space__interface(merTmp.dwMousePosition, chCurrentChoice);
    change_tetris_customize_blocks_space__data_processor(chTetris, 
      SPACE_VALUE_TYPE_A, 
      (merTmp.dwMousePosition.X - 
      INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X * 2) / 2, 
      merTmp.dwMousePosition.Y - 
      INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y);
    break;
  //����û�����˽��棬����ʲô��ûѡ�������㣺
  case 0:
    //ʲôҲ����
    break;
  default:
    printf("ERROR:handle_mouse_input__customize_blocks DEBUG");
    break;
  }

  return 0;
}