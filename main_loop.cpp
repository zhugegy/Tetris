#include "stdafx.h"
#include "main_loop.h"

#include "data_processor.h"
#include "interface.h"
#include "chain_list_processor.h"
#include "engine.h"

#include <conio.h>

static int switch_session_block__main_loop(Param *pstParam, int *pnCurrentBlock,
	int *pnNextBlock, PlayerVSCOMControlFlag eFlag);

//��Һ͵��Զ�����ͬһ��ѭ����
int tetris_loop__main_loop(Param *pstParam)
{
  int nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;

  int nCurrentBlockPlayer = 0;
  int nNextBlockPlayer = 0;
  DWORD dwTimeCounterPlayer = 0;
  bool bIsSessionEndedPlayer = false;

  //int nCurrentBlockCom[MAX_COM_NUM] = {0};    ���ص��ԣ��Ժ��ټ�
  int nCurrentBlockCOM = 0;
  int nNextBlockCOM = 0;
  DWORD dwTimeCounterCOM = 0;
  bool bIsSessionEndedCOM = false;

  unsigned int k = 0;    //�û�����(�¼�)����
  //��ȡ������
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  //������������¼��������ʵ�ʽ��յ�����
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  //�������һ�����飨��ʵ���ǵ�һ�����飬��Ϊ���Ͼ�session���£�
  nNextBlockPlayer = get_a_random_block(pstParam);

  //session����
  switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer, 
    &nNextBlockPlayer, PLAYER_CONTROL);

  //�ٻ��µķ���
  summon_new_block__engine(pstParam, &nControlFlag, PLAYER_CONTROL);

  //�趨��ʼʱ��
  dwTimeCounterPlayer = GetTickCount();

  //������˻���սģʽ����Ե���һ�ߣ�������
  if (pstParam->eStageFlag == STAGE_PLAY_VS_COM)
  {
    //�������һ�����飨��ʵ���ǵ�һ�����飬��Ϊ���Ͼ�session���£�
    nNextBlockCOM = get_a_random_block(pstParam);
    
    //session����
    switch_session_block__main_loop(pstParam, &nCurrentBlockCOM,
      &nNextBlockCOM, COM_CONTROL);
    
    //�ٻ��µķ���
    summon_new_block__engine(pstParam, &nControlFlag, COM_CONTROL);
    
    //�趨��ʼʱ��
    dwTimeCounterCOM = GetTickCount();
  }

  while (nControlFlag == CONTROL_FLAG_MAIN_LOOP_RUNNING)
  {
    //���һ����ҵķ�������ˣ�session����

    //�˴���COM��sessionending��Ǳ�ڳ�ͻ��Ӧ�û���pstParam�����һ��ȫ�ֱ�־λ���
    /*if (nControlFlag == CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER)*/
    if (bIsSessionEndedPlayer == true)
    {
      nCurrentBlockPlayer = nNextBlockPlayer;
      switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer,
        &nNextBlockPlayer, PLAYER_CONTROL);
      //(����)����
      clean_line__engine(pstParam, PLAYER_CONTROL);

      //�ٻ��µķ��飨��ң�
      summon_new_block__engine(pstParam, &nControlFlag, PLAYER_CONTROL);

      bIsSessionEndedPlayer = false;
    }

    //������˻���սģʽ�����һ�����Եķ�������ˣ�session����
    if (pstParam->eStageFlag == STAGE_PLAY_VS_COM && bIsSessionEndedCOM == true)
    {
      nCurrentBlockCOM = nNextBlockCOM;
      switch_session_block__main_loop(pstParam, &nCurrentBlockCOM,
        &nNextBlockCOM, COM_CONTROL);
      //(����)����
      clean_line__engine(pstParam, COM_CONTROL);

      //�ٻ��µķ��飨��ң�
      summon_new_block__engine(pstParam, &nControlFlag, COM_CONTROL);

      bIsSessionEndedCOM = false;
    }
     
    //������ʱ����ж��Զ�����
    if (GetTickCount() - dwTimeCounterPlayer > 800)
    {
      dwTimeCounterPlayer = GetTickCount();
      move_down_block__engine(pstParam, &nControlFlag, &bIsSessionEndedPlayer,
        &bIsSessionEndedCOM, PLAYER_CONTROL);
    }

    //������˻���սģʽ,���Է�����ʱ����ж��Զ�����
    if (pstParam->eStageFlag == STAGE_PLAY_VS_COM &&
      GetTickCount() - dwTimeCounterCOM > 800)
    {
      dwTimeCounterCOM = GetTickCount();
      move_down_block__engine(pstParam, &nControlFlag, &bIsSessionEndedPlayer,
        &bIsSessionEndedCOM, COM_CONTROL);
    }

    if (_kbhit())
    {
      ReadConsoleInput(hStdIn, irBuf, 128, &nNumRead);
      //�����¼�
      for (k = 0; k < nNumRead; k++)
      {
        if (irBuf[k].Event.KeyEvent.bKeyDown == 1)
        {
          //�����¼����ࣩ��������ʽ
          switch (irBuf[k].Event.KeyEvent.wVirtualKeyCode)
          {
          case VK_LEFT:
            move_left_block__engine(pstParam, PLAYER_CONTROL);
            break;
          case VK_RIGHT:
            move_right_block__engine(pstParam, PLAYER_CONTROL);
            break;
          case VK_DOWN:
            move_down_block__engine(pstParam, &nControlFlag,
              &bIsSessionEndedPlayer, &bIsSessionEndedCOM, PLAYER_CONTROL);
            break;
          case VK_UP:
            rotate_block_player__engine(pstParam);
            break;
          //COM_DEBUG:���ذ��������Կ��Ƶ�����Ϊ
          case VK_NUMPAD4:
            move_left_block__engine(pstParam, COM_CONTROL);
            break;
          case VK_NUMPAD6:
            move_right_block__engine(pstParam, COM_CONTROL);
            break;
          case VK_NUMPAD5:
            move_down_block__engine(pstParam, &nControlFlag,
              &bIsSessionEndedPlayer, &bIsSessionEndedCOM, COM_CONTROL);;
            break;
          default:
            break;
          }
        }
      }
    }
  
  }

  return 0;
}


    /*//debug start
    move_down_block_player__engine(pstParam, &nControlFlag);
   // move_left_block_player__engine(pstParam);
   

    move_down_block_player__engine(pstParam, &nControlFlag);
    move_down_block_player__engine(pstParam, &nControlFlag);
    //move_right_block_player__engine(pstParam);
    rotate_block_player__engine(pstParam);
    rotate_block_player__engine(pstParam);
    rotate_block_player__engine(pstParam);
    rotate_block_player__engine(pstParam);
    rotate_block_player__engine(pstParam);

    move_down_block_player__engine(pstParam, &nControlFlag);
    
    //debug end*/

  /* DEBUG��
  CustomizedBlock *pstTmp = NULL;

  while (true)
  {
    nNextBlockPlayer = get_a_random_block(pstParam);
    //�����һ���������Ϣ�����ַ
    pstTmp = pstParam->pstFastArray[nNextBlockPlayer];

    //����һ��������NextBlock�����ӡ����
    print_tetris_customize_blocks_ground__interface(
      pstTmp->TetrisCustomizeBlocksSpace, 
      INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X, 
      INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y);

    getchar();
  }*/
  

