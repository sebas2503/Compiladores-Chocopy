#ifndef AST_h
#define AST_h

#include <iostream>
#include <vector>
#include <queue>
#include "ScannerAST.h"
using namespace std;
//tipo -> valor
vector<pair<string, string>> tok;

class AST
{
public:
    string valor;
    string tipo; // para ID
    vector<AST*> hijos;
    bool isleaf;
    AST(string v, bool l = false, string _tipo = "")
    {
        valor = v;
        isleaf = l;
        tipo = _tipo;
    }

    void agregar(AST* nodo)
    {
        //cout << "NODO AGREGADO: " << nodo->valor << endl;
        hijos.push_back(nodo);
    }
    int canthijos()
    {
        return hijos.size();
    }
};

class ParserAST
{
public:
    AST* root = nullptr; // root del arbol PARSEO
    AST* statement;
    AST* ast; // root de AST
    int pos;
    pair<string, string> token;
    pair<string, string> nextToken();
    vector<string> listaerrores;
    vector<AST*> astvector;
    void Error(string);
    void Imprimir2();
    void ImprimirErrores();
    void match(string);

    void printAST(AST* root);
    void printAST2(AST* root);
    void imprimirAST(); // parseo
    void imprimirAST2(); // AST
    //---Funciones/Produccines---
    AST* Program();
    AST* DefList();
    AST* Def();
    AST* TypedVar();
    AST* Type();
    AST* TypedVarList();
    AST* TypedVarListTail();
    AST* Return();
    AST* Block();
    AST* StatementList();
    AST* Statement();
    AST* ElifList();
    AST* Elif();
    AST* Else();
    AST* SimpleStatement();
    AST* SSTail();
    AST* ReturnExpr();
    AST* Expr();
    AST* ExprPrime();
    AST* orExpr();
    AST* orExprPrime();
    AST* andExpr();
    AST* andExprPrime();
    AST* notExpr();
    AST* notExprPrime();
    AST* CompExpr();
    AST* CompExprPrime();
    AST* intExpr();
    AST* intExprPrime();
    AST* CompOp();
    AST* Term();
    AST* TermPrime();
    AST* Factor();
    AST* Name();
    AST* NameTail();
    AST* Literal();
    AST* ExprList();
    AST* ExprListTail();
    AST* List();

    ///
    void AST_Tree();
    void imprimirAST_Tree(); // astvector
    AST* generateAST(AST* root);
};

#endif /* AST_h */
