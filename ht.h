#include "peg-client.h"
/*************************/
/* 客户端接口 */
/*************************/
// typedef ______* htelem;
// typedef _______ htkey;

int HASH(htkey k);

bool htkey_equal(htkey k1, htkey k2);

htkey htelem_key(htelem e)
//@requires e != NULL;
;

/**************************/
/* 库接口 */
/**************************/
struct ht_header;
typedef struct ht_header* ht;

ht ht_new(int capacity)
//@requires capacity > 0;
;
htelem ht_lookup(ht H, htkey k); /* 平均O(1) */
void ht_insert(ht H, htelem e)   /* 平均O(1) */
//@requires e != NULL;
;
int ht_size(ht H);               /* O(1) */
void ht_stats(ht H);             /* 打印有关表的调试信息 */

/*******************************/
/* 库实现 */
/*******************************/
struct chain_node {
    htelem data;                   /* data != NULL */
    struct chain_node* next;
};
typedef struct chain_node chain;

int chain_length(chain* p) {
    int i = 0;
    while (p != NULL) {
        i++;
        p = p->next;
    }
    return i;
}

struct ht_header {
    int size;                     /* size >= 0 */
    int capacity;                 /* capacity > 0 */
    chain* table[];               /* \length(table) == capacity */
};

bool is_ht(ht H) {
    if (H == NULL) return false;
    if (!(H->size >= 0)) return false;
    if (!(H->capacity > 0)) return false;
    //@assert \length(H->table) == H->capacity;
    /* 检查表的每个元素是否为有效链 */
    /* 包括检查所有元素是否非空 */
    return true;
}

ht ht_new(int capacity)
//@requires capacity > 0;
//@ensures is_ht(\result);
{
    ht H =  (ht)malloc(sizeof(ht_header));
    H->size = 0;
    H->capacity = capacity;
    for (int i = 0; i < capacity; i++)H->table[i] = NULL;
    for (int i = 0; i < 1000; i++) {
        if (H->table[i] != NULL)std::cout << i << "??????????" << std::endl;
    }
    /* 初始化为 NULL */
    return H;
}

int ht_size(ht H)
//@requires is_ht(H);
{
    return H->size;
}

/* 如果键 k 不在 H 中，则 ht_lookup(H, k) 返回 NULL */
htelem ht_lookup(ht H, htkey k)
//@requires is_ht(H);
{
    int i = abs(HASH(k) % H->capacity);
    chain* p = H->table[i];

    for (int i = 0; i < 1000; i++) {
        if (H->table[i] != NULL)std::cout << i << "??????????" << std::endl;
    }
    while (p != NULL)
        // 循环不变量：p 指向链
    {
        //@assert p->data != NULL;
        if (htkey_equal(htelem_key(p->data), k))
            return p->data;
        else
            p = p->next;
    }
    /* 不在链中 */
    return NULL;
}

void ht_resize(ht H, int new_capacity)
//@requires is_ht(H);
//@requires new_capacity > 0;
//@ensures is_ht(H);
//@ensures H->capacity == new_capacity;
{
    std::cout << "(将哈希表调整到容量 " << new_capacity << ")\n";
    chain* old_table[] = { NULL };
    chain* new_table[] = { NULL };
    memcpy(old_table, H->table, sizeof(chain*)*H->capacity);  
    memcpy(new_table, new chain * [new_capacity], sizeof(chain*)*H->capacity);

    /* new_table 初始化为全部 NULL */
    for (int i = 0; i < H->capacity; i++)
        //@loop_invariant 0 <= i;
    {
        chain* p = old_table[i];
        while (p != NULL) {
            chain* q = p->next;   /* 保存 p->next */
            htelem e = p->data;
            //@assert e != NULL;
            int h = abs(HASH(htelem_key(e)) % new_capacity);
            p->next = new_table[h];
            new_table[h] = p;
            p = q;
        }
    }
    /* 在原地修改 ht H */
    /* H->size 保持不变 */
    H->capacity = new_capacity;
    memcpy(H->table, new_table, sizeof(new_table));
    return;
}

void ht_insert(ht H, htelem e)
//@requires is_ht(H);
//@requires e != NULL;
//@ensures is_ht(H);
//@ensures ht_lookup(H, htelem_key(e)) != NULL;
{
    htkey k = htelem_key(e);
    int i = abs(HASH(k) % H->capacity);

    chain* p = H->table[i];
    while (p != NULL)
        // 循环不变量：p 指向链
    {
        //@assert p->data != NULL;
        if (htkey_equal(htelem_key(p->data), k)) {
            /* 覆盖现有元素 */
            p->data = e;
            return;
        }
        else {
            p = p->next;
        }
    }
    //@assert p == NULL;

    /* 新元素放在链的最前面 */
    chain* q = (chain*) malloc(sizeof chain_node);
    q->data = e;
    q->next = H->table[i];
    H->table[i] = q;
    (H->size)++;

    /* 如果负载因子 > 1，则调整哈希表大小 */
    if (H->size > H->capacity && H->capacity < INT_MAX / 2) {
        /* 负载因子 > 1 */
        assert(H->capacity < INT_MAX / 2);
        ht_resize(H, 2 * H->capacity);
    }

    return;
}

// 调试：报告哈希表的分布情况
void ht_stats(ht H)
//@requires is_ht(H);
{
    int max = 0;
    int *A = (int*)malloc(sizeof(int)*11);
    for (int i = 0; i < H->capacity; i++) {
        int j = chain_length(H->table[i]);
        if (j > 9) A[10]++;
        else A[j]++;
        if (j > max) max = j;
    }

    std::cout << "哈希表分布情况：链表大小...\n";
    for (int i = 0; i < 10; i++) {
        std::cout << "... " << i << ":   " << A[i] << "\n";
    }
    std::cout << "... 10+: " << A[10] << "\n";
    std::cout << "最长链: " << max << "\n";
}
