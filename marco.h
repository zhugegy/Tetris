#pragma once
//���ļ���¼�����еĺꡣ

/*�������˹�������Ͷ�Ӧ����ֵ�����ޣ����51�����80�����������30����ͬ�ķ�����
��*/
/*
#define BLOCK_VALUE_MIN 51
#define BLOCK_VALUE_MAX 80*/

#define FILE_LOCATION_LINK_INFO "C:\\blockinfo.txt"

#define WALL_VALUE 100    //ǽ����ֵ
#define SOLID_BLOCK_VALUE 110    //�̻��ķ�����ֵ

//Ϊ������Ҹ��õ�ʶ������ֱ�ߣ�����ֱ��֮����ɫ���в����������Խλ��ƺ��
#define SPACE_VALUE_TYPE_A 52    //�ո���ֵ:TYPE_A
/*#define SPACE_VALUE_TYPE_B 51    //�ո���ֵ��TYPE_B*/
#define SPACE_VALUE_TYPE_B 52

//�жϿո�����ͣ�X������������TYPE_A,ż������TYPE_B
#define SPACE_TYPE(x) x % 2 == 0 ? SPACE_VALUE_TYPE_B : SPACE_VALUE_TYPE_A 



#define MAX_STRING_LENGTH 1024    //�ַ�����󳤶ȣ������ڳ�ʼ����ʱ�ַ�������

//������ز���
#define PROGRAM_TITLE "����˹����"
#define PROGRAM_VERSION "Version 0.01"

//��������������Ĭ�ϵĶ���˹������7�֣�
#define MAX_BLOCK_NUM 100

//control flag ��ʵ������enum
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_RUNNING 1
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_QUIT 0
#define CONTROL_FLAG_CUSTOMIZE_BLOCKS_SAVE 99

#define CONTROL_FLAG_MAIN_LOOP_RUNNING 1
#define CONTROL_FLAG_MAIN_LOOP_GAME_OVER_PLAYER 20
#define CONTROL_FLAG_MAIN_LOOP_GAME_OVER_COM 21
//#define CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER 11 

//������ɫ
#define INTERFACE_BASIC_BACKGROUND "30"    //���ף����֣�
#define INTERFACE_PLAY_BACKGROUND "0F"    //�ڵ�(����)

//������״������ɫ����ɫҲͬʱӦ���������ӡ��ͬʱҲ��Ϊ�������ֵ���ڴ洢��
#define INTERFACE_BLOCK_FIGURE "��"
#define INTERFACE_BLOCK_COLOR_BLACK_WHITE 0x0F    //�ڵװ�ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_CYAN 0x0B    //�ڵ���ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_BLUE 0x09    //�ڵ���ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW 0x06    //�ڵװ���ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_YELLOW 0x0E    //�ڵ׻�ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE 0x05    //�ڵװ���ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_PURPLE 0x0D    //�ڵ���ɫ
#define INTERFACE_BLOCK_COLOR_BLACK_RED 0x0C    //�ڵ׺�ɫ


//�յ���״������ɫ
#define INTERFACE_SPACE_FIGURE "  "
#define INTERFACE_SPACE_COLOR_TYPE_A 0xFF    //�ո񴿰׵״���
#define INTERFACE_SPACE_COLOR_TYPE_B 0x88    //�ո��ɫ�׻�ɫ

//�̻�����ɫ���̶��ķ��飩
#define INTERFACE_SOLID_BLOCK_COLOR 0x13

//ǽ��״������ɫ
#define INTERFACE_WALL_FIGURE "��"
#define INTERFACE_WALL_COLOR 0x02    //�ڵ�ǳ��

//��������Ļ�ϵ����꣬ͬʱҲ���ڴ��ڻ�������С�����ڴ�С������10��
#define INTERFACE_WINDOW_COORD_X0_BASIC 0
#define INTERFACE_WINDOW_COORD_Y0_BASIC 0
#define INTERFACE_WINDOW_COORD_X1_BASIC 800
//270 maxever 470 �޸Ĵ���ֵ���ڵ��� ������ͬʱ�޸� TETRIS_PLAY_SPACE_Y
#define INTERFACE_WINDOW_COORD_Y1_BASIC 370 

#define INTERFACE_WINDOW_COORD_X1_WIDE 1320

#define TETRIS_PLAY_SPACE_X 14    //12 + 2��ǽ�� ��
//23��  maxever 43 �޸Ĵ���ֵ���ڵ��� ������ͬʱ�޸� INTERFACE_WINDOW_COORD_Y1_BASIC
#define TETRIS_PLAY_SPACE_Y 33    

