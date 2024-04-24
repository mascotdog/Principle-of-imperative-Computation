#pragma once
#include "stacks.h"
#include<unordered_map>
using namespace std;

unordered_map<long long, bool>state;

typedef int MOVE;//9-11-row1 6-8-col1 3-5-row2 0-2-col2
int jump_r[4] = { -2,0,2,0 };
int jump_c[4] = { 0,2,0,-2 };
int mid_r[4] = { -1,0,1,0 };
int mid_c[4] = { 0,1,0,-1 };

int symmetry[64] = {
	6, 5, 4, 3, 2, 1, 0, 7,
	14,13,12,11,10,9 ,8,15,
	22,21,20,19,18,17,16,23,
	30,29,28,27,26,25,24,31,
	38,37,36,35,34,33,32,39,
	46,45,44,43,42,41,40,47,
	54,53,52,51,50,49,48,55,
	56,57,58,59,60,61,62,63
};

int left_rotate[64] = {
	48,40,32,24,16,8, 0, 7,
	49,41,33,25,17,9, 1, 15,
	50,42,34,26,18,10,2, 23,
	51,43,35,27,19,11,3, 31,
	52,44,36,28,20,12,4, 39,
	53,45,37,29,21,13,5, 47,
	54,46,38,30,22,14,6, 55,
	56,57,58,59,60,61,62,63
};

bool flag = 0;

htkey board_state_get(int* B) {
	int num = 0;
	int i1 = 0;
	int i2 = 0;
	for (int i = 0; i < 32; i++)
		if (B[i] == 1)i1 += 1 << i, num++;
	for (int i = 32; i < 64; i++)
		if (B[i] == 1)i2 += 1 << (i - 32), num++;
	two_ints x = { i1,i2,num };
	htkey p = &x;
	return p;
}

long long state_get(int* B) {
	long long x = 0;
	for (int i = 0; i < 64; i++)
		if (B[i] == 1)
			x += (long long)1 << i;
	return x;
}

void update_map(int* B) {
	int a[64] = { 0 };
	int b[64] = { 0 };
	for (int i = 0; i < 64; i++) a[i] = B[symmetry[i]];
	for (int i = 0; i < 64; i++) b[i] = B[i];
	long long state1 = state_get(a);
	long long state2 = state_get(b);
	state[state1] = true;
	state[state2] = true;
	for (int i = 0; i < 3; i++) {
		int a_temp[64];
		int b_temp[64];
		memcpy(a_temp, a, sizeof a);
		memcpy(b_temp, b, sizeof b);
		for (int i = 0; i < 64; i++) a[i] = a_temp[left_rotate[i]];
		for (int i = 0; i < 64; i++) b[i] = b_temp[left_rotate[i]];
		state1 = state_get(a);
		state2 = state_get(b);
		state[state1] = true;
		state[state2] = true;
	}
}

int row_start(MOVE m) {
	return m >> 9 & 7;
}


int col_start(MOVE m) {
	return m >> 6 & 7;
}

int row_end(MOVE m) {
	return m >> 3 & 7;
}


int col_end(MOVE m) {
	return m & 7;
}

bool operation_check(int* B,int row_mid,int col_mid,int row_e,int col_e) {
	if (row_mid < 0 || col_mid < 0 || row_e < 0 || col_e < 0)return false;
	return B[row_mid * 8 + col_mid] == 1 && B[row_e * 8 + col_e] == 0;
}

void get_operation(int* B,int position, stack operation) {
	int row_s = position / 8;
	int col_s = position % 8;

	for (int i = 0; i < 4; i++) {
		int row_e = row_s + jump_r[i];
		int col_e = col_s + jump_c[i];
		int row_mid = row_s + mid_r[i];
		int col_mid = col_s + mid_c[i];
		if (operation_check(B,row_mid,col_mid,row_e,col_e)) {
			int op = row_s << 9 | col_s << 6 | row_e << 3 | col_e;
			push(operation, op);
		}	
	}
	return;
}
//void excute(int* B, int row_s, int col_s, int row_e, int col_e, bool op, htkey state)
void excute(int* B, int row_s, int col_s, int row_e, int col_e,long long* st) {
	int mid_r = row_s + row_e >> 1;
	int mid_c = col_s + col_e >> 1;
	int s = row_s * 8 + col_s;
	int m = mid_r * 8 + mid_c;
	int e = row_e * 8 + col_e;
	B[s] ^= 1;
	B[m] ^= 1;
	B[e] ^= 1;
	*st ^= (long long)1<<s;
	*st ^= (long long)1<<m;
	*st ^= (long long)1<<e;
	//s < 32 ? state->i1 ^= 1 << s : state->i2 ^= 1 << (s - 32);
	//m < 32 ? state->i1 ^= 1 << m : state->i2 ^= 1 << (m - 32);
	//e < 32 ? state->i1 ^= 1 << e : state->i2 ^= 1 << (e - 32);
	//op ? state->best_num_pegs-- : state->best_num_pegs++;
}
//bool solve(int* B, stack S, int num, ht board_ht, htkey state)
bool solve(int* B,stack S,int num, long long* st) {

	if (num == 1)flag = 1;
	if (flag == 1)return true;

	for (int i = 0; i < 8 * 8; i++)
		if (B[i] == 1) {
			stack operation = stack_new();
			get_operation(B,i, operation);
			while (!stack_empty(operation)) {
				int op = pop(operation);
				excute(B, row_start(op), col_start(op), row_end(op), col_end(op),st);
				//excute(B, row_start(op), col_start(op), row_end(op), col_end(op), true, state);
				//print_board(B);
				//if (!ht_lookup(board_ht, state) && solve(B, S, num - 1, board_ht, state))
				if (!state.count(*st) && solve(B, S, num - 1,st)) {
					push(S, op);
					return true;
				}
				excute(B, row_start(op), col_start(op), row_end(op), col_end(op),st);
				//excute(B, row_start(op), col_start(op), row_end(op), col_end(op), false, state);
				//print_board(B);
			}
			free(operation);
			operation = NULL;
		}
	update_map(B);
	//state[state_get(B)] = true;
	//ht_insert(board_ht, state);
	return false;
}


int peg_solve(int* B, stack S) {
	int num = 0;
	for (int i = 0; i < 8 * 8; i++)if (B[i] == 1)num++;
	long long start_state = state_get(B);
	long long* st = &start_state;
	//ht board_ht = ht_new(1000);
	//htkey start_state = board_state_get(B);
	//return solve(B, S, num, board_ht, start_state);
	return solve(B, S, num, st);
}
//@requires is_board(B);
//@requires num_pegs(B) >= 1;
//@requires stack_empty(S);
//@ensures is_board(B);
//@ensures result >= 1;
;
