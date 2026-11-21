#define _CRT_SECURE_NO_WARNINGS 1
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAXSIZE 100
#define OK 1
#define ERROR 0

typedef struct
{
    double data[MAXSIZE];
    int top;
} NumStack;

typedef struct
{
    char data[MAXSIZE];
    int top;
} CharStack;

void InitNumStack(NumStack *s);
bool NumStackEmpty(NumStack *s);
bool NumPush(NumStack *s, double x);
bool NumPop(NumStack *s, double *x);
bool NumTop(NumStack *s, double *x);

void InitCharStack(CharStack *s);
bool CharStackEmpty(CharStack *s);
bool CharPush(CharStack *s, char x);
bool CharPop(CharStack *s, char *x);
bool CharTop(CharStack *s, char *x);

typedef struct LNode
{
    int data;
    struct LNode *next;
} LNode, *LinkStack;

void InitLinkStack(LinkStack *S);
bool LinkStackEmpty(LinkStack S);
bool LinkPush(LinkStack *S, int e);
bool LinkPop(LinkStack *S, int *e);
void ClearLinkStack(LinkStack *S);

void RunExpressionCalc(void);
void RunBaseConvert(void);
void RunBracketCheck(void);
void DecToM(int n, int m);

#endif
