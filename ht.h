#include "peg-client.h"
/*************************/
/* �ͻ��˽ӿ� */
/*************************/
// typedef ______* htelem;
// typedef _______ htkey;

int HASH(htkey k);

bool htkey_equal(htkey k1, htkey k2);

htkey htelem_key(htelem e)
//@requires e != NULL;
;

/**************************/
/* ��ӿ� */
/**************************/
struct ht_header;
typedef struct ht_header* ht;

ht ht_new(int capacity)
//@requires capacity > 0;
;
htelem ht_lookup(ht H, htkey k); /* ƽ��O(1) */
void ht_insert(ht H, htelem e)   /* ƽ��O(1) */
//@requires e != NULL;
;
int ht_size(ht H);               /* O(1) */
void ht_stats(ht H);             /* ��ӡ�йر�ĵ�����Ϣ */

/*******************************/
/* ��ʵ�� */
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
    /* �����ÿ��Ԫ���Ƿ�Ϊ��Ч�� */
    /* �����������Ԫ���Ƿ�ǿ� */
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
    /* ��ʼ��Ϊ NULL */
    return H;
}

int ht_size(ht H)
//@requires is_ht(H);
{
    return H->size;
}

/* ����� k ���� H �У��� ht_lookup(H, k) ���� NULL */
htelem ht_lookup(ht H, htkey k)
//@requires is_ht(H);
{
    int i = abs(HASH(k) % H->capacity);
    chain* p = H->table[i];

    for (int i = 0; i < 1000; i++) {
        if (H->table[i] != NULL)std::cout << i << "??????????" << std::endl;
    }
    while (p != NULL)
        // ѭ����������p ָ����
    {
        //@assert p->data != NULL;
        if (htkey_equal(htelem_key(p->data), k))
            return p->data;
        else
            p = p->next;
    }
    /* �������� */
    return NULL;
}

void ht_resize(ht H, int new_capacity)
//@requires is_ht(H);
//@requires new_capacity > 0;
//@ensures is_ht(H);
//@ensures H->capacity == new_capacity;
{
    std::cout << "(����ϣ����������� " << new_capacity << ")\n";
    chain* old_table[] = { NULL };
    chain* new_table[] = { NULL };
    memcpy(old_table, H->table, sizeof(chain*)*H->capacity);  
    memcpy(new_table, new chain * [new_capacity], sizeof(chain*)*H->capacity);

    /* new_table ��ʼ��Ϊȫ�� NULL */
    for (int i = 0; i < H->capacity; i++)
        //@loop_invariant 0 <= i;
    {
        chain* p = old_table[i];
        while (p != NULL) {
            chain* q = p->next;   /* ���� p->next */
            htelem e = p->data;
            //@assert e != NULL;
            int h = abs(HASH(htelem_key(e)) % new_capacity);
            p->next = new_table[h];
            new_table[h] = p;
            p = q;
        }
    }
    /* ��ԭ���޸� ht H */
    /* H->size ���ֲ��� */
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
        // ѭ����������p ָ����
    {
        //@assert p->data != NULL;
        if (htkey_equal(htelem_key(p->data), k)) {
            /* ��������Ԫ�� */
            p->data = e;
            return;
        }
        else {
            p = p->next;
        }
    }
    //@assert p == NULL;

    /* ��Ԫ�ط���������ǰ�� */
    chain* q = (chain*) malloc(sizeof chain_node);
    q->data = e;
    q->next = H->table[i];
    H->table[i] = q;
    (H->size)++;

    /* ����������� > 1���������ϣ���С */
    if (H->size > H->capacity && H->capacity < INT_MAX / 2) {
        /* �������� > 1 */
        assert(H->capacity < INT_MAX / 2);
        ht_resize(H, 2 * H->capacity);
    }

    return;
}

// ���ԣ������ϣ��ķֲ����
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

    std::cout << "��ϣ��ֲ�����������С...\n";
    for (int i = 0; i < 10; i++) {
        std::cout << "... " << i << ":   " << A[i] << "\n";
    }
    std::cout << "... 10+: " << A[10] << "\n";
    std::cout << "���: " << max << "\n";
}
