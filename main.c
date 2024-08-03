
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

struct statement
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

statement create_compound_statement(statement first, statement second);
statement create_assignment_statement(string id, expression exp);
statement create_print_statement(expression_list exps);

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
            binary_operation operation;
            expression right;
        } op;
        struct
        {
            statement stm;
            expression exp;
        } eseq;
    } data;
};

expression create_id_expression(string id);
expression create_numeric_expression(int num);
expression create_operator_expression(expression left, binary_operation op, expression right);
expression create_expression_sequence_expression(statement stm, expression exp);

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

int main(int, char **) {  }
