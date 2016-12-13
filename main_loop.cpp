#include "stdafx.h"
#include "main_loop.h"

#include "data_processor.h"
#include "interface.h"
#include "chain_list_processor.h"
#include "engine.h"

#include <conio.h>

static int switch_session_block__main_loop(Param *pstParam, int *pnCurrentBlock,
                                           int *pnNextBlock);

//玩家和电脑都在这同一个循环中
int tetris_loop__main_loop(Param *pstParam)
{
  int nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;
  int nCurrentBlockPlayer = 0;
  int nNextBlockPlayer = 0;
  //int nCurrentBlockCom[MAX_COM_NUM] = {0};    电脑以后再加
  DWORD dwTimeCounter = 0;
  unsigned int k = 0;    //用户输入(事件)计数
  //获取输入句柄
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  //定义接收输入事件的数组和实际接收到的量
  INPUT_RECORD irBuf[128];
  DWORD nNumRead;

  //随机出下一个方块（其实就是第一个方块，因为马上就session更新）
  nNextBlockPlayer = get_a_random_block(pstParam);

  //session更新
  switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer, 
    &nNextBlockPlayer);
  summon_new_block__engine(pstParam, &nControlFlag);

  //设定起始时间
  dwTimeCounter = GetTickCount();

  while (nControlFlag != CONTROL_FLAG_MAIN_LOOP_GAME_OVER)
  {
    //如果一个方块完毕了，session更新(玩家)
    if (nControlFlag == CONTROL_FLAG_MAIN_LOOP_SESSION_ENDING_PLAYER)
    {
      nCurrentBlockPlayer = nNextBlockPlayer;
      switch_session_block__main_loop(pstParam, &nCurrentBlockPlayer,
        &nNextBlockPlayer);
      //(尝试)消行
      clean_line__engine(pstParam);

      //召唤新的方块（玩家）
      summon_new_block__engine(pstParam, &nControlFlag);

      nControlFlag = CONTROL_FLAG_MAIN_LOOP_RUNNING;
    }

    //==如果一个方块完毕了，换下一个方块(电脑）    以后再加
     
    if (GetTickCount() - dwTimeCounter > 800)
    {
      dwTimeCounter = GetTickCount();
      move_down_block_player__engine(pstParam, &nControlFlag);
    }

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
                                           int *pnNextBlock)
{
  CustomizedBlock *pstTmp = NULL;

  //当前方块的资源释放
  if (pstParam->pstFirstBlockElement != NULL)
  {
    free_current_block_element__chain_list_processor(pstParam);
  }

  //当前方块等于下一个方块
  *pnCurrentBlock = *pnNextBlock;

  //把当前所有方块元素加入链表(再次分配内存资源)
  build_block_element_chain__data_processor(pstParam, *pnCurrentBlock);
  //链表构建完成后，再次处理链表元素中的pCenter成员
  after_process_block_element_chain__data_processor(pstParam);

  //随机下一个方块
  *pnNextBlock = get_a_random_block(pstParam);

  //获得下一个方块的信息链表地址
  pstTmp = pstParam->pstFastArray[*pnNextBlock];

  //把下一个方块在NextBlock区域打印出来
  print_tetris_customize_blocks_ground__interface(
    pstTmp->TetrisCustomizeBlocksSpace, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_X * 2, 
    INTERFACE_PLAY_SOLO_ANCHOR_POINT_NEXT_BLOCK_Y);

  return 0;
}