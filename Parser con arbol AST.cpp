
//
//  main.cpp
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#include <iostream>
#include <vector>
#include <queue>
#include "Scanner.h"
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

class Parser
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
void Parser::Error(string error)
{
    //listaerrores.push_back("Unexpected error " + error + " " + token.first + " " + token.second);
    listaerrores.push_back("Unexpected error " + error);
    while (token.first != "NEWLINE")
    {
        token = nextToken();
        cout << "Probando: (" << token.first << "," << token.second << ") ";
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
    if (pos + 1 < tok.size())
    {
        pos += 1;
    }
    cout << "entre: " << pos << endl;
    return tok[pos];
    //else
      //  return tok[pos-1];
}
void Parser::Imprimir2()
{
    for (int i = 0; i < tok.size(); i++)
    {
        cout << i << "->";
        cout << "(" << tok[i].first << "," << tok[i].second << ") ";
    }
}
void Parser::ImprimirErrores()
{
    if (listaerrores.size())
    {
        for (int i = 0; i < listaerrores.size(); i++)
            cout << listaerrores[i] << endl;
    }
    else
        cout << "No hubo errores" << endl;
}

void Parser::imprimirAST()
{
    printAST(root);
}

void Parser::printAST2(AST* raiz)
{
    if (raiz == nullptr) {
        return;
    }

    queue<AST*> cola;
    cola.push(raiz);

    int nodosNivelActual = 1;  // Número de nodos en el nivel actual
    int nodosNivelSiguiente = 0;  // Número de nodos en el siguiente nivel
    int flag = 0;
    int bloque = -1;
    int boolbloque = 0;
    while (!cola.empty()) {
        AST* nodo = cola.front();
        cola.pop();

        if (nodo != nullptr) {

            
            astvector.push_back(nodo);
            //std::cout << nodo->valor << " ";//tipo: "<<nodo->tipo;
            // Agregar los hijos del nodo actual a la cola y actualizar el contador de nodos en el siguiente nivel
            for (AST* hijo : nodo->hijos) {
                cola.push(hijo);
                nodosNivelSiguiente++;
            }
        }

        // Restar el número de nodos procesados en el nivel actual
        nodosNivelActual--;

        // Si se ha procesado el último nodo del nivel actual, imprimir un salto de línea y actualizar el contador de nodos en el nivel actual
        if (nodosNivelActual == 0) {
            std::cout << std::endl;
            nodosNivelActual = nodosNivelSiguiente;
            nodosNivelSiguiente = 0;
        }
    }
}


void Parser::printAST(AST* raiz)
{
    if (raiz == nullptr) {
        return;
    }

    queue<AST*> cola;
    cola.push(raiz);

    int nodosNivelActual = 1;  // Número de nodos en el nivel actual
    int nodosNivelSiguiente = 0;  // Número de nodos en el siguiente nivel

    while (!cola.empty()) {
        AST* nodo = cola.front();
        cola.pop();

        if (nodo != nullptr) {

            std::cout << nodo->valor << " ";

            // Agregar los hijos del nodo actual a la cola y actualizar el contador de nodos en el siguiente nivel
            for (AST* hijo : nodo->hijos) {
                cola.push(hijo);
                nodosNivelSiguiente++;
            }
        }

        // Restar el número de nodos procesados en el nivel actual
        nodosNivelActual--;

        // Si se ha procesado el último nodo del nivel actual, imprimir un salto de línea y actualizar el contador de nodos en el nivel actual
        if (nodosNivelActual == 0) {
            std::cout << std::endl;
            nodosNivelActual = nodosNivelSiguiente;
            nodosNivelSiguiente = 0;
        }
    }
}


void Parser::imprimirAST2()
{
    printAST(root);
}
void Parser::AST_Tree()
{
    ast = generateAST(root);
    printAST2(ast);
}

void Parser::imprimirAST_Tree()
{
    int id = 0;
    int id2 = -1;
    bool salto = false;
    for (int i = 0; i < astvector.size(); i++) // recorre todos los nodos AST
    {
            
        if ((astvector[i]->tipo == "ID" && astvector[i]->valor.size()>3) || astvector[i]->valor == "if" || astvector[i]->valor == "else" || astvector[i]->valor == "elif" || astvector[i]->valor == "return" || astvector[i]->valor == "for" || astvector[i]->valor == "while" || astvector[i]->valor == "in" || astvector[i]->valor == "True" || astvector[i]->valor == "False")
        {
            id++; // tabulaciones
            cout << "\n";
            for (int j = 0; j < id; j++)
            {
                cout << "   ";
            }
            cout<<"| --" << astvector[i]->valor << "\n ";
        }
        else
        {
            for (int j = 0; j < id; j++)
            {
                cout << "   ";
            }
            cout << "\t|---   " << astvector[i]->valor << "\n ";
        }
    }
}

AST* Parser::generateAST(AST* rootNode) {
    if (!rootNode)
        return nullptr;

    AST* astRoot = nullptr;
    AST* currentLevelNode = nullptr;
    
    if (rootNode->isleaf) {
        // Crear un nuevo nodo en el árbol AST solo con las hojas
        astRoot = new AST(rootNode->valor,rootNode->isleaf,rootNode->tipo);
        currentLevelNode = astRoot;
       
    }
    else {
        for (AST* child : rootNode->hijos) {
            AST* childAstTree = generateAST(child);  // Generar el árbol AST para cada hijo
            if (childAstTree) {
                if (!astRoot) {
                    // Si es el primer hijo con hojas, se convierte en el nodo raíz del árbol AST
                    astRoot = childAstTree;
                    currentLevelNode = astRoot;

                }
                else {
                    // Agregar el árbol AST del hijo al mismo nivel en el árbol AST
                    currentLevelNode->agregar(childAstTree);
                }
            }
        }
    }

    return astRoot;
}




/// <summary>
/// /
/// </summary>
/// <returns></returns>
AST* Parser::Program()
{
    root = new AST("Program");
    astvector.push_back(root);
    if (AST* h1 = DefList())
    {
        //cout << "DEFLIST";
        root->agregar(h1);
        //printAST(h1);
        if (AST* h2 = StatementList())
        {
            root->agregar(h2); //cout << "EXITO";
            return root;
        }
    }
    //if (token.second != "$")
        //Error("$");
    return 0;

}
AST* Parser::DefList()
{
    AST* h1 = new AST("DefList");
    if (AST* h2 = Def())
    {
        root->agregar(h2);
        if (AST* h3 = DefList())
        {

            root->agregar(h3);
            printAST(h1);
            return h1;
        }
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[")
    {
        //root->agregar(new AST(token.second));
        //printAST(h1);
        return h1;
    }
    return 0;
}
AST* Parser::Def()
{
    AST* h1 = new AST("Def");
    if (token.second == "def")
    {
        root->agregar(new AST("def"));
        token = nextToken(); //cout << "HAGAHA";
        if (token.first == "ID")
        {
            AST* idi = new AST("ID");
            idi->agregar(new AST(token.second, true, "ID"));
            root->agregar(idi);
            token = nextToken();
            if (token.second == "(")
            {
                root->agregar(new AST("(", false, "("));
                token = nextToken();
                if (AST* h2 = TypedVarList())
                {

                    root->agregar(h2);
                    //printAST(h1);
                    if (token.second == ")")
                    {
                        root->agregar(new AST(")", false, ")"));
                        token = nextToken();
                        if (AST* h3 = Return())
                        {
                            //cout << "RETURN: ";

                            root->agregar(h3);
                            //printAST(h1);
                            if (token.second == ":")
                            {
                                cout << " DOS PUNTOS: ";
                                root->agregar(new AST(":"));
                                printAST(h1);
                                token = nextToken();
                                if (AST* h4 = Block()) //retorna 0
                                {
                                    cout << "BLOCK: ";
                                    root->agregar(h4);
                                    printAST(h1);
                                    return h1;
                                }

                            }
                            //Error(":");
                        }
                    }
                    //Error(")");
                }
            }
            //Error("(");
        }
        //Error("ID");
    }
    return 0;
}
AST* Parser::TypedVar()
{
    AST* h1 = new AST("TypedVar");
    if (token.first == "ID")
    {
        AST* idi = new AST("ID");
        idi->agregar(new AST(token.second, true, "ID"));
        root->agregar(idi);
        token = nextToken();
        if (token.second == ":")
        {
            root->agregar(new AST(":"));
            token = nextToken();
            if (AST* h2 = Type())
            {
                root->agregar(h2);
                return h1;
            }
        }
        //Error(":");
    }
    //Error("ID");
    return 0;
}
AST* Parser::Type()
{
    AST* h1 = new AST("Type");
    if (token.second == "int" || token.second == "str")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        return h1;
    }
    if (token.second == "[")
    {
        root->agregar(new AST(token.second));
        token = nextToken();
        if (AST* h2 = Type())
        {
            root->agregar(h2);
            if (token.second == "]")
            {
                root->agregar(new AST(token.second));
                token = nextToken();
                return h1;
            }
            //Error("]");
        }
    }
    //if (token.second != "int" || token.second != "str" || token.second != "[")
        //Error(token.second);
    return 0;
}
AST* Parser::TypedVarList()
{
    AST* h1 = new AST("TypedVarList");
    if (AST* h2 = TypedVar())
    {
        printAST(h2);
        root->agregar(h2);
        if (AST* h3 = TypedVarListTail())
        {
            root->agregar(h3);
            return h1;
        }
    }
    if (token.second == ")")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::TypedVarListTail()
{
    AST* h1 = new AST("TypeVarListTail");
    if (token.second == ",")
    {
        root->agregar(new AST(token.second));
        token = nextToken();
        if (AST* h2 = TypedVar())
        {
            root->agregar(h2);
            if (AST* h3 = TypedVarListTail())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //Error(",");->Creo q esta bien
    if (token.second == ")")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Return()
{
    AST* h1 = new AST("Return");
    if (token.second == "->")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Type())
        {
            root->agregar(h2);
            return h1;
        }
    }
    if (token.second == ":")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Block()
{
    AST* h1 = new AST("Block");
    if (token.first == "NEWLINE")
    {
        root->agregar(new AST(token.first));
        token = nextToken();
        if (token.first == "INDENT")
        {
            root->agregar(new AST(token.first));
            token = nextToken();
            if (AST* h2 = Statement())
            {
                root->agregar(h2);
                if (AST* h3 = StatementList())
                {
                    root->agregar(h3);
                    if (token.first == "DEDENT")
                    {
                        root->agregar(new AST(token.first));
                        token = nextToken();
                        return h1;
                    }
                    //else
                      //  Error("DEDENT");
                }
                //else
                  //  Error("StatementList");
            }
            //  else
              //    Error("INDENT");
        }
    }
    return 0;
}
AST* Parser::StatementList()
{
    AST* h1 = new AST("StatementList");
    if (AST* h2 = Statement())
    {
        
        if (AST* h3 = StatementList())
        {
            root->agregar(h3); //printAST(h3);

            return h1;
        }
    }

    //cout << "NO ENTRA\n";
    //{$,DEDENT}
    if (token.second == "$" || token.first == "DEDENT") // ENCUENTRA AL $ Y REEMPLAZA EL ARBOL CON $
    {
        //root->agregar(new AST(token.first));
        return h1;
    }
    return 0;
}
AST* Parser::Statement()
{
    AST* h1 = new AST("Statement");
    if (token.second == "if")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Expr())
        {
            root->agregar(h2);
            if (token.second == ":")
            {
                root->agregar(new AST(token.second));
                token = nextToken();
                if (AST* h3 = Block())
                {
                    root->agregar(h3);
                    if (AST* h4 = ElifList())
                    {
                        root->agregar(h4);
                        if (AST* h5 = Else())
                        {
                            //cout << "RECIBIDO ELSE--------------------";
                            //printAST(h5);
                            root->agregar(h5);
                            /*cout << "\nH1 ELSE: \n";
                            printAST(h1);
                            cout << "\nFIN H1 ELSE: \n";*/
                            return h1;
                        }
                    }
                }
            }
            // Error(":");
        }
    }
    if (token.second == "while")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h6 = Expr())
        {
            root->agregar(h6);
            if (token.second == ":")
            {
                root->agregar(new AST(token.second));
                token = nextToken();
                if (AST* h7 = Block())
                {
                    root->agregar(h7);
                    return h1;
                }
            }
            //Error(":");
        }
    }
    if (token.second == "for")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (token.first == "ID")
        {
            AST* idi = new AST("ID");
            idi->agregar(new AST(token.second, true, "ID"));
            root->agregar(idi);
            token = nextToken();
            if (token.second == "in")
            {
                root->agregar(new AST(token.second, true));
                token = nextToken();
                if (AST* h8 = Expr())
                {
                    root->agregar(h8);
                    if (token.second == ":")
                    {
                        root->agregar(new AST(token.second));
                        token = nextToken();
                        if (AST* h9 = Block())
                        {
                            root->agregar(h9);

                            return h1;
                        }
                    }
                    //else
                      //  Error(":");
                }
            }
            //else
              //  Error("in");
        }
        //else
          //  Error("ID");
    }
    if (AST* h10 = SimpleStatement())
    {
        root->agregar(h10);
        if (token.first == "NEWLINE")
        {
            root->agregar(new AST(token.first));
            token = nextToken();
            return h1;
        }
    }
    return nullptr;
}
AST* Parser::ElifList()
{
    AST* h1 = new AST("ElifList");
    if (AST* h2 = Elif())
    {
        root->agregar(h2);
        if (AST* h3 = ElifList())
        {
            root->agregar(h3);
            return h1;
        }
    }
    //DEDENT, if, while, for, pass, return, -, (, ID, None, True, False, INTEGER, STRING, [, else, $
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[,def,DEDENT,else,elif}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[" || token.second == "DEDENT" || token.second == "else" || token.second == "elif")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Elif()
{
    AST* h1 = new AST("Elif");
    if (token.second == "elif")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Expr())
        {
            root->agregar(h2);
            if (token.second == ":")
            {
                root->agregar(new AST(token.second));
                token = nextToken();
                if (AST* h3 = Block())
                {
                    root->agregar(h3);
                    return h1;
                }
            }
            // else
              //   Error(":");
        }
    }
    return 0;
}
AST* Parser::Else()
{
    AST* h1 = new AST("Else");
    if (token.second == "else")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (token.second == ":")
        {
            root->agregar(new AST(token.second));
            token = nextToken();
            if (AST* h2 = Block())
            {
                root->agregar(h2);
                return h1;
            }
        }
        //else
          //  Error(":");
    }
    //{$,if,while,for,pass,return,-,(,ID,None,True,False,INTEGER,STRING,[,def,DEDENT,elif,else}
    if (token.second == "$" || token.second == "if" || token.second == "while" || token.second == "for" || token.second == "pass" || token.second == "return" || token.second == "-" || token.second == "(" || token.first == "ID" || token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING" || token.second == "[" || token.first == "DEDENT" || token.second == "def" || token.second == "elif" || token.second == "else")
    {
       //root->agregar(new AST(token.second));
        /*cout << "WTF\n";
        printAST(h1);
        cout << "ENDWTF\n";*/
        return h1;
    }
    return 0;
}
AST* Parser::SSTail()
{
    AST* h1 = new AST("SSTail");
    if (token.second == "=")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Expr())
        {
            root->agregar(h2);
            return h1;
        }
    }
    if (token.first == "NEWLINE")
    {
        //root->agregar(new AST(token.first));
        return h1;
    }
    return 0;
}
AST* Parser::SimpleStatement()
{
    AST* h1 = new AST("SimpleStatement");
    if (AST* h2 = Expr())
    {
        root->agregar(h2);
        if (AST* h3 = SSTail())
        {
            root->agregar(h3);
            return h1;
        }
    }
    if (token.second == "pass")
    {
        root->agregar(new AST("pass", true));
        token = nextToken();
        return h1;
    }
    if (token.second == "return")
    {
        root->agregar(new AST("return", true));
        token = nextToken();
        if (AST* h4 = ReturnExpr())
        {
            root->agregar(h4);
            return h1;
        }
    }
    return 0;
}
AST* Parser::ReturnExpr()
{
    AST* h1 = new AST("ReturnExpr");
    if (AST* h2 = Expr())
    {
        root->agregar(h2);
        return h1;
    }
    //{NEWLINE}
    if (token.first == "NEWLINE")
    {
        //root->agregar(new AST("NEWLINE"));
        return h1;
    }
    return 0;
}
AST* Parser::Expr()
{
    AST* h1 = new AST("Expr");
    if (AST* h2 = orExpr())
    {
        root->agregar(h2);
        if (AST* h3 = ExprPrime())
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::ExprPrime()
{
    AST* h1 = new AST("ExprPrime");
    if (token.second == "if")
    {
        root->agregar(new AST("if", true));
        token = nextToken();
        if (AST* h2 = andExpr())
        {
            root->agregar(h2);
            if (token.second == "else")
            {
                root->agregar(new AST(token.second, true));
                token = nextToken();
                if (AST* h3 = andExpr())
                {
                    root->agregar(h3);
                    if (AST* h4 = ExprPrime())
                    {
                        root->agregar(h4);
                        return h1;
                    }
                }
            }
            // else
              //   Error("else");
        }
        // else
          //   Error("if");
    }
    //{:,NEWLINE,=,),],,}
    if (token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::orExpr()
{
    AST* h1 = new AST("orExpr");
    if (AST* h2 = andExpr())
    {
        root->agregar(h2);
        if (AST* h3 = orExprPrime())
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::orExprPrime()
{
    AST* h1 = new AST("orExprPrime");
    if (token.second == "or")
    {
        root->agregar(new AST("or", true));
        token = nextToken();
        if (AST* h2 = andExpr())
        {
            root->agregar(h2);
            if (AST* h3 = orExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //{if,:,NEWLINE,=,),],,}
    if (token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::andExpr()
{
    AST* h1 = new AST("andExpr");
    if (AST* h2 = notExpr())
    {
        root->agregar(h2);// BIEN
        if (AST* h3 = andExprPrime())
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::andExprPrime()
{
    AST* h1 = new AST("andExprPrime");
    if (token.second == "and")
    {
        root->agregar(new AST("and", true));
        token = nextToken();
        if (AST* h2 = notExpr())
        {
            root->agregar(h2);
            if (AST* h3 = andExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //{else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::notExpr()
{
    AST* h1 = new AST("notExpr");
    if (AST* h2 = CompExpr())
    {
        root->agregar(h2); // BIEN
        if (AST* h3 = notExprPrime())
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::notExprPrime()
{
    AST* h1 = new AST("notExprPrime");
    if (token.second == "not")
    {
        root->agregar(new AST("not", true));
        token = nextToken();
        if (AST* h2 = CompExpr())
        {
            root->agregar(h2);
            if (AST* h3 = notExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //{and,else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::CompExpr()
{
    AST* h1 = new AST("CompExpr");
    if (AST* h2 = intExpr())
    {
        root->agregar(h2); // HASTA AQUI BIEN
        if (AST* h3 = CompExprPrime()) // BIEN
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::CompExprPrime()
{
    AST* h1 = new AST("CompExprPrime");
    if (AST* h2 = CompOp())
    {
        root->agregar(h2);
        if (AST* h3 = intExpr())
        {
            root->agregar(h3);
            if (AST* h4 = CompExprPrime())
            {
                root->agregar(h4);
                return h1;
            }
        }
    }
    //{not,and,else,if,:,NEWLINE,=,or,),],,}
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::intExpr()
{
    AST* h1 = new AST("intExpr");
    if (AST* h2 = Term())
    {
        root->agregar(h2);
        //root->agregar(h1);
        if (AST* h3 = intExprPrime()) // RETORNA FALSO BUSCANDO $ PERDIDA
        {
            root->agregar(h3);
            return h1;
        }
    }
    return 0;
}
AST* Parser::intExprPrime()
{
    AST* h1 = new AST("intExprPrime");
    if (token.second == "+" || token.second == "-")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Term())
        {
            root->agregar(h2);
            if (AST* h3 = intExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //{not,and,else,if,:,NEWLINE,=,or,==,!=,<,>,<=,>=,is,),],,}
    if (token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Term()
{
    AST* h1 = new AST("Term");
    if (AST* h2 = Factor())
    {
        root->agregar(h2);
        if (AST* h3 = TermPrime()) // BUSCA $
        {
            root->agregar(h3);
            return h1;
        } // NO LO ENCUENTRA Y RETORNA 0, EL ARBOL SE PIERDE
    }
    return 0;
}
AST* Parser::TermPrime()
{
    AST* h1 = new AST("TermPrime");
    if (token.second == "*" || token.second == "//" || token.second == "%")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        if (AST* h2 = Factor())
        {
            root->agregar(h2);
            if (AST* h3 = TermPrime())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    //:, NEWLINE, =, if, else, or, and, not, ==, !=, <, >, <=, >=, is, +, -, ), ], ,
    if (token.second == "$" || token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Factor()
{
    AST* h1 = new AST("Factor");
    if (token.second == "-")
    {
        root->agregar(new AST(token.second));
        token = nextToken();
        if (AST* h2 = Factor())
        {
            root->agregar(h2);
            return h1;
        }
    }
    if (AST* h3 = Name())
    {
        root->agregar(h3); // TODO EL ARBOL ANTES DE $
        return h1;
    }
    if (AST* h4 = Literal())
    {
        root->agregar(h4);
        return h1;
    }
    if (AST* h5 = List())
    {
        root->agregar(h5);
        return h1;
    }
    if (token.second == "(")
    {
        root->agregar(new AST(token.second, false, "("));
        token = nextToken();
        if (AST* h6 = Expr())
        {
            root->agregar(h6);
            if (token.second == ")")
            {
                root->agregar(new AST(token.second, false, ")"));
                token = nextToken();
                return h1;
            }
            // else
              //   Error(")");
        }
    }
    return 0;
}
AST* Parser::Name()
{
    AST* h1 = new AST("Name");
    if (token.first == "ID")
    {
        AST* idi = new AST("ID");
        idi->agregar(new AST(token.second, true, "ID"));
        root->agregar(idi);
        token = nextToken();
        if (AST* h2 = NameTail())
        {
            root->agregar(h2);
            return h1;
        }
    }
    return 0;
}
AST* Parser::NameTail()
{
    AST* h1 = new AST("NameTail");
    if (token.second == "(")
    {
        root->agregar(new AST("(", false, "("));
        token = nextToken();
        if (AST* h2 = ExprList())
        {
            root->agregar(h2);
            if (token.second == ")")
            {
                root->agregar(new AST(")", false, ")"));
                //root->agregar(h1);
                token = nextToken(); // AQUI PASA A BUSCAR AL $
                return h1;
            }
            // else
              //   Error(")");
        }
    }
    if (AST* h3 = List())
    {
        root->agregar(h3);
        return h1;
    }
    //{*,//,%,+,-,not,and,else,if,:,NEWLINE,=,or,==,!=,<,>,<=,>=,is,),],,}
    if (token.second == "*" || token.second == "//" || token.second == "%" || token.second == "+" || token.second == "-" || token.second == "not" || token.second == "and" || token.second == "else" || token.second == "if" || token.second == ":" || token.first == "NEWLINE" || token.second == "=" || token.second == "or" || token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is" || token.second == ")" || token.second == "]" || token.second == ",")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::Literal()
{
    AST* h1 = new AST("Literal");
    if (token.second == "None" || token.second == "True" || token.second == "False" || token.first == "INTEGER" || token.first == "STRING")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        return h1;
    }
    return 0;
}
AST* Parser::List()
{
    AST* h1 = new AST("List");
    if (token.second == "[")
    {
        root->agregar(new AST("["));
        token = nextToken();
        if (AST* h2 = ExprList())
        {
            root->agregar(h2);
            if (token.second == "]")
            {
                root->agregar(new AST("]"));
                token = nextToken();
                return h1;
            }
            // else
              //   Error("]");
        }
    }
    return 0;
}
AST* Parser::ExprList()
{
    AST* h1 = new AST("ExprList");
    if (AST* h2 = Expr())
    {// BIEN
        root->agregar(h2);
        printAST(h1);
        if (AST* h3 = ExprListTail())
        {
            root->agregar(h3);
            return h1;
        }
    }
    if (token.second == ")" || token.second == "]")
    {
        //root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::ExprListTail()
{
    AST* h1 = new AST("ExprListTail");
    if (token.second == ",")
    {
        root->agregar(new AST(","));
        token = nextToken();
        if (AST* h2 = Expr())
        {
            root->agregar(h2);
            if (AST* h3 = ExprListTail())
            {
                root->agregar(h3);
                return h1;
            }
        }
    }
    if (token.second == ")" || token.second == "]")
    {
        root->agregar(new AST(token.second));
        return h1;
    }
    return 0;
}
AST* Parser::CompOp()
{
    AST* h1 = new AST("CompOp");
    if (token.second == "==" || token.second == "!=" || token.second == "<" || token.second == ">" || token.second == "<=" || token.second == ">=" || token.second == "is")
    {
        root->agregar(new AST(token.second, true));
        token = nextToken();
        return h1;
    }
    return 0;
}
int main(int argc, const char* argv[])
{
    tok = Realizar();
    Parser x;
    x.token = tok[0];
    x.Imprimir2();
    cout << endl;
    cout << "---------Parser---------\n";
    if (x.Program())
        cout << "True" << endl;
    else
        cout << "False" << endl;
    cout << "pos: " << x.pos << endl;
    x.ImprimirErrores();
    cout << "---------PARSER TREE---------\n";
    x.imprimirAST();
    cout << "---------AST TREE---------\n";
    x.AST_Tree();
    x.imprimirAST_Tree();

    return 0;
}
