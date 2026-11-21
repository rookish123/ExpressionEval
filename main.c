#define _CRT_SECURE_NO_WARNINGS 1
#include "stack.h"

static void Pause(void)
{
    char line[64];
    printf("按回车键返回主菜单...");
    if (fgets(line, sizeof(line), stdin) == NULL) { }
}

static void ShowMenu(void)
{
    printf("\n=================================================\n");
    printf("          数据结构实验（选做）演示程序\n");
    printf("=================================================\n");
    printf("   1. 表达式求值（中缀转后缀 + 顺序栈）\n");
    printf("   2. 十进制转 M 进制（链栈）\n");
    printf("   3. 括号匹配检查（输入括号串）\n");
    printf("   0. 退出\n");
    printf("-------------------------------------------------\n");
    printf("请选择功能（0-3）：");
}

int main(void)
{
    char line[64];

    while (1)
    {
        ShowMenu();

        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '0') break;

        switch (line[0])
        {
        case '1':
            RunExpressionCalc();
            Pause();
            break;
        case '2':
            RunBaseConvert();
            Pause();
            break;
        case '3':
            RunBracketCheck();
            Pause();
            break;
        default:
            printf("选择无效，请输入 0 到 3 之间的数字。\n");
            break;
        }
    }

    printf("\n程序结束。\n");
    return 0;
}
