#include "stdafx.h"
#include "MessageContainer.h"

#include "COM_control_list.h"

MessageContainer::MessageContainer()
{
  reset_message_list();
}


MessageContainer::~MessageContainer()
{
}

//重置所有指令、读指令地址和写指令地址。
int MessageContainer::reset_message_list()
{
  int i = 0;

  for (i = 0; i < MESSAGE_CONTAINER_MAX_NUM; i++)
  {
    MessageList[i] = COM_CONTROL_DEFAULT;
  }

  nReadingPostion = 0;
  nWritingPostion = 0;

  return 0;
}

//写指令
int MessageContainer::write_message(int *pMsgList, int nMessageNum)
{
  int i = 0;

  for (i = 0; i < nMessageNum; i++)
  {
    MessageList[nWritingPostion] = *(pMsgList + i);
    nWritingPostion++;
  }

  return 0;
}

//写指令
int MessageContainer::write_message(int nCmd)
{
  MessageList[nWritingPostion] = nCmd;
  nWritingPostion++;

  return 0;
}

//读指令
int MessageContainer::read_message()
{
  int nMessage = 0;

  nMessage = MessageList[nReadingPostion];
  nReadingPostion++;

  //简陋的防溢出。。先用着
  if (nReadingPostion >= MESSAGE_CONTAINER_MAX_NUM)
  {
    nReadingPostion = MESSAGE_CONTAINER_MAX_NUM - 10;
  }

  return nMessage;
}

int MessageContainer::backwards_reading_postion()
{
  nReadingPostion--;

  return 0;
}
