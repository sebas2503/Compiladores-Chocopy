
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
vector<Token> tok;
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
    bool deteccionError;
    void AgregarError(string,int);
    void SaltarError();
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
    //Imprimir2();//->Usarlo solo para debugear
    cout<<endl;
    cout <<"-------------------Parser------------------\n";
    Program();
    if(deteccionError == false)
        return 1;
    //cout<<"Ultima posicion del recorrido del parser: "<<pos<<endl; //Usarlo para debugear
    return 0;
}
void Parser::AgregarError(string error, int pos2)
{
    class Error aux(error,pos2);
    deteccionError = true;
    listaerrores.push_back(aux);
    
}
void Parser::SaltarError()
{
    while(token.tipo != "NEWLINE")
    {
        token = nextToken();
        if(token.tipo == "NEWLINE")
        {
            pos = pos - 1;
            token = tok[pos]; //Hacer un retroceso.
            break;
        }
        if(token.tipo == "$")
            break;
    }
}
Token Parser::nextToken()
{
    if(pos+1 < tok.size())
    {
        pos+=1;
    }
    return tok[pos];
}
void Parser::Imprimir2()
{
    for (int i = 0; i < tok.size(); i++)
    {
        cout<<i<<"->";
        cout << "(" << tok[i].valor << "," << tok[i].tipo << ") ";
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
        cout << listaerrores[i].msj << "  linea "<<listaerrores[i].posicion<< endl;
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
bool Parser::DefList() //Funcion con vacio
{
    if(token.valor == "def") //First de Def
    {
        Def();
        if(DefList())
            return 1;
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[}
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[")
        return 1;
    //Error
    AgregarError("Error en la produccion DefList",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Def()
{
    if (token.valor == "def")
    {
        token = nextToken();
        if (token.tipo == "ID")
        {
            token = nextToken();
            if (token.valor == "(")
            {
                token = nextToken();
                if (TypedVarList())
                {
                    if (token.valor == ")")
                    {
                        token = nextToken();
                        if (Return())
                        {
                            if (token.valor == ":")
                            {
                                token = nextToken();
                                if(token.tipo == "NEWLINE")
                                {
                                    Block();
                                    return 1;
                                }
                                else
                                {
                                    AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                                    SaltarError();
                                    if(token.tipo != "NEWLINE")
                                        token = nextToken();
                                    Block();
                                    return 1;
                                }
                            }
                            else
                            {
                                AgregarError("Error de : (Produccion Def) ", token.fila);
                                SaltarError();
                                if(token.tipo != "NEWLINE")
                                    token = nextToken();
                                Block();
                                return 1;
                            }
                        }
                        else
                        {
                            SaltarError();
                            if(token.tipo != "NEWLINE")
                                token = nextToken();
                            Block();
                            return 1;
                        }
                    }
                    else
                    {
                        AgregarError("Error de ) (Produccion Def) ", token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de ( (Produccion Def) ", token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }

        }
        else
        {
            AgregarError("Error de ID (Produccion Def) ", token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    return 0;
}
bool Parser::TypedVar()
{
    if (token.tipo == "ID")
    {
        token = nextToken();
        if (token.valor == ":")
        {
            token = nextToken();
            if(token.valor == "int" || token.valor == "str" || token.valor == "[") //Firsts de Type
            {
                Type();
                return 1;
            }
            else
            {
                AgregarError("Error en la produccion TypedVar",token.fila);
            }
        }
        else
        {
            AgregarError("Error de : (Produccion TypedVar) ",token.fila);
        }
    }
    return 0;
}
bool Parser::Type()
{
    if (token.valor == "int")
    {
        token = nextToken();
        return 1;
    }
    if (token.valor == "str")
    {
        token = nextToken();
        return 1;
    }
    if (token.valor == "[")
    {
        token = nextToken();
        if (token.valor == "int" || token.valor == "str" || token.valor == "[")
        {
            Type();
            if (token.valor == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error de ] (Produccion Type) ",token.fila);
            }
        }
        else
        {
            AgregarError("Error en el contenido de los [ ] (Produccion Type)",token.fila);
        }
    }
    return 0;
}
bool Parser::TypedVarList() //Funcion con vacio
{
    if (token.tipo == "ID") //First de TypedVar
    {
        TypedVar();
        if(TypedVarListTail())
            return 1;
        else
            return 0;
    }
    if (token.valor == ")")
        return 1;
    AgregarError("Error en la produccion TypedVarList", token.fila);
    //return 0; -> Por si solo quiere que se muestre el error de TypedVarList
    return 1; //-> Por si quiere que se muestre el error de cierre de parentesis
}
bool Parser::TypedVarListTail() //Funcion con vacio
{
    if (token.valor == ",")
    {
        token = nextToken();
        if (token.tipo == "ID") //First TypedVar
        {
            TypedVar();
            if(TypedVarListTail())
                return 1;
        }
        else
        {
            AgregarError("Error, no le sigue un ID (Produccion TypedVarListTail)",token.fila);
            return 0;
        }
    }
    if (token.valor == ")")
        return 1;
    AgregarError("Error, token inesperado (Produccion TypedVarListTail)",token.fila);
    return 0;
}
bool Parser::Return() //Funcion con vacio
{
    if (token.valor == "->")
    {
        token = nextToken();
        if (token.valor == "int" || token.valor == "str" || token.valor == "[") //First de type
        {
            Type();
            return 1;
        }
    }
    //Follow
    if (token.valor == ":")
        return 1;
    AgregarError("Error en la produccion Return (def)",token.fila);
    return 0;
}
bool Parser::Block()
{
    if (token.tipo == "NEWLINE")
    {
        token = nextToken();
        if (token.tipo == "INDENT")
        {
            token = nextToken();
            if (token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Statement
            {
                Statement();
                if(StatementList())
                {
                    if (token.tipo == "DEDENT")
                    {
                        token = nextToken();
                        return 1;
                    }
                    else
                    {
                        if(token.tipo != "$")
                            AgregarError("Error de DEDENT",token.fila);
                        SaltarError(); //Not sure about this. If it doesnt work i will change it
                        token = nextToken();
                        token = nextToken();
                        if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                            token = nextToken();
                        StatementList();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error de StatementList",token.fila);
                    SaltarError();
                    token = nextToken(); //Not sure about this. If it doesnt work i will change it
                    token = nextToken();
                    if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                        token = nextToken();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de Statement",token.fila);
                SaltarError();
                token = nextToken();
                token = nextToken();
                if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                    token = nextToken();
                StatementList();
                return 1;
            }
        }
        else
        {
            if(token.tipo != "$")
                AgregarError("Error de INDENT",token.fila);
            SaltarError(); //Not sure about this. If it doesnt work i will change it
            token = nextToken();
            token = nextToken();
            if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                token = nextToken();
            StatementList();
        }
    }
    return 0;
}
bool Parser::StatementList() //Funcion con vacio
{
    if (token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Statement
    {
        Statement();
        if(StatementList())
            return 1;
    }
    //{$,DEDENT}
    if (token.valor == "$" || token.tipo == "DEDENT")
        return 1;
    return 0;
}
bool Parser::Statement()
{
    if (token.valor == "if")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr)
        {
            if(Expr())
                if (token.valor == ":")
                {
                    token = nextToken();
                    if (token.tipo == "NEWLINE")
                    {
                        Block();
                        if (ElifList())
                        {
                            if(Else())
                                return 1;
                        }
                    }
                    else
                    {
                        AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        Block();
                        ElifList();
                        Else();
                        return 1;
                    }
                }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                ElifList();
                Else();
                return 1;
            }
            else
            {
                AgregarError("Error de : (Produccion Statement)-if ",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                ElifList();
                Else();
                return 1;
            }
        }
        else
        {
            AgregarError("Error Expr (Produccion Statement)-if",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            ElifList();
            Else();
            return 1;
        }
    }
    if(token.valor == "while")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
        {
            if(Expr())
                if(token.valor == ":")
                {
                    token = nextToken();
                    if(token.tipo == "NEWLINE")
                    {
                        Block();
                        return 1;
                    }
                    else
                    {
                        AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error de : (Produccion Statement)-while",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }
        }
        else
        {
            AgregarError("Error Expr (Produccion Statement)-while",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    if (token.valor == "for")
    {
        token = nextToken();
        if (token.tipo == "ID")
        {
            token = nextToken();
            if (token.valor == "in")
            {
                token = nextToken();
                if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
                {
                    if(Expr())
                        if (token.valor == ":")
                        {
                            token = nextToken();
                            if(token.tipo == "NEWLINE")
                            {
                                Block();
                                return 1;
                            }
                            else
                            {
                                AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                                SaltarError();
                                if(token.tipo != "NEWLINE")
                                    token = nextToken();
                                Block();
                                return 1;
                            }
                        }
                        else
                        {
                            AgregarError("Error, se esperaba un : (Produccion Statement)-for ",token.fila);
                            SaltarError();
                            if(token.tipo != "NEWLINE")
                                token = nextToken();
                            Block();
                            return 1;
                        }
                    else
                    {
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error Expr (Produccion Statemet)-for",token.fila);
                    SaltarError();
                    if (token.tipo != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error, se esperaba un in (Produccion Statement)-for",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }
        }
        else
        {
            AgregarError("Error, se esperaba un ID (Produccion Statement)-for",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    if (token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "("  || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo ==  "STRING" || token.valor == "[")
    {
        SimpleStatement();
        if (token.tipo == "NEWLINE")
        {
            token = nextToken();
            return 1;
        }
        else
        {
            AgregarError("Token inesperado se esperaba un NEWLINE (Produccion Statement)-SimpleStatement",token.fila);
            SaltarError();
            if (token.tipo != "NEWLINE")
                token = nextToken();
            StatementList();
            return 1;
        }
    }
    return 0;
}
bool Parser::ElifList() //Funcion con vacio
{
    if (token.valor == "elif")
    {
        Elif();
        if(ElifList())
            return 1;
        else
            return 0;
    }
    //Follow
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "["|| token.valor == "DEDENT" || token.valor == "else" || token.valor == "elif")
        return 1;
    //Colocar error
    AgregarError("Error en la produccion ElifList",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Elif()
{
    if (token.valor == "elif")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
            
        {
            if(Expr())
                if (token.valor == ":")
                {
                    token = nextToken();
                    if(token.tipo == "NEWLINE")
                    {
                        Block();
                        return 1;
                    }
                    else
                    {
                        AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error de : (Produccion Elif) ",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }
        }
        else
        {
            AgregarError("Error Expr (Produccion Elif)",token.fila);
            SaltarError();
            if (token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    return 0;
}
bool Parser::Else() //Funcion con vacios
{
    if (token.valor == "else")
    {
        token = nextToken();
        if (token.valor == ":")
        {
            token = nextToken();
            if(token.tipo == "NEWLINE")
            {
                Block();
                return 1;
            }
            else
            {
                AgregarError("Error token inesperado se esperaba un NEWLINE",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }

        }
        else
        {
            AgregarError("Error de : (Produccion Else)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    //Follow
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[" ||token.tipo == "DEDENT" || token.valor == "def" || token.valor == "elif" || token.valor == "else")
        return 1;
    //Colocar error
    AgregarError("Error en la produccion else",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::SSTail() //Funcion con vacio
{
    if (token.valor == "=")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr)
        {
            if(Expr())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error, asignacion no terminada (Produccion SSTail)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.tipo == "NEWLINE")
        return 1;
    //Colocar error
    AgregarError("No se termino la Produccion SSTail",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::SimpleStatement()
{
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
    {
        if(Expr())
            if(SSTail())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    if (token.valor == "pass")
    {
        token = nextToken();
        return 1;
    }
    if (token.valor == "return")
    {
        token = nextToken();
        if(ReturnExpr())
            return 1;
    }
    return 0;
}
bool Parser::ReturnExpr() //Funcion con vacio
{
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
    {
        if(Expr())
            return 1;
        else
            return 0;
    }
    //Follows
    if (token.tipo == "NEWLINE")
        return 1;
    //Error
    AgregarError("Error en el return (Produccion ReturnExpr)",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Expr()
{
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de orExpr
    {
        if(orExpr())
            if(ExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::ExprPrime() //Funcion con vacio
{
    if (token.valor == "if")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
        {
            if(andExpr())
                if (token.valor == "else")
                {
                    token = nextToken();
                    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr)
                    {
                        if(andExpr())
                            if(ExprPrime())
                                return 1;
                            else
                                return 0;
                        else
                            return 0;
                    }
                    else
                    {
                        AgregarError("Error no se termino el else (Produccion Exprime)",token.fila);
                        SaltarError();
                        if (token.tipo != "NEWLINE")
                            token = nextToken();
                        return 0;
                    }
                }
            else
                return 0;
            else
            {
                AgregarError("Error de else (Produccion ExprPrime)",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
        else
        {
            AgregarError("Error AndExpr en la produccion ExprPrime",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //{:,NEWLINE,=,),],,}
    if (token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion ExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::orExpr()
{
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
    {
        if(andExpr())
            if(orExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::orExprPrime() //Funcion con vacio
{
    if (token.valor == "or")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
        {
            if(andExpr())
                if(orExprPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error AndExpr, en la produccion OrExprPrime",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion orExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::andExpr()
{
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de notExpr
    {
        if(notExpr())
            if(andExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::andExprPrime() //Funcion con vacio
{
    if (token.valor == "and")
    {
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de notExpr
        {
            if(notExpr())
                if(andExprPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error NotExpr, en la produccion AndExprPrime",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;

        }
    }
    //Follow
    if (token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion andExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::notExpr()
{
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de CompExpr
    {
        if(CompExpr())
            if(notExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::notExprPrime() //Funcion con vacio
{
    if (token.valor == "not")
    {
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de CompExpr
        {
            if(CompExpr())
                if(notExprPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error CompExpr, en la produccion notExprPrime",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion notExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::CompExpr()
{
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de intExpr
    {
        if(intExpr())
            if(CompExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::CompExprPrime() //Funcion con vacio
{
    if (token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is") //First de CompOp
    {
        CompOp();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de intExpr
        {
            if(intExpr())
                if(CompExprPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error de comparacion (Produccion CompExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion CompExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::intExpr()
{
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Term
    {
        if(Term())
            if(intExprPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::intExprPrime() //Funcion con vacio
{
    if (token.valor == "+" || token.valor == "-")
    {
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Term
        {
            if(Term())
                if(intExprPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error Term, no se pudo hacer la operacion (+/-) (Produccion intExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion intExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Term()
{
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
    {
        if(Factor())
            if(TermPrime())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    return 0;
}
bool Parser::TermPrime() //Funcion con vacio
{
    if (token.valor == "*" || token.valor == "//" || token.valor == "%")
    {
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
        {
            if(Factor())
                if(TermPrime())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error Factor, no se pudo hacer la operacion (* _ % _ //) (Produccion TermPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "+" || token.valor == "-" || token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    AgregarError("Error en la produccion TermPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Factor()
{
    if (token.valor == "-")
    {
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
        {
            if(Factor())
                return 1;
            else
                return 0;
        }
    }
    if (token.tipo == "ID")//First de Name
    {
        if(Name())
            return 1;
        else
            return 0;
    }
    if (token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING") //First de Literal
    {
        Literal();
        return 1;
    }
    if (token.valor == "[") //First List
    {
        if(List())
            return 1;
        else
            return 0;
    }
    if (token.valor == "(")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[")//First Expr
        {
            if(Expr())
                if (token.valor == ")")
                {
                    token = nextToken();
                    return 1;
                }
                else
                {
                    AgregarError("Error, se esperaba un ) (Produccion Factor)",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    return 0;
                }
            else
                return 0;
        }
        else
        {
            AgregarError("Error Expr, token inesperado (Produccion Factor)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    return 0;
}
bool Parser::Name()
{
    if (token.tipo == "ID")
    {
        token = nextToken();
        if(NameTail())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::NameTail() //Funcion con vacio
{
    if (token.valor == "(")
    {
        token = nextToken();
        if(ExprList())
        {
            if (token.valor == ")")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error, se esperaba un ) (Produccion NameTail)",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
    }
    if (token.valor == "[")
    {
        List();
        return 1;
    }
    //Follow
    if (token.valor == "*" || token.valor == "//" || token.valor == "%" || token.valor == "+" || token.valor == "-" || token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion NameTail",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Literal()
{
    //First
    if (token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING")
    {
        token = nextToken();
        return 1;
    }
    return 0;
}
bool Parser::List()
{
    if (token.valor == "[")
    {
        token = nextToken();
        if(ExprList())
        {
            if (token.valor == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error ] (Produccion Lista)",token.fila);
                SaltarError(); //Not sure about this.
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
    }
    return 0;
}
bool Parser::ExprList() //Funcion con vacio
{
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First Expr
    {
        if(Expr())
            if(ExprListTail())
                return 1;
            else
                return 0;
        else
            return 0;
    }
    //Follows
    if (token.valor == ")" || token.valor == "]")
        return 1;
    //Colocar Error
    AgregarError("Error en contenido de la lista (Produccion ExprList) ",token.fila);
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::ExprListTail() //Funcion con vacio
{
    if (token.valor == ",")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First Expr
        {
            if(Expr())
                if(ExprListTail())
                    return 1;
                else
                    return 0;
            else
                return 0;
        }
        else
        {
            AgregarError("Error no se termino la produccion ExprListTail",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follows
    if (token.valor == ")" || token.valor == "]")
        return 1;
    //Colocar error
    AgregarError("Error, token inesperado (Produccion ExprListTail)",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::CompOp()
{
    //First's
    if(token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is")
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
        cout<<"Parser completado sin errores - Gramatica Aceptada"<<endl;
    else
        cout<<"Parser completado con errores - Gramatica No Aceptada"<<endl;
    parser.ImprimirErrores();
    return 0;
}

