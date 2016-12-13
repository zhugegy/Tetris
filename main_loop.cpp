#include "stdafx.h"
#include "main_loop.h"

#include "data_processor.h"
#include "interface.h"
#include "chain_list_processor.h"
#include "engine.h"

#include <conio.h>

static int switch_session_block__main_loop(Param *pstParam, int *pnCurrentBlock,
                                           int *pnNextBlock);

//��Һ͵��Զ�����ͬһ��ѭ����
int tetris_loop__main_loop(Param *pstParam)
{
  int nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;
  int nCurrentBlockPlayer = 0;
  int nNextBlockPlayer = 0;
  //int nCurrentBlockCom[MAX_COM_NUM] = {0};    �����Ժ��ټ�
  DWORD dwTimeCounter = 0;
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
    &nNextBlockPlayer);
  summon_new_block__engine(pstParam, &nControlFlag);

  //�趨��ʼʱ��
  dwTimeCounter = GetTickCount();

  while (nControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER)
  {
    //���һ����������ˣ�session����(���)
    if (nControlFlag == CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER)
    {
      nCurrentBlockPlayer = nNextBlockPlayer;
      switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer,
        &nNextBlockPlayer);
      //(����)����
      clean_line__engine(pstParam);

      //�ٻ��µķ��飨��ң�
      summon_new_block__engine(pstParam, &nControlFlag);

      nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;
    }

    //==���һ����������ˣ�����һ������(���ԣ�    �Ժ��ټ�
     
    if (GetTickCount() - dwTimeCounter > 800)
    {
      dwTimeCounter = GetTickCount();
      move_down_block_player__engine(pstParam, &nControlFlag);
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
            move_left_block_player__engine(pstParam);
            break;
          case VK_RIGHT:
            move_right_block_player__engine(pstParam);
            break;
          case VK_DOWN:
            move_down_block_player__engine(pstParam, &nControlFlag);
            break;
          case VK_UP:
            rotate_block_player__engine(pstParam);
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
                                           int *pnNextBlock)
{
  CustomizedBlock *pstTmp = NULL;

  //��ǰ�������Դ�ͷ�
  if (pstParam->pstFirstBlockElement != NULL)
  {
    free_current_block_element__chain_list_processor(pstParam);
  }

  //��ǰ���������һ������
  *pnCurrentBlock = *pnNextBlock;

  //�ѵ�ǰ���з���Ԫ�ؼ�������(�ٴη����ڴ���Դ)
  build_block_element_chain__data_processor(pstParam, *pnCurrentBlock);
  //��������ɺ��ٴδ�������Ԫ���е�pCenter��Ա
  after_process_block_element_chain__data_processor(pstParam);

  //�����һ������
  *pnNextBlock = get_a_random_block(pstParam);

  //�����һ���������Ϣ�����ַ
  pstTmp = pstParam->pstFastArray[*pnNextBlock];

  //����һ��������NextBlock�����ӡ����
  print_tetris_customize_blocks_ground__interface(
    pstTmp->TetrisCustomizeBlocksSpace, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y);

  return 0;
}