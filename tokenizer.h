#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

typedef enum { TOK_NUMBER, TOK_OPERATOR, TOK_VARIABLE } token_type_t;
typedef enum {
    OP_PLUS, OP_MINUS,
    OP_OPEN_PAREN, OP_CLOSE_PAREN,
    OP_MULTIPLY, OP_DIVIDE,
    OP_EXPONENT, 
    OP_EQUALITY,
} token_operator_t;

class Token {
public:
    Token(token_type_t type) : type(type) {}
    virtual void print() = 0;
    token_type_t get_type() { return type; }

    virtual bool is_operator(token_operator_t op) { return false; }

protected:
    token_type_t type;
};

class VariableToken : public Token {
public:
    VariableToken(const char *name) : Token(TOK_VARIABLE), name(name) {}
    void print() override;
    const char *get_name() { return name; }

private:
    const char *name;
};

class NumberToken : public Token {
public:
    NumberToken(double value) : Token(TOK_NUMBER), value(value) {}
    void print() override;
    double get_value() { return value; }

private:
    double value;
};

class OperatorToken : public Token {
public:
    OperatorToken(token_operator_t op) : Token(TOK_OPERATOR), op(op) {}
    void print() override;
    token_operator_t get_operator() { return op; }
    bool is_operator(token_operator_t op) { return op == this->op; }

private:
    token_operator_t op;
};

class Tokenizer {
public:
    Tokenizer(const char *input);
    bool pop();
    bool pop(Token **token);
    bool peek(Token **token);
    void reset();

private:
    const char *input;
    int pos = 0;
    Token *next_token = NULL;

    void get_next_token();
};

#endif
