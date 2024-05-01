#include "peg-client.h"
#include "ht.h"
#include "stacks.h"
#include "peg-util.h"
#include "peg1.h"
using namespace std;

/* У���岽 */

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

/* B ���봦�ڳ�ʼλ�� */
/* ��һ�������� S �Ķ��� */

bool verify_solution(int* B, stack S) {
    /* ����Ӧ���岽��ͬʱ������� */
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
            cout << "��һ�����Ϸ�\n";
            return false;
        }
        //print_board(B);
    }
    if (!(num_pegs(B) == 1)) {
        cout << "����λ�ò���Ӯ�����\n";
        return false;
    }
    return true;
}

/* ������ */

int peg_main(int* B_init) {
    print_board(B_init);

    /* ������ʼ���̵ĸ��� */
    int* B = (int*)malloc(sizeof(int)*64);
    array_copy(B, B_init, 8 * 8);

    stack S = stack_new(); /* �ӿս�·����ʼ */
    int final_pegs = peg_solve(B, S);
    if (final_pegs == 1) {
        /* �ҵ������������֤����ӡ */
        /* �ӳ�ʼλ�ÿ�ʼ��֤ */
        if (verify_solution(B_init, S)) return 0;
        else return -1;
    }
    else {
        return final_pegs;
    }
}

int main() {
    int* B_init = (int*)malloc(sizeof(int) * 64);

    int argc;//������
    cout << "��ѡ���������ͣ�0.Ĭ������ 1.�ֶ�����"<<endl;
    cin >> argc;

    if (argc == 0) {
        cout << "ʹ��Ӣ�İ�������ΪĬ��\n";
        init_board(B_init);
    }
    else if (argc == 1) {
        string filename;
        cout << "�������ļ���" << endl;
        cin >> filename;

        read_board(filename, B_init, 8 * 8);
    }

    int ans = peg_main(B_init);

    if (ans == 0) {
        cout << "���������֤ͨ����" << endl;
    }
    else if (ans == -1) {
        cout << "���������֤ʧ�ܣ�" << endl;
    }
    else {
        cout << "δ�ҵ������������ӽ����Ե���������Ϊ ";
        if (ans < 10) cout << "  ����\b\b\b\b\b\b\b";
        else cout << "   ����\b\b\b\b\b\b\b\b";
    }

    return ans;
}
