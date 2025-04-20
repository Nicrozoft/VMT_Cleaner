#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

bool isAdmin() {
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL b = AllocateAndInitializeSid(
	             &NtAuthority,
	             2,
	             SECURITY_BUILTIN_DOMAIN_RID,
	             DOMAIN_ALIAS_RID_ADMINS,
	             0, 0, 0, 0, 0, 0,
	             &AdministratorsGroup);
	if(b) {
		CheckTokenMembership(NULL, AdministratorsGroup, &b);
		FreeSid(AdministratorsGroup);
	}
	return b == TRUE;
}
void AdminRun(const char* strApp) {
    SHELLEXECUTEINFO execinfo;
    memset(&execinfo, 0, sizeof(execinfo));
    execinfo.lpFile = strApp;
    execinfo.cbSize = sizeof(execinfo);
    execinfo.lpVerb = "runas";
    execinfo.fMask = SEE_MASK_NO_CONSOLE;
    execinfo.nShow = SW_SHOWDEFAULT;
    ShellExecuteEx(&execinfo);
}
string exePath() {
	char buffer[PATH_MAX * 2 + 1] = { 0 };
	int n = -1;
	n = GetModuleFileNameA(NULL, buffer, sizeof(buffer));
	string filePath;
	if (n <= 0) {
		filePath = ".\\";
	} else {
		filePath = buffer;
	}
	return filePath;
}

int main() {
	// 获取管理员权限
	if (isAdmin()) goto clean;
	AdminRun(exePath().c_str());
	return 0;
clean:
	system("title Nicrozoft VMTools 清理工具");
	cout << "该工具由 Nicrozoft 编写，用于在物理机上移除 VMware Tools。\n如果你正在使用物理机，请输入 Y 以继续，否则请输入其他内容以退出。";
	char c;
	cin >> c;
	switch (c) {
		case 'Y':
		case 'y':
			break;
		default:
			exit(0);
	}
	system("cls");
	cout << "清理即将开始，";
	system("pause");
	// 清除服务
	system("sc stop VGAuthService");
	system("sc stop VMTools");
	system("sc stop vm3dservice");
	system("sc delete VGAuthService");
	system("sc delete VMTools");
	system("sc delete vm3dservice");	
	// 结束进程
	system("taskkill -im vmtoolsd.exe -f");
	system("taskkill -im VGAuthService.exe -f");
	system("taskkill -im vm3dservice.exe -f");
	// 移除注册表
	system("reg delete \"HKEY_LOCAL_MACHINE\\SOFTWARE\\VMware, Inc.\" /va /f");
	// 移除文件
	system("rd \"C:\\Program Files\\VMware\" /s /q");
	// 修改 DriverDesc 以防止被误判为虚拟机
	system("reg add \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000\" /v DriverDesc /t reg_sz /d \"\" /f");
	system("cls");
	cout << "清理已完成\n";
	system("pause");
	return 0;
}
