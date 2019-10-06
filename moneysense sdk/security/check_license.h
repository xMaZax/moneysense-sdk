#pragma once
#include <urlmon.h>
#include <wininet.h>
#include "../hack/includes.hpp"
#pragma comment(lib, "wininet")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "urlmon.lib")
std::string ReadHWFile() {
	std::ifstream ifs("C:\\h.txt");
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return content;
}
// Функция которая узнает серийник ПК.
std::string serial() {
	DWORD  lVolSerialNbr = 0;
	char sHDSerial[255] = "";
	GetVolumeInformation("C:\\", 0, 0, &lVolSerialNbr, 0, 0, 0, 0);
	_ultoa_s(lVolSerialNbr, sHDSerial, 10);
	std::string c(sHDSerial);
	return c;
}

bool hwid_checker() { // Сама функция проверки, если проверка успешная выходит - true / не успешная - false
/*	HINTERNET connect = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0); // Создаем канал для подключения к сети
	if (!connect) // проверка на успешность создания канала для подключения к сети
	{
		return false;
	}
	std::string URL = "https://panel213.000webhostapp.com/XAKLDSKHGLAKHK2H2L2HL2KL2KL2H.ini"; // указываем в качестве ссылки для последующего открытия и чтения файла, адрес сайта + файл серийник.ini
	LPCSTR Final = URL.c_str();
	HINTERNET OpenAddress = InternetOpenUrl(connect, Final, NULL, INTERNET_DEFAULT_HTTPS_PORT, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	Sleep(10); // задержка в секундах 10.f для оптимизации хуков.
	if (!OpenAddress) // Проверка на доступ к сайту, aka подключение к сети
	{
		InternetCloseHandle(connect);
		return false;
		MessageBox(NULL, "error", "failed connect to cheat.", MB_OK);
	}
	char DataReceived[4096];
	DWORD NumberOfBytesRead = 0;
	while (InternetReadFile(OpenAddress, DataReceived, 4096, &NumberOfBytesRead) && NumberOfBytesRead) {} // Читает файл и записывает данные в переменные
	InternetCloseHandle(OpenAddress); // выключаем интерпутер
	InternetCloseHandle(connect); // выключаем интерпутер
	if (strstr(DataReceived, "AWSGKAJWG2J9J2390G32JG2J030J923GJ923J023J09G2J90G2J90G239JG923J0GJ92H90J0hjhw2hwh9jw39h2jh2h0")) // проверяем на нужный текст в файле, например HGF%R3
	{
		return true;
	}
	else
	{
		//hooks::shutdown(); // Здесь функция выгрузки вашего софта, в каждом своя.
		static TCHAR path[MAX_PATH]; // создаем переменную под адресс
		std::string serial1 = serial(); // создаем и приравниваем переменной серийник, для последующей записи
		LPCSTR Final1 = serial1.c_str(); // Просто забиваем в ещё одну переменную серийник, даунский мув конечно.
		std::string folder, file, serial; // создаем больше переменных, ибо чет мало было
		file = std::string(path) + ("C:\\serialkey.ini"); // прописываем адресс для сохранения файла с серийником
		WritePrivateProfileString(Final1, Final1, Final1, file.c_str()); // Записываем серийник в файл указанный строкой выше, дабы покупатель вашей пасты радовался.
		//MessageBox(NULL, "error enabled cheat.", "Error #666", MB_OK); // Сообщение о отсуствии подписки.
	}
	return false; */
	return true;
}