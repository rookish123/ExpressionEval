#define _CRT_SECURE_NO_WARNINGS 1
#include "stack.h"

void InitNumStack(NumStack *s) { s->top = -1; }
bool NumStackEmpty(NumStack *s) { return s->top == -1; }

bool NumPush(NumStack *s, double x)
{
    if (s->top >= MAXSIZE - 1) return false;
    s->data[++s->top] = x;
    return true;
}

bool NumPop(NumStack *s, double *x)
{
    if (NumStackEmpty(s)) return false;
    *x = s->data[s->top--];
    return true;
}

bool NumTop(NumStack *s, double *x)
{
    if (NumStackEmpty(s)) return false;
    *x = s->data[s->top];
    return true;
}

void InitCharStack(CharStack *s) { s->top = -1; }
bool CharStackEmpty(CharStack *s) { return s->top == -1; }

bool CharPush(CharStack *s, char x)
{
    if (s->top >= MAXSIZE - 1) return false;
    s->data[++s->top] = x;
    return true;
}

bool CharPop(CharStack *s, char *x)
{
    if (CharStackEmpty(s)) return false;
    *x = s->data[s->top--];
    return true;
}

bool CharTop(CharStack *s, char *x)
{
    if (CharStackEmpty(s)) return false;
    *x = s->data[s->top];
    return true;
}

