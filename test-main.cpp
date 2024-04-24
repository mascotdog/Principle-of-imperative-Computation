#include "peg-client.h"
#include "ht.h"
#include "stacks.h"
#include "peg-util.h"
#include "peg1.h"
using namespace std;

/* 校验棋步 */

void print_coords(int row, int col) {
    cout << row << ":" << col;
    return;
}

bool is_coord(int x) {
    return 0 <= x && x < 8;
}

bool legal_move(int* B, int row1, int col1, int row2, int col2)
//@requires is_board(B);
{
    if (!(is_coord(row1) && is_coord(col1)
        && is_coord(row2) && is_coord(col2)))
        return false;
    if (!((abs(row1 - row2) == 2 && col1 == col2)
        || (abs(col1 - col2) == 2 && row1 == row2)))
        return false;
    int i = 8 * row1 + col1;
    int k = 8 * row2 + col2;
    int j = (i + k) / 2;
    if (!(B[i] == 1 && B[j] == 1 && B[k] == 0))
        return false;
    return true;
}

void apply_move(int* B, int row1, int col1, int row2, int col2)
//@requires is_board(B);
//@requires legal_move(B, row1, col1, row2, col2);
//@ensures is_board(B);
{
    int i = 8 * row1 + col1;
    int k = 8 * row2 + col2;
    int j = (i + k) / 2;
    B[i] = 1 - B[i];
    B[j] = 1 - B[j];
    B[k] = 1 - B[j];
    return;
}

/* B 必须处于初始位置 */
/* 第一步必须在 S 的顶部 */

bool verify_solution(int* B, stack S) {
    /* 重新应用棋步，同时检查它们 */
    while (!stack_empty(S)) {
        MOVE m = pop(S);
        int row1 = row_start(m);
        int col1 = col_start(m);
        int row2 = row_end(m);
        int col2 = col_end(m);
        print_coords(row1, col1);
        cout << " -> ";
        print_coords(row2, col2);
        cout << endl;
        

        if (legal_move(B, row1, col1, row2, col2)) {
            apply_move(B, row1, col1, row2, col2);
        }
        else {
            cout << "上一步不合法\n";
            return false;
        }
        //print_board(B);
    }
    if (!(num_pegs(B) == 1)) {
        cout << "最终位置不是赢棋局面\n";
        return false;
    }
    return true;
}

/* 主函数 */

int peg_main(int* B_init) {
    print_board(B_init);

    /* 制作初始棋盘的副本 */
    int* B = (int*)malloc(sizeof(int)*64);
    array_copy(B, B_init, 8 * 8);

    stack S = stack_new(); /* 从空解路径开始 */
    int final_pegs = peg_solve(B, S);
    if (final_pegs == 1) {
        /* 找到解决方案；验证并打印 */
        /* 从初始位置开始验证 */
        if (verify_solution(B_init, S)) return 0;
        else return -1;
    }
    else {
        return final_pegs;
    }
}

int main() {
    int* B_init = (int*)malloc(sizeof(int) * 64);

    int argc;//操作数
    cout << "请选择棋盘类型：0.默认棋盘 1.手动输入"<<endl;
    cin >> argc;

    if (argc == 0) {
        cout << "使用英文版棋盘作为默认\n";
        init_board(B_init);
    }
    else if (argc == 1) {
        string filename;
        cout << "请输入文件名" << endl;
        cin >> filename;

        read_board(filename, B_init, 8 * 8);
    }

    int ans = peg_main(B_init);

    if (ans == 0) {
        cout << "解决方案验证通过！" << endl;
    }
    else if (ans == -1) {
        cout << "解决方案验证失败！" << endl;
    }
    else {
        cout << "未找到解决方案，最接近尝试的棋子数量为 ";
        if (ans < 10) cout << "  个。\b\b\b\b\b\b\b";
        else cout << "   个。\b\b\b\b\b\b\b\b";
    }

    return ans;
}
