//
//  Parser.h
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#ifndef Parser_h
#define Parser_h
#include <iostream>
#include <vector>
#include <queue>
#include "Scanner.h"
using namespace std;
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
class Error
{
public:
    string msj;
    int posicion;
    Error(string s,int p)
    {
        msj = s;
        posicion = p;
    }
    Error()
    {
        msj = " ";
        posicion = 0;
    }
};
class Parser
{
private:
    int pos;
    Token token;
    Token nextToken();
    vector<Error> listaerrores;
    vector<Token> tok;
    vector<AST*> astvector;
    AST* ast;
    AST* statement;
    AST* root = nullptr;
    bool deteccionError;
    void AgregarError(string, int);
    void SaltarError();
    void Imprimir2();
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

public:
    void AST_Tree();
    void imprimirAST_Tree(); // astvector
    AST* generateAST(AST* root);
    bool Init();
    void ImprimirErrores();
};
#endif /* Parser_h */
