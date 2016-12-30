#pragma once
//���ļ���¼�����еĽṹ��

#include "MessageContainer.h"

//��̨����
enum StageList{
  STAGE_MAIN_MENU, 
  STAGE_PLAY_SOLO, 
  STAGE_PLAY_VS_COM, 
  STAGE_HIGH_SCORES,
  STAGE_CUSTOMIZE_BLOCKS,
  STAGE_QUIT
};

//�Զ��巽�����Ƶ��
enum CustomBlockFrequency {
  CUSTOMIZED_BLOCK_FREQUENCY_VERY_LOW = 1,
  CUSTOMIZED_BLOCK_FREQUENCY_LOW,
  CUSTOMIZED_BLOCK_FREQUENCY_NORMAL,
  CUSTOMIZED_BLOCK_FREQUENCY_HIGH,
  CUSTOMIZED_BLOCK_FREQUENCY_VERY_HIGH
};

//�Զ����˻���ս���Ʊ�ʾ��
enum PlayerVSCOMControlFlag {
	PLAYER_CONTROL = 1,
	COM_CONTROL
};

//������CUSTOMIZE_BLOCK�������ͷţ�Param->pstCustomizedBlockNodes��
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

//�Զ��巽��
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

//�Զ�������
typedef struct ElementCoord
{
  int nX;
  int nY;
}ElementCoord;

//����˹����ÿһ��Ľṹ��
typedef struct BlockElement
{
  ElementCoord stCoord;
  int nValue;    //��ֵ
  int nTailValue;    //β����ֵ�������ƶ�ʱ����ʱ�洢
  char isCenter;

  BlockElement *pCenter;
  BlockElement *pNext;

  //COM_AIר��,������ʱ������ת������ꡣ
  ElementCoord stCoordAItmp;
}BlockElement;

//��Ҫ����
typedef struct Param
{
  StageList eStageFlag;

  //��Ϸʱ�ķ��鷽��ռ�(���)
  unsigned char TetrisPlaySpacePlayer[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  //��Ϸʱ�ķ��鷽��ռ�(����)
  unsigned char TetrisPlaySpaceCOM[TETRIS_PLAY_SPACE_Y][TETRIS_PLAY_SPACE_X];

  //�Զ���ģʽ�ķ��鷽��ռ�
  unsigned char TetrisCustomizeBlocksSpace[TETRIS_CUSTOMIZE_BLOCKS_SPACE_Y]
  [TETRIS_CUSTOMIZE_BLOCKS_SPACE_X];

  //���з��������
  CustomizedBlock *pstCustomizedBlockNodes;

  //�洢��ǰ��������
  int nBlockNum;

  //�洢�����ַ�Ŀ������
  CustomizedBlock *pstFastArray[MAX_BLOCK_NUM];

  //�洢��ǰ����������Ԫ�ص�����������Ϸ�е��������Ҳ�����(���)
  BlockElement *pstFirstBlockElementPlayer;

  //�洢��ǰ����������Ԫ�ص�����������Ϸ�е��������Ҳ�����(����)
  BlockElement *pstFirstBlockElementCOM;

  //����AI����ָ��洢����
  MessageContainer COMControlMsg;

  //����AI�ٶȣ��Ѷȣ�
  int nCOMSpeedList[MAX_COM_SPEED_LIST_NUM];
  int nCOMLevel;

  /*//AIdebug
  int BlockList[100];
  int CurrentPointerToBlockList;*/
}Param;