#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h> 
#include <WS2tcpip.h>
#pragma warning(disable: 4996)

int read(SOCKET s, char* buf, size_t len); //безопасное принятие запроса
int readn(SOCKET s, char* buf, size_t len); //для глобальной сети (возвращение частей файлов)
