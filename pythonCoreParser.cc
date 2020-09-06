#include <pythonCoreParser.h>

using namespace PythonCore::Runtime;

// Parser start rules /////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseSingleInput(std::shared_ptr<PythonCoreTokenizer> lexer) 
{ 
    if (lexer == nullptr) return nullptr;
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<Token> newline = nullptr;
    std::shared_ptr<ASTStatementNode> node = nullptr;
    m_Lexer->advance();
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_NEWLINE:
            newline = m_CurSymbol;
            m_Lexer->advance();
            break;
        case Token::TokenKind::PY_IF:
        case Token::TokenKind::PY_FOR:
        case Token::TokenKind::PY_WHILE:
        case Token::TokenKind::PY_TRY:
        case Token::TokenKind::PY_WITH:
        case Token::TokenKind::PY_MATRICE:
        case Token::TokenKind::PY_CLASS:
        case Token::TokenKind::PY_DEF:
        case Token::TokenKind::PY_ASYNC:
            node = parseCompoundStmt();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE) throw ;
            newline = m_CurSymbol;
            m_Lexer->advance();
            break;
        default:
            node = parseSimpleStmt();
            break;
    }
    return std::make_shared<ASTSingleInputNode>(start, m_Lexer->getPosition(), newline, node);
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseFileInput(std::shared_ptr<PythonCoreTokenizer> lexer) 
{ 
    if (lexer == nullptr) return nullptr;
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> newlines;
    m_Lexer->advance();
    while (m_CurSymbol->kind() != Token::TokenKind::PY_EOF)
    {
        if (m_CurSymbol->kind() == Token::TokenKind::PY_NEWLINE)
        {
            newlines->push_back(m_CurSymbol);
            m_Lexer->advance();
        }
        else
        {
            nodes->push_back(parseStmt());
        }
    }
    return std::make_shared<ASTFileInputNode>(start, m_Lexer->getPosition(), nodes, newlines, m_CurSymbol); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseEvalInput(std::shared_ptr<PythonCoreTokenizer> lexer) 
{ 
    if (lexer == nullptr) return nullptr;
    unsigned int start = m_Lexer->getPosition();
    auto right = parseTestList();
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> newlines;
    while (m_CurSymbol->kind() == Token::TokenKind::PY_NEWLINE)
    {
        newlines->push_back(m_CurSymbol);
        m_Lexer->advance();
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_EOF) throw ;
    return std::make_shared<ASTEvalInputNode>(start, m_Lexer->getPosition(), right, newlines, m_CurSymbol); 
}


// Statement rules ////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseDecorator() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol; // '@'
    m_Lexer->advance();
    auto left = parseDottedName();
    std::shared_ptr<Token> op2;
    std::shared_ptr<ASTExpressionNode> right;
    std::shared_ptr<Token> op3;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_PAREN)
    {
        op2 = m_CurSymbol; // '('
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) right = parseArgList();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) throw ;
        op3 = m_CurSymbol; // '('
        m_Lexer->advance();
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE) throw ;
    auto op4 = m_CurSymbol; // 'NEWLINE'
    m_Lexer->advance();
    return std::make_shared<ASTDecoratorStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right, op3, op4); 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseDecorators() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto res = std::make_shared<ASTStatementListNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_DECORATORS);
    do
    {
        res->addNode(parseDecorator());
    } while (m_CurSymbol->kind() == Token::TokenKind::PY_MATRICE);
    res->addEndPosition(m_Lexer->getPosition());
    return res;
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseDecorated() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseDecorators();
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_CLASS:
            {
                auto right = parseClassDef();
                return std::make_shared<ASTDecoratedStatementNode>(start, m_Lexer->getPosition(), left, right);
            }
        case Token::TokenKind::PY_DEF:
            {
                auto right = parseFuncDef();
                return std::make_shared<ASTDecoratedStatementNode>(start, m_Lexer->getPosition(), left, right);
            }
        case Token::TokenKind::PY_ASYNC:
            {
                auto right = parseAsyncFuncDef();
                return std::make_shared<ASTDecoratedStatementNode>(start, m_Lexer->getPosition(), left, right);
            }
        default:
            throw ;
    }
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseAsyncFuncDef() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol; // 'async'
    m_Lexer->advance();
    auto right = parseFuncDef();
    return std::make_shared<ASTAsyncFuncDefStatementNode>(start, m_Lexer->getPosition(), op, right); 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseFuncDef() 
{
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol; // 'def'
    m_Lexer->advance();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
    auto op2 = m_CurSymbol; // 'NAME'
    m_Lexer->advance();
    auto left = parseParameters();
    std::shared_ptr<Token> op3;
    std::shared_ptr<ASTExpressionNode> right;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ARROW) // '->'
    {
        op3 = m_CurSymbol;
        m_Lexer->advance();
        right = parseTest();
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op4 = m_CurSymbol; // ':'
    m_Lexer->advance();
    std::shared_ptr<ASTExpressionNode> comment;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_TYPE_COMMENT)
    {
        // Later...
    }
    auto next = parseFuncBodySuite();
    return std::make_shared<ASTFuncDefStatementNode>(start, m_Lexer->getPosition(), op1, op2, left, op3, right, op4, comment, next); 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseParameters() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_LEFT_PAREN) throw ;
    auto op1 = m_CurSymbol; // '('
    m_Lexer->advance();
    std::shared_ptr<ASTStatementNode> right = nullptr;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) right = parseTypedArgsList();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) throw ;
    auto op2 = m_CurSymbol; // ')'
    m_Lexer->advance();
    return std::make_shared<ASTParameterStatementNode>(start, m_Lexer->getPosition(), op1, right, op2); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseTypedArgsList() { return nullptr; }

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseTFPDef() { return nullptr; }

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseVarArgsList() { return nullptr; }

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseVFPDef() { return nullptr; }

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseStmt() 
{ 
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_IF:
        case Token::TokenKind::PY_FOR:
        case Token::TokenKind::PY_WHILE:
        case Token::TokenKind::PY_TRY:
        case Token::TokenKind::PY_WITH:
        case Token::TokenKind::PY_MATRICE:
        case Token::TokenKind::PY_CLASS:
        case Token::TokenKind::PY_DEF:
        case Token::TokenKind::PY_ASYNC:
            return parseCompoundStmt();
        default:
            return parseSimpleStmt();
    }
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseSimpleStmt() 
{
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> separators;
    nodes->push_back(parseSmallStmt());
    while (m_CurSymbol->kind() == Token::TokenKind::PY_SEMICOLON)
    {
        separators->push_back(m_CurSymbol);
        m_Lexer->advance();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_NEWLINE) continue;
        nodes->push_back(parseSmallStmt());
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE) throw ;
    auto op1 = m_CurSymbol; // 'NEWLINE'
    m_Lexer->advance();
    return std::make_shared<ASTListSimpleStatementNode>(start, m_Lexer->getPosition(), nodes, separators, op1);
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseSmallStmt() 
{ 
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_DEL:
            return parseDelStmt();
        case Token::TokenKind::PY_PASS:
            return parsePassStmt();
        case Token::TokenKind::PY_BREAK:
        case Token::TokenKind::PY_CONTINUE:
        case Token::TokenKind::PY_RETURN:
        case Token::TokenKind::PY_RAISE:
        case Token::TokenKind::PY_YIELD:
            return parseFlowStmt();
        case Token::TokenKind::PY_IMPORT:
        case Token::TokenKind::PY_FROM:
            return parseImportStmt();
        case Token::TokenKind::PY_GLOBAL:
            return parseGlobalStmt();
        case Token::TokenKind::PY_NONLOCAL:
            return parseNonlocalStmt();
        case Token::TokenKind::PY_ASSIGN:
            return parseAssertStmt();
        default:
            return parseExprStatement();
    }
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseExprStatement() { return nullptr; }

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseTestListStarExpr() { return nullptr; }

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseDelStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseExprList();
    return std::make_shared<ASTDelStatementNode>(start, m_Lexer->getPosition(), op, right); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parsePassStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    return std::make_shared<ASTPassStatementNode>(start, m_Lexer->getPosition(), op); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseFlowStmt() 
{ 
    std::shared_ptr<ASTStatementNode> res;
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_BREAK:
            res = parseBreakStmt();
            break;
        case Token::TokenKind::PY_CONTINUE:
            res = parseContinueStmt();
            break;
        case Token::TokenKind::PY_RETURN:
            res = parseReturnStmt();
            break;
        case Token::TokenKind::PY_RAISE:
            res = parseRaiseStmt();
            break;
        case Token::TokenKind::PY_YIELD:
            res = parseYieldStmt();
            break;
        default:    throw ;
    }
    return res;
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseBreakStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    return std::make_shared<ASTSimpleFlowStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_BREAK, op); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseContinueStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    return std::make_shared<ASTSimpleFlowStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_BREAK, op);  
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseReturnStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_SEMICOLON && m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE)
    {
        auto right = parseTestListStarExpr();
        return std::make_shared<ASTReturnStatementNode>(start, m_Lexer->getPosition(), op, right);
    }
    return std::make_shared<ASTReturnStatementNode>(start, m_Lexer->getPosition(), op, nullptr);;
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseYieldStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto right = parseYieldExpr();
    return std::make_shared<ASTYieldStatementNode>(start, m_Lexer->getPosition(), right); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseRaiseStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    std::shared_ptr<ASTExpressionNode> left = nullptr;
    std::shared_ptr<Token> op2 = nullptr;
    std::shared_ptr<ASTExpressionNode> right = nullptr;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_SEMICOLON && m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE)
    {
        left = parseTest();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_FROM)
        {
            op2 = m_CurSymbol;
            m_Lexer->advance();
            right = parseTest();
        }
    }
    return std::make_shared<ASTRaiseStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseImportStmt() 
{ 
    return m_CurSymbol->kind() == Token::TokenKind::PY_IMPORT ? parseImportName() : parseImportFrom();
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseImportName() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseDottedAsNames();
    return std::make_shared<ASTImportStatementNode>(start, m_Lexer->getPosition(), op1, right); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseImportFrom() { return nullptr; }

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseImportAsName() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    std::shared_ptr<Token> op2 = nullptr;
    std::shared_ptr<Token> op3 = nullptr;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AS)
    {
        op2 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        op3 = m_CurSymbol;
        m_Lexer->advance();
    }
    return std::make_shared<ASTImportAsNameStatementNode>(start, m_Lexer->getPosition(), op1, op2, op3); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseDottedAsName() 
{
    unsigned int start = m_Lexer->getPosition();
    auto left = parseDottedName();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AS)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        return std::make_shared<ASTDottedNameAsStatementNode>(start, m_Lexer->getPosition(), left, op1, op2);
    }
    return left; 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseImportAsNames() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas;
    auto node = parseImportAsName();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        commas->push_back(m_CurSymbol);
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_SEMICOLON && m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE)
        {
            nodes->push_back(node); // First node
            nodes->push_back(parseImportAsName()); // second node
            while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
            {
                commas->push_back(m_CurSymbol);
                m_Lexer->advance();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_SEMICOLON && m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE)
                {
                    nodes->push_back(parseImportAsName());
                }
            }
        }
        return std::make_shared<ASTImportContainerStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_IMPORT_AS_NAMES, nodes, commas);
    }
    return node; 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseDottedAsNames() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas;
    auto node = parseDottedAsName();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        nodes->push_back(node);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            commas->push_back(m_CurSymbol);
            m_Lexer->advance();
            nodes->push_back(parseDottedAsName());
        }
        return std::make_shared<ASTImportContainerStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_DOTTED_AS_NAMES, nodes, commas);
    }
    return node; 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseDottedName() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> dots;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME)  throw ;
    nodes->push_back(m_CurSymbol);
    m_Lexer->advance();
    while (m_CurSymbol->kind() == Token::TokenKind::PY_PERIOD)
    {
        dots->push_back(m_CurSymbol);
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME)  throw ;
        nodes->push_back(m_CurSymbol);
        m_Lexer->advance();
    }
    return std::make_shared<ASTDottedNameStatementNode>(start, m_Lexer->getPosition(), nodes, dots); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseGlobalStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> separators;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
    while (m_CurSymbol->kind() == Token::TokenKind::PY_NAME)
    {
        nodes->push_back(m_CurSymbol);
        m_Lexer->advance();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_NAME) throw ;
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            separators->push_back(m_CurSymbol);
            m_Lexer->advance();
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw ;
        }
    }
    return std::make_shared<ASTGlobalNonLocalStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_GLOBAL_STMT, op1, nodes, separators); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseNonlocalStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> separators;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
    while (m_CurSymbol->kind() == Token::TokenKind::PY_NAME)
    {
        nodes->push_back(m_CurSymbol);
        m_Lexer->advance();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_NAME) throw ;
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            separators->push_back(m_CurSymbol);
            m_Lexer->advance();
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw ;
        }
    }
    return std::make_shared<ASTGlobalNonLocalStatementNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NONLOCAL_STMT, op1, nodes, separators); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseAssertStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto left = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseTest();
        return std::make_shared<ASTAssertStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right);
    }
    return std::make_shared<ASTAssertStatementNode>(start, m_Lexer->getPosition(), op1, left, nullptr, nullptr); 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseCompoundStmt() 
{ 
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_ASYNC:
            return parseAsyncStmt();
        case Token::TokenKind::PY_IF:
            return parseIfStmt();
        case Token::TokenKind::PY_WHILE:
            return parseWhileStmt();
        case Token::TokenKind::PY_FOR:
            return parseForStmt();
        case Token::TokenKind::PY_TRY:
            return parseTryStmt();
        case Token::TokenKind::PY_WITH:
            return parseWithStmt();
        case Token::TokenKind::PY_MATRICE:
        case Token::TokenKind::PY_DEF:
            return nullptr;
        case Token::TokenKind::PY_CLASS:
            return parseClassDef();
        default:    throw ;
    }
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseAsyncStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_FOR:
            {
                auto right = parseForStmt();
                return std::make_shared<ASTAsyncStatementNode>(start, m_Lexer->getPosition(), op, right);
            }
        case Token::TokenKind::PY_WITH:
            {
                auto right = parseWithStmt();
                return std::make_shared<ASTAsyncStatementNode>(start, m_Lexer->getPosition(), op, right);
            }
        case Token::TokenKind::PY_DEF:
            {
                auto right = parseFuncDef();
                return std::make_shared<ASTAsyncFuncDefStatementNode>(start, m_Lexer->getPosition(), op, right);
            }
        default:
            throw ;
    }
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseIfStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto left = parseNamedTest();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseSuite();
    auto res = std::make_shared<ASTIfStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right);
    while (m_CurSymbol->kind() == Token::TokenKind::PY_ELIF)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        auto left = parseNamedTest();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseSuite();
        auto node = std::make_shared<ASTElifStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right);
        res->addElifStatement(node);
    }
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ELSE) res->addElseStatement(parseElseStmt());
    res->addEndPosition(m_Lexer->getPosition());
    return res; 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseElseStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseSuite();
    return std::make_shared<ASTElseStatementNode>(start, m_Lexer->getPosition(), op1, op2, right);
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseWhileStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto left = parseNamedTest();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseSuite();
    auto res = std::make_shared<ASTWhileStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right);
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ELSE) res->addElseStatement(parseElseStmt());
    res->addEndPosition(m_Lexer->getPosition());
    return res; 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseForStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto left = parseExprList();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_IN) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseTestList();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op3 = m_CurSymbol;
    m_Lexer->advance();
    // Handle TypeComment here later.
    auto next = parseSuite();
    auto res = std::make_shared<ASTForStatementNode>(start, m_Lexer->getPosition(), op1, left, op2, right, op3, nullptr, next);
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ELSE) res->addElseStatement(parseElseStmt());
    res->addEndPosition(m_Lexer->getPosition());
    return res; 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseTryStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol; // 'try'
    m_Lexer->advance();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol; // ':'
    m_Lexer->advance();
    auto left = parseSuite();
    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes; // 'except'
    std::shared_ptr<ASTStatementNode> elsePart;
    std::shared_ptr<ASTStatementNode> finallyPart;
    bool hasSeenException = false;
    while (m_CurSymbol->kind() == Token::TokenKind::PY_EXCEPT)
    {
        auto op5 = m_CurSymbol; // 'except'
        m_Lexer->advance();
        auto first = m_CurSymbol->kind() != Token::TokenKind::PY_COLON ? parseExceptClause() : nullptr;
        if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
        auto op6 = m_CurSymbol; // ':'
        m_Lexer->advance();
        auto second = parseSuite();
        auto node = std::make_shared<ASTExceptStatementNode>(start, m_Lexer->getPosition(), op5, first, op6, second);
        nodes->push_back(node);
        hasSeenException = true;   
    }
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ELSE) elsePart = parseElseStmt();
    std::shared_ptr<Token> op3;
    std::shared_ptr<Token> op4;
    std::shared_ptr<ASTStatementNode> right;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_FINALLY && !hasSeenException) throw ;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_FINALLY)
    {
        op3 = m_CurSymbol; // 'try'
        m_Lexer->advance();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON) throw ;
        op4 = m_CurSymbol; // ':'
        m_Lexer->advance();
        right = parseSuite();
        finallyPart = std::make_shared<ASTFinallyStatementNode>(start, m_Lexer->getPosition(), op3, op4, right);
    }
    return std::make_shared<ASTTryStatementNode>(start, m_Lexer->getPosition(), op1, op2, left, nodes, elsePart, finallyPart);
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseWithStmt() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> nodes;
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas;
    nodes->push_back(parseWithItem());
    while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        commas->push_back(m_CurSymbol);
        m_Lexer->advance();
        nodes->push_back(parseWithItem());
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    // Handle TypeComment here later!
    auto right = parseSuite();
    return std::make_shared<ASTWithStatementNode>(start, m_Lexer->getPosition(), op1, nodes, commas, op2, nullptr, right); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseWithItem() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AS)
    {
        auto op = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseExpr();
        return std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_WITH_ITEM, left, op, right);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseExceptClause() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AS)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        auto op2 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        return std::make_shared<ASTExceptionClauseExpressionNode>(start, m_Lexer->getPosition(), left, op1, op2);
    }
    return left; 
}

