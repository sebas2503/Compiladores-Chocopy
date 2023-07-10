
//
//  main.cpp
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#include <iostream>
#include <vector>
#include "Scanner2.h"
using namespace std;
//tipo -> valor
vector<pair<string, string>> tok;
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
    pair<string, string> token;
    pair<string,string> nextToken();
    vector<Error> listaerrores;
    bool deteccionError;
    void AgregarError(string,int);
    void Imprimir2();
    void match(string);
    //---Funciones/Produccines---
    bool Program();
    bool DefList();
    bool Def();
    bool TypedVar();
    bool Type();
    bool TypedVarList();
    bool TypedVarListTail();
    bool Return();
    bool Block();
    bool StatementList();
    bool Statement();
    bool ElifList();
    bool Elif();
    bool Else();
    bool SimpleStatement();
    bool SSTail();
    bool ReturnExpr();
    bool Expr();
    bool ExprPrime();
    bool orExpr();
    bool orExprPrime();
    bool andExpr();
    bool andExprPrime();
    bool notExpr();
    bool notExprPrime();
    bool CompExpr();
    bool CompExprPrime();
    bool intExpr();
    bool intExprPrime();
    bool CompOp();
    bool Term();
    bool TermPrime();
    bool Factor();
    bool Name();
    bool NameTail();
    bool Literal();
    bool ExprList();
    bool ExprListTail();
    bool List();
public:
    bool Init();
    void ImprimirErrores();
};
bool Parser::Init()
{
    tok = Realizar();
    pos = 0;
    deteccionError = false;
    token = tok[0];
    Imprimir2();
    cout<<endl;
    cout <<"---------Parser---------\n";
    if(Program())
    {
        if(deteccionError == false)
            return 1;
    }
    cout<<"pos: "<<pos<<endl;
    return 0;
}
void Parser::AgregarError(string error, int pos2)
{
    class Error aux(error,pos2);
    deteccionError = true;
    listaerrores.push_back(aux);
    while(token.first != "NEWLINE" && tok[pos+1].first != "$")
    {
        //cout<<"Antes del salto en la pos "<<pos<<endl;
        //cout<<token.first<<" "<<token.second<<endl;
        token = nextToken();
        //cout<<"Despues del salto en la pos "<<pos<<endl;
        //cout<<token.first<<" "<<token.second<<endl;
        if(token.first == "NEWLINE")
        {
            pos = pos - 1;
            token = tok[pos]; //Hacer un retroceso.
            //cout<<"Encontre un newline: "<<token.first<<" "<<token.second<<endl;
            break;
        }
        if(token.first == "$")
            break;
    }
    
}
pair<string, string> Parser::nextToken()
{
    /*pos = pos + 1;
    if(pos > tok.size()-1)
    {
        pos=pos-1;
    }
    return tok[pos];*/
    if(pos+1 < tok.size())
    {
        pos+=1;
    }
    return tok[pos];
    //else
      //  return tok[pos-1];
}
void Parser::Imprimir2()
{
    for (int i = 0; i < tok.size(); i++)
    {
        cout<<i<<"->";
        cout << "(" << tok[i].first << "," << tok[i].second << ") ";
    }
}
void Parser::ImprimirErrores()
{
    if(listaerrores.size() != 0)
    {
        if(listaerrores[listaerrores.size()-1].msj == "Error de DEDENT" || listaerrores[listaerrores.size()-1].msj == "Error de INDENT ")
            listaerrores.pop_back();
    }
    for (int i = 0; i < listaerrores.size(); i++)
        cout << listaerrores[i].msj << " "<<listaerrores[i].posicion<< endl;
}

