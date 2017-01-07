#pragma once
//���ļ���¼�����еĺꡣ

/*�������˹�������Ͷ�Ӧ����ֵ�����ޣ����51�����80�����������30����ͬ�ķ�����
��*/
/*
#define BLOCK_VALUE_MIN 51
#define BLOCK_VALUE_MAX 80*/

#define FILE_LOCATION_LINK_INFO "game_data//block_info.txt"
#define FILE_LOCATION_LINK_INFO_BACKUP_FILE "game_data//backup//block_info_backup.txt"

#define WALL_VALUE 100    //ǽ����ֵ
#define SOLID_BLOCK_VALUE 110    //�̻��ķ�����ֵ

//Ϊ������Ҹ��õ�ʶ������ֱ�ߣ�����ֱ��֮����ɫ���в����������Խλ��ƺ��
//20161231���䱸ע֮ǰ�ĸı䣺����Խλ��ƺ������Ѿ���ȡ���ˡ����ڿյ�һ����52��
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
#define INTERFACE_BLOCK_COLOR_CYAN_BLACK 0x30    //��׺�ɫ
#define INTERFACE_BLOCK_COLOR_BLUE_BLACK 0x90    //���׺�ɫ
#define INTERFACE_BLOCK_COLOR_WHITE_BLACK 0xF0    //�׵׺�ɫ
#define INTERFACE_BLOCK_COLOR_CYAN_WHITE 0x3F    //��׺�ɫ
#define INTERFACE_BLOCK_COLOR_CYAN_YELLOW 0x3E    //��׻�ɫ



//�յ���״������ɫ
#define INTERFACE_SPACE_FIGURE "  "
#define INTERFACE_SPACE_COLOR_TYPE_A 0xFF    //�ո񴿰׵״���
#define INTERFACE_SPACE_COLOR_TYPE_B 0x88    //�ո��ɫ�׻�ɫ

//�̻�����ɫ���̶��ķ��飩
#define INTERFACE_SOLID_BLOCK_COLOR 0x13  //һ�ֺܺÿ�����ɫ

//ǽ��״������ɫ
#define INTERFACE_WALL_FIGURE "��"
#define INTERFACE_WALL_COLOR 0x02    //�ڵ�ǳ��

//��������Ļ�ϵ����꣬ͬʱҲ���ڴ��ڻ�������С�����ڴ�С������10��
#define INTERFACE_WINDOW_COORD_X0_BASIC 0
#define INTERFACE_WINDOW_COORD_Y0_BASIC 0
//#define INTERFACE_WINDOW_COORD_X1_BASIC 800
#define INTERFACE_WINDOW_COORD_X1_BASIC 1320    //��ʱ��ô����
//270 maxever 470 �޸Ĵ���ֵ���ڵ��� ������ͬʱ�޸� TETRIS_PLAY_SPACE_Y
#define INTERFACE_WINDOW_COORD_Y1_BASIC 330 

//WIDE���ڿ��ڣ��˻���ս�������ڵ�����Ϸʱ��BASIC��
#define INTERFACE_WINDOW_COORD_X1_WIDE 1320

#define TETRIS_PLAY_SPACE_X 14    //12 + 2��ǽ�� ��
//23��  maxever 43 �޸Ĵ���ֵ���ڵ��� ������ͬʱ�޸� INTERFACE_WINDOW_COORD_Y1_BASIC
#define TETRIS_PLAY_SPACE_Y 29    

#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y 6    //���߶�Ϊ6���Զ��巽��ռ�
#define TETRIS_CUSTOMIZE_BLOCKS_SPACE_X TETRIS_PLAY_SPACE_X    //�Զ��巽��ռ���

//�˵���ӡê�������꣨���Ͻǣ�
#define INTERFACE_MENU_ANCHOR_POINT_X 22    //12
#define INTERFACE_MENU_ANCHOR_POINT_Y 4

//�˵�ָʾ��ê�������꣨���Ͻǣ�
#define INTERFACE_MENU_POINTER_ANCHOR_POINT_X \
  (INTERFACE_MENU_ANCHOR_POINT_X + 2)
