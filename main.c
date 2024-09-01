#include "util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

tgr_statement tgr_create_compound_statement(tgr_statement first,
                                            tgr_statement second)
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
    result->data.assign.id = create_string(id);
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
    result->data.id = create_string(id);
    return result;
}

tgr_expression tgr_create_numeric_expression(int num)
{
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = numeric_expression;
    result->data.num = num;
    return result;
}

tgr_expression tgr_create_operator_expression(tgr_expression left,
                                              tgr_binary_operation op,
                                              tgr_expression right)
{
    tgr_expression result = checked_malloc(sizeof(*result));
    result->kind = operator_expression;
    result->data.op.left = left;
    result->data.op.op = op;
    result->data.op.right = right;
    return result;
}

tgr_expression tgr_create_expression_sequence_expression(tgr_statement stm,
                                                         tgr_expression exp)
{
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
            tgr_expression_list tail;
        } pair;
        tgr_expression last;
    } data;
};

tgr_expression_list tgr_create_pair_expression_list(tgr_expression head,
                                                    tgr_expression_list tail)
{
    tgr_expression_list result = checked_malloc(sizeof(*result));
    result->kind = pair_expression_list;
    result->data.pair.head = head;
    result->data.pair.tail = tail;
    return result;
}

tgr_expression_list tgr_create_last_expression_list(tgr_expression last)
{
    tgr_expression_list result = checked_malloc(sizeof(*result));
    result->kind = last_expression_list;
    result->data.last = last;
    return result;
}

const int max_variable_count = 256;

struct resolution_table
{
    string ids[max_variable_count];
    int values[max_variable_count];
    int count;
};

struct resolution_table table = {{}, {}, 0};

void variable_assign_value(const string id, int value)
{
    for (int index = 0; index < table.count; ++index)
    {
        if (strcmp(table.ids[index], id) == 0)
        {
            table.values[index] = value;
            return;
        }
    }
    assert(table.count < max_variable_count);

    // ToDo: do I need to make a copy?
    table.ids[table.count] = create_string(id);
    table.values[table.count] = value;
    table.count++;
}

int variable_get_value(const string id)
{
    for (int index = 0; index < table.count; ++index)
    {
        if (strcmp(table.ids[index], id) == 0)
        {
            return table.values[index];
        }
    }
    assert(0);
}

int evaluate_expression(const tgr_expression exp);
void evaluate_statement(const tgr_statement stm);

int evaluate_id_expression(const tgr_expression exp)
{
    assert(exp->kind == id_expression);
    return variable_get_value(exp->data.id);
}

int evaluate_numeric_expression(const tgr_expression exp)
{
    assert(exp->kind == numeric_expression);
    return exp->data.num;
}

int evaluate_operator_expression(const tgr_expression exp)
{
    assert(exp->kind == operator_expression);
    const int left = evaluate_expression(exp->data.op.left);
    const int right = evaluate_expression(exp->data.op.right);

    switch (exp->data.op.op)
    {
    case tgr_plus:
        return left + right;
    case tgr_minus:
        return left - right;
    case tgr_times:
        return left * right;
    case tgr_divide:
        return left / right;
    default:
        assert(0);
        break;
    }
    return 0;
}

int evaluate_expression_sequence_expression(const tgr_expression exp)
{
    assert(exp->kind == expression_sequence_expression);
    evaluate_statement(exp->data.eseq.stm);
    return evaluate_expression(exp->data.eseq.exp);
}

int evaluate_expression(const tgr_expression exp)
{
    switch (exp->kind)
    {
    case id_expression:
        return evaluate_id_expression(exp);
    case numeric_expression:
        return evaluate_numeric_expression(exp);
    case operator_expression:
        return evaluate_operator_expression(exp);
    case expression_sequence_expression:
        return evaluate_expression_sequence_expression(exp);
    default:
        assert(0);
        break;
    }
    return 0;
}

void evaluate_compound_statement(const tgr_statement stm)
{
    assert(stm->kind == compound_statement);
    evaluate_statement(stm->data.compound.first);
    evaluate_statement(stm->data.compound.second);
}

void evaluate_assignment_statement(const tgr_statement stm)
{
    assert(stm->kind == assignment_statement);
    int value = evaluate_expression(stm->data.assign.exp);
    variable_assign_value(stm->data.assign.id, value);
}

void evaluate_print_statement(const tgr_statement stm)
{
    assert(stm->kind == print_statement);
    tgr_expression_list curr = stm->data.print.exps;

    while (curr->kind == pair_expression_list)
    {
        assert(curr->kind == pair_expression_list);
        const int value = evaluate_expression(curr->data.pair.head);
        printf("%d ", value);
        curr = curr->data.pair.tail;
    }
    assert(curr->kind == last_expression_list);
    const int value = evaluate_expression(curr->data.last);
    printf("%d\n", value);
}

void evaluate_statement(const tgr_statement stm)
{
    switch (stm->kind)
    {
    case compound_statement:
        evaluate_compound_statement(stm);
        break;
    case assignment_statement:
        evaluate_assignment_statement(stm);
        break;
    case print_statement:
        evaluate_print_statement(stm);
        break;
    default:
        assert(0);
        break;
    }
}

void interpret(const tgr_statement program) { evaluate_statement(program); }

int main(int, char **)
{
    // a := 5+3; b := (print(a, a-1), 10*a); print(b)
    tgr_statement program = tgr_create_compound_statement(
        tgr_create_assignment_statement(
            "a", tgr_create_operator_expression(
                     tgr_create_numeric_expression(5), tgr_plus,
                     tgr_create_numeric_expression(3))),
        tgr_create_compound_statement(
            tgr_create_assignment_statement(
                "b",
                tgr_create_expression_sequence_expression(
                    tgr_create_print_statement(tgr_create_pair_expression_list(
                        tgr_create_id_expression("a"),
                        tgr_create_last_expression_list(
                            tgr_create_operator_expression(
                                tgr_create_id_expression("a"), tgr_minus,
                                tgr_create_numeric_expression(1))))),
                    tgr_create_operator_expression(
                        tgr_create_numeric_expression(10), tgr_times,
                        tgr_create_id_expression("a")))),
            tgr_create_print_statement(
                tgr_create_last_expression_list(tgr_create_id_expression("b")))));

    interpret(program);
}
