
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

                PY_LAMBDA, PY_IF, PY_ELSE,

                PY_COLON_ASSIGN
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
            unsigned int getPosition() { return -1; };
            std::shared_ptr<Token> advance() { return nullptr; };
    };

}

#endif 