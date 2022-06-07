#include "Tcp.h"
#include <iostream>

int read(SOCKET s, char* buf, size_t len) //� ��� ��� ���
{
	int rc = 1;

	while (true)
	{
		rc = recv(s, buf, len, 0);

		if (rc > 0)
			break; //���� ���� ��� ������� ����

		if (rc == 0) exit(0); //���� ���������� ��� ����� �� ����
	}
	return len;
}


int readn(SOCKET s, char* buf, size_t len) //��� ���������� ���� (����������� ������ ������) (������� ����� �� �����)
{ 
	int cnt;
	int rc;
	cnt = len;

	while (cnt > 0)
	{
		rc = recv(s, buf, cnt, 0);
		if (rc < 0) // ������ ������? 
		{
			if (errno == EINTR) // ����� �������? 
				continue; // ��������� ������. 
			return -1; // ������� ��� ������. 
		}
		if (rc == 0) // ����� �����? 
			return len - cnt; // ������� �������� �������. 
		buf += rc;
		cnt -= rc;

		Sleep(200);
	}
	return len;
}