bool In(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

static int Priority(char c)
{
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

char Precede(char t1, char t2)
{
    if (t1 == '(' && t2 == ')') return '=';
    if (t1 == '(' || t2 == '(') return '<';
    if (t2 == ')') return '>';

    int p1 = Priority(t1), p2 = Priority(t2);
    if (p1 > p2) return '>';
    if (p1 < p2) return '<';
    return '=';
}

bool Operate(double a, char theta, double b, double *result)
{
    switch (theta)
    {
    case '+': *result = a + b; return true;
    case '-': *result = a - b; return true;
    case '*': *result = a * b; return true;
    case '/':
        if (fabs(b) < 1e-12) return false;
        *result = a / b;
        return true;
    default:
        return false;
    }
}

#define MAXTOKEN 128
#define TOKLEN 32

static bool IsUnaryContext(char prev)
{
    return prev == '\0' || prev == '(' ||
           prev == '+' || prev == '-' || prev == '*' || prev == '/';
}

static bool ReadNumber(const char *s, int *idx, char *buf)
{
    int i = *idx, k = 0, dots = 0, digits = 0;

    while (s[i] && ((s[i] >= '0' && s[i] <= '9') || s[i] == '.'))
    {
        if (s[i] == '.')
        {
            if (++dots > 1) return false;
        }
        else digits++;

        if (k < TOKLEN - 1) buf[k++] = s[i];
        i++;
    }

    if (digits == 0) return false;

    buf[k] = '\0';
    *idx = i;
    return true;
}

static bool Tokenize(const char *s, char tok[][TOKLEN], int *count)
{
    int n = 0, i = 0, len = (int)strlen(s);
    char prev = '\0';

    while (i < len)
    {
        char c = s[i];

        if (c == ' ' || c == '\t') { i++; continue; }

        if ((c >= '0' && c <= '9') || c == '.')
        {
            char num[TOKLEN];
            if (!ReadNumber(s, &i, num)) return false;
            if (n >= MAXTOKEN) return false;
            strcpy(tok[n++], num);
            prev = '0';
        }
        else if (In(c))
        {
            if ((c == '-' || c == '+') && IsUnaryContext(prev))
            {
                if (c == '+')
                {
                    prev = c;
                    i++;
                    continue;
                }

                int j = i + 1;
                while (j < len && (s[j] == ' ' || s[j] == '\t')) j++;
                if (j >= len) return false;

                if (s[j] == '(')
                {
                    if (n + 2 > MAXTOKEN) return false;
                    strcpy(tok[n++], "0");
                    strcpy(tok[n++], "-");
                    prev = '-';
                    i++;
                    continue;
                }
                else if ((s[j] >= '0' && s[j] <= '9') || s[j] == '.')
                {
                    char num[TOKLEN];
                    int k = j;
                    if (!ReadNumber(s, &k, num)) return false;
                    if (n + 5 > MAXTOKEN) return false;
                    strcpy(tok[n++], "(");
                    strcpy(tok[n++], "0");
                    strcpy(tok[n++], "-");
                    strcpy(tok[n++], num);
                    strcpy(tok[n++], ")");
                    prev = '0';
                    i = k;
                    continue;
                }
                return false;
            }

            if (n >= MAXTOKEN) return false;
            tok[n][0] = c;
            tok[n][1] = '\0';
            n++;
            prev = c;
            i++;
        }
        else
        {
            return false;
        }
    }

    *count = n;
    return n > 0;
}

bool InfixToPostfix(const char *infix, char tok[][TOKLEN], int *count)
{
    char t[MAXTOKEN][TOKLEN];
    char out[MAXTOKEN][TOKLEN];
    int n = 0, pn = 0;
    char top;
    CharStack op;

    if (!Tokenize(infix, t, &n)) return false;

    InitCharStack(&op);

    for (int i = 0; i < n; i++)
    {
        if (!In(t[i][0]))
        {
            if (pn >= MAXTOKEN) return false;
            strcpy(out[pn++], t[i]);
        }
        else if (t[i][0] == '(')
        {
            if (!CharPush(&op, '(')) return false;
        }
        else if (t[i][0] == ')')
        {
            bool matched = false;
            while (CharPop(&op, &top))
            {
                if (top == '(') { matched = true; break; }
                if (pn >= MAXTOKEN) return false;
                out[pn][0] = top;
                out[pn][1] = '\0';
                pn++;
            }
            if (!matched) return false;
        }
        else
        {
            while (CharTop(&op, &top) && top != '(' &&
                   Priority(top) >= Priority(t[i][0]))
            {
                CharPop(&op, &top);
                if (pn >= MAXTOKEN) return false;
                out[pn][0] = top;
                out[pn][1] = '\0';
                pn++;
            }
            if (!CharPush(&op, t[i][0])) return false;
        }
    }

    while (CharPop(&op, &top))
    {
        if (top == '(') return false;
        if (pn >= MAXTOKEN) return false;
        out[pn][0] = top;
        out[pn][1] = '\0';
        pn++;
    }

    for (int k = 0; k < pn; k++) strcpy(tok[k], out[k]);
    *count = pn;
    return pn > 0;
}

bool EvalPostfix(char tok[][TOKLEN], int n, double *result)
{
    NumStack st;
    InitNumStack(&st);

    for (int i = 0; i < n; i++)
    {
        if (!In(tok[i][0]))
        {
            if (!NumPush(&st, atof(tok[i]))) return false;
        }
        else
        {
            double left, right, value;
            if (!NumPop(&st, &right)) return false;
            if (!NumPop(&st, &left)) return false;
            if (!Operate(left, tok[i][0], right, &value)) return false;
            if (!NumPush(&st, value)) return false;
        }
    }

    if (st.top != 0) return false;
    return NumPop(&st, result);
}

static bool CheckLegal(const char *s)
{
    int depth = 0, digits = 0;
    for (int i = 0; s[i]; i++)
    {
        char c = s[i];
        if (c == ' ' || c == '\t') continue;
        if (!In(c) && !(c >= '0' && c <= '9') && c != '.') return false;
        if (c >= '0' && c <= '9') digits++;
        if (c == '(') depth++;
        if (c == ')') { depth--; if (depth < 0) return false; }
    }
    return depth == 0 && digits > 0;
}

void RunExpressionCalc(void)
{
    char line[512];

    printf("\n--- 1. 表达式求值（中缀转后缀） ---\n");
    printf("支持 + - * / 和括号，操作数可以是多位整数、小数或负数；\n");
    printf("例如：(3+4)*5-6/2 或 12.5*(3-1)，输入 q 返回主菜单\n\n");

    while (1)
    {
        printf("请输入中缀表达式：");
        if (fgets(line, sizeof(line), stdin) == NULL) return;

        if (strchr(line, '\n') == NULL && !feof(stdin))
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("输入过长，请把表达式控制在 500 个字符以内。\n\n");
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';

        int len = (int)strlen(line);
        while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '=')) line[--len] = '\0';

        if (line[0] == 'q' || line[0] == 'Q') return;
        if (line[0] == '\0') continue;

        if (!CheckLegal(line))
        {
            printf("输入不合法：只能包含数字、小数点、+ - * / 和括号，且括号要成对。\n\n");
            continue;
        }

        char tok[MAXTOKEN][TOKLEN];
        int n = 0;
        if (!InfixToPostfix(line, tok, &n))
        {
            printf("转换失败：请检查括号是否配对、小数点是否写错。\n\n");
            continue;
        }

        printf("后缀表达式：");
        for (int i = 0; i < n; i++) printf("%s ", tok[i]);
        printf("\n");

        double value;
        if (!EvalPostfix(tok, n, &value))
        {
            printf("求值失败：表达式有误，或出现除以 0。\n\n");
            continue;
        }

        printf("计算结果：%.6g\n\n", value);
    }
}
