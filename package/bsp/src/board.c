#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include "port.h"

struct port_cfg my_port[] = {
	{
		.port_id = "ZB",
		.port_num = 6,
		.baudrate = 9600,
		.databits = 8,
		.parity = 0,
		.stopbits = 1,
	},
};

void set_port_cfg(struct port_cfg *cfg, const char *s_port_num, const char *s_baudrate, const char *s_databits, const char *s_parity, const char *s_stopbits)
{
	cfg->port_num = atoi(&s_port_num[3]);
	cfg->baudrate = atoi(s_baudrate);
	cfg->databits = atoi(s_databits);
	cfg->stopbits = atoi(s_stopbits);
	cfg->parity = 0;
	if (strcmp(s_parity, "Odd") == 0) {
		cfg->parity = 1;
	} else if (strcmp(s_parity, "Even") == 0) {
		cfg->parity = 2;
	}
}

void save_settings_to_ini(const char *port_id, const char *port, const char *baud_rate, const char *data_bits, const char *parity, const char *stop_bits)
{
	char ini_path[MAX_PATH];
	GetModuleFileName(NULL, ini_path, MAX_PATH);
	PathRemoveFileSpec(ini_path);
	PathAppend(ini_path, "config.ini");

	char section[50];
	snprintf(section, sizeof(section), "PortConfig_%s", port_id);

	WritePrivateProfileString(section, "Port", port, ini_path);
	WritePrivateProfileString(section, "BaudRate", baud_rate, ini_path);
	WritePrivateProfileString(section, "DataBits", data_bits, ini_path);
	WritePrivateProfileString(section, "Parity", parity, ini_path);
	WritePrivateProfileString(section, "StopBits", stop_bits, ini_path);

	int i;
	for (i=0;i<(sizeof(my_port)/sizeof(my_port[0]));i++)
	{
		if (strcmp(my_port[i].port_id, port_id) == 0) {
			set_port_cfg(&my_port[i], port, baud_rate, data_bits, parity, stop_bits);
			break;
		}
	}
}

void load_settings_from_ini(const char *port_id, char *port, char *baud_rate, char *data_bits, char *parity, char *stop_bits, char *auto_open)
{
	char ini_path[MAX_PATH];
	GetModuleFileName(NULL, ini_path, MAX_PATH);
	PathRemoveFileSpec(ini_path);
	PathAppend(ini_path, "config.ini");

	char section[50];
	snprintf(section, sizeof(section), "PortConfig_%s", port_id);

	GetPrivateProfileString(section, "Port", "COM1", port, sizeof(port), ini_path);
	GetPrivateProfileString(section, "BaudRate", "9600", baud_rate, sizeof(baud_rate), ini_path);
	GetPrivateProfileString(section, "DataBits", "8", data_bits, sizeof(data_bits), ini_path);
	GetPrivateProfileString(section, "Parity", "None", parity, sizeof(parity), ini_path);
	GetPrivateProfileString(section, "StopBits", "1", stop_bits, sizeof(stop_bits), ini_path);

	GetPrivateProfileString("AutoOpen", "AutoOpen", "init", auto_open, sizeof(auto_open), ini_path);
	// 如果读取到的值是默认值，说明键不存在，写入默认值
	if (strcmp(auto_open, "init") == 0) {
		WritePrivateProfileString("AutoOpen", "AutoOpen", "False", ini_path);
	}

	int i;
	for (i=0;i<(sizeof(my_port)/sizeof(my_port[0]));i++)
	{
		if (strcmp(my_port[i].port_id, port_id) == 0) {
			set_port_cfg(&my_port[i], port, baud_rate, data_bits, parity, stop_bits);
			break;
		}
	}
}

