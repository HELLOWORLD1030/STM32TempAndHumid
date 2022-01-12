
#include "list.h"

/**�����ȸı��־λ**/
uint8_t	Flag_Change_List;
/**�����ȱ���**/
uint16_t List_Lenght;
/**����洢����**/
uint16_t Flash_List[STM_SECTOR_SIZE/2];

/**
* @brief ��������
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
* @brief ɾ����������
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
* @brief ɾ����������(OK)
* @param[in] **header �����ַָ��
* @param[in] num ɾ����ID
* @return 0:�ɹ�	1:ʧ��
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
* @brief ����һ������ڵ�
* @param[in] **header	����ͷ��ַ��ָ��
* @param[in] num	��˭�������ӽڵ��λ�� 0:��ͷǰ
* @return 0:�ɹ�	1:ʧ��
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
* @brief ��������ڵ���
* @param[in] *header	����ͷ��ַ
* @return �����������	0:�յ�ַ
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
* @brief ������д������
* @param[in] *header	����ͷ��ַ
* @param[in] *pbuf	Ҫ���������ָ��
* @return 0:�ɹ�	1:ʧ��
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