std::shared_ptr<ASTStatementNode> PythonCoreParser::parseSuite() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_NEWLINE)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_INDENT) throw ;
        auto op2 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes;
        nodes->push_back(parseStmt());
        while (m_CurSymbol->kind() != Token::TokenKind::PY_DEDENT) nodes->push_back(parseStmt());
        auto op3 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        return std::make_shared<ASTSuiteStatementNode>(start, m_Lexer->getPosition(), op1, op2, nodes, op3);
    }
    return parseSimpleStmt(); 
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseClassDef()
{
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol; // 'class'
    m_CurSymbol = m_Lexer->advance();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
    auto op2 = m_CurSymbol; // 'NAME'
    m_CurSymbol = m_Lexer->advance();
    std::shared_ptr<Token> op3;
    std::shared_ptr<ASTExpressionNode> left;
    std::shared_ptr<Token> op4;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_PAREN)
    {
        auto op3 = m_CurSymbol; // '('
        m_CurSymbol = m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN)
        {
            left = parseArgList();
        }
        if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) throw ;
        auto op4 = m_CurSymbol; // ')'
        m_CurSymbol = m_Lexer->advance();
    }
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op5 = m_CurSymbol; // ':'
    m_CurSymbol = m_Lexer->advance();
    auto right = parseSuite();
    return std::make_shared<ASTClassDefStatementNode>(start, m_Lexer->getPosition(), op1, op2, op3, left, op4, op5, right);
}

