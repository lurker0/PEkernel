// pipe_windbg.cpp : 定义控制台应用程序的入口点。
//
// NamedPipe_host.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"

HANDLE  gPipeHnd = NULL;


BOOLEAN
BlComInitialize(
    UINT8 PortNumber,
    UINT32 BaudRate
    )
{
	return TRUE;
}

BOOLEAN
BlComSendByte(
    UINT8 PortNumber,
    UINT8 Byte
    )
{
	DWORD dwWritten;
	if (!WriteFile(gPipeHnd, &Byte, 1, &dwWritten, NULL))
	{
		printf("WriteFile failed\n");
		return FALSE;
	}
	return TRUE;
}

BOOLEAN
BlComDataAvailable(
    UINT8 PortNumber
    )
{
	return TRUE;
}

UINT8
BlComReceiveByte(
    UINT8 PortNumber
    )
{
	UINT8 byte=0;
	DWORD		dwWritten;
	if (!ReadFile(gPipeHnd, &byte, 1, &dwWritten, NULL))
	{
		printf("ReadFile failed\n");
		return 0;
	}
	return byte;
}


#define  THE_PIPE L"\\\\.\\pipe\\testpipe"

VOID
BlKdInitialize(
    VOID
    );

BOOLEAN
BlKdPrintf(
    PCSTR Format,
    ...
    );

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE		hOut;
	WCHAR		buf[1024];
	DWORD		len;
	DWORD		dwWritten;

	hOut = CreateNamedPipe(THE_PIPE, 	// Name
		PIPE_ACCESS_DUPLEX | WRITE_DAC, // OpenMode
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, // PipeMode
		2, // MaxInstances
		1024, // OutBufferSize
		1024, // InBuffersize
		2000, // TimeOut
		NULL); // Security
	if (hOut == INVALID_HANDLE_VALUE)
	{
		printf("Could not create the pipe\n");
		exit(1);
	}
	printf("hPipe=%p\n", hOut);
	wprintf(L"pipe:%s\n",THE_PIPE);
	gPipeHnd = hOut;

	printf("connect...\n");
	ConnectNamedPipe(hOut, NULL);
	printf("...connected\n");
	BlKdInitialize();
	for (int i = 0; i < 50; i++)
	{

       BlKdPrintf("Test Pipe %d\n", i );
   }

   DisconnectNamedPipe(hOut);

   CloseHandle(hOut);

	return 0;
}
