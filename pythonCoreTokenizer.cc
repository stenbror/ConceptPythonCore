
#include <pythonCoreTokenizer.h>

using namespace PythonCore::Runtime;


Token::TokenKind Token::kind()
{
    return m_Kind;
}