std::shared_ptr<ASTStatementNode>  PythonCoreParser::parseFuncBodySuite() { return nullptr; }


// Expression rules ///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseNamedTest() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON_ASSIGN)
    {
        auto op = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseTest();
        return std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NAMED_EXPR, left, op, right);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTest() 
{   
    if (m_CurSymbol->kind() == Token::TokenKind::PY_LAMBDA) return parseLambdaCommon(true);
    unsigned int start = m_Lexer->getPosition();
    auto left = parseOrTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_IF)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseOrTest();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_ELSE) throw ;
        auto op2 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto next = parseTest();
        return std::make_shared<ASTTestExpressionNode>(start, m_Lexer->getPosition(), left, op1, right, op2, next);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTestNoCond() 
{ 
    return m_CurSymbol->kind() == Token::TokenKind::PY_LAMBDA ? parseLambdaCommon(false) : parseOrTest();
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseLambdaCommon(bool isConditional)
{
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto left = m_CurSymbol->kind() == Token::TokenKind::PY_COLON ? nullptr : nullptr /* VarArgsList */ ;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto right = isConditional ? parseTest() : parseOrTest();
    return std::make_shared<ASTLambdaExpressionNode>(start, m_Lexer->getPosition(), op1, left, op2, right); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseOrTest() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseAndTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_OR)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseAndTest();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR_TEST, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_OR)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseAndTest();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR_TEST, res, op1, right);
        }
        return res;
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAndTest() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseNotTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AND)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseNotTest();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND_TEST, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_AND)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseNotTest();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND_TEST, res, op1, right);
        }
        return res;
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseNotTest() 
{ 
    if (m_CurSymbol->kind() == Token::TokenKind::PY_NOT)
    {
        unsigned int start = m_Lexer->getPosition();
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseNotTest();
        return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NOT_TEST, op1, right);
    }
    return parseComparison(); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseComparison() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseExpr();
    auto symbol = m_CurSymbol->kind();
    if (symbol == Token::TokenKind::PY_LESS || symbol == Token::TokenKind::PY_LESS_EQUAL || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_GREATER_EQUAL ||
        symbol == Token::TokenKind::PY_GREATER || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_NOT_EQUAL || symbol == Token::TokenKind::PY_IS ||
        symbol == Token::TokenKind::PY_NOT || symbol == Token::TokenKind::PY_IN)
        {
            std::shared_ptr<ASTExpressionNode> res = left;
            std::shared_ptr<Token> op1;
            std::shared_ptr<Token> op2;
            auto kind = ASTNode::NodeKind::NK_INVALID;
            while ( symbol == Token::TokenKind::PY_LESS || symbol == Token::TokenKind::PY_LESS_EQUAL || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_GREATER_EQUAL ||
                    symbol == Token::TokenKind::PY_GREATER || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_NOT_EQUAL || symbol == Token::TokenKind::PY_IS ||
                    symbol == Token::TokenKind::PY_NOT || symbol == Token::TokenKind::PY_IN)
                    {
                        switch (symbol)
                        {
                            case Token::TokenKind::PY_LESS:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_LESS;
                                break;
                            case Token::TokenKind::PY_LESS_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_LESS_EQUAL;
                                break;
                            case Token::TokenKind::PY_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_EQUAL;
                                break;
                            case Token::TokenKind::PY_GREATER_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_GREATER_EQUAL;
                                break;
                            case Token::TokenKind::PY_GREATER:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_GREATER;
                                break;
                            case Token::TokenKind::PY_NOT_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_NOT_EQUAL;
                                break;
                            case Token::TokenKind::PY_IN:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_IN;
                                break;
                            case Token::TokenKind::PY_NOT:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                if (m_CurSymbol->kind() != Token::TokenKind::PY_IN) throw ;
                                op2 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_NOT_IN;
                                break;
                            case Token::TokenKind::PY_IS:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                if (m_CurSymbol->kind() == Token::TokenKind::PY_NOT)
                                {
                                     op2 = m_CurSymbol;
                                    m_CurSymbol = m_Lexer->advance();
                                    kind = ASTNode::NodeKind::NK_IS_NOT;
                                }
                                else kind = ASTNode::NodeKind::NK_IS;
                                break;
                            default:
                                break;
                        }
                        auto right = parseExpr();
                        res = std::make_shared<ASTComparisonExpressionNode>(start, m_Lexer->getPosition(), kind, res, op1, op2, right );
                    }
            return res;
        }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseStarExpr() 
{   
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto right = parseExpr();
    return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_STAR_EXPR, op1, right); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseXorExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_OR)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseXorExpr();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_OR)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseXorExpr();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR, res, op1, right);
        }
        return res;
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseXorExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseAndExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_XOR)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseAndExpr();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_XOR, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_XOR)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseAndExpr();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_XOR, res, op1, right);
        }
        return res;
    }
    return left;
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAndExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseShiftExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_AND)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseShiftExpr();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_BIT_AND)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseShiftExpr();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND, res, op1, right);
        }
        return res;
    }
    return left;
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseShiftExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseArithExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_SHIFT_LEFT || m_CurSymbol->kind() == Token::TokenKind::PY_SHIFT_RIGHT)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseArithExpr();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), op1->kind() == Token::TokenKind::PY_SHIFT_LEFT ? ASTNode::NodeKind::NK_SHIFT_LEFT : ASTNode::NodeKind::NK_SHIFT_RIGHT, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_SHIFT_LEFT || m_CurSymbol->kind() == Token::TokenKind::PY_SHIFT_RIGHT)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseArithExpr();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), op1->kind() == Token::TokenKind::PY_SHIFT_LEFT ? ASTNode::NodeKind::NK_SHIFT_LEFT : ASTNode::NodeKind::NK_SHIFT_RIGHT, res, op1, right);
        }
        return res;
    }
    return left;
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseArithExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseTerm();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_PLUS || m_CurSymbol->kind() == Token::TokenKind::PY_MINUS)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseTerm();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), op1->kind() == Token::TokenKind::PY_PLUS ? ASTNode::NodeKind::NK_PLUS : ASTNode::NodeKind::NK_MINUS, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_PLUS || m_CurSymbol->kind() == Token::TokenKind::PY_MINUS)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseTerm();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), op1->kind() == Token::TokenKind::PY_PLUS ? ASTNode::NodeKind::NK_PLUS : ASTNode::NodeKind::NK_MINUS, res, op1, right);
        }
        return res;
    }
    return left;
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTerm() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseFactor();
    auto symbol = m_CurSymbol->kind();
    if (symbol == Token::TokenKind::PY_MUL || symbol == Token::TokenKind::PY_DIV || symbol == Token::TokenKind::PY_FLOOR_DIV || symbol == Token::TokenKind::PY_MODULO ||
        symbol == Token::TokenKind::PY_MATRICE)
        {
            std::shared_ptr<ASTExpressionNode> res = left;
            std::shared_ptr<Token> op1;
            auto kind = ASTNode::NodeKind::NK_INVALID;
            while ( symbol == Token::TokenKind::PY_MUL || symbol == Token::TokenKind::PY_DIV || symbol == Token::TokenKind::PY_FLOOR_DIV || symbol == Token::TokenKind::PY_MODULO ||
                    symbol == Token::TokenKind::PY_MATRICE)
                    {
                        switch (symbol)
                        {
                            case Token::TokenKind::PY_MUL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_MUL;
                                break;
                            case Token::TokenKind::PY_DIV:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_DIV;
                                break;
                            case Token::TokenKind::PY_FLOOR_DIV:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_FLOOR_DIV;
                                break;
                            case Token::TokenKind::PY_MODULO:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_MODULO;
                                break;
                            case Token::TokenKind::PY_MATRICE:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_MATRICE;
                                break;
                            default:
                                break;
                        }
                        auto right = parseFactor();
                        res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), kind, res, op1, right );
                    }
            return res;
        }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseFactor() 
{
    if (m_CurSymbol->kind() == Token::TokenKind::PY_PLUS || m_CurSymbol->kind() == Token::TokenKind::PY_MINUS || m_CurSymbol->kind() == Token::TokenKind::PY_BIT_INVERT)
    {
        unsigned int start = m_Lexer->getPosition();
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseFactor();
        return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), op1->kind() == Token::TokenKind::PY_PLUS ? ASTNode::NodeKind::NK_PLUS : op1->kind() == Token::TokenKind::PY_MINUS ? ASTNode::NodeKind::NK_MINUS : ASTNode::NodeKind::NK_BIT_INVERT, op1, right);
    } 
    return parsePower(); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parsePower() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseAtomExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_POWER)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseFactor();
        return std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_POWER, left, op1, right);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAtomExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<Token> await;
    std::shared_ptr<ASTExpressionNode> right;
    bool isAwait = false;
    bool hasTrailer = false;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AWAIT)
    {
        await = m_CurSymbol;
        m_Lexer->advance();
        isAwait = true;
    }
    auto left = parseAtom();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_PAREN || m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_BRACKET || m_CurSymbol->kind() == Token::TokenKind::PY_PERIOD)
    {
        hasTrailer = true;
        right = parseTrailer();
    }
    if (!isAwait && !hasTrailer) return left; 
    return std::make_shared<ASTAtomExpressionNode>(start, m_Lexer->getPosition(), await, left, right);
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAtom() 
{ 
    unsigned int start = m_Lexer->getPosition();
    switch (m_CurSymbol->kind())
    {
        case Token::TokenKind::PY_NONE:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NONE, op1);
            }
            break;
        case Token::TokenKind::PY_FALSE:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_FALSE, op1);
            }
            break;
        case Token::TokenKind::PY_TRUE:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_TRUE, op1);
            }
            break;
        case Token::TokenKind::PY_ELIPSIS:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_ELIPSIS, op1);
            }
            break;
        case Token::TokenKind::PY_NAME:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NAME, op1);
            }
            break;
        case Token::TokenKind::PY_NUMBER:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NUMBER, op1);
            }
            break;
        case Token::TokenKind::PY_STRING:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_STRING)
                {
                    return std::make_shared<ASTLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_STRING, op1);
                }
                auto res = std::make_shared<ASTStringListExpressionNode>(start, m_Lexer->getPosition());
                res->addStringNode(op1);
                while (m_CurSymbol->kind() == Token::TokenKind::PY_STRING)
                {
                    op1 = m_CurSymbol;
                    m_Lexer->advance();
                    res->addStringNode(op1);
                }
                res->setEndPosition(m_Lexer->getPosition());
                return res;
            }
            break;
        case Token::TokenKind::PY_LEFT_PAREN:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                std::shared_ptr<ASTExpressionNode> right = nullptr;
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) 
                {
                    right = m_CurSymbol->kind() == Token::TokenKind::PY_YIELD ? parseYieldExpr() : parseTestListComp();
                }
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) throw ;
                auto op2 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTCompoundLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_TUPLE, op1, right, op2);
            }
        case Token::TokenKind::PY_LEFT_BRACKET:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                std::shared_ptr<ASTExpressionNode> right = nullptr;
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET) 
                {
                    right = parseYieldExpr();
                }
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET) throw ;
                auto op2 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTCompoundLiteralExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_LIST, op1, right, op2);
            }
        case Token::TokenKind::PY_LEFT_CURLY:
            {
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                std::shared_ptr<ASTExpressionNode> right = nullptr;
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_CURLY) 
                {
                    right = parseDictorSetMaker();
                }
                if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_CURLY) throw ;
                auto op2 = m_CurSymbol;
                m_Lexer->advance();
                return std::make_shared<ASTCompoundLiteralExpressionNode>(  start, m_Lexer->getPosition(), 
                                                                            right->kind() == ASTNode::NodeKind::NK_KV_LIST ? 
                                                                                ASTNode::NodeKind::NK_DICTIONARY : ASTNode::NodeKind::NK_SET
                                                                            , op1, right, op2);
            }
        default:    throw ;
    }
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTestListComp() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto node = m_CurSymbol->kind() == Token::TokenKind::PY_MUL ? parseStarExpr() : parseNamedTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC || m_CurSymbol->kind() == Token::TokenKind::PY_FOR)
    {
        auto right = parseCompFor();
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_TEST_LIST_COMP);
        res->addNodes(node, nullptr);
        res->addNodes(right, nullptr);
        return res;
    }
    else if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_TEST_LIST_COMP);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            res->addNodes(node, op);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw;
            if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET || m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN)
            {
                node = parseSubscript();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA) 
                {
                    res->addNodes(node, nullptr);
                }
            }
        }
        return res;
    }
    return node; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTrailer() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto res = std::make_shared<ASTTrailerListExpressionNode>(start, m_Lexer->getPosition());
    while (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_PAREN || m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_BRACKET || m_CurSymbol->kind() == Token::TokenKind::PY_PERIOD)
    {
        if (m_CurSymbol->kind() == Token::TokenKind::PY_PERIOD)
        {
            auto op1 = m_CurSymbol;
            m_Lexer->advance();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
            auto op2 = m_CurSymbol;
            m_Lexer->advance();
            auto node = std::make_shared<ASTDotNameExpressionNode>(start, m_Lexer->getPosition(), op1, op2);
            res->addTrailerNode(node);
        }
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_BRACKET)
        {
            auto op1 = m_CurSymbol;
            m_Lexer->advance();
            auto right = parseSubscriptList();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET) throw ;
            auto op2 = m_CurSymbol;
            m_Lexer->advance();
            auto node = std::make_shared<ASTIndexExpressionNode>(start, m_Lexer->getPosition(), op1, right, op2);
            res->addTrailerNode(node);
        }
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_LEFT_PAREN)
        {
            auto op1 = m_CurSymbol;
            m_Lexer->advance();
            auto right = parseArgList();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN) throw ;
            auto op2 = m_CurSymbol;
            m_Lexer->advance();
            auto node = std::make_shared<ASTCallExpressionNode>(start, m_Lexer->getPosition(), op1, right, op2);
            res->addTrailerNode(node);   
        }
    }
    res->addEndPosition(m_Lexer->getPosition());
    return res; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseSubscriptList() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto node = parseSubscript();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_SUBSCRIPTION_LIST);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            res->addNodes(node, op);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw;
            if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET)
            {
                node = parseSubscript();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA) 
                {
                    res->addNodes(node, nullptr);
                }
            }
        }
        return res;
    }
    return node;
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseSubscript() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<ASTExpressionNode> left, right, next;
    std::shared_ptr<Token> op1, op2;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON)
    {
        left = parseTest();
    }
    if (left == nullptr && m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON)
    {
        op1 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON && m_CurSymbol->kind() != Token::TokenKind::PY_COMMA && m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET)
        {
            right = parseTest();
        }
        if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON)
        {
            op2 = m_CurSymbol;
            m_Lexer->advance();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA && m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_BRACKET)
            {
                next = parseTest();
            }
        }
    }
    return std::make_shared<ASTSubscriptionExpressionNode>(start, m_Lexer->getPosition(), left, op1, right, op2, next);
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseExprList() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto node = m_CurSymbol->kind() == Token::TokenKind::PY_MUL ? parseStarExpr() : parseExpr();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_EXPR_LIST);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            res->addNodes(node, op);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw;
            if (m_CurSymbol->kind() != Token::TokenKind::PY_IN)
            {
                node = m_CurSymbol->kind() == Token::TokenKind::PY_MUL ? parseStarExpr() : parseExpr();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA) 
                {
                    res->addNodes(node, nullptr);
                }
            }
        }
        return res;
    }
    return node; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTestList() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto node = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_TEST_LIST);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            res->addNodes(node, op);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw;
            if (m_CurSymbol->kind() != Token::TokenKind::PY_SEMICOLON && m_CurSymbol->kind() != Token::TokenKind::PY_NEWLINE)
            {
                node = parseTest();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA) 
                {
                    res->addNodes(node, nullptr);
                }
            }
        }
        return res;
    }
    return node; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseDictorSetMaker() 
{ 
    unsigned int start = m_Lexer->getPosition();
    std::shared_ptr<ASTExpressionNode> key = nullptr;
    std::shared_ptr<Token> colon = nullptr;
    std::shared_ptr<ASTExpressionNode> values = nullptr;
    std::shared_ptr<Token> Comma = nullptr;
    std::shared_ptr<ASTDictionarySetDataExpressionNode> res = nullptr;
    bool isDictionary = true;

    if (m_CurSymbol->kind() == Token::TokenKind::PY_MUL)
    {
        key = parseStarExpr();
        res = std::make_shared<ASTDictionarySetDataExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_SET_LIST);
        res->addSetEntry(key);
        isDictionary = false;
    }
    else if (m_CurSymbol->kind() == Token::TokenKind::PY_POWER)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseExpr();
        key = std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_POWER, op1, right);
        res = std::make_shared<ASTDictionarySetDataExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_KV_LIST);
        res->addSetEntry(key); // Yes, even though we are dictionary.
    }
    else
    {
        key = parseTest();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            auto value = parseTest();
            res = std::make_shared<ASTDictionarySetDataExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_KV_LIST);
            res->addDictionaryEntry(key, op, value);
        }
        else
        {
            res = std::make_shared<ASTDictionarySetDataExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_SET_LIST);
            res->addSetEntry(key);
            isDictionary = false;
        }
    }

    if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC || m_CurSymbol->kind() == Token::TokenKind::PY_FOR)
    {
        auto node = parseCompFor();
        res->addSetEntry(key); // Yes, regardless of dictionary or set.
    }
    else
    {
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            Comma = m_CurSymbol;
            m_Lexer->advance();
            res->addComma(Comma);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw ;
            if (m_CurSymbol->kind() == Token::TokenKind::PY_RIGHT_CURLY) continue;

            if (m_CurSymbol->kind() == Token::TokenKind::PY_MUL)
            {
                if (isDictionary) throw ;
                key = parseStarExpr();
                res->addSetEntry(key);
                isDictionary = false;
            }
            else if (m_CurSymbol->kind() == Token::TokenKind::PY_POWER)
            {
                if (!isDictionary) throw ;
                auto op1 = m_CurSymbol;
                m_Lexer->advance();
                auto right = parseExpr();
                key = std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_POWER, op1, right);
                res->addSetEntry(key); // Yes, even though we are dictionary.
            }
            else
            {
                key = parseTest();
                if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON)
                {
                    if (!isDictionary) throw ;
                    auto op = m_CurSymbol;
                    m_Lexer->advance();
                    auto value = parseTest();
                    res->addDictionaryEntry(key, op, value);
                }
                else
                {
                    if (isDictionary) throw ;
                    res->addSetEntry(key);
                    isDictionary = false;
                }
            }
        }
    }
    res->addEndPosition(m_Lexer->getPosition());
    return res; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseArgList() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto node = parseArgument();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
    {
        auto res = std::make_shared<ASTListExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_ARGLIST);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA)
        {
            auto op = m_CurSymbol;
            m_Lexer->advance();
            res->addNodes(node, op);
            if (m_CurSymbol->kind() == Token::TokenKind::PY_COMMA) throw;
            if (m_CurSymbol->kind() != Token::TokenKind::PY_RIGHT_PAREN)
            {
                node = parseArgument();
                if (m_CurSymbol->kind() != Token::TokenKind::PY_COMMA) 
                {
                    res->addNodes(node, nullptr);
                }
            }
        }
        return res;
    }
    return node; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseArgument() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_MUL)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        return std::make_shared<ASTListArgumentExpressionNode>(start, m_Lexer->getPosition(), op1, op2);
    }
    else if (m_CurSymbol->kind() == Token::TokenKind::PY_POWER)
    {
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        return std::make_shared<ASTKWArgumentExpressionNode>(start, m_Lexer->getPosition(), op1, op2);
    }
    else
    {
        if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
        auto op1 = m_CurSymbol;
        m_Lexer->advance();
        if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC || m_CurSymbol->kind() == Token::TokenKind::PY_FOR)
        {
            auto right = parseCompFor();
            return std::make_shared<ASTCompArgumentExpressionNode>(start, m_Lexer->getPosition(), op1, right);
        }
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON_ASSIGN)
        {
            auto op2 = m_CurSymbol;
            m_Lexer->advance();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
            auto op3 = m_CurSymbol;
            m_Lexer->advance();
            return std::make_shared<ASTColonAssignArgumentExpressionNode>(start, m_Lexer->getPosition(), op1, op2, op3);
        }
        else if (m_CurSymbol->kind() == Token::TokenKind::PY_ASSIGN)
        {
            auto op2 = m_CurSymbol;
            m_Lexer->advance();
            if (m_CurSymbol->kind() != Token::TokenKind::PY_NAME) throw ;
            auto op3 = m_CurSymbol;
            m_Lexer->advance();
            return std::make_shared<ASTAssignArgumentExpressionNode>(start, m_Lexer->getPosition(), op1, op2, op3);
        }
        else
        {
            return std::make_shared<ASTArgumentExpressionNode>(start, m_Lexer->getPosition(), op1);
        }  
    }
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseCompIter() 
{ 
    return m_CurSymbol->kind() == Token::TokenKind::PY_FOR || m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC ? parseCompFor() : parseCompIf(); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseSyncCompFor() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_FOR) throw ;
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    auto left = parseExprList();
    if (m_CurSymbol->kind() != Token::TokenKind::PY_IN) throw ;
    auto op2 = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseOrTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC || m_CurSymbol->kind() == Token::TokenKind::PY_FOR || m_CurSymbol->kind() == Token::TokenKind::PY_IF)
    {
        auto next = parseCompIter();
        return std::make_shared<ASTSyncCompForExpressionNode>(start, m_Lexer->getPosition(), op1, left, op2, right, next);
    }
    return std::make_shared<ASTSyncCompForExpressionNode>(start, m_Lexer->getPosition(), op1, left, op2, right, nullptr); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseCompFor() 
{ 
    unsigned int start = m_Lexer->getPosition();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC)
    {
        auto op = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseSyncCompFor();
        return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_COMP_FOR, op, right);
    }
    return parseSyncCompFor(); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseCompIf() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op = m_CurSymbol;
    m_Lexer->advance();
    auto right = parseTestNoCond();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_ASYNC || m_CurSymbol->kind() == Token::TokenKind::PY_FOR || m_CurSymbol->kind() == Token::TokenKind::PY_IF)
    {
        auto next = parseCompIter();
        return std::make_shared<ASTIfComprehensionExpressionNode>(start, m_Lexer->getPosition(), op, right, next);
    }
    return std::make_shared<ASTIfComprehensionExpressionNode>(start, m_Lexer->getPosition(), op, right, nullptr);
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseYieldExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_Lexer->advance();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_FROM)
    {
        auto op2 = m_CurSymbol;
        m_Lexer->advance();
        auto right = parseTest();
        return std::make_shared<ASTYieldFromExpressionNode>(start, m_Lexer->getPosition(), op1, op2, right);
    }
    auto right = parseTestListStarExpr();
    return std::make_shared<ASTYieldExpression>(start, m_Lexer->getPosition(), op1, right); 
}