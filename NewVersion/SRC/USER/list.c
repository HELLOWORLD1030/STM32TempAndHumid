
#include "list.h"

/**链表长度改变标志位**/
uint8_t	Flag_Change_List;
/**链表长度变量**/
uint16_t List_Lenght;
/**链表存储数组**/
uint16_t Flash_List[STM_SECTOR_SIZE/2];

/**
* @brief 创建链表
* @param[in] none
* @return user *header
* @ref list.h
*/
user *Creat_UserStuct(void)
{
	int id = 0, num = 0;
	user *header = NULL;
	user *p = NULL, *q = NULL;
	p = q = (user*)mymalloc(sizeof(user));
	while(id != 0)
	{
		p->number = num++;
		p->touchid = id;
		if(num == 1)
			header = p;
		else
			q->next = p;
		q = p;
		p = (user*)mymalloc(sizeof(user));
	}
	q->next = NULL;
	myfree(p);
	return header;
}

/**
* @brief 删除整个链表
* @param[in] none
* @return none
* @ref list.h
*/
void Del_List_All(user *header)
{
	user *next = header;
	while (next != NULL)
	{
		header = next;
		next = header->next;
		myfree(header);
	}
}

/**
* @brief 删除单个链表(OK)
* @param[in] **header 链表地址指针
* @param[in] num 删除的ID
* @return 0:成功	1:失败
* @ref list.h
*/
uint8_t Del_List_Single(user **header, uint16_t num)
{
	user *p = *header;
	user *plast = NULL;
	if (*header == NULL)
		return 1;
	if ((*header)->touchid == num)
	{
		p = *header;
		*header = (*header)->next;
		myfree(p);
		return 0;
	}
	while (p->touchid != num)
	{
		plast = p;
		p = p->next;
		if (p == NULL)
			return 1;
	}
	if (p->next == NULL)
	{
		plast->next = NULL;
		myfree(p);
		return 0;
	}
	plast->next = p->next;
	myfree(p);
	return 0;
}

/**
* @brief 增加一个链表节点
* @param[in] **header	链表头地址的指针
* @param[in] num	在谁后面增加节点的位置 0:表头前
* @return 0:成功	1:失败
* @ref list.h
*/
uint8_t Add_List_Single(user **header, int num, int id)
{
	user *p = NULL;
	user *q = *header;
	p = (user*)mymalloc(sizeof(user));
	p->touchid = id;
	if (num == 0)
	{
		p->next = *header;
		*header = p;
		return 0;
	}
	while (q->touchid != num)
	{
		q = q->next;
		if (q == NULL)
			return 1;
	}
	p->next = q->next;
	q->next = p;
	return 0;
}

/**
* @brief 计算链表节点数
* @param[in] *header	链表头地址
* @return 返回链表个数	0:空地址
* @ref list.h
*/
uint8_t Count_List_Lenght(user *header)
{
	if (header == NULL)
		return 0;
	while (header->next != NULL)
		header = header->next;
	return header->number;
}

/**
* @brief 把链表写入数组
* @param[in] *header	链表头地址
* @param[in] *pbuf	要定入的数组指针
* @return 0:成功	1:失败
* @ref list.h
*/
uint8_t Write_List_Flash(user *header, uint16_t *pbuf)
{
	uint16_t count = 0, num = 1;
	if (header == NULL)
		return 1;
	while (header != NULL)
	{
		pbuf[0] = 0xaa;
		pbuf[1 + SIZE_USER] = header->number;
		pbuf[2+ SIZE_USER] = header->opendoorflag;
		while ((header->passwd + count != '\0') && (count < sizeof(header->passwd)))
		{
			pbuf[3 + count + SIZE_USER] = header->passwd[0 + count];
			count++;
		}
		pbuf[4 + count + SIZE_USER] = header->touchid;
		pbuf[5 + count + SIZE_USER] = header->openttime;
		pbuf[6 + count + SIZE_USER] = 0xff;
		count = 0;
		num++;
		header = header->next;
	}
	return 0;
}
