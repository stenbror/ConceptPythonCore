
#include <pythonCoreTokenizer.h>

using namespace PythonCore::Runtime;

Token::TokenKind Token::kind()
{
    return m_Kind;
}


PythonCoreTokenizer::PythonCoreTokenizer(std::shared_ptr<std::basic_string<char32_t>> sourceCode, unsigned int tabSize, bool isInteractive)
{
    pBuffer = sourceCode->data(); p = pBuffer; pTokenStart = p;
}


std::shared_ptr<Token> PythonCoreTokenizer::advance()
{
    return nullptr;
}


bool PythonCoreTokenizer::isLetterOrDigit(char32_t check) noexcept
{
    return false;
}

Token::TokenKind PythonCoreTokenizer::isOperatorOrDelimiter() noexcept
{
    switch (*p)
    {
        case U'+':
            if (p[1] == '=')
            {
                p += 2;
                return Token::TokenKind::PY_PLUS_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_PLUS;
        case u'-':
            if (p[1] == U'>')
            {
                p += 2;
                return Token::TokenKind::PY_ARROW;
            }
            else if (p[1] == '=')
            {
                p += 2;
                return Token::TokenKind::PY_MINUS_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_MINUS;
        case U'*':
            if (p[1] == U'*')
            {
                if (p[2] == U'=')
                {
                    p += 3;
                    return Token::TokenKind::PY_POWER_ASSIGN;
                }
                p += 2;
                return Token::TokenKind::PY_POWER;
            }
            else if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_MUL_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_MUL;
        case U'/':
            if (p[1] == U'/')
            {
                if (p[2] == U'=')
                {
                    p += 3;
                    return Token::TokenKind::PY_FLOOR_DIV_ASSIGN;
                }
                p += 2;
                return Token::TokenKind::PY_FLOOR_DIV;
            }
            else if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_DIV_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_DIV;
        case U'%':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_MODULO_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_MODULO;
        case U'@':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_MATRICE_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_MATRICE;
        case U'<':
            if (p[1] == U'<')
            {
                if (p[2] == U'=')
                {
                    p += 3;
                    return Token::TokenKind::PY_SHIFT_LEFT_ASSIGN;
                }
                p += 2;
                return Token::TokenKind::PY_SHIFT_LEFT;
            }
            else if (p[1] == U'>')
            {
                p += 2;
                return Token::TokenKind::PY_NOT_EQUAL;
            }
            else if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_LESS_EQUAL;
            }
            p++;
            return Token::TokenKind::PY_LESS;
        case U'>':
            if (p[1] == U'>')
            {
                if (p[2] == U'=')
                {
                    p += 3;
                    return Token::TokenKind::PY_SHIFT_RIGHT_ASSIGN;
                }
                p += 2;
                return Token::TokenKind::PY_SHIFT_RIGHT;
            }
            else if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_GREATER_EQUAL;
            }
            p++;
            return Token::TokenKind::PY_GREATER;
        case U'=':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_EQUAL;
        case U'!':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_NOT_EQUAL;
            }
            return Token::TokenKind::PY_INVALID;
        case U'&':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_BIT_AND_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_BIT_AND;
        case U'|':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_BIT_OR_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_BIT_OR;
        case U'^':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_BIT_XOR_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_BIT_XOR;
        case U'~':
            p++;
            return Token::TokenKind::PY_BIT_INVERT;
        case U':':
            if (p[1] == U'=')
            {
                p += 2;
                return Token::TokenKind::PY_COLON_ASSIGN;
            }
            p++;
            return Token::TokenKind::PY_COLON;
        case U';':
            p++;
            return Token::TokenKind::PY_SEMICOLON;
        case U',':
            p++;
            return Token::TokenKind::PY_COMMA;
        case U'(':
            p++;
            return Token::TokenKind::PY_LEFT_PAREN;
        case U'[':
            p++;
            return Token::TokenKind::PY_LEFT_BRACKET;
        case U'{':
            p++;
            return Token::TokenKind::PY_LEFT_CURLY;
        case U')':
            p++;
            return Token::TokenKind::PY_RIGHT_PAREN;
        case U']':
            p++;
            return Token::TokenKind::PY_RIGHT_BRACKET;
        case U'}':
            p++;
            return Token::TokenKind::PY_RIGHT_CURLY;
    }
    return Token::TokenKind::PY_INVALID;
}

