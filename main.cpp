/*
 *  findTool
 *  Qt 5.4.2 MinGW
 *  Qt Creator 3.4.1
 *
 */

// include C
#include <iostream>
#include <dirent.h>
#include <list>
#include <string>
#include <stdlib.h>

// include Qt
//#include <QDirIterator>

void help(void);
void find_file_type(char *path, std::list<std::string> &output);

// 程式進入口
int main(int argc, char *argv[])
{
    char *fileType = NULL;
    int fileTypeLen = 0;
    char *path = NULL;
    std::list<std::string> outputList;
    std::list<std::string>::iterator it;
    int strLen = 0;
    int i = 0;

    // 參數處理
    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (stricmp(argv[i], "--fileType") == 0) {
                if (i + 1 >= argc) {
                    help();
                    return 0;
                }
                fileType = argv[i + 1];
                fileTypeLen = strlen(argv[i + 1]);
                i++;
                continue;
            } else if (stricmp(argv[i], "--path") == 0) {
                if (i + 1 >= argc) {
                    help();
                    return 0;
                }
                path = argv[i + 1];
                i++;
                continue;
            } else {
                help();
                return 0;
            }
        }
    } else {
        help();
        return 0;
    }

    //*
    outputList.clear(); // 初始化
    find_file_type(path, outputList);   // 尋找檔案
    // 逐一取出
    for (it = outputList.begin(); it != outputList.end(); it++) {
        // 取得資料長度
        strLen = it->length();
        // 輸出結尾符合fileType的資料
        if (strLen > fileTypeLen && stricmp(&it->data()[strLen - fileTypeLen], fileType) == 0) {
            std::cout << (*it) << std::endl;
        }
    }
    //*/

    /* only Qt
    QStringList strList(QString("*.%1").arg(fileType));
    QDirIterator dirIt(path, strList, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        std::cout << dirIt.next().toStdString().data() << std::endl;
    }
    //*/

    // 離開程式
    return 0;
}

// 尋找檔案
// 說明 取得path路徑下所有檔案存至output內
// 參數 path:路徑, output:輸出清單
void find_file_type(char *path, std::list<std::string> &output)
{
    DIR *dp = NULL;
    struct dirent *filename = NULL;
    char *pathStr = NULL;
    int pathStrLen = 0;
    char gap = '\\';    // 目錄分隔符號

    // 防呆
    if (path == NULL) {
        return;
    }

    // 非目錄
    if ((dp = opendir(path)) == NULL) {
        // 存檔名
        output.push_back(path);
        return;
    }

    // 逐一取資料
    while ((filename = readdir(dp)) != NULL) {
        // 跳過當前及母目錄
        if (strcmp(filename->d_name, "..") == 0 || strcmp(filename->d_name, ".") == 0) {
            continue;
        }
        // 計算目錄總長度(路徑+分隔符號+檔名+結束字元)
        pathStrLen = (strlen(path) + 1 + strlen(filename->d_name) + 1);
        // 動態配置記憶體
        pathStr = (char *)malloc(sizeof(char) * pathStrLen);
        // 初始化
        memset(pathStr, 0x00, pathStrLen);
        // 組路徑加名稱
        sprintf(pathStr, "%s%c%s", path, gap, filename->d_name);
        // 繼續下一層目錄
        find_file_type(pathStr, output);
    }

    // 關閉
    closedir(dp);
    // 釋放
    free(pathStr);
}

// help資訊
void help(void)
{
    std::cout << "findTool.exe --fileType [fileType] --path [folderName]" << std::endl;
    std::cout << "Example: findTool.exe --fileType exe --path app" << std::endl;
}
