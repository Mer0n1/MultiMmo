#include "Tcp.h"
#include <iostream>

int read(SOCKET s, char* buf, size_t len) //а вот это моя
{
	int rc = 1;

	while (true)
	{
		rc = recv(s, buf, len, 0);

		if (rc > 0)
			break; //если файл цел закрыть цикл

		if (rc == 0) exit(0); //если соединения нет выйти из игры
	}
	return len;
}


int readn(SOCKET s, char* buf, size_t len) //для глобальной сети (возвращение частей файлов) (функция взята из книги)
{ 
	int cnt;
	int rc;
	cnt = len;

	while (cnt > 0)
	{
		rc = recv(s, buf, cnt, 0);
		if (rc < 0) // Ошибка чтения? 
		{
			if (errno == EINTR) // Вызов прерван? 
				continue; // Повторить чтение. 
			return -1; // Вернуть код ошибки. 
		}
		if (rc == 0) // Конец файла? 
			return len - cnt; // Вернуть неполный счетчик. 
		buf += rc;
		cnt -= rc;

		Sleep(200);
	}
	return len;
}