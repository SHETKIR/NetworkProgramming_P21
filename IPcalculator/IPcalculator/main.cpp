#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>		
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"	

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}
CHAR* ParseAddress(CHAR sz_address[], DWORD dw_address)
{
	sprintf
	(
		sz_address,
		"%s:\t\t\t%i.%i.%i.%i",
		FIRST_IPADDRESS(dw_address),
		SECOND_IPADDRESS(dw_address),
		THIRD_IPADDRESS(dw_address),
		FOURTH_IPADDRESS(dw_address)
	);
	return sz_address;
}
CHAR* ParseAddress(CHAR sz_address[], CONST CHAR sz_description[], DWORD dw_address)
{
	sprintf
	(
		sz_address,
		"%s%i.%i.%i.%i", sz_description,
		FIRST_IPADDRESS(dw_address),
		SECOND_IPADDRESS(dw_address),
		THIRD_IPADDRESS(dw_address),
		FOURTH_IPADDRESS(dw_address)
	);
	return sz_address;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int a = 2;
	{
		int a = 2;
	}
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hUpDown = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		SendMessage(hUpDown, UDM_SETRANGE32, 0, 32);

		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		SetFocus(hIPaddress);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			CONST INT SIZE = 256;
			CHAR sz_info[SIZE]{};

			HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dw_address;
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				INT first = FIRST_IPADDRESS(dw_address);
				INT second = SECOND_IPADDRESS(dw_address);
				INT third = THIRD_IPADDRESS(dw_address);
				INT fourth = FOURTH_IPADDRESS(dw_address);

				sprintf_s(sz_info, SIZE, "Info:\nFirst: %i, Second: %i, Third: %i, Forth: %i", first, second, third, fourth);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);


				if (first < 128)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"8");
				else if (first < 192)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"16");
				else if (first < 224)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"24");
			}
		}
		break;
		case IDC_IPMASK:
		{
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			DWORD dw_mask = ~0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CONST INT SIZE_PREFIX = 8;
				CHAR sz_buffer[SIZE_PREFIX];
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE_PREFIX, (LPARAM)sz_buffer);
				INT shift = atoi(sz_buffer);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask<<= (32 - shift));

				CONST INT SIZE = 256;
				CHAR sz_info[SIZE]{};
				CHAR sz_network_address[SIZE]{};
				CHAR sz_broadcast_address[SIZE]{};
				CHAR sz_number_of_hosts[SIZE]{};
				CHAR sz_first_address[SIZE] = "Начальный IP-адрес:";
				CHAR sz_last_address[SIZE] = "Последний IP-адрес:";

				DWORD dw_address = 0;
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				DWORD dw_network_address = dw_address & dw_mask;
				ParseAddress(sz_network_address, "Адрес сети:\t\t\t", dw_network_address);

				DWORD dw_broadcast_address = ~dw_mask | dw_network_address;
				ParseAddress(sz_broadcast_address, "Широковещательный адрес:\t", dw_broadcast_address);

				DWORD dw_number_of_hosts = dw_broadcast_address - dw_network_address - 1;
				sprintf(sz_number_of_hosts, "Количество узлов:\t\t%u", dw_number_of_hosts);

				ParseAddress(sz_first_address, "Начальный IP-адрес:\t\t", dw_network_address + 1);
				ParseAddress(sz_last_address, "Последний IP-адрес:\t\t", dw_broadcast_address - 1);

				sprintf
				(
					sz_info,
					"Info:\n%s\n%s\n%s\n%s\n%s",
					sz_network_address,
					sz_broadcast_address,
					sz_number_of_hosts,
					sz_first_address,
					sz_last_address
				);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
			}
		}
		break;
		case IDOK: MessageBox(hwnd, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_NOTIFY:
	{
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		switch (wParam)
		{
		case IDC_IPMASK:
		{
			DWORD dw_mask = 0;
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
			int i = 32;
			for (; dw_mask & 1 ^ 1; i--)dw_mask >>= 1;
			CHAR sz_prefix[5]{};
			sprintf(sz_prefix, "%i", i);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
		}
		break;
		}
		break;
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}