static int switch_session_block__main_loop(Param *pstParam, int *pnCurrentBlock,
                 int *pnNextBlock, PlayerVSCOMControlFlag eFlag)
{
  CustomizedBlock *pstTmp = NULL;

  //��ǰ�������Դ�ͷ�
  free_current_block_element__chain_list_processor(pstParam, eFlag);

  //��ǰ���������һ������
  *pnCurrentBlock = *pnNextBlock;

  //�ѵ�ǰ���з���Ԫ�ؼ�������(�ٴη����ڴ���Դ)
  build_block_element_chain__data_processor(pstParam, *pnCurrentBlock, eFlag);
  //��������ɺ��ٴδ�������Ԫ���е�pCenter��Ա
  after_process_block_element_chain__data_processor(pstParam, eFlag);

  //�����һ������
  *pnNextBlock = get_a_random_block(pstParam);

  //�����һ���������Ϣ�����ַ
  pstTmp = pstParam->pstFastArray[*pnNextBlock];

  //����һ��������NextBlock�����ӡ����
  if (eFlag == PLAYER_CONTROL)
  {
    print_tetris_customize_blocks_ground__interface(
      pstTmp->TetrisCustomizeBlocksSpace,
      INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X * 2,
      INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y);
  }
  if (eFlag == COM_CONTROL)
  {
    print_tetris_customize_blocks_ground__interface(
      pstTmp->TetrisCustomizeBlocksSpace,
      INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_X * 2,
      INTERFACE_PLAYER_VS_COM_ANCHOR_POINT_NEXT_BLOCK_Y);
  }

  return 0;
}