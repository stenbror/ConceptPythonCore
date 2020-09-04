
#ifndef PYTHONCORE_PARSER_H
#define PYTHONCORE_PARSER_H

#include <abstractSyntaxTree.h>
#include <pythonCoreTokenizer.h>

namespace PythonCore::Runtime
{
    class PythonCoreParser
    {
        public:
            std::shared_ptr<ASTStatementNode>  parseSingleInput(std::shared_ptr<PythonCoreTokenizer> lexer);
            std::shared_ptr<ASTStatementNode>  parseFileInput(std::shared_ptr<PythonCoreTokenizer> lexer);
            std::shared_ptr<ASTStatementNode>  parseEvalInput(std::shared_ptr<PythonCoreTokenizer> lexer);

        protected:
            std::shared_ptr<ASTStatementNode>  parseDecorator();
            std::shared_ptr<ASTStatementNode>  parseDecorators();
            std::shared_ptr<ASTStatementNode>  parseDecorated();
            std::shared_ptr<ASTStatementNode>  parseAsyncFuncDef();
            std::shared_ptr<ASTStatementNode>  parseFuncDef();
            std::shared_ptr<ASTStatementNode>  parseParameters();
            std::shared_ptr<ASTStatementNode>  parseTypedArgsList();
            std::shared_ptr<ASTStatementNode>  parseTFPDef();
            std::shared_ptr<ASTStatementNode>  parseVarArgsList();
            std::shared_ptr<ASTStatementNode>  parseVFPDef();
            std::shared_ptr<ASTStatementNode>  parseStmt();
            std::shared_ptr<ASTStatementNode>  parseSimpleStmt();
            std::shared_ptr<ASTStatementNode>  parseSmallStmt();
            std::shared_ptr<ASTStatementNode>  parseExprStatement();
            std::shared_ptr<ASTStatementNode>  parseTestListStarExpr();
            std::shared_ptr<ASTStatementNode>  parseDelStmt();
            std::shared_ptr<ASTStatementNode>  parsePassStmt();
            std::shared_ptr<ASTStatementNode>  parseFlowStmt();
            std::shared_ptr<ASTStatementNode>  parseBreakStmt();
            std::shared_ptr<ASTStatementNode>  parseContinueStmt();
            std::shared_ptr<ASTStatementNode>  parseReturnStmt();
            std::shared_ptr<ASTStatementNode>  parseYieldStmt();
            std::shared_ptr<ASTStatementNode>  parseRaiseStmt();
            std::shared_ptr<ASTStatementNode>  parseImportStmt();
            std::shared_ptr<ASTStatementNode>  parseImportName();
            std::shared_ptr<ASTStatementNode>  parseImportFrom();
            std::shared_ptr<ASTStatementNode>  parseImportAsName();
            std::shared_ptr<ASTStatementNode>  parseDottedAsName();
            std::shared_ptr<ASTStatementNode>  parseImportAsNames();
            std::shared_ptr<ASTStatementNode>  parseDottedAsNames();
            std::shared_ptr<ASTStatementNode>  parseDottedName();
            std::shared_ptr<ASTStatementNode>  parseGlobalStmt();
            std::shared_ptr<ASTStatementNode>  parseNonlocalStmt();
            std::shared_ptr<ASTStatementNode>  parseAssertStmt();
            std::shared_ptr<ASTStatementNode>  parseCompoundStmt();
            std::shared_ptr<ASTStatementNode>  parseAsyncStmt();
            std::shared_ptr<ASTStatementNode>  parseIfStmt();
            std::shared_ptr<ASTStatementNode>  parseElseStmt();
            std::shared_ptr<ASTStatementNode>  parseWhileStmt();
            std::shared_ptr<ASTStatementNode>  parseForStmt();
            std::shared_ptr<ASTStatementNode>  parseTryStmt();
            std::shared_ptr<ASTStatementNode>  parseWithStmt();
            std::shared_ptr<ASTExpressionNode> parseWithItem();
            std::shared_ptr<ASTExpressionNode> parseExceptClause();
            std::shared_ptr<ASTStatementNode>  parseSuite();
            std::shared_ptr<ASTStatementNode>  parseClassDef();

            std::shared_ptr<ASTExpressionNode> parseNamedTest();
            std::shared_ptr<ASTExpressionNode> parseTest();
            std::shared_ptr<ASTExpressionNode> parseTestNoCond();
            std::shared_ptr<ASTExpressionNode> parseLambdaCommon(bool isConditional);
            std::shared_ptr<ASTExpressionNode> parseOrTest();
            std::shared_ptr<ASTExpressionNode> parseAndTest();
            std::shared_ptr<ASTExpressionNode> parseNotTest();
            std::shared_ptr<ASTExpressionNode> parseComparison();
            std::shared_ptr<ASTExpressionNode> parseStarExpr();
            std::shared_ptr<ASTExpressionNode> parseExpr();
            std::shared_ptr<ASTExpressionNode> parseXorExpr();
            std::shared_ptr<ASTExpressionNode> parseAndExpr();
            std::shared_ptr<ASTExpressionNode> parseShiftExpr();
            std::shared_ptr<ASTExpressionNode> parseArithExpr();
            std::shared_ptr<ASTExpressionNode> parseTerm();
            std::shared_ptr<ASTExpressionNode> parseFactor();
            std::shared_ptr<ASTExpressionNode> parsePower();
            std::shared_ptr<ASTExpressionNode> parseAtomExpr();
            std::shared_ptr<ASTExpressionNode> parseAtom();
            std::shared_ptr<ASTExpressionNode> parseTestListComp();
            std::shared_ptr<ASTExpressionNode> parseTrailer();
            std::shared_ptr<ASTExpressionNode> parseSubscriptList();
            std::shared_ptr<ASTExpressionNode> parseSubscript();
            std::shared_ptr<ASTExpressionNode> parseExprList();
            std::shared_ptr<ASTExpressionNode> parseTestList();
            std::shared_ptr<ASTExpressionNode> parseDictorSetMaker();
            std::shared_ptr<ASTExpressionNode> parseArgList();
            std::shared_ptr<ASTExpressionNode> parseArgument();
            std::shared_ptr<ASTExpressionNode> parseCompIter();
            std::shared_ptr<ASTExpressionNode> parseSyncCompFor();
            std::shared_ptr<ASTExpressionNode> parseCompFor();
            std::shared_ptr<ASTExpressionNode> parseCompIf();
            std::shared_ptr<ASTExpressionNode> parseYieldExpr();


        protected:
            std::shared_ptr<Token> m_CurSymbol;
            std::shared_ptr<PythonCoreTokenizer> m_Lexer;
    };
} // namespace PythonCore::Runtime

#endif