#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y 6    //���߶�Ϊ6���Զ��巽��ռ�
#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_X TETRIS_PLAY_SPACE_X    //�Զ��巽��ռ���

//���solo����˹�������д�ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y 2

//���solo����˹����NextBlock��ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//���VS���ԣ����ԵĶ���˹���������д�ӡê�������꣨���Ͻ�)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 32)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//���VS���ԣ�����˹����NextBlock��ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y

//�Զ������˹�������д�ӡê�������꣨���Ͻǣ�
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y 2

//�Զ������˹����˵����ӡê�������꣨���Ͻǣ�
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_X \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_INTRO_Y 2

//�Զ������˹���鷽�����Ƶ�ʴ�ӡê�������꣨���Ͻǣ�
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 3)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_TETRIS_SPACE_Y + 8)

//�Զ������˹���鷽����־���Ƶ�ʴ�ӡê�������꣨���Ͻǣ�
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_X \
  (INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_X + 9)
#define INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_EXACT_FREQUENCY_Y \
  INTERFACE_CUSTOMIZE_BLOCKS_ANCHOR_POINT_BLOCK_FREQUENCY_Y

//const������C++�﷨
/*�볣���ַ�����szCustomizeBlocksIntroColor��һһ��Ӧ�Ĺ�ϵ*/
const char szCustomizeBlocksIntro[16][MAX_STRING_LENGTH] = {
  "����ѡɫ��������������",/*[0]*/
  "",/*[1]*/
  "��A������ɫ",/*[2]*/
  "��S������ɫ",/*[3]*/
  "��D��������ɫ",/*[4]*/
  "��F������ɫ",/*[5]*/
  "��G��������ɫ",/*[6]*/
  "��H������ɫ",/*[7]*/
  "��J������ɫ",/*[8]*/
  "��E��������",/*[9]*/
  "",/*[10]*/
  "�����֣����ڳ���Ƶ��",/*[11]*/
  "",/*[12]*/
  "��Q��������",/*[13]*/
  "",/*[14]*/
  "��ESC������������"/*[15]*/
};

//const������C++�﷨
/*�볣���ַ�������szCustomizeBlocksIntro��һһ��Ӧ�Ĺ�ϵ*/
const unsigned short szCustomizeBlocksIntroColor[16] = {
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[0]*//*�ڵװ���*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[1]*/
  INTERFACE_BLOCK_COLOR_BLACK_CYAN,/*[2]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_BLUE,/*[3]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_LIGHT_YELLOW,/*[4]*//*�ڵװ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[5]*//*�ڵ׻�ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_LIGHT_PURPLE,/*[6]*//*�ڵװ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_PURPLE,/*[7]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_RED,/*[8]*//*�ڵ׺�ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[9]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[10]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[11]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[12]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[13]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[14]*/
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[15]*/
};

#define INTERFACE_CUSTOMIZE_BLOCKS_FREQUENCY_STRING "�������Ƶ�ʣ�"

//const������C++�﷨
/*�볣���ַ�������szCustomizeBlocksFrequencyColor��һһ��Ӧ�Ĺ�ϵ*/
const char szCustomizeBlocksFrequency[6][MAX_STRING_LENGTH] = {
  "ȱʡ",/*[0]*/
  "����",/*[1]*/
  "��  ",/*[2]*/
  "�е�",/*[3]*/
  "��  ",/*[4]*/
  "����",/*[5]*/
};

//const������C++�﷨
/*�볣���ַ�������szCustomizeBlocksFrequency��һһ��Ӧ�Ĺ�ϵ*/
const unsigned short szCustomizeBlocksFrequencyColor[6] = {
  INTERFACE_BLOCK_COLOR_BLACK_WHITE,/*[0]*//*�ڵװ���*/
  INTERFACE_BLOCK_COLOR_BLACK_BLUE,/*[1]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_CYAN,/*[2]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[3]*//*�ڵ׻�ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_PURPLE,/*[4]*//*�ڵ���ɫ*/
  INTERFACE_BLOCK_COLOR_BLACK_RED,/*[5]*//*�ڵ׺�ɫ*/
};

#define MAX_COM_SPEED_LIST_NUM 11

//const������C++�﷨
/*���Բ����ٶȣ��Ѷȣ���10��*/
const int nCOMSpeed[MAX_COM_SPEED_LIST_NUM] = {
  500,  //0ȱʡ
  500,  //1
  300,  //2
  200,  //3
  100,  //4
  50,   //5
  20,   //6
  10,   //7
  5,    //8
  2,    //9
  1     //10
};