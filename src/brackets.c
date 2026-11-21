#define _CRT_SECURE_NO_WARNINGS 1
#include "stack.h"

typedef struct
{
    char c;
    int no;
} BItem;

typedef struct
{
    BItem *data;
    int top;
    int cap;
} BStack;

static bool BInit(BStack *s)
{
    s->cap = 32;
    s->top = -1;
    s->data = (BItem *)malloc(sizeof(BItem) * s->cap);
    return s->data != NULL;
}

static bool BPush(BStack *s, BItem it)
{
    if (s->top + 1 >= s->cap)
    {
        int nc = s->cap * 2;
        BItem *p = (BItem *)realloc(s->data, sizeof(BItem) * nc);
        if (p == NULL) return false;
        s->data = p;
        s->cap = nc;
    }
    s->data[++s->top] = it;
    return true;
}

static bool BPop(BStack *s, BItem *it)
{
    if (s->top == -1) return false;
    *it = s->data[s->top--];
    return true;
}

static bool BEmpty(BStack *s) { return s->top == -1; }

static void BFree(BStack *s)
{
    free(s->data);
    s->data = NULL;
    s->top = -1;
    s->cap = 0;
}

static bool IsOpen(char c)  { return c == '(' || c == '[' || c == '{'; }
static bool IsClose(char c) { return c == ')' || c == ']' || c == '}'; }

static bool Match(char left, char right)
{
    return (left == '(' && right == ')') ||
           (left == '[' && right == ']') ||
           (left == '{' && right == '}');
}

static int CheckBracketText(const char *s)
{
    BStack st;
    BItem it, left;
    int no = 0;
    int pairs = 0;
    int result = 1;

    if (!BInit(&st))
    {
        printf("内存不足。\n");
        return 0;
    }

    for (int i = 0; s[i]; i++)
    {
        char c = s[i];
        if (!IsOpen(c) && !IsClose(c)) continue;

        no++;

        if (IsOpen(c))
        {
            it.c = c;
            it.no = no;
            if (!BPush(&st, it))
            {
                printf("括号太多，内存不足。\n");
                BFree(&st);
                return 0;
            }
        }
        else
        {
            if (!BPop(&st, &left))
            {
                printf("括号不配对：第 %d 个括号 '%c' 多余，前面没有与之配对的左括号。\n", no, c);
                result = 0;
                break;
            }
            if (!Match(left.c, c))
            {
                printf("括号不配对：第 %d 个括号 '%c' 与第 %d 个括号 '%c' 类型不匹配。\n",
                       no, c, left.no, left.c);
                result = 0;
                break;
            }
            pairs++;
        }
    }

    if (result == 1 && !BEmpty(&st))
    {
        int unclosed = 0;
        int firstNo = 0;
        char firstChar = '\0';

        while (BPop(&st, &left))
        {
            unclosed++;
            firstNo = left.no;
            firstChar = left.c;
        }

        printf("括号不配对：第 %d 个括号 '%c' 缺少右括号，共有 %d 个左括号没有闭合。\n",
               firstNo, firstChar, unclosed);
        result = 0;
    }

    if (result == 1)
    {
        printf("括号配对正确，共 %d 对括号。\n", pairs);
    }

    BFree(&st);
    return result;
}

void RunBracketCheck(void)
{
    char line[1024];
    int len;

    printf("\n--- 3. 括号匹配检查 ---\n");
    printf("直接输入一串括号即可，例如 {[()]}、([{}])；也可以输 ([)] 看看它是怎么报错的\n");
    printf("除 ( ) [ ] { } 以外的字符会被忽略，输入 q 返回主菜单\n\n");

    while (1)
    {
        printf("请输入：");
        if (fgets(line, sizeof(line), stdin) == NULL) return;

        if (strchr(line, '\n') == NULL && !feof(stdin))
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("输入过长，请控制在 1000 个字符以内。\n\n");
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';
        len = (int)strlen(line);

        if (len == 0) continue;
        if (line[0] == 'q' || line[0] == 'Q') return;

        CheckBracketText(line);
        printf("\n");
    }
}
