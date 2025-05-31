#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <fstream>
#include <sstream>
#include <limits> // For std::numeric_limits

// 颜色定义
#define RESET_COLOR 7     // 默认颜色
#define RED_TEXT 12       // 红色
#define GREEN_TEXT 10     // 绿色
#define YELLOW_TEXT 14    // 黄色
#define CYAN_TEXT 11      // 青色

enum Language { ENGLISH, CHINESE };
Language currentLanguage = ENGLISH; // 默认为 English

// 变更控制台颜色
void color(int a) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
}

// 调整控制台窗口大小并命名
void setConsoleSize(int width, int height, const std::string& title) {
    SetConsoleTitleA(title.c_str());

    // 获取控制台句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 设置控制台缓冲区大小
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 设置控制台窗口大小
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

// 检查指定进程是否正在运行
bool isProcessRunning(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "Unable to create process snapshot." << std::endl;
        else
            std::cerr << "无法创建进程快照。" << std::endl;
        color(RESET_COLOR);
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (processName.compare(processEntry.szExeFile) == 0) {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);
    return false;
}

// 显示菜单
void showMenu() {
    std::cout << "\n";
    color(CYAN_TEXT);
    if (currentLanguage == ENGLISH) {
        std::cout << "  _   _           _                  _    \n";
	    std::cout << " | | | |  _ __   | |   ___     ___  | | __\n";
	    std::cout << " | | | | | '_ \\  | |  / _ \\   / __| | |/ /\n";
	    std::cout << " | |_| | | | | | | | | (_) | | (__  |   < \n";
	    std::cout << "  \\___/  |_| |_| |_|  \\___/   \\___| |_|\\_\\\n";
	    std::cout << "                                          \n";
	    
	    std::cout << " |_   _|   ___     ___   | |              \n";
	    std::cout << "   | |    / _ \\   / _ \\  | |              \n";
	    std::cout << "   | |   | (_) | | (_) | | |              \n";
	    std::cout << "   |_|    \\___/   \\___/  |_|              \n";
	    
	    std::cout << "-----------------------------" << std::endl;
	    std::cout << "Github:GreenHub-UnlockTool\n";
	    std::cout << " \n";
	    std::cout << "Ver:1.0.3 END \n";
	    std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "1. Activation Code Generator\n";
        std::cout << "2. Extend Time\n";
        std::cout << "3. Launch GreenHub\n";
        std::cout << "4. Switch to Chinese\n";
    } else {
        std::cout << "  _   _           _                  _    \n";
	    std::cout << " | | | |  _ __   | |   ___     ___  | | __\n";
	    std::cout << " | | | | | '_ \\  | |  / _ \\   / __| | |/ /\n";
	    std::cout << " | |_| | | | | | | | | (_) | | (__  |   < \n";
	    std::cout << "  \\___/  |_| |_| |_|  \\___/   \\___| |_|\\_\\\n";
	    std::cout << "                                          \n";
	    std::cout << "  _____                   _               \n";
	    std::cout << " |_   _|   ___     ___   | |              \n";
	    std::cout << "   | |    / _ \\   / _ \\  | |              \n";
	    std::cout << "   | |   | (_) | | (_) | | |              \n";
	    std::cout << "   |_|    \\___/   \\___/  |_|              \n";
	    std::cout << "                                          \n";
	    std::cout << "Github.com/Hy1Fly/GreenHub-UnlockTool\n";
	    std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "1. 激活码生成器\n";
        std::cout << "2. 时间延长\n";
        std::cout << "3. 启动 GreenHub\n";
        std::cout << "4. 切换到 English\n";
    }
    color(RESET_COLOR);
}

// 切换语言
void switchLanguage() {
    if (currentLanguage == ENGLISH) {
        currentLanguage = CHINESE;
    } else {
        currentLanguage = ENGLISH;
    }
}

// 获取用户配置路径
std::string getUserProfilePath() {
    char userProfilePath[MAX_PATH];
    if (GetEnvironmentVariableA("USERPROFILE", userProfilePath, MAX_PATH)) {
        return std::string(userProfilePath);
    } else {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "Unable to get USERPROFILE environment variable." << std::endl;
        else
            std::cerr << "无法获取 USERPROFILE 环境变量。" << std::endl;
        color(RESET_COLOR);
        return "";
    }
}

// 修改配置文件中的分钟数
void modifyConfigJson() {
    std::string userProfilePath = getUserProfilePath();
    if (userProfilePath.empty()) return;

    const std::string configPath = userProfilePath + "\\AppData\\Roaming\\GreenHub\\config.json";
    std::ifstream inputFile(configPath);
    std::vector<std::string> lines;
    std::string line;

    if (!inputFile.is_open()) {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "Unable to open config.json file." << std::endl;
        else
            std::cerr << "无法打开 config.json 文件。" << std::endl;
        color(RESET_COLOR);
        return;
    }

    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    if (lines.size() < 21) {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "config.json file has less than 21 lines." << std::endl;
        else
            std::cerr << "config.json 文件少于21行。" << std::endl;
        color(RESET_COLOR);
        return;
    }

    std::string minutesLine = lines[20];
    color(GREEN_TEXT);
    if (currentLanguage == ENGLISH)
        std::cout << "\nCurrent line 21: " << RESET_COLOR << minutesLine << std::endl;
    else
        std::cout << "\n当前第21行: " << RESET_COLOR << minutesLine << std::endl;

    size_t pos = minutesLine.find("\"minutes\":");
    if (pos == std::string::npos) {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "\"minutes\": not found on line 21." << std::endl;
        else
            std::cerr << "\"minutes\": 在第21行中未找到。" << std::endl;
        color(RESET_COLOR);
        return;
    }

    int newTime;
    if (currentLanguage == ENGLISH)
        std::cout << "Enter new minute value: ";
    else
        std::cout << "请输入新的分钟值: ";

    while (!(std::cin >> newTime) || newTime < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cout << "Invalid input. Please enter a valid positive integer: ";
        else
            std::cout << "无效输入。请输入一个有效的正整数: ";
        color(RESET_COLOR);
    }

    std::stringstream ss;
    ss << "\"minutes\": " << newTime;
    minutesLine.replace(pos, minutesLine.length() - pos, ss.str());
    lines[20] = minutesLine;

    std::ofstream outputFile(configPath);
    for (const auto& l : lines) {
        outputFile << l << std::endl;
    }
    outputFile.close();

    color(GREEN_TEXT);
    if (currentLanguage == ENGLISH)
        std::cout << "\"minutes\" value updated successfully." << RESET_COLOR << std::endl;
    else
        std::cout << "\"minutes\" 值更新成功。" << RESET_COLOR << std::endl;
}

// 启动新进程
void startProcessInNewWindow(const std::string& exePath, const std::string& processName) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(exePath.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        color(GREEN_TEXT);
        if (currentLanguage == ENGLISH)
            std::cout << "Successfully started " << processName << RESET_COLOR << std::endl;
        else
            std::cout << "成功启动 " << processName << RESET_COLOR << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        color(RED_TEXT);
        if (currentLanguage == ENGLISH)
            std::cerr << "Failed to start " << processName << ", error code: " << GetLastError() << RESET_COLOR << std::endl;
        else
            std::cerr << "无法启动 " << processName << "，错误代码: " << GetLastError() << RESET_COLOR << std::endl;
    }
}

