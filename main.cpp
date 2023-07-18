
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
    Imprimir2();//->Usarlo solo para debugear
    cout<<endl;
    cout <<"-------------------Parser------------------\n";
    if(Program())
    {
        if(deteccionError == false)
            return 1;
    }
    cout<<"Ultima posicion del recorrido del parser: "<<pos<<endl; //Usarlo para debugear
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
    while(token.first != "NEWLINE")
    {
        token = nextToken();
        if(token.first == "NEWLINE")
        {
            
            /*if(pos+1 == tok.size()-1) //Saber si estoy viendo el ultimo NEWLINE, aun no hago test de esto
            {
                pos++;
                token = tok[pos];
                break;
            }*/
            pos = pos - 1;
            token = tok[pos]; //Hacer un retroceso.
            break;
        }
        if(token.first == "$")
            break;
    }
}
pair<string, string> Parser::nextToken()
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
/*
 Puntos a considerar
 -Solo al final de cada produccion si encuentro un error irme hasta el newline. otherwise sigo normal
 -Ver los errores en las porducciones vacias excepto las q ya usa el def esas andan bien y verificar toda la linea de recursividad del Expr
 */
bool Parser::DefList() //Funcion con vacio
{
    if(token.second == "def") //First de Def
    {
        Def();
        if(DefList())
            return 1;
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[")
        return 1;
    //Error
    AgregarError("Error en la produccion DefList",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
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
                                if(token.first == "NEWLINE")
                                {
                                    Block();
                                    return 1;
                                }
                                else
                                {
                                    AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                                    SaltarError();
                                    if(token.first != "NEWLINE")
                                        token = nextToken();
                                    Block();
                                    return 1;
                                }
                            }
                            else
                            {
                                AgregarError("Error de : (Produccion Def) ", pos);
                                SaltarError();
                                if(token.first != "NEWLINE")
                                    token = nextToken();
                                Block();
                                return 1;
                            }
                        }
                        else
                        {
                            SaltarError();
                            if(token.first != "NEWLINE")
                                token = nextToken();
                            Block();
                            return 1;
                        }
                    }
                    else
                    {
                        AgregarError("Error de ) (Produccion Def) ", pos);
                        SaltarError();
                        if(token.first != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    SaltarError();
                    if(token.first != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de ( (Produccion Def) ", pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }

        }
        else
        {
            AgregarError("Error de ID (Produccion Def) ", pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            Block();
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
            if(token.second == "int" || token.second == "str" || token.second == "[") //Firsts de Type
            {
                Type();
                return 1;
            }
            else
            {
                AgregarError("Error en la produccion TypedVar",pos);
            }
        }
        else
        {
            AgregarError("Error de : (Produccion TypedVar) ",pos);
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
        if (token.second == "int" || token.second == "str" || token.second == "[")
        {
            Type();
            if (token.second == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error de ] (Produccion Type) ",pos);
                //return 1;
            }
        }
        else
        {
            AgregarError("Error en el contenido de los [ ] (Produccion Type)",pos);
        }
    }
    return 0;
}
bool Parser::TypedVarList() //Funcion con vacio
{
    if (token.first == "ID") //First de TypedVar
    {
        TypedVar();
        if(TypedVarListTail())
            return 1;
        else
            return 0;
    }
    if (token.second == ")")
        return 1;
    AgregarError("Error en la produccion TypedVarList", pos);
    //return 0; -> Por si solo quiere que se muestre el error de TypedVarList
    return 1; //-> Por si quiere que se muestre el error de cierre de parentesis
}
bool Parser::TypedVarListTail() //Funcion con vacio
{
    if (token.second == ",")
    {
        token = nextToken();
        if (token.first == "ID") //First TypedVar
        {
            TypedVar();
            if(TypedVarListTail())
                return 1;
        }
        else
        {
            AgregarError("Error, no le sigue un ID (Produccion TypedVarListTail)",pos);
            return 0;
        }
    }
    if (token.second == ")")
        return 1;
    AgregarError("Error, token inesperado (Produccion TypedVarListTail)",pos);
    return 0;
}
bool Parser::Return() //Funcion con vacio
{
    if (token.second == "->")
    {
        token = nextToken();
        if (token.second == "int" || token.second == "str" || token.second == "[") //First de type
        {
            Type();
            return 1;
        }
    }
    //Follow
    if (token.second == ":")
        return 1;
    AgregarError("Error en la produccion Return (def)",pos);
    return 0;
}
bool Parser::Block()
{
    if (token.first == "NEWLINE")
    {
        token = nextToken();
        if (token.first == "INDENT")
        {
            token = nextToken();
            if (token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Statement
            {
                Statement();
                if(StatementList())
                {
                    if (token.first == "DEDENT")
                    {
                        token = nextToken();
                        return 1;
                    }
                    else
                    {
                        if(token.first != "$")
                            AgregarError("Error de DEDENT",pos);
                        SaltarError(); //Not sure about this. If it doesnt work i will change it
                        token = nextToken();
                        token = nextToken();
                        if(token.first == "INDENT" || token.first == "DEDENT")
                            token = nextToken();
                        StatementList();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error de StatementList",pos);
                    SaltarError();
                    token = nextToken(); //Not sure about this. If it doesnt work i will change it
                    token = nextToken();
                    if(token.first == "INDENT" || token.first == "DEDENT")
                        token = nextToken();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de Statement",pos);
                SaltarError();
                token = nextToken();
                token = nextToken();
                if(token.first == "INDENT" || token.first == "DEDENT")
                    token = nextToken();
                StatementList();
                return 1;
            }
        }
        else
        {
            if(token.first != "$")
                AgregarError("Error de INDENT",pos);
            SaltarError(); //Not sure about this. If it doesnt work i will change it
            token = nextToken();
            token = nextToken();
            if(token.first == "INDENT" || token.first == "DEDENT")
                token = nextToken();
            StatementList();
        }
    }
    return 0;
}
bool Parser::StatementList() //Funcion con vacio
{
    if (token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Statement
    {
        Statement();
        if(StatementList())
        {
            return 1;
        }
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
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr)
        {
            Expr();
            if (token.second == ":")
            {
                token = nextToken();
                if (token.first == "NEWLINE")
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
                    AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                    SaltarError();
                    if(token.first != "NEWLINE")
                        token = nextToken();
                    Block();
                    ElifList();
                    Else();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de : (Produccion Statement)-if ",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                Block();
                ElifList();
                Else();
                return 1;
            }
        }
        else
        {
            AgregarError("Error Expr (Produccion Statement)-if",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            Block();
            ElifList();
            Else();
            return 1;
        }
    }
    if(token.second == "while")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr
        {
            Expr();
            if(token.second == ":")
            {
                token = nextToken();
                if(token.first == "NEWLINE")
                {
                    Block();
                    return 1;
                }
                else
                {
                    AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                    SaltarError();
                    if(token.first != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error de : (Produccion Statement)-while",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }
        }
        else
        {
            AgregarError("Error Expr (Produccion Statement)-while",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
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
                if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr
                {
                    Expr();
                    if (token.second == ":")
                    {
                        token = nextToken();
                        if(token.first == "NEWLINE")
                        {
                            Block();
                            return 1;
                        }
                        else
                        {
                            AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                            SaltarError();
                            if(token.first != "NEWLINE")
                                token = nextToken();
                            Block();
                            return 1;
                        }
                    }
                    else
                    {
                        AgregarError("Error, se esperaba un : (Produccion Statement)-for ",pos);
                        SaltarError();
                        if(token.first != "NEWLINE")
                            token = nextToken();
                        Block();
                        return 1;
                    }
                }
                else
                {
                    AgregarError("Error Expr (Produccion Statemet)-for",pos);
                    SaltarError();
                    if (token.first != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
            else
            {
                AgregarError("Error, se esperaba un in (Produccion Statement)-for",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }
        }
        else
        {
            AgregarError("Error, se esperaba un ID (Produccion Statement)-for",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    if (token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "("  || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first ==  "STRING" || token.second == "[")
    {
        SimpleStatement();
        if (token.first == "NEWLINE")
        {
            token = nextToken();
            return 1;
        }
        else
        {
            AgregarError("Token inesperado se esperaba un NEWLINE (Produccion Statement)-SimpleStatement",pos);
            SaltarError();
            if (token.first != "NEWLINE")
                token = nextToken();
            StatementList();
            return 1;
        }
    }
    return 0;
}
bool Parser::ElifList() //Funcion con vacio
{
    if (token.second == "elif")
    {
        Elif();
        if(ElifList())
            return 1;
        else
            return 0;
    }
    //Follow
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "["|| token.second == "DEDENT" || token.second == "else" || token.second == "elif")
        return 1;
    //Colocar error
    AgregarError("Error en la produccion ElifList",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Elif()
{
    if (token.second == "elif")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr
            
        {
            Expr();
            if (token.second == ":")
            {
                token = nextToken();
                if(token.first == "NEWLINE")
                {
                    Block();
                    return 1;
                }
                else
                {
                    AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                    SaltarError();
                    if(token.first != "NEWLINE")
                        token = nextToken();
                    Block();
                    return 1;
                }
            }
           else
           {
               AgregarError("Error de : (Produccion Elif) ",pos);
               SaltarError();
               if(token.first != "NEWLINE")
                   token = nextToken();
               Block();
               return 1;
           }
        }
        else
        {
            AgregarError("Error Expr (Produccion Elif)",pos);
            SaltarError();
            if (token.first != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    return 0;
}
bool Parser::Else() //Funcion con vacios
{
    if (token.second == "else")
    {
        token = nextToken();
        if (token.second == ":")
        {
            token = nextToken();
            if(token.first == "NEWLINE")
            {
                Block();
                return 1;
            }
            else
            {
                AgregarError("Error token inesperado se esperaba un NEWLINE",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                Block();
                return 1;
            }

        }
        else
        {
            AgregarError("Error de : (Produccion Else)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            Block();
            return 1;
        }
    }
    //Follow
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[" ||token.first == "DEDENT" || token.second == "def" || token.second == "elif" || token.second == "else")
        return 1;
    //Colocar error
    AgregarError("Error en la produccion else",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::SSTail() //Funcion con vacio
{
    if (token.second == "=")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr)
        {
            Expr();
            return 1;
        }
        else
        {
            AgregarError("Error, asignacion no terminada (Produccion SSTail)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.first == "NEWLINE")
        return 1;
    //Colocar error
    AgregarError("No se termino la Produccion SSTail",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::SimpleStatement()
{
    if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr
    {
        Expr();
        if(SSTail())
            return 1;
        else
            return 0;
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
bool Parser::ReturnExpr() //Funcion con vacio
{
    if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Expr
    {
        Expr();
        return 1;
    }
    //Follows
    if (token.first == "NEWLINE")
        return 1;
    //Error
    AgregarError("Error en el return (Produccion ReturnExpr)",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Expr()
{
    if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de orExpr
    {
        orExpr();
        if(ExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::ExprPrime() //Funcion con vacio
{
    if (token.second == "if")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de andExpr
        {
            andExpr();
            if (token.second == "else")
            {
                token = nextToken();
                if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de andExpr)
                {
                    andExpr();
                    if(ExprPrime())
                        return 1;
                    else
                        return 0;
                }
                else
                {
                    AgregarError("Error no se termino el else (Produccion Exprime)",pos);
                    SaltarError();
                    if (token.first != "NEWLINE")
                        token = nextToken();
                    return 0;
                }
            }
            else
            {
                AgregarError("Error de else (Produccion ExprPrime)",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
        else
        {
            AgregarError("Error AndExpr en la produccion ExprPrime",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //{:,NEWLINE,=,),],,}
    if (token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion ExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::orExpr()
{
    if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de andExpr
    {
        andExpr();
        if(orExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::orExprPrime() //Funcion con vacio
{
    if (token.second == "or")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de andExpr
        {
            andExpr();
            if(orExprPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error AndExpr, en la produccion OrExprPrime",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion orExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::andExpr()
{
    if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de notExpr
    {
        notExpr();
        if(andExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::andExprPrime() //Funcion con vacio
{
    if (token.second == "and")
    {
        token = nextToken();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de notExpr
        {
            notExpr();
            if(andExprPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error NotExpr, en la produccion AndExprPrime",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;

        }
    }
    //Follow
    if (token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion andExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::notExpr()
{
    if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de CompExpr
    {
        CompExpr();
        if(notExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::notExprPrime() //Funcion con vacio
{
    if (token.second == "not")
    {
        token = nextToken();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de CompExpr
        {
            CompExpr();
            if(notExprPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error CompExpr, en la produccion notExprPrime",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion notExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::CompExpr()
{
    if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de intExpr
    {
        intExpr();
        if(CompExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::CompExprPrime() //Funcion con vacio
{
    if (token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is") //First de CompOp
    {
        CompOp();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de intExpr
        {
            intExpr();
            if(CompExprPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error de comparacion (Produccion CompExprPrime)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion CompExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::intExpr()
{
    if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Term
    {
        Term();
        if(intExprPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::intExprPrime() //Funcion con vacio
{
    if (token.second == "+" || token.second == "-")
    {
        token = nextToken();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Term
        {
            Term();
            if(intExprPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error Term, no se pudo hacer la operacion (+/-) (Produccion intExprPrime)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion intExprPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Term()
{
    if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Factor
    {
        Factor();
        if(TermPrime())
            return 1;
        else
            return 0;
    }
    return 0;
}
bool Parser::TermPrime() //Funcion con vacio
{
    if (token.second == "*" || token.second == "//" || token.second == "%")
    {
        token = nextToken();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Factor
        {
            Factor();
            if(TermPrime())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error Factor, no se pudo hacer la operacion (* _ % _ //) (Produccion TermPrime)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    AgregarError("Error en la produccion TermPrime",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Factor()
{
    if (token.second == "-")
    {
        token = nextToken();
        if(token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First de Factor
        {
            Factor();
            return 1;
        }
    }
    if (token.first == "ID")//First de Name
    {
        Name();
        return 1;
    }
    if (token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING") //First de Literal
    {
        Literal();
        return 1;
    }
    if (token.second == "[") //First List
    {
        List();
        return 1;
    }
    if (token.second == "(")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[")//First Expr
        {
            Expr();
            if (token.second == ")")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error, se esperaba un ) (Produccion Factor)",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
        else
        {
            AgregarError("Error Expr, token inesperado (Produccion Factor)",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
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
        else
            return 0;
    }
    return 0;
}
bool Parser::NameTail() //Funcion con vacio
{
    if (token.second == "(")
    {
        token = nextToken();
        if(ExprList())
        {
            if (token.second == ")")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error, se esperaba un ) (Produccion NameTail)",pos);
                SaltarError();
                if(token.first != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
    }
    if (token.second == "[")
    {
        List();
        return 1;
    }
    //Follow
    if (token.second == "*" || token.second == "//" || token.second == "%" || token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
        return 1;
    //Error
    AgregarError("Error en la produccion NameTail",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::Literal()
{
    //First
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
        if(ExprList())
        {
            if (token.second == "]")
            {
                token = nextToken();
                return 1;
            }
            else
            {
                AgregarError("Error ] (Produccion Lista)",pos);
                SaltarError(); //Not sure about this.
                if(token.first != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
    }
    return 0;
}
bool Parser::ExprList() //Funcion con vacio
{
    if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First Expr
    {
        Expr();
        if(ExprListTail())
            return 1;
        else
            return 0;
    }
    //Follows
    if (token.second == ")" || token.second == "]")
        return 1;
    //Colocar Error
    AgregarError("Error en contenido de la lista (Produccion ExprList) ",pos);
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::ExprListTail() //Funcion con vacio
{
    if (token.second == ",")
    {
        token = nextToken();
        if (token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second =="True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[") //First Expr
        {
            Expr();
            if(ExprListTail())
                return 1;
            else
                return 0;
        }
        else
        {
            AgregarError("Error no se termino la produccion ExprListTail",pos);
            SaltarError();
            if(token.first != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follows
    if (token.second == ")" || token.second == "]")
        return 1;
    //Colocar error
    AgregarError("Error, token inesperado (Produccion ExprListTail)",pos);
    SaltarError();
    if(token.first != "NEWLINE")
        token = nextToken();
    return 0;
}
bool Parser::CompOp()
{
    //First's
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
        cout<<"Parser completado sin errores - Gramatica Aceptada"<<endl;
    }
    else
    {
        cout<<"Parser completado con errores - Gramatica No Aceptada"<<endl;
    }
    parser.ImprimirErrores();
    return 0;
}
