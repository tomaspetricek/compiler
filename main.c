#include <stdlib.h>
#include <assert.h>

typedef char *string;
typedef struct statement_t *statement;
typedef struct expression_t *expression;
typedef struct expression_list_t *expression_list;
typedef enum
{
    plus,
    minus,
    times,
    divide
} binary_operation;

struct statement_t
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
            statement first, second;
        } compound;
        struct
        {
            string id;
            expression exp;
        } assign;
        struct
        {
            expression_list exps;
        } print;
    } data;
};

void *checked_malloc(int length)
{
    void *p = malloc(length);
    assert(p);
    return p;
}

statement create_compound_statement(statement first, statement second)
{
    statement result = checked_malloc(sizeof(*result));
    result->kind = compound_statement;
    result->data.compound.first = first;
    result->data.compound.second = second;
    return result;
}

statement create_assignment_statement(string id, expression exp)
{
    statement result = checked_malloc(sizeof(*result));
    result->kind = assignment_statement;
    result->data.assign.id = id;
    result->data.assign.exp = exp;
    return result;
}

statement create_print_statement(expression_list exps)
{
    statement result = checked_malloc(sizeof(*result));
    result->kind = print_statement;
    result->data.print.exps = exps;
    return result;
}

struct expression_t
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
            expression left;
            binary_operation op;
            expression right;
        } op;
        struct
        {
            statement stm;
            expression exp;
        } eseq;
    } data;
};

expression create_id_expression(string id)
{
    expression result = checked_malloc(sizeof(*result));
    result->kind = id_expression;
    result->data.id = id;
    return result;
}

expression create_numeric_expression(int num)
{
    expression result = checked_malloc(sizeof(*result));
    result->kind = numeric_expression;
    result->data.num = num;
    return result;
}

expression create_operator_expression(expression left, binary_operation op, expression right)
{
    expression result = checked_malloc(sizeof(*result));
    result->kind = operator_expression;
    result->data.op.left = left;
    result->data.op.op = op;
    result->data.op.right = right;
    return result;
}

expression create_expression_sequence_expression(statement stm, expression exp) {
    expression result = checked_malloc(sizeof(*result));
    result->kind = expression_sequence_expression;
    result->data.eseq.stm = stm;
    result->data.eseq.exp = exp;
    return result;
}

struct expression_list_t
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
            expression head;
            expression tail;
        } pair;
        expression last;
    } data;
};

int main(int, char **) {}
