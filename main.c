#include <stdlib.h>
#include <assert.h>

typedef char *string;
typedef struct tgr_statement_t *tgr_statement;
typedef struct tgr_expression_t *tgr_expression;
typedef struct tgr_expression_list_t *tgr_expression_list;
typedef enum
{
    tgr_plus,
    tgr_minus,
    tgr_times,
    tgr_divide
} tgr_binary_operation;

struct tgr_statement_t
{
    enum
    {
        compound_statement,
        assignment_statement,
        print_statement
    } kind;
    union
    {
        struct
        {
            tgr_statement first, second;
        } compound;
        struct
        {
            string id;
            tgr_expression exp;
        } assign;
        struct
        {
            tgr_expression_list exps;
        } print;
    } data;
};

void *checked_malloc(int length)
{
    void *p = malloc(length);
    assert(p);
    return p;
}

tgr_statement tgr_create_compound_statement(tgr_statement first, tgr_statement second)
{
    tgr_statement result = checked_malloc(sizeof(*result));
    result->kind = compound_statement;
    result->data.compound.first = first;
    result->data.compound.second = second;
    return result;
}

tgr_statement tgr_create_assignment_statement(string id, tgr_expression exp)
{
    tgr_statement result = checked_malloc(sizeof(*result));
    result->kind = assignment_statement;
    result->data.assign.id = id;
    result->data.assign.exp = exp;
    return result;
}

tgr_statement tgr_create_print_statement(tgr_expression_list exps)
{
    tgr_statement result = checked_malloc(sizeof(*result));
    result->kind = print_statement;
    result->data.print.exps = exps;
    return result;
}

struct tgr_expression_t
{
    enum
    {
        id_expression,
        numeric_expression,
        operator_expression,
        expression_sequence_expression
    } kind;
    union
    {
        string id;
        int num;
        struct
        {
            tgr_expression left;
            tgr_binary_operation op;
            tgr_expression right;
        } op;
        struct
        {
            tgr_statement stm;
            tgr_expression exp;
        } eseq;
    } data;
};

tgr_expression tgr_create_id_expression(string id)
{
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = id_expression;
    result->data.id = id;
    return result;
}

tgr_expression tgr_create_numeric_expression(int num)
{
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = numeric_expression;
    result->data.num = num;
    return result;
}

tgr_expression tgr_create_operator_expression(tgr_expression left, tgr_binary_operation op, tgr_expression right)
{
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = operator_expression;
    result->data.op.left = left;
    result->data.op.op = op;
    result->data.op.right = right;
    return result;
}

tgr_expression tgr_create_expression_sequence_expression(tgr_statement stm, tgr_expression exp) {
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = expression_sequence_expression;
    result->data.eseq.stm = stm;
    result->data.eseq.exp = exp;
    return result;
}

struct tgr_expression_list_t
{
    enum
    {
        pair_expression_list,
        last_expression_list
    } kind;
    union
    {
        struct
        {
            tgr_expression head;
            tgr_expression tail;
        } pair;
        tgr_expression last;
    } data;
};

int main(int, char **) {}
