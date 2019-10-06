#include <stdio.h>
#include <string>
#include <iostream>
#include <ctime>
#include <Windows.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "security/check_license.h"
#include <TlHelp32.h>
#include <WinInet.h>
#include "config.h"
#include "Hack/includes.hpp"
#include <wchar.h>
#include <thread>
#include <webapplication.h>
#include <fstream>
#include "menu/cmenu.hpp"
#include "utils/csgo.hpp"
#define OWO 1337
using namespace std;
UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

bool unload = false;
HINSTANCE HThisModule;
bool DoUnload;
/*
PVOID AntiReversing(HMODULE dwModule)
{
	PVOID pEntry = NULL;
	PIMAGE_DOS_HEADER pId = (PIMAGE_DOS_HEADER)dwModule;
	PIMAGE_NT_HEADERS pInt = (PIMAGE_NT_HEADERS)(dwModule + pId->e_lfanew);
	pEntry = dwModule + pInt->OptionalHeader.BaseOfCode;
	return pEntry;
}

void AntiHeaders(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov dwPEB_LDR_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, hModule
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList
			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

void HideModule(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov dwPEB_LDR_DATA, eax
			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]
			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, hModule
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList
			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList
			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]
			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList
			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]
			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList
			Finished :
		popfd;
		popad;
	}
}*/

bool IsProcessRun(const char * const processName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	while (1) {
		if (strcmp(pe.szExeFile, processName) == 0) return true;
		if (!Process32Next(hSnapshot, &pe)) return false;
	}
}

DWORD WINAPI OnDllAttach(PVOID base) {

#ifdef _DEBUG
	fridge::AttachConsole();
#endif
	//fridge::ConsolePrint("Initializing...\n");

	//if (hwid_checker() == true)
//	{
		materials::get().setup();
		render::get().setup();
		cfg_manager->setup();
		setup_main_menu();
		netvars::get().init();
		hooks::load();
//	}
//	else
	//{
//		MessageBox(NULL, "license failed.", "error", MB_OK);
//		ExitProcess(0);
//		exit(0);
	//}

	fridge::ConsolePrint("Finished Initializing!\n");
	while (!unload) {
		using namespace literals::chrono_literals;
		this_thread::sleep_for(1s);
	}
	fridge::ConsolePrint("Uninjecting...!\n");
	this_thread::sleep_for(chrono::milliseconds(100));
	hooks::shutdown();
	this_thread::sleep_for(chrono::milliseconds(100));
	fridge::ConsolePrint("Uninjected!\n");
	this_thread::sleep_for(chrono::milliseconds(100));
	fridge::DetachConsole();
	this_thread::sleep_for(chrono::milliseconds(100));
	fclose((FILE*)stdin);
	fclose((FILE*)stdout);
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{

	if (IsProcessRun("idaq64.exe"))
	{
		MessageBox(0, "Мать жива?", "Пентагон", MB_OK);
		exit(1);
	}

	if (IsProcessRun("idaq32.exe"))
	{
		MessageBox(0, "Ау дверь чек там вроде к те пришли", "Пентагон", MB_OK);
		exit(1);
	}

	if (IsProcessRun("ollydbg.exe"))
	{
		MessageBox(0, "ебать щас бы с ольги реверсить", "Пентагон", MB_OK);
		exit(1);
	}

	if (IsProcessRun("ollydbg64.exe"))
	{
		MessageBox(0, "ебать щас бы с ольги реверсить", "Пентагон", MB_OK);
		exit(1);
	}

	if (IsProcessRun("loaddll.exe"))
	{
		MessageBox(0, "у тя матери нет", "Пентагон", MB_OK);
		exit(1);
	}


	if (IsProcessRun("httpdebugger.exe"))
	{
		MessageBox(0, "ебать ты даун длл в байтах саси", "Пентагон", MB_OK);
		exit(1);
	}


	if (IsProcessRun("windowrenamer.exe"))
	{
		MessageBox(0, "щас бы быть в 2к08", "Пентагон", MB_OK);
		exit(1);
	}


	if (IsProcessRun("processhacker.exe"))
	{
		MessageBox(0, "Эйй! Салам алейкум", "Ержан", MB_OK);
		exit(1);
	}


	std::time_t result = std::time(nullptr);
	if (dwReason == DLL_PROCESS_ATTACH) {

		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		g_ctx.m_globals.lpReserved = hModule;
		DisableThreadLibraryCalls(hModule);

		CreateThread(nullptr, NULL, OnDllAttach, hModule, NULL, nullptr);
		//CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(menu::render), nullptr, 0, nullptr);
	}
	return TRUE;
}