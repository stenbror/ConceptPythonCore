
#ifndef PYTHONCORE_TOKENIZER_H
#define PYTHONCORE_TOKENIZER_H

#include <memory>

namespace PythonCore::Runtime
{

    class Token
    {
        public:
            enum class TokenKind
            {
                PY_INVALID, PY_EOF, PY_INDENT, PY_DEDENT, PY_NEWLINE, 
                PY_FALSE, PY_NONE, PY_TRUE, PY_AND, PY_AS, PY_ASSERT, PY_ASYNC, PY_AWAIT, PY_BREAK, PY_CLASS, PY_CONTINUE, PY_DEF,
                PY_DEL, PY_ELIF, PY_ELSE, PY_EXCEPT, PY_FINALLY, PY_FOR, PY_FROM, PY_GLOBAL, PY_IF, PY_IMPORT, PY_IN, PY_IS,
                PY_LAMBDA, PY_NONLOCAL, PY_NOT, PY_OR, PY_PASS, PY_RAISE, PY_RETURN, PY_TRY, PY_WHILE, PY_WITH, PY_YIELD,
                PY_PLUS, PY_MINUS, PY_MUL, PY_POWER, PY_DIV, PY_FLOOR_DIV, PY_MODULO, PY_MATRICE, PY_SHIFT_LEFT, PY_SHIFT_RIGHT,
                PY_BIT_AND, PY_BIT_OR, PY_BIT_XOR, PY_BIT_INVERT, PY_LESS, PY_LESS_EQUAL, PY_GREATER, PY_GREATER_EQUAL, PY_EQUAL,
                PY_NOT_EQUAL, PY_LEFT_PAREN, PY_RIGHT_PAREN, PY_LEFT_BRACKET, PY_RIGHT_BRACKET, PY_LEFT_CURLY, PY_RIGHT_CURLY,
                PY_COMMA, PY_COLON, PY_COLON_ASSIGN, PY_PERIOD, PY_ELIPSIS, PY_SEMICOLON, PY_ASSIGN, PY_ARROW, PY_PLUS_ASSIGN,
                PY_MINUS_ASSIGN, PY_MUL_ASSIGN, PY_DIV_ASSIGN, PY_FLOOR_DIV_ASSIGN, PY_MODULO_ASSIGN, PY_MATRICE_ASSIGN,
                PY_BIT_AND_ASSIGN, PY_BIT_OR_ASSIGN, PY_BIT_XOR_ASSIGN, PY_POWER_ASSIGN, PY_SHIFT_LEFT_ASSIGN, PY_SHIFT_RIGHT_ASSIGN,
                PY_NAME, PY_NUMBER, PY_STRING, PY_TYPE_COMMENT
            };

        public:

            TokenKind kind();


        protected:
            TokenKind m_Kind;
            unsigned int m_StartPos;
            unsigned int m_EndPos;

    };

    class PythonCoreTokenizer
    {

        public:
            PythonCoreTokenizer(std::shared_ptr<std::basic_string<char32_t>> sourceCode, unsigned int tabSize, bool isInteractive = false);

            unsigned int getPosition() { return p - pBuffer; };
            std::shared_ptr<Token> advance();


        protected:
            Token::TokenKind isReservedKeywordOrLiteralName();
            Token::TokenKind isOperatorOrDelimiter();
            bool isLetterOrDigit(char32_t check);


        protected:
            const char32_t *p = nullptr, *pTokenStart = nullptr;
            const char32_t *pBuffer;
    };

}

#endif 