// Check for valid reserved keyword in Python language 3.9 ////////////////////////////////////////////////////////////
Token::TokenKind PythonCoreTokenizer::isReservedKeywordOrLiteralName() noexcept
{
    switch (*p)
    {
        case U'F':
            if (p[1] == U'a' && p[2] == U'l' && p[3] == U's' && p[4] == U'e' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_FALSE;
            }
            break;
        case U'N':
            if (p[1] == U'o' && p[2] == U'n' && p[3] == U'e' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_NONE;
            }
            break;
        case U'T':
            if (p[1] == U'r' && p[2] == U'u' && p[3] == U'e' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_TRUE;
            }
            break;
        case U'a':
            if (p[1] == U'n' && p[2] == U'd'  && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_AND;
            }
            else if (p[1] == U's' && !isLetterOrDigit(p[2]))
            {
                p += 2;
                return Token::TokenKind::PY_AS;
            }
            else if (p[1] == U's' && p[2] == U's' && p[3] == U'e' && p[4] == U'r' && p[5] == U't' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_ASSERT;
            }
            else if (p[1] == U's' && p[2] == U'y' && p[3] == U'n' && p[4] == U'c' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_ASYNC;
            }
            else if (p[1] == U'w' && p[2] == U'a' && p[3] == U'i' && p[4] == U't' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_AWAIT;
            }
            break;
        case U'b':
            if (p[1] == U'r' && p[2] == U'e' && p[3] == U'a' && p[4] == U'k' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_BREAK;
            }
            break;
        case U'c':
            if (p[1] == U'l' && p[2] == U'a' && p[3] == U's' && p[4] == U's' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_CLASS;
            }
            else if (p[1] == U'o' && p[2] == U'n' && p[3] == U't' && p[4] == U'i' && p[5] == U'n' && p[6] == U'u' && p[7] == U'e' && !isLetterOrDigit(p[8]))
            {
                p += 8;
                return Token::TokenKind::PY_CONTINUE;
            }
            break;
        case U'd':
            if (p[1] == U'e' && p[2] == U'l' && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_DEL;
            }
            else if (p[1] == U'e' && p[2] == U'f' && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_DEF;
            }
            break;
        case U'e':
            if (p[1] == U'l' && p[2] == U'i' && p[3] == U'f' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_ELIF;
            }
            else if (p[1] == U'l' && p[2] == U's' && p[3] == U'e' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_ELSE;
            }
            else if (p[1] == U'x' && p[2] == U'c' && p[3] == U'e' && p[4] == U'p' && p[5] == U't' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_EXCEPT;
            }
            break;
        case U'f':
            if (p[1] == U'i' && p[2] == U'n' && p[3] == U'a' && p[4] == U'l' && p[5] == U'l' && p[6] == U'y' && !isLetterOrDigit(p[7]))
            {
                p += 7;
                return Token::TokenKind::PY_FINALLY;
            }
            else if (p[1] == U'o' && p[2] == U'r' && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_FOR;
            }
            else if (p[1] == U'r' && p[2] == U'o' && p[3] == U'm' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_FROM;
            }
            break;
        case U'g':
            if (p[1] == U'l' && p[2] == U'o' && p[3] == U'b' && p[4] == U'a' && p[5] == U'l' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_GLOBAL;
            }
            break;
        case U'i':
            if (p[1] == U'f' && !isLetterOrDigit(p[2]))
            {
                p += 2;
                return Token::TokenKind::PY_IF;
            }
            else if (p[1] == U'm' && p[2] == U'p' && p[3] == U'o' && p[4] == U'r' && p[5] == U't' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_IMPORT;
            }
            else if (p[1] == U'n' && !isLetterOrDigit(p[2]))
            {
                p += 2;
                return Token::TokenKind::PY_IN;
            }
            else if (p[1] == U's' && !isLetterOrDigit(p[2]))
            {
                p += 2;
                return Token::TokenKind::PY_IS;
            }
            break;
        case U'l':
            if (p[1] == U'a' && p[2] == U'm' && p[3] == U'b' && p[4] == U'd' && p[5] == U'a' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_LAMBDA;
            }
            break;
        case U'n':
            if (p[1] == U'o' && p[2] == U'n' && p[3] == U'l' && p[4] == U'o' && p[5] == U'c' && p[6] == U'a' && p[7] == U'l' && !isLetterOrDigit(p[8]))
            {
                p += 8;
                return Token::TokenKind::PY_NONLOCAL;
            }
            else if (p[1] == U'o' && p[2] == U't' && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_NOT;
            }
            break;
        case U'o':
            if (p[1] == U'r' && !isLetterOrDigit(p[2]))
            {
                p += 2;
                return Token::TokenKind::PY_OR;
            }
            break;
        case U'p':
            if (p[1] == U'a' && p[2] == U's' && p[3] == U's' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_PASS;
            }
            break;
        case U'r':
            if (p[1] == U'a' && p[2] == U'i' && p[3] == U's' && p[4] == U'e' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_RAISE;
            }
            else if (p[1] == U'e' && p[2] == U't' && p[3] == U'u' && p[4] == U'r' && p[5] == U'n' && !isLetterOrDigit(p[6]))
            {
                p += 6;
                return Token::TokenKind::PY_RETURN;
            }
            break;
        case U't':
            if (p[1] == U'r' && p[2] == U'y' && !isLetterOrDigit(p[3]))
            {
                p += 3;
                return Token::TokenKind::PY_TRY;
            }
            break;
        case U'w':
            if (p[1] == U'h' && p[2] == U'i' && p[3] == U'l' && p[4] == U'e' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_WHILE;
            }
            else if (p[1] == U'i' && p[2] == U't' && p[3] == U'h' && !isLetterOrDigit(p[4]))
            {
                p += 4;
                return Token::TokenKind::PY_WITH;
            }
            break;
        case U'y':
            if (p[1] == U'i' && p[2] == U'e' && p[3] == U'l' && p[4] == U'd' && !isLetterOrDigit(p[5]))
            {
                p += 5;
                return Token::TokenKind::PY_YIELD;
            }
            break;
    }
    while (isLetterOrDigit(*p)) p++;
    return Token::TokenKind::PY_NAME;
}