#define INTERFACE_MENU_POINTER_ANCHOR_POINT_Y \
  (INTERFACE_MENU_ANCHOR_POINT_Y + 3)

//���solo����˹�������д�ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X 2
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y 2

//���solo����˹����NextBlock��ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y

//���solo�÷���Ϣ��ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_TETRIS_SPACE_Y + 10)

//���solo����÷ִ�ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_EXACT_SCORING_X \
  (INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_X + 7)
#define INTERFACE_PLAY_SOLO_ANCHOR_POINT_EXACT_SCORING_Y \
  INTERFACE_PLAY_SOLO_ANCHOR_POINT_SCORING_Y

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

//���VS���ԣ��÷���Ϣ��ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_X + 16)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_TETRIS_SPACE_Y + 10)

//���VS���ԣ�����÷ִ�ӡê�������꣨���Ͻǣ�
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_EXACT_SCORING_X \
  (INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_X + 7)
#define INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_EXACT_SCORING_Y \
  INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_SCORING_Y

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
/*�볣���ַ�����szMenuStringColor��һһ��Ӧ�Ĺ�ϵ*/
const char szMenuString[11][MAX_STRING_LENGTH] = {
  " ����˹����(���Զ��巽��) ",/*[0]*/
  "",/*[1]*/
  "",/*[2]*/
  "      ������Ϸ",/*[3]*/
  "      �˻���ս",/*[4]*/
  "      �Զ��巽��",/*[5]*/
  "      �˳���Ϸ",/*[6]*/
  "",/*[7]*/
  "",/*[8]*/
  "���Ƶ����ٶȣ�С����1��2;���Ƶ����Ƿ�������䣺С����3��",/*[9]*/
  "��Ϸ�а�ESC���������˵���",/*[10]*/
};

//�˵��ַ�����ͷ����Чѡ����⣬����,β����˵��
#define VOID_SLOT_IN_MENU 7

//const������C++�﷨
/*�볣���ַ�������szMenuString��һһ��Ӧ�Ĺ�ϵ*/
const unsigned short szMenuStringColor[11] = {
  INTERFACE_BLOCK_COLOR_BLACK_YELLOW,/*[0]*//*�ڵ׻���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[1]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[2]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[3]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[4]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[5]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[6]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[7]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_BLACK,/*[8]*//*��׺���*/
  INTERFACE_BLOCK_COLOR_CYAN_YELLOW,/*[9]*//*��׻���*/
  INTERFACE_BLOCK_COLOR_CYAN_YELLOW,/*[10]*//*��׻���*/
};

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

#define INTERFACE_SCORING_STRING "������������"


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

//���Բ����ٶȱ���ֵ������������10�����٣���0��Ԫ�أ��ȼ�0�����ã�
#define MAX_COM_SPEED_LIST_NUM 11

//const������C++�﷨
/*���Բ����ٶȣ��Ѷȣ���10������ֵ�ǵ��Զ�ȡָ���б��ʱ������ÿ���µķ������ʱ��ָ��
�б���AI���ɡ�*/
const int nCOMSpeed[MAX_COM_SPEED_LIST_NUM] = {
  500,  //�ȼ�0��ȱʡ
  500,  //�ȼ�1
  300,  //�ȼ�2
  200,  //3
  100,  //4
  50,   //5
  20,   //6
  10,   //7
  5,    //8
  2,    //9
  1     //10
};

//AI���Բ���������AI���Կ���ֱ���޸������������ֵ
//AI�Ƿ��ʹ�ÿ�������:1�ᣬ0����
#define AI_STRATEGY_STRAIGHT_DOWN 0

//��λ���·��пո��ͷ��������ȣ�Ĭ��3
#define AI_STRATEGY_PUNISHMENT_DEEPTH 3

//��λ���·��пո��ͷ����ͷ���ֵ��Ĭ��40
#define AI_STRATEGY_PUNISHMENT_VALUE 40

//��λ������߶Ȳ��Ȩ�أ�Ĭ��5
#define AI_STRATEGY_HEIGHT_DISTANCE_FACTOR 5

//��λ�󣬵�ǰ��������ĵ�߶�Ȩ�أ�Ĭ��5
#define AI_STRATEGY_CENTER_POINT_HEIGHT_FACTOR 5

