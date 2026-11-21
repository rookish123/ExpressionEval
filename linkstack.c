#define _CRT_SECURE_NO_WARNINGS 1
#include "stack.h"

void InitLinkStack(LinkStack *S)
{
    *S = NULL;
}

bool LinkStackEmpty(LinkStack S)
{
    return S == NULL;
}

bool LinkPush(LinkStack *S, int e)
{
    LNode *p = (LNode *)malloc(sizeof(LNode));
    if (p == NULL) return false;
    p->data = e;
    p->next = *S;
    *S = p;
    return true;
}

bool LinkPop(LinkStack *S, int *e)
{
    if (LinkStackEmpty(*S)) return false;
    LNode *p = *S;
    *e = p->data;
    *S = p->next;
    free(p);
    return true;
}

void ClearLinkStack(LinkStack *S)
{
    int tmp;
    while (LinkPop(S, &tmp)) { }
}

void DecToM(int n, int m)
{
    LinkStack S;
    int rem;

    InitLinkStack(&S);

    if (n == 0)
    {
        printf("0");
        return;
    }

    while (n > 0)
    {
        LinkPush(&S, n % m);
        n /= m;
    }

    while (!LinkStackEmpty(S))
    {
        LinkPop(&S, &rem);
        if (rem < 10) printf("%d", rem);
        else printf("%c", 'A' + rem - 10);
    }
}

void RunBaseConvert(void)
{
    int num, base;
    int ch;

    printf("\n--- 2. 十进制转 M 进制（链栈实现） ---\n");
    printf("请输入十进制数和目标进制（2~16），用空格分开，例如 255 16\n");

    while (1)
    {
        printf("请输入（输入 q 返回主菜单）：");

        char line[128];
        if (fgets(line, sizeof(line), stdin) == NULL) return;

        if (strchr(line, '\n') == NULL && !feof(stdin))
        {
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("输入过长，请重新输入。\n\n");
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == 'q' || line[0] == 'Q') return;
        if (line[0] == '\0') continue;

        if (sscanf(line, "%d %d", &num, &base) != 2)
        {
            printf("输入格式错误，请按“十进制数 目标进制”的格式输入。\n\n");
            continue;
        }
        if (base < 2 || base > 16)
        {
            printf("进制数超出范围：M 必须在 2 到 16 之间。\n\n");
            continue;
        }
        if (num < 0)
        {
            printf("被转换的数不能为负数。\n\n");
            continue;
        }

        printf("结果：%d 的 %d 进制表示为 ", num, base);
        DecToM(num, base);
        printf("\n\n");
    }
}
