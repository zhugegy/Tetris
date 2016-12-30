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

//��������ָ���ָ���ַ��дָ���ַ��
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

//дָ��
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

//дָ��
int MessageContainer::write_message(int nCmd)
{
  MessageList[nWritingPostion] = nCmd;
  nWritingPostion++;

  return 0;
}

//��ָ��
int MessageContainer::read_message()
{
  int nMessage = 0;

  nMessage = MessageList[nReadingPostion];
  nReadingPostion++;

  //��ª�ķ��������������
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
