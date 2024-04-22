#pragma once

#include <string>
#include <iostream>
#include <fstream>

 /* 洞盘使用整数数组表示 */
typedef int board[];

bool is_point(int x) {
    return x == -1                /* 被封闭的方格 */
        || x == 0                   /* 洞 */
        || x == 1;                  /* 针孔（即填充的洞） */
}

bool is_board(int B[])
{
    //@assert \length(B) == 8*8;
    for (int i = 0; i < 8 * 8; i++)
        if (!is_point(B[i])) return false;
    return true;
}

/* num_pegs(B) = B 上的针孔数量 */
int num_pegs(board B)
//@requires is_board(B);
{
    int count = 0;
    for (int i = 0; i < 8 * 8; i++)
        if (B[i] == 1) count++;
    return count;
}

/* init_board(B) 设置标准的英式棋盘 */
void init_board(int B[])
//@requires \length(B) == 8*8;
//@ensures is_board(B);
{
    for (int col = 0; col < 8; col++)
        for (int row = 0; row < 8; row++)
            if (((2 <= col && col <= 4)
                || (2 <= row && row <= 4))
                && col != 7 && row != 7)
                B[8 * row + col] = 1;       /* 洞内有针孔 */
            else
                B[8 * row + col] = -1;      /* 被封闭的 */
    B[3 * 8 + 3] = 0;                 /* 初始空洞 */
}

/* array_copy(dst, src, n) 复制 src[0..n) 到 dst[0..n) */
void array_copy(int dst[], int src[], int n)
//@requires 0 <= n && n <= \length(dst) && n <= \length(src);
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
    return;
}

void print_board(board B)
//@requires is_board(B);
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int x = B[8 * row + col];
            if (x == 0) std::cout << "o ";        /* o = 洞 */
            else if (x == 1) std::cout << "* ";   /* * = 针孔 */
            else if (x == -1) std::cout << ". ";  /* . = 被封闭的 */
        }
        std::cout << std::endl;
    }
    return;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int string2board(std::string s, int B[], int i, int n)
//@requires 0 <= i && i < n && n <= \length(B);
{
    int len = s.length();
    int cur = 0;
    int k = i;
    while (k < n && cur < len) {
        /* 跳过空白 */
        while (cur < len && is_whitespace(s[cur]))
            //@loop_invariant 0 <= cur && cur <= len;
            cur++;
        if (cur == len) return k;   /* 到达字符串末尾 */
        char c = s[cur];
        if (c == '.') { B[k] = -1; }      /* 被封闭的 */
        else if (c == 'o') { B[k] = 0; }  /* 洞 */
        else if (c == '*') { B[k] = 1; }  /* 针孔（在洞中） */
        else throw std::invalid_argument("Illegal character '" + std::string(1, c) + "'");
        k++;    /* 下一个数组索引 */
        cur++;  /* 下一个字符串索引 */
    }
    return k;
}

void read_board(std::string filename, int B[], int n)
//@requires 0 <= n && n <= \length(B);
{
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Error: file does not exist");
    int i = 0;
    std::string line;
    while (i < n && std::getline(file, line)) {
        i = string2board(line, B, i, n);
    }
    file.close();
    return;
}
