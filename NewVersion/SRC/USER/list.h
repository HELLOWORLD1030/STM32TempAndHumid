
#ifndef __LIST_H
#define __LIST_H

#include "stm32f10x.h"
#include "stdio.h"
#include "malloc.h"
#include "stm32flash.h"

#define SIZE_USER (num * (sizeof(header) - sizeof(user*)))

/*�û���Ϣ�ṹ��*/
typedef struct USER{
	unsigned char number;
	unsigned char opendoorflag;
	char passwd[6];
	unsigned int touchid;
	int openttime;
	struct USER *next;
}user;

/**@name �û�API����
*@{
*/
user *Creat_UserStuct(void);
void Del_List_All(user *header);
uint8_t Del_List_Single(user **header, uint16_t num);
uint8_t Add_List_Single(user **header, int num, int id);
/**@} */
#endif