int main() {
    // 设置控制台大小为 79x20 字符大小，并命名为 "GreenHub"
    setConsoleSize(79, 20, " ");

    const std::string processName = "GreenHub.exe";
    const std::string executablePath = "C:\\Program Files\\GreenHub\\GreenHub.exe";

    // 检查 GreenHub 是否在运行
    if (isProcessRunning(processName)) {
        color(GREEN_TEXT);
        if (currentLanguage == ENGLISH)
            std::cout << processName << " is already running" << RESET_COLOR << std::endl;
        else
            std::cout << processName << " GH 已经在运行" << RESET_COLOR << std::endl;
    } else {
        color(YELLOW_TEXT);
        if (currentLanguage == ENGLISH)
            std::cout << processName << " is not running, attempting to launch" << RESET_COLOR << std::endl;
        else
            std::cout << processName << " GH 未运行，尝试启动" << RESET_COLOR << std::endl;
        startProcessInNewWindow(executablePath, processName);
    }

    int choice;
    do {
        showMenu();
        std::cout << (currentLanguage == ENGLISH ? "Enter your choice: " : "请输入你的选择: ");
        std::cin >> choice;

        switch (choice) {
            case 1: {
                color(YELLOW_TEXT);
                if (currentLanguage == ENGLISH)
                    std::cout << "Launching ACG..." << RESET_COLOR << std::endl;
                else
                    std::cout << "正在启动 ACG..." << RESET_COLOR << std::endl;
                startProcessInNewWindow("ACG.exe", "ACG.exe");
                break;
            }
            case 2:
                modifyConfigJson();
                break;
            case 3:
                startProcessInNewWindow(executablePath, processName);
                break;
            case 4:
                switchLanguage();
                break;
            default:
                color(RED_TEXT);
                if (currentLanguage == ENGLISH)
                    std::cout << "Invalid choice." << RESET_COLOR << std::endl;
                else
                    std::cout << "无效的选择。" << RESET_COLOR << std::endl;
                break;
        }
    } while (choice != 0);

    return 0;
}
