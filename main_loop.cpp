#include "stdafx.h"
#include "main_loop.h"

#include "data_processor.h"
#include "interface.h"
#include "chain_list_processor.h"
#include "engine.h"
#include "COM_control_list.h"
#include "COM_control_AI.h"


#include <conio.h>

static int switch_session_block__main_loop(Param *pstParam, int *pnCurrentBlock,
	int *pnNextBlock, PlayerVSCOMControlFlag eFlag);


//玩家和电脑都在这同一个循环中
int tetris_loop__main_loop(Param *pstParam)
{
  int nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;

  int nCurrentBlockPlayer = 0;
  int nNextBlockPlayer = 0;
  DWORD dwTimeCounterPlayer = 0;
  bool bIsSessionEndedPlayer = false;

  //int nCurrentBlockCom[MAX_COM_NUM] = {0};    多重电脑，以后再加
  int nCurrentBlockCOM = 0;
  int nNextBlockCOM = 0;
  DWORD dwTimeCounterCOM = 0;
  bool bIsSessionEndedCOM = false;

  //电脑AI的操作间隔计时器
  DWORD dwTimeCounterCOMAI = 0;
  int nCOMAICmd = 0;

  unsigned int k = 0;    //用户输入(事件)计数
  //获取输入句柄
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  //定义接收输入事件的数组和实际接收到的量
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  //随机出下一个方块（其实就是第一个方块，因为马上就session更新）
  nNextBlockPlayer = get_a_random_block(pstParam, -1);

  //session更新
  switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer, 
    &nNextBlockPlayer, PLAYER_CONTROL);

  //召唤新的方块
  summon_new_block__engine(pstParam, &nControlFlag, PLAYER_CONTROL);

  //设定起始时间
  dwTimeCounterPlayer = GetTickCount();

  //如果是人机对战模式，针对电脑一边，单独：
  if (pstParam->eStageFlag == STAGE_PLAY_VS_COM)
  {
    //随机出下一个方块（其实就是第一个方块，因为马上就session更新）
    nNextBlockCOM = get_a_random_block(pstParam, -1);
    
    nNextBlockCOM = 1;
    
    //session更新
    switch_session_block__main_loop(pstParam, &nCurrentBlockCOM,
      &nNextBlockCOM, COM_CONTROL);
    
    //召唤新的方块
    summon_new_block__engine(pstParam, &nControlFlag, COM_CONTROL);
    
    //设定起始时间
    dwTimeCounterCOM = GetTickCount();
  }

  while (nControlFlag == CONTROL_FLAG_MAIN_LOOP_RUNNING)
  {
    //如果一个玩家的方块完毕了，session更新

    //此处和COM的sessionending有潜在冲突，应该换成pstParam里面的一个全局标志位最好
    /*if (nControlFlag == CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER)*/
    if (bIsSessionEndedPlayer == true)
    {
      nCurrentBlockPlayer = nNextBlockPlayer;
      switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer,
        &nNextBlockPlayer, PLAYER_CONTROL);
      //(尝试)消行
      clean_line__engine(pstParam, PLAYER_CONTROL);

      //召唤新的方块（玩家）
      summon_new_block__engine(pstParam, &nControlFlag, PLAYER_CONTROL);

      bIsSessionEndedPlayer = false;
    }

    //如果是人机对战模式，如果一个电脑的方块完毕了，session更新
    if (pstParam->eStageFlag == STAGE_PLAY_VS_COM && bIsSessionEndedCOM == true)
    {
      nCurrentBlockCOM = nNextBlockCOM;
      switch_session_block__main_loop(pstParam, &nCurrentBlockCOM,
        &nNextBlockCOM, COM_CONTROL);
      //(尝试)消行
      clean_line__engine(pstParam, COM_CONTROL);

      //召唤新的方块（电脑）
      summon_new_block__engine(pstParam, &nControlFlag, COM_CONTROL);

      bIsSessionEndedCOM = false;
    }
     
    //方块随时间进行而自动下落
    if (GetTickCount() - dwTimeCounterPlayer > 800)
    {
      dwTimeCounterPlayer = GetTickCount();
      move_down_block__engine(pstParam, &nControlFlag, &bIsSessionEndedPlayer, 
        PLAYER_CONTROL);
    }

    //如果是人机对战模式,电脑方框随时间进行而自动下落
    if (pstParam->eStageFlag == STAGE_PLAY_VS_COM &&
      GetTickCount() - dwTimeCounterCOM > 800)
    {
      dwTimeCounterCOM = GetTickCount();
      move_down_block__engine(pstParam, &nControlFlag, &bIsSessionEndedCOM, 
        COM_CONTROL);
    }

    //按键处理
    if (_kbhit())
    {
      ReadConsoleInput(hStdIn, irBuf, 128, &nNumRead);
      //遍历事件
      for (k = 0; k < nNumRead; k++)
      {
        if (irBuf[k].Event.KeyEvent.bKeyDown == 1)
        {
          //根据事件种类）决定处理方式
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
              &bIsSessionEndedPlayer, PLAYER_CONTROL);
            break;
          case VK_UP:
            rotate_block__engine(pstParam, PLAYER_CONTROL);
            break;
          case VK_SPACE:
            while (bIsSessionEndedPlayer == false)
            {
              move_down_block__engine(pstParam, &nControlFlag,
                &bIsSessionEndedPlayer, PLAYER_CONTROL);
            }
            break;
          //COM_DEBUG:隐藏按键：可以控制电脑行为
          case VK_NUMPAD4:
            move_left_block__engine(pstParam, COM_CONTROL);
            break;
          case VK_NUMPAD6:
            move_right_block__engine(pstParam, COM_CONTROL);
            break;
          case VK_NUMPAD5:
            move_down_block__engine(pstParam, &nControlFlag,
              &bIsSessionEndedCOM, COM_CONTROL);
            break;
          case VK_NUMPAD8:
            rotate_block__engine(pstParam, COM_CONTROL);
            break;
          case VK_NUMPAD0:
            while (bIsSessionEndedCOM == false)
            {
              move_down_block__engine(pstParam, &nControlFlag,
                &bIsSessionEndedCOM, COM_CONTROL);
            }
            break;
          case VK_NUMPAD1:
            //电脑难度-1
            if (pstParam->nCOMLevel > 1)
            {
              pstParam->nCOMLevel--;
            }
            break;
          case VK_NUMPAD2:
            if (pstParam->nCOMLevel < MAX_COM_SPEED_LIST_NUM - 1)
            {
              pstParam->nCOMLevel++;
            }
            break;
          default:
            break;
          }
        }
      }
    }

    //如果是人机对战模式，读取MessageContainer的指令序列（AI指令）
    if (pstParam->eStageFlag == STAGE_PLAY_VS_COM &&
      GetTickCount() - dwTimeCounterCOMAI > 
      pstParam->nCOMSpeedList[pstParam->nCOMLevel])
    {
      dwTimeCounterCOMAI = GetTickCount();
      nCOMAICmd = pstParam->COMControlMsg.read_message();
      //翻译并执行
      translate_COM_cmd_to_action__COM_control_AI(pstParam, nCOMAICmd,
        &nControlFlag, &bIsSessionEndedCOM);
    }
  
  }

  getchar();

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

  /* DEBUG用
  CustomizedBlock *pstTmp = NULL;

  while (true)
  {
    nNextBlockPlayer = get_a_random_block(pstParam);
    //获得下一个方块的信息链表地址
    pstTmp = pstParam->pstFastArray[nNextBlockPlayer];

    //把下一个方块在NextBlock区域打印出来
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

  //当前方块的资源释放
  free_current_block_element__chain_list_processor(pstParam, eFlag);

  //当前方块等于下一个方块
  *pnCurrentBlock = *pnNextBlock;

  //把当前所有方块元素加入链表(再次分配内存资源)
  build_block_element_chain__data_processor(pstParam, *pnCurrentBlock, eFlag);
  //链表构建完成后，再次处理链表元素中的pCenter成员
  after_process_block_element_chain__data_processor(pstParam, eFlag);

  //随机下一个方块
  *pnNextBlock = get_a_random_block(pstParam, *pnCurrentBlock);
  /*//AIdebug
  if (eFlag == COM_CONTROL)
  {
    *pnNextBlock = pstParam->BlockList[pstParam->CurrentPointerToBlockList];
    pstParam->CurrentPointerToBlockList += 1;
    if (pstParam->CurrentPointerToBlockList >=80)
    {
      pstParam->CurrentPointerToBlockList = 0;
    }
  }*/
  
  //获得下一个方块的信息链表地址
  pstTmp = pstParam->pstFastArray[*pnNextBlock];

  //把下一个方块在NextBlock区域打印出来
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