bool Parser::Program()
{
    if (DefList())
    {
        if(StatementList())
            return 1;
    }
    return 0;

}
bool Parser::DefList()
{
    if (Def())
    {
        if(DefList())
            return 1;
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[")
        return 1;
    return 0;
}
bool Parser::Def()
{
    if (token.second == "def")
    {
        token = nextToken();
        if (token.first == "ID")
        {
            token = nextToken();
            if (token.second == "(")
            {
                token = nextToken();
                if (TypedVarList())
                {
                    if (token.second == ")")
                    {
                        token = nextToken();
                        if (Return())
                        {
                            if (token.second == ":")
                            {
                                token = nextToken();
                                if(Block())
                                    return 1;
                            }
                            else
                            {
                                AgregarError("Error de : (Produccion Def) ", pos);
                                token = nextToken();
                                cout<<"token: "<<token.first<<" "<<token.second<<endl;
                                if(Block())
                                    return 1;
                            }
                        }
                        else
                        {
                            AgregarError("Error def incompleto se esperaba como follow un : ",pos);
                            cout<<"token: "<<token.first<<" "<<token.second<<endl;
                            if(Block())
                                return 1;
                        }
                    }
                    else
                    {
                        AgregarError("Error de ) (Produccion Def) ", pos);
                        token = nextToken();
                        cout<<"token: "<<token.first<<" "<<token.second<<endl;
                        if(Block())
                            return 1;
                    }
                }
            }
            else
            {
                AgregarError("Error de ( (Produccion Def) ", pos);
                token = nextToken();
                cout<<"token: "<<token.first<<" "<<token.second<<endl;
                if(Block())
                    return 1;
            }

        }
        else
        {
            AgregarError("Error de ID (Produccion Def) ", pos);
            token = nextToken();
            cout<<"token: "<<token.first<<" "<<token.second<<endl;
            if(Block())
                return 1;
        }
    }
    return 0;
}
bool Parser::TypedVar()
{
    if (token.first == "ID")
    {
        token = nextToken();
        if (token.second == ":")
        {
            token = nextToken();
            if (Type())
                return 1;
        }
        else
        {
            AgregarError("Error de : (Produccion TypedVar) ",pos);
            return 1;
        }
    }
    return 0;
}
bool Parser::Type()
{
    if (token.second == "int")
    {
        token = nextToken();
        return 1;
    }
    if (token.second == "str")
    {
        token = nextToken();
        return 1;
    }
    if (token.second == "[")
    {
        token = nextToken();
        if (Type())
        {
            if (token.second == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error de ] (Produccion Type) ",pos);
                return 1;
            }
        }
    }
    return 0;
}
bool Parser::TypedVarList()
{
    if (TypedVar())
    {
        if (TypedVarListTail())
            return 1;
    }
    if (token.second == ")")
        return 1;
    AgregarError("Se esperaba como follow un )", pos);
    return 1;
}
bool Parser::TypedVarListTail()
{
    if (token.second == ",")
    {
        token = nextToken();
        if (TypedVar())
        {
            if(TypedVarListTail())
                return 1;
        }
    }
    if (token.second == ")")
        return 1;
    AgregarError("Se esperaba como follow un )",pos);
    return 1;
}
bool Parser::Return()
{
    if (token.second == "->")
    {
        token = nextToken();
        if (Type())
            return 1;
    }
    if (token.second == ":")
        return 1;
    return 0;
    //return 1;
}
bool Parser::Block()
{
    if (token.first == "NEWLINE")
    {
        token = nextToken();
        if (token.first == "INDENT")
        {
            token = nextToken();
            if (Statement())
            {
                if (StatementList())
                {
                    if (token.first == "DEDENT")
                    {
                        token = nextToken();
                        return 1;
                    }
                    else
                    {
                        AgregarError("Error de DEDENT",pos);
                        token = nextToken();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error de StatementList",pos);
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de INDENT ",pos);
                return 1;
            }
        }
    }
    return 0;
}
bool Parser::StatementList()
{
    if (Statement())
    {
        if(StatementList())
            return 1;
    }
    //{$,DEDENT}
    if (token.second == "$" || token.first == "DEDENT")
        return 1;
    return 0;
}
bool Parser::Statement()
{
    if (token.second == "if")
    {
        token = nextToken();
        if (Expr())
        {
            if (token.second == ":")
            {
                token = nextToken();
                if (Block())
                {
                    if (ElifList())
                    {
                        if(Else())
                            return 1;
                    }
                }
            }
            else
            {
                AgregarError("Error de : (Produccion Statement)-if ",pos);
                token = nextToken();
                cout<<"token: "<<token.first<<" "<<token.second<<endl;
                if(Block())
                    return 1;
            }
        }
    }
    if(token.second == "while")
    {
        token = nextToken();
        if(Expr())
        {
            if(token.second == ":")
            {
                token = nextToken();
                if(Block())
                    return 1;
            }
            else
            {
                AgregarError("Error de : (Produccion Statement)-while",pos);
                token = nextToken();
                cout<<"token: "<<token.first<<" "<<token.second<<endl;
                if(Block())
                    return 1;
            }
        }
    }
    if (token.second == "for")
    {
        token = nextToken();
        if (token.first == "ID")
        {
            token = nextToken();
            if (token.second == "in")
            {
                token = nextToken();
                if (Expr())
                {
                    if (token.second == ":")
                    {
                        token = nextToken();
                        if(Block())
                            return 1;
                    }
                    else
                    {
                        AgregarError("Error de : (Produccion Statement)-for ",pos);
                        token = nextToken();
                        //token = nextToken();
                        cout<<"token: "<<token.first<<" "<<token.second<<endl;
                        if(Block())
                            return 1;
                    }
                }
            }
            else
            {
                AgregarError("Error de in (Produccion Statement)-for",pos);
                token = nextToken();
                //token = nextToken();
                cout<<"token: "<<token.first<<" "<<token.second<<endl;
                if(Block())
                    return 1;
            }
        }
        else
        {
            AgregarError("Error de ID (Produccion Statement)-for",pos);
            token = nextToken();
            //token = nextToken();
            cout<<"token: "<<token.first<<" "<<token.second<<endl;
            if(Block())
                return 1;
        }
    }
    if (SimpleStatement())
    {
        if (token.first == "NEWLINE")
        {
            token = nextToken();
            return 1;
        }
    }
    return 0;
}
bool Parser::ElifList()
{
    if (Elif())
    {
        if(ElifList())
            return 1;
    }
    //DEDENT, if, while, for, pass, return, -, (, ID, None, True, False, INTEGER, STRING, [, else, $
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[,def,DEDENT,else,elif}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "["|| token.second == "DEDENT" || token.second == "else" || token.second == "elif")
        return 1;
    return 0;
}
bool Parser::Elif()
{
    if (token.second == "elif")
    {
        token = nextToken();
        if (Expr())
        {
            if (token.second == ":")
            {
                token = nextToken();
                if(Block())
                    return 1;
            }
           else
           {
               AgregarError("Error de : (Produccion Elif) ",pos);
               token = nextToken();
               cout<<"token: "<<token.first<<" "<<token.second<<endl;
               if(Block())
                   return 1;
           }
        }
    }
    return 0;
}
bool Parser::Else()
{
    if (token.second == "else")
    {
        token = nextToken();
        if (token.second == ":")
        {
            token = nextToken();
            if(Block())
                return 1;
        }
        else
        {
            AgregarError("Error de : (Produccion Else)",pos);
            if(Block())
                return 1;
        }
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[,def,DEDENT,elif,else}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[" ||token.first == "DEDENT" || token.second == "def" || token.second == "elif" || token.second == "else")
        return 1;
    return 0;
}
bool Parser::SSTail()
{
    if (token.second == "=")
    {
        token = nextToken();
        if(Expr())
            return 1;
    }
    if (token.first == "NEWLINE")
        return 1;
    AgregarError("Se esperaba como follow un NEWLINE (Produccion SSTail) ",pos);
    return 0;
}
bool Parser::SimpleStatement()
{
    if (Expr())
    {
        if(SSTail())
            return 1;
    }
    if (token.second == "pass")
    {
        token = nextToken();
        return 1;
    }
    if (token.second == "return")
    {
        token = nextToken();
        if(ReturnExpr())
            return 1;
    }
    return 0;
}
bool Parser::ReturnExpr()
{
    if (Expr())
        return 1;
    //{NEWLINE}
    if (token.first == "NEWLINE")
        return 1;
    AgregarError("Se esperaba como follow un NEWLINE (Produccion ReturnExpr)",pos);
    return 0;
}
bool Parser::Expr()
{
    if (orExpr())
        if (ExprPrime())
            return 1;
    return 0;
}
bool Parser::ExprPrime()
{
    if (token.second == "if")
    {
        token = nextToken();
        if (andExpr())
        {
            if (token.second == "else")
            {
                token = nextToken();
                if (andExpr())
                    if(ExprPrime())
                        return 1;
            }
            else
            {
                AgregarError("Error de else (Produccion ExprPrime)",pos);
                return 1;
            }
        }
    }
    //{:,NEWLINE,=,),],,}
    if (token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::orExpr()
{
    if (andExpr())
        if(orExprPrime())
           return 1;
    return 0;
}
bool Parser::orExprPrime()
{
    if (token.second == "or")
    {
        token = nextToken();
        if (andExpr())
            if(orExprPrime())
                return 1;
    }
    //{if,:,NEWLINE,=,),],,}
    if (token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::andExpr()
{
    if (notExpr())
        if(andExprPrime())
            return 1;
    return 0;
}
bool Parser::andExprPrime()
{
    if (token.second == "and")
    {
        token = nextToken();
        if (notExpr())
            if(andExprPrime())
                return 1;
    }
    //{else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::notExpr()
{
    if (CompExpr())
        if(notExprPrime())
            return 1;
    return 0;
}
bool Parser::notExprPrime()
{
    if (token.second == "not")
    {
        token = nextToken();
        if (CompExpr())
            if(notExprPrime())
                return 1;
    }
    //{and,else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::CompExpr()
{
    if (intExpr())
        if(CompExprPrime())
            return 1;
    return 0;
}
bool Parser::CompExprPrime()
{
    if (CompOp())
    {
        if (intExpr())
            if(CompExprPrime())
                return 1;
    }
    //{not,and,else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::intExpr()
{
    if (Term())
        if(intExprPrime())
            return 1;
    return 0;
}
bool Parser::intExprPrime()
{
    if (token.second == "+" || token.second == "-")
    {
        token = nextToken();
        if (Term())
        {
            if(intExprPrime())
                return 1;
        }
        else
        {
            AgregarError("Error en la produccion falta un ID",pos);
            return 0;
        }
    }
    //{not,and,else,if,:,NEWLINE,=,or,==,!=,<,>,<=,>=,is,),],,}
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::Term()
{
    if (Factor())
        if(TermPrime())
            return 1;
    return 0;
}
bool Parser::TermPrime()
{
    if (token.second == "*" || token.second == "//" || token.second == "%")
    {
        token = nextToken();
        if (Factor())
        {
            if(TermPrime())
                return 1;
        }
        else
        {
            AgregarError("Error en la produccion TermPrime ",pos);
            return 1;
        }
    }
    //:, NEWLINE, =, if, else, or, and, not, ==, !=, <, >, <=, >=, is, +, -, ), ], ,
    if (token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::Factor()
{
    if (token.second == "-")
    {
        token = nextToken();
        if(Factor())
            return 1;
    }
    if (Name())
        return 1;
    if (Literal())
        return 1;
    if (List())
        return 1;
    if (token.second == "(")
    {
        token = nextToken();
        if (Expr())
        {
            if (token.second == ")")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error en ) (Produccion Factor)",pos);
                token = nextToken();
                return 1;
            }
        }
    }
    return 0;
}
bool Parser::Name()
{
    if (token.first == "ID")
    {
        token = nextToken();
        if(NameTail())
            return 1;
        //else
          //  AgregarError("Error solo se uso ID",pos);
    }
    return 0;
}
bool Parser::NameTail()
{
    if (token.second == "(")
    {
        token = nextToken();
        if (ExprList())
        {
            if (token.second == ")")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error en ) (Produccion NameTail)",pos);
                token = nextToken();
                return 1;
            }
        }
    }
    if (List())
        return 1;
    //{*,//,%,+,-,not,and,else,if,:,NEWLINE,=,or,==,!=,<,>,<=,>=,is,),],,}
    if (token.second == "*" || token.second == "//" || token.second == "%" || token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    return 0;
}
bool Parser::Literal()
{
    if (token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING")
    {
        token = nextToken();
        return 1;
    }
    return 0;
}
bool Parser::List()
{
    if (token.second == "[")
    {
        token = nextToken();
        if (ExprList())
        {
            if (token.second == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error ] (Produccion Lista)",pos);
                token = nextToken();
                return 1;
            }
        }
 //       else
   //         return 1;
    }
    return 0;
}
bool Parser::ExprList()
{
    if (Expr())
    {
        if(ExprListTail())
            return 1;
    }
    if (token.second == ")" || token.second == "]")
        return 1;
    AgregarError("Error en contenido de la lista (Produccion ExprList) ",pos);
    return 0;
}
bool Parser::ExprListTail()
{
    if (token.second == ",")
    {
        token = nextToken();
        if (Expr())
        {
            if(ExprListTail())
                return 1;
        }
        else
        {
            AgregarError("Error de , (Produccion ExprListTail)",pos);
            return 1;
        }
    }
    if (token.second == ")" || token.second == "]")
        return 1;
    return 0;
}
bool Parser::CompOp()
{
    if(token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is")
    {
        token = nextToken();
        return 1;
    }
    return 0;
}
int main(int argc, const char* argv[])
{
    Parser parser;
    bool  j = parser.Init();
    if (j)
    {
        cout<<"Parser completado sin errores"<<endl;
    }
    else
    {
        cout<<"Parse completado con errores"<<endl;
        parser.ImprimirErrores();
    }
    return 0;
}