#define ID_CONFIRM 1001
#define ID_PORT_NUM 1002
#define ID_BAUD_RATE 1003
#define ID_DATA_BITS 1004
#define ID_PARITY 1005
#define ID_STOP_BITS 1006
// 窗口过程函数声明
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char selected_port[20];
    char selected_baud_rate[10];
    char selected_data_bits[5];
    char selected_parity[10];
    char selected_stop_bits[5];
    char chk_auto_open[10];
	switch (msg) {
	case WM_CREATE:
		// 加载设置
		load_settings_from_ini("ZB", selected_port, selected_baud_rate, selected_data_bits, selected_parity, selected_stop_bits, chk_auto_open);

		// 创建确认按钮
		HWND hButtonConfirm = CreateWindow(
			"BUTTON", "Confirm",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 10, 100, 20,
			hwnd,
			(HMENU)ID_CONFIRM,
			GetModuleHandle(NULL),
			NULL);
		if (!hButtonConfirm) {
			MessageBox(hwnd, "Failed to create confirm button", "Error", MB_ICONERROR);
			return -1;
		}

		// 创建端口号下拉列表
		HWND hComboBoxPort = CreateWindow(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
			120, 10, 100, 1000,
			hwnd,
			(HMENU)ID_PORT_NUM,
			GetModuleHandle(NULL),
			NULL);
		if (!hComboBoxPort) {
			MessageBox(hwnd, "Failed to create combo box for port number", "Error", MB_ICONERROR);
			return -1;
		}
		// 填充端口号下拉列表
		int port_num = 0;
		for (size_t i = 0; i < 256; i++) {
			char port_name[20];
			if (port_scan(port_name, i) == 0) {
				port_num++;
				SendMessage(hComboBoxPort, CB_ADDSTRING, 0, (LPARAM)port_name);
			}
		}
		// 设置端口号下拉列表的默认值
		if (port_num > 0) {
			int index = SendMessage(hComboBoxPort, CB_FINDSTRING, -1, (LPARAM)selected_port);
			if (index == CB_ERR) index = 0;
			SendMessage(hComboBoxPort, CB_SETCURSEL, index, 0);
		}

		// 创建波特率下拉列表
		HWND hComboBoxBaudRate = CreateWindow(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
			230, 10, 100, 1000,
			hwnd,
			(HMENU)ID_BAUD_RATE,
			GetModuleHandle(NULL),
			NULL);
		if (!hComboBoxBaudRate) {
			MessageBox(hwnd, "Failed to create combo box for baud rate", "Error", MB_ICONERROR);
			return -1;
		}
		// 填充波特率下拉列表
		const char *baudRates[] = {"4800", "9600", "19200", "38400", "57600", "115200"};
		for (int i = 0; i < 6; i++) {
			SendMessage(hComboBoxBaudRate, CB_ADDSTRING, 0, (LPARAM)baudRates[i]);
		}
		// 设置波特率下拉列表的默认值
		int index = SendMessage(hComboBoxBaudRate, CB_FINDSTRING, -1, (LPARAM)selected_baud_rate);
		if (index == CB_ERR) index = 1;
		SendMessage(hComboBoxBaudRate, CB_SETCURSEL, index, 0);

		// 创建数据位下拉列表
		HWND hComboBoxDataBits = CreateWindow(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
			10, 40, 100, 1000,
			hwnd,
			(HMENU)ID_DATA_BITS,
			GetModuleHandle(NULL),
			NULL);
		if (!hComboBoxDataBits) {
			MessageBox(hwnd, "Failed to create combo box for data bits", "Error", MB_ICONERROR);
			return -1;
		}
		// 填充数据位下拉列表
		const char *dataBits[] = {"5", "6", "7", "8"};
		for (int i = 0; i < 4; i++) {
			SendMessage(hComboBoxDataBits, CB_ADDSTRING, 0, (LPARAM)dataBits[i]);
		}
		// 设置数据位下拉列表的默认值
		index = SendMessage(hComboBoxDataBits, CB_FINDSTRING, -1, (LPARAM)selected_data_bits);
		if (index == CB_ERR) index = 3;
		SendMessage(hComboBoxDataBits, CB_SETCURSEL, index, 0);

		// 创建校验位下拉列表
		HWND hComboBoxParity = CreateWindow(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
			120, 40, 100, 1000,
			hwnd,
			(HMENU)ID_PARITY,
			GetModuleHandle(NULL),
			NULL);
		if (!hComboBoxParity) {
			MessageBox(hwnd, "Failed to create combo box for parity", "Error", MB_ICONERROR);
			return -1;
		}
		// 填充校验位下拉列表
		const char *parities[] = {"None", "Odd", "Even"};
		for (int i = 0; i < 3; i++) {
			SendMessage(hComboBoxParity, CB_ADDSTRING, 0, (LPARAM)parities[i]);
		}
		// 设置校验位下拉列表的默认值
		index = SendMessage(hComboBoxParity, CB_FINDSTRING, -1, (LPARAM)selected_parity);
		if (index == CB_ERR) index = 0;
		SendMessage(hComboBoxParity, CB_SETCURSEL, index, 0);

		// 创建停止位下拉列表
		HWND hComboBoxStopBits = CreateWindow(
			"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
			230, 40, 100, 1000,
			hwnd,
			(HMENU)ID_STOP_BITS,
			GetModuleHandle(NULL),
			NULL);
		if (!hComboBoxStopBits) {
			MessageBox(hwnd, "Failed to create combo box for stop bits", "Error", MB_ICONERROR);
			return -1;
		}
		// 填充停止位下拉列表
		const char *stopBits[] = {"1", "1.5", "2"};
		for (int i = 0; i < 3; i++) {
			SendMessage(hComboBoxStopBits, CB_ADDSTRING, 0, (LPARAM)stopBits[i]);
		}
		// 设置停止位下拉列表的默认值
		index = SendMessage(hComboBoxStopBits, CB_FINDSTRING, -1, (LPARAM)selected_stop_bits);
		if (index == CB_ERR) index = 0;
		SendMessage(hComboBoxStopBits, CB_SETCURSEL, index, 0);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CONFIRM) {
			// 处理确认按钮点击事件
			HWND hComboBoxPort = GetDlgItem(hwnd, ID_PORT_NUM);
			HWND hComboBoxBaudRate = GetDlgItem(hwnd, ID_BAUD_RATE);
			HWND hComboBoxDataBits = GetDlgItem(hwnd, ID_DATA_BITS);
			HWND hComboBoxParity = GetDlgItem(hwnd, ID_PARITY);
			HWND hComboBoxStopBits = GetDlgItem(hwnd, ID_STOP_BITS);

			int curSelPort = SendMessage(hComboBoxPort, CB_GETCURSEL, 0, 0);
			int curSelBaudRate = SendMessage(hComboBoxBaudRate, CB_GETCURSEL, 0, 0);
			int curSelDataBits = SendMessage(hComboBoxDataBits, CB_GETCURSEL, 0, 0);
			int curSelParity = SendMessage(hComboBoxParity, CB_GETCURSEL, 0, 0);
			int curSelStopBits = SendMessage(hComboBoxStopBits, CB_GETCURSEL, 0, 0);

			if (curSelPort != CB_ERR && curSelBaudRate != CB_ERR && curSelDataBits != CB_ERR && curSelParity != CB_ERR && curSelStopBits != CB_ERR) {

				SendMessage(hComboBoxPort, CB_GETLBTEXT, curSelPort, (LPARAM)selected_port);
				SendMessage(hComboBoxBaudRate, CB_GETLBTEXT, curSelBaudRate, (LPARAM)selected_baud_rate);
				SendMessage(hComboBoxDataBits, CB_GETLBTEXT, curSelDataBits, (LPARAM)selected_data_bits);
				SendMessage(hComboBoxParity, CB_GETLBTEXT, curSelParity, (LPARAM)selected_parity);
				SendMessage(hComboBoxStopBits, CB_GETLBTEXT, curSelStopBits, (LPARAM)selected_stop_bits);

				// 保存设置
				save_settings_to_ini("ZB", selected_port, selected_baud_rate, selected_data_bits, selected_parity, selected_stop_bits);

				// 在这里可以添加打开串口的逻辑
				port_init(my_port, sizeof(my_port)/sizeof(my_port[0]));

				// 销毁窗口
				DestroyWindow(hwnd);
			} else {
				MessageBox(hwnd, "Please select all options", "Error", MB_ICONERROR);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int board_init(void)
{
	char selected_port[20];
	char selected_baud_rate[10];
	char selected_data_bits[5];
	char selected_parity[10];
	char selected_stop_bits[5];
	char chk_auto_open[10];
	// 加载设置
	load_settings_from_ini("ZB", selected_port, selected_baud_rate, selected_data_bits, selected_parity, selected_stop_bits, chk_auto_open);
	if (strcmp(chk_auto_open, "True") == 0) {
		port_init(my_port, sizeof(my_port)/sizeof(my_port[0]));
		return 0;
	}

	const char *title = "Select Serial Port";
	const char *class_name = "Grway";

	WNDCLASS wc = {0};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = class_name;

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Register fail", "Error", MB_ICONERROR);
		return -1;
	}

	// 获取屏幕的宽度和高度
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的宽度和高度
	int windowWidth = 360;
	int windowHeight = 110;

	// 计算窗口左上角的位置，使其居中
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

	HWND hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		class_name,
		title,
		WS_OVERLAPPEDWINDOW,

		x, y, windowWidth, windowHeight,

		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (!hWnd) {
		MessageBox(NULL, "Register fail", "Error", MB_ICONERROR);
		return -1;
	}

	// 设置窗口置顶
	SetWindowPos(hWnd, HWND_TOPMOST, x, y, windowWidth, windowHeight, SWP_SHOWWINDOW);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}