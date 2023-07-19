//
//  Parser.cpp
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#include "Parser.h"
#include "Scanner.h"
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

        if ((astvector[i]->tipo == "ID" && astvector[i]->valor.size() > 3) || astvector[i]->valor == "if" || astvector[i]->valor == "else" || astvector[i]->valor == "elif" || astvector[i]->valor == "return" || astvector[i]->valor == "for" || astvector[i]->valor == "while" || astvector[i]->valor == "in" || astvector[i]->valor == "True" || astvector[i]->valor == "False")
        {
            id++; // tabulaciones
            cout << "\n";
            for (int j = 0; j < id; j++)
            {
                cout << "   ";
            }
            cout << "| --" << astvector[i]->valor << "\n ";
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
        astRoot = new AST(rootNode->valor, rootNode->isleaf, rootNode->tipo);
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


AST* Parser::Program()
{
    root = new AST("Program");
    astvector.push_back(root);
    if (AST* h1 = DefList())
    {
        root->agregar(h1);
        if(AST* h2 = StatementList())
        {
            root->agregar(h2);
            return root;
        }
    }
    return 0;

}
AST* Parser::DefList() //Funcion con vacio
{
    AST* h1 = new AST("DefList");
    if(token.valor == "def") //First de Def
    {
        if (AST* h2 = Def())
        {
            root->agregar(h2);
            if (AST* h3 = DefList())
            {

                root->agregar(h3);
                printAST(h1);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    //Follow
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[")
        return h1;
    //Error
    AgregarError("Error, token inesperado en la produccion DefList",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Def()
{
    AST* h1 = new AST("Def");
    if (token.valor == "def")
    {
        root->agregar(new AST("def"));
        token = nextToken();
        if (token.tipo == "ID")
        {
            AST* idi = new AST("ID");
            idi->agregar(new AST(token.valor, true, "ID"));
            root->agregar(idi);
            token = nextToken();
            if (token.valor == "(")
            {
                root->agregar(new AST("(", false, "("));
                token = nextToken();
                if (AST* h2 = TypedVarList())
                {
                    root->agregar(h2);
                    if (token.valor == ")")
                    {
                        root->agregar(new AST(")", false, ")"));
                        token = nextToken();
                        if (AST* h3 = Return())
                        {
                            root->agregar(h3);
                            if (token.valor == ":")
                            {
                                root->agregar(new AST(":"));
                                token = nextToken();
                                if(token.tipo == "NEWLINE")
                                {
                                    if (AST* h4 = Block()) //retorna 0
                                    {
                                        root->agregar(h4);
                                        return h1;
                                    }
                                    else
                                        return 0;
                                }
                                else
                                {
                                    AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Def)",token.fila);
                                    SaltarError();
                                    if(token.tipo != "NEWLINE")
                                        token = nextToken();
                                    if(AST* h5 = Block())
                                    {
                                        root->agregar(h5);
                                        return h1;
                                    }
                                    else
                                        return 0;
                                }
                            }
                            else
                            {
                                AgregarError("Error, se esperba un : (Produccion Def) ", token.fila);
                                SaltarError();
                                if(token.tipo != "NEWLINE")
                                    token = nextToken();
                                if(AST* h6 = Block())
                                {
                                    root->agregar(h6);
                                    return h1;
                                }
                                else
                                    return 0;
                            }
                        }
                        else
                        {
                            SaltarError();
                            if(token.tipo != "NEWLINE")
                                token = nextToken();
                            if(AST* h7 = Block())
                            {
                                root->agregar(h7);
                                return h1;
                            }
                            else
                                return 0;
                        }
                    }
                    else
                    {
                        AgregarError("Error, se esperaba un ) (Produccion Def) ", token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(AST* h8 = Block())
                        {
                            root->agregar(h8);
                            return h1;
                        }
                        else
                            return 0;
                    }
                }
                else
                {
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    if(AST* h9 = Block())
                    {
                        root->agregar(h9);
                        return h1;
                    }
                    else
                        return 0;
                }
            }
            else
            {
                AgregarError("Error, se esperaba un ( (Produccion Def) ", token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h10 = Block())
                {
                    root->agregar(h10);
                    return h1;
                }
                else
                    return 0;
            }

        }
        else
        {
            AgregarError("Error, se esperaba un ID (Produccion Def) ", token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h11 = Block())
            {
                root->agregar(h11);
                return h1;
            }
            else
                return 0;
        }
    }
    return 0;
}
AST* Parser::TypedVar()
{
    AST* h1 = new AST("TypedVar");
    if (token.tipo == "ID")
    {
        AST* idi = new AST("ID");
        idi->agregar(new AST(token.valor, true, "ID"));
        root->agregar(idi);
        token = nextToken();
        if (token.valor == ":")
        {
            root->agregar(new AST(":"));
            token = nextToken();
            if(token.valor == "int" || token.valor == "str" || token.valor == "[") //Firsts de Type
            {
                if(AST* h2 = Type())
                {
                    root->agregar(h2);
                    return h1;
                }
                else
                    return 0;
            }
            else
            {
                AgregarError("Error despues de los : (Produccion TypedVar)",token.fila);
                return 0;
            }
        }
        else
        {
            AgregarError("Error se esperaba un : (Produccion TypedVar) ",token.fila);
            return 0;
        }
    }
    return 0;
}
AST* Parser::Type()
{
    AST* h1 = new AST("Type");
    if (token.valor == "int")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        return h1;
    }
    if (token.valor == "str")
    {
        root->agregar(new AST(token.valor));
        token = nextToken();
        return h1;
    }
    if (token.valor == "[")
    {
        token = nextToken();
        if (token.valor == "int" || token.valor == "str" || token.valor == "[")
        {
            if(AST* h2 = Type())
            {
                root->agregar(h2);
                if (token.valor == "]")
                {
                    root->agregar(new AST(token.valor));
                    token = nextToken();
                    return h1;
                }
                else
                {
                    AgregarError("Error, se esperaba un ] (Produccion Type) ",token.fila);
                    return 0;
                }
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error en el contenido de los [ ] (Produccion Type)",token.fila);
            return 0;
        }
    }
    return 0;
}
AST* Parser::TypedVarList() //Funcion con vacio
{
    AST* h1 = new AST("TypedVarList");
    if (token.tipo == "ID") //First de TypedVar
    {
        if(AST* h2 = TypedVar())
        {
            root->agregar(h2);
            if(AST* h3 = TypedVarListTail())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    if (token.valor == ")")
        return h1;
    AgregarError("Error, token inesperado en la produccion TypedVarList", token.fila);
    return 0; //-> return 1 Por si quiere que se muestre el error de cierre de parentesis
}
AST* Parser::TypedVarListTail() //Funcion con vacio
{
    AST* h1 = new AST("TypeVarListTail");
    if (token.valor == ",")
    {
        root->agregar(new AST(token.valor));
        token = nextToken();
        if (token.tipo == "ID") //First TypedVar
        {
            if(AST* h2 = TypedVar())
            {
                root->agregar(h2);
                if(AST* h3 = TypedVarListTail())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error, no le sigue un ID (Produccion TypedVarListTail)",token.fila);
            return 0;
        }
    }
    if (token.valor == ")")
        return h1;
    AgregarError("Error, token inesperado: "+ token.tipo + " (Produccion TypedVarListTail)",token.fila);
    return 0;
}
AST* Parser::Return() //Funcion con vacio
{
    AST* h1 = new AST("Return");
    if (token.valor == "->")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == "int" || token.valor == "str" || token.valor == "[") //First de type
        {
            if(AST* h2 = Type())
            {
                root->agregar(h2);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    //Follow
    if (token.valor == ":")
        return h1;
    AgregarError("Error, token inesperado en la produccion Return (def)",token.fila);
    return 0;
}
AST* Parser::Block()
{
    AST* h1 = new AST("Block");
    if (token.tipo == "NEWLINE")
    {
        root->agregar(new AST(token.tipo));
        token = nextToken();
        if (token.tipo == "INDENT")
        {
            root->agregar(new AST(token.tipo));
            token = nextToken();
            if (token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Statement
            {
                if(AST* h2 = Statement())
                {
                    root->agregar(h2);
                    if(AST* h3 = StatementList())
                    {
                        root->agregar(h3);
                        if (token.tipo == "DEDENT")
                        {
                            root->agregar(new AST(token.tipo));
                            token = nextToken();
                            return h1;
                        }
                        else
                        {
                            if(token.tipo != "$")
                                AgregarError("Error de DEDENT",token.fila);
                            SaltarError(); //Not sure about this. If it doesnt work i will change it
                            if(token.tipo != "NEWLINE")
                                token = nextToken();
                            if(token.tipo == "NEWLINE")
                                token = nextToken(); //Not sure about this. If it doesnt work i will change it
                            if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                                token = nextToken();
                            if(AST* h4 = StatementList())
                            {
                                root->agregar(h4);
                                return h1;
                            }
                            else
                                return 0;
                        }
                    }
                    else
                    {
                        AgregarError("Error de StatementList, token inesperado: "+token.valor,token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(token.tipo == "NEWLINE")
                            token = nextToken(); //Not sure about this. If it doesnt work i will change it
                        if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                            token = nextToken();
                        return h1;
                    }
                }
            }
            else
            {
                AgregarError("Error de Statement, token inesperado: "+token.valor,token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(token.tipo == "NEWLINE")
                    token = nextToken(); //Not sure about this. If it doesnt work i will change it
                if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                    token = nextToken();
                if(AST* h5 = StatementList())
                {
                    root->agregar(h5);
                    return h1;
                }
                else
                    return 0;
            }
        }
        else
        {
            if(token.tipo != "$")
                AgregarError("Error de INDENT",token.fila);
            SaltarError(); //Not sure about this. If it doesnt work i will change it
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(token.tipo == "NEWLINE")
                token = nextToken(); //Not sure about this. If it doesnt work i will change it
            if(token.tipo == "INDENT" || token.tipo == "DEDENT")
                token = nextToken();
            if(AST* h6 = StatementList())
            {
                root->agregar(h6);
                return h1;
            }
            else
                return 0;
        }
    }
    return 0;
}
AST* Parser::StatementList() //Funcion con vacio
{
    AST* h1 = new AST("StatementList");
    if (token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Statement
    {
        if(AST* h2 = Statement())
        {
            
            if(AST* h3 = StatementList())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    //{$,DEDENT}
    if (token.valor == "$" || token.tipo == "DEDENT")
        return h1;
    return 0;
}
AST* Parser::Statement()
{
    AST* h1 = new AST("Statement");
    if (token.valor == "if")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr)
        {
            if(AST* h2 = Expr())
            {
                root->agregar(h2);
                if (token.valor == ":")
                {
                    root->agregar(new AST(token.valor));
                    token = nextToken();
                    if (token.tipo == "NEWLINE")
                    {
                        if(AST* h3 =Block())
                        {
                            root->agregar(h3);
                            if (AST* h4 = ElifList())
                            {
                                root->agregar(h4);
                                if(AST* h5 = Else())
                                {
                                    root->agregar(h5);
                                    return h1;
                                }
                                else
                                    return 0;
                            }
                            else
                                return 0;
                        }
                        else
                            return 0;
                    }
                    else
                    {
                        AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Statement)-if",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(AST* h6 = Block())
                            if(AST* h7 = ElifList())
                                if(AST* h8 = Else())
                                {
                                        root->agregar(h6);
                                        root->agregar(h7);
                                        root->agregar(h8);
                                        return h1;
                                }
                                else
                                    return 0;
                            else
                                return 0;
                        else
                            return 0;
                    }
                }
                else
                {
                    AgregarError("Error, se esperaba un : (Produccion Statement)-if ",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    if(AST* h9 = Block())
                        if(AST* h10 = ElifList())
                            if(AST* h11 = Else())
                            {
                                    root->agregar(h9);
                                    root->agregar(h10);
                                    root->agregar(h11);
                                    return h1;
                            }
                            else
                                return 0;
                        else
                            return 0;
                    else
                        return 0;
                }
            }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h12 = Block())
                    if(AST* h13 = ElifList())
                        if(AST* h14 = Else())
                        {
                                root->agregar(h12);
                                root->agregar(h13);
                                root->agregar(h14);
                                return h1;
                        }
                        else
                            return 0;
                    else
                        return 0;
                else
                    return 0;
            }
        }
        else
        {
            AgregarError("Error Expr, error despues del token if (Produccion Statement)-if",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h15 = Block())
                if(AST* h16 = ElifList())
                    if(AST* h17 = Else())
                    {
                            root->agregar(h15);
                            root->agregar(h16);
                            root->agregar(h17);
                            return h1;
                    }
                    else
                        return 0;
                else
                    return 0;
            else
                return 0;
        }
    }
    if(token.valor == "while")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
        {
            if(AST* h6 = Expr())
            {
                root->agregar(h6);
                if(token.valor == ":")
                {
                    root->agregar(new AST(token.valor));
                    token = nextToken();
                    if(token.tipo == "NEWLINE")
                    {
                        if (AST* h7 = Block())
                        {
                            root->agregar(h7);
                            return h1;
                        }
                        else
                            return 0;
                    }
                    else
                    {
                        AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Statement)-while",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(AST* h8 = Block())
                        {
                            root->agregar(h8);
                            return h1;
                        }
                        else
                            return 0;
                    }
                }
                else
                {
                    AgregarError("Error, se esperaba un : (Produccion Statement)-while",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    if(AST* h8 = Block())
                    {
                        root->agregar(h8);
                        return h1;
                    }
                    else
                        return 0;
                }
            }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h8 = Block())
                {
                    root->agregar(h8);
                    return h1;
                }
                else
                    return 0;
            }
        }
        else
        {
            AgregarError("Error Expr, error despues del token while (Produccion Statement)-while",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h8 = Block())
            {
                root->agregar(h8);
                return h1;
            }
            else
                return 0;
        }
    }
    if (token.valor == "for")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.tipo == "ID")
        {
            AST* idi = new AST("ID");
            idi->agregar(new AST(token.valor, true, "ID"));
            token = nextToken();
            root->agregar(idi);
            if (token.valor == "in")
            {
                root->agregar(new AST(token.valor, true));
                token = nextToken();
                if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
                {
                    if(AST* h8 = Expr())
                    {
                        root->agregar(h8);
                        if (token.valor == ":")
                        {
                            root->agregar(new AST(token.valor));
                            token = nextToken();
                            if(token.tipo == "NEWLINE")
                            {
                                if (AST* h9 = Block())
                                {
                                    root->agregar(h9);
                                    return h1;
                                }
                                else
                                    return 0;
                            }
                            else
                            {
                                AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Statement)-for",token.fila);
                                SaltarError();
                                if(token.tipo != "NEWLINE")
                                    token = nextToken();
                                if(AST* h8 = Block())
                                {
                                    root->agregar(h8);
                                    return h1;
                                }
                                else
                                    return 0;
                            }
                        }
                        else
                        {
                            AgregarError("Error, se esperaba un : (Produccion Statement)-for ",token.fila);
                            SaltarError();
                            if(token.tipo != "NEWLINE")
                                token = nextToken();
                            if(AST* h8 = Block())
                            {
                                root->agregar(h8);
                                return h1;
                            }
                            else
                                return 0;
                        }
                    }
                    else
                    {
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(AST* h9 = Block())
                        {
                            root->agregar(h9);
                            return h1;
                        }
                        else
                            return 0;
                    }
                }
                else
                {
                    AgregarError("Error Expr, error despues del in (Produccion Statemet)-for",token.fila);
                    SaltarError();
                    if (token.tipo != "NEWLINE")
                        token = nextToken();
                    if(AST* h10 = Block())
                    {
                        root->agregar(h10);
                        return h1;
                    }
                    else
                        return 0;
                }
            }
            else
            {
                AgregarError("Error, se esperaba un in (Produccion Statement)-for",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h11 = Block())
                {
                    root->agregar(h11);
                    return h1;
                }
                else
                    return 0;
            }
        }
        else
        {
            AgregarError("Error, se esperaba un ID (Produccion Statement)-for",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h12 = Block())
            {
                root->agregar(h12);
                return h1;
            }
            else
                return 0;
        }
    }
    if (token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "("  || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo ==  "STRING" || token.valor == "[")
    {
        if(AST* h13 = SimpleStatement())
        {
            root->agregar(h13);
            if (token.tipo == "NEWLINE")
            {
                root->agregar(new AST(token.tipo));
                token = nextToken();
                return h1;
            }
            else
            {
                AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Statement)-SimpleStatement",token.fila);
                SaltarError();
                if (token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h14 = StatementList())
                {
                    root->agregar(h14);
                    return h1;
                }
                else
                    return 0;
            }
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::ElifList() //Funcion con vacio
{
    AST* h1 = new AST("ElifList");
    if (token.valor == "elif")
    {
        if(AST* h2 = Elif())
        {
            root->agregar(h2);
            if(AST* h3 = ElifList())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    //Follow
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "["|| token.valor == "DEDENT" || token.valor == "else" || token.valor == "elif")
        return h1;
    //Colocar error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion ElifList",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Elif()
{
    AST* h1 = new AST("Elif");
    if (token.valor == "elif")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
            
        {
            if(AST* h2 = Expr())
            {
                root->agregar(h2);
                if (token.valor == ":")
                {
                    root->agregar(new AST(token.valor));
                    token = nextToken();
                    if(token.tipo == "NEWLINE")
                    {
                        if (AST* h3 = Block())
                        {
                            root->agregar(h3);
                            return h1;
                        }
                    }
                    else
                    {
                        AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Elif)",token.fila);
                        SaltarError();
                        if(token.tipo != "NEWLINE")
                            token = nextToken();
                        if(AST* h4 = Block())
                        {
                            root->agregar(h4);
                            return h1;
                        }
                        else
                            return 0;
                    }
                }
                else
                {
                    AgregarError("Error, se esperaba un : (Produccion Elif) ",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    if(AST* h4 = Block())
                    {
                        root->agregar(h4);
                        return h1;
                    }
                    else
                        return 0;
                }
            }
            else
            {
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h4 = Block())
                {
                    root->agregar(h4);
                    return h1;
                }
                else
                    return 0;
            }
        }
        else
        {
            AgregarError("Error Expr, error despues del elif (Produccion Elif)",token.fila);
            SaltarError();
            if (token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h4 = Block())
            {
                root->agregar(h4);
                return h1;
            }
            else
                return 0;
        }
    }
    return 0;
}
AST* Parser::Else() //Funcion con vacios
{
    AST* h1 = new AST("Else");
    if (token.valor == "else")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == ":")
        {
            root->agregar(new AST(token.valor));
            token = nextToken();
            if(token.tipo == "NEWLINE")
            {
                 if (AST* h2 = Block())
                 {
                     root->agregar(h2);
                     return h1;
                 }
                else
                    return 0;
            }
            else
            {
                AgregarError("Error, token inesperado se esperaba un NEWLINE (Produccion Else)",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                if(AST* h4 = Block())
                {
                    root->agregar(h4);
                    return h1;
                }
                else
                    return 0;
            }

        }
        else
        {
            AgregarError("Error, se esperaba un : (Produccion Else)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            if(AST* h4 = Block())
            {
                root->agregar(h4);
                return h1;
            }
            else
                return 0;
        }
    }
    //Follow
    if (token.valor == "$" || token.valor == "if" || token.valor == "while" || token.valor == "for" || token.valor == "pass" || token.valor == "return" || token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[" ||token.tipo == "DEDENT" || token.valor == "def" || token.valor == "elif" || token.valor == "else")
        return h1;
    //Colocar error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion else",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::SSTail() //Funcion con vacio
{
    AST* h1 = new AST("SSTail");
    if (token.valor == "=")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr)
        {
            if(AST* h2 = Expr())
            {
                root->agregar(h2);
                return h1;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error en la asignacion (Produccion SSTail)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.tipo == "NEWLINE")
        return h1;
    //Colocar error
    AgregarError("Error, token inesperado no se termino la Produccion SSTail",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::SimpleStatement()
{
    AST* h1 = new AST("SimpleStatement");
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
    {
        if(AST* h2 = Expr())
        {
            root->agregar(h2);
            if(AST* h3 = SSTail())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    if (token.valor == "pass")
    {
        root->agregar(new AST("pass", true));
        token = nextToken();
        return h1;
    }
    if (token.valor == "return")
    {
         root->agregar(new AST("return", true));
         token = nextToken();
         if (AST* h4 = ReturnExpr())
         {
             root->agregar(h4);
             return h1;
         }
        else
            return 0;
    }
    return 0;
}
AST* Parser::ReturnExpr() //Funcion con vacio
{
    AST* h1 = new AST("ReturnExpr");
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Expr
    {
        if(AST* h2 = Expr())
        {
            root->agregar(h2);
            return h1;
        }
        else
            return 0;
    }
    //Follows
    if (token.tipo == "NEWLINE")
        return h1;
    //Error
    AgregarError("Error en el return (Produccion ReturnExpr)",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Expr()
{
    AST* h1 = new AST("Expr");
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de orExpr
    {
        if(AST* h2 = orExpr())
        {
            root->agregar(h2);
            if(AST* h3 = ExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::ExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("ExprPrime");
    if (token.valor == "if")
    {
        root->agregar(new AST("if", true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
        {
            if(AST* h2 = andExpr())
            {
                root->agregar(h2);
                if (token.valor == "else")
                {
                    root->agregar(new AST(token.valor, true));
                    token = nextToken();
                    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr)
                    {
                        if(AST* h3 = andExpr())
                        {
                            root->agregar(h3);
                            if(AST* h4 = ExprPrime())
                            {
                                root->agregar(h4);
                                return h1;
                            }
                            else
                                return 0;
                        }
                        else
                            return 0;
                    }
                    else
                    {
                        AgregarError("Error, token inesperado despues del else (Produccion Exprime)",token.fila);
                        SaltarError();
                        if (token.tipo != "NEWLINE")
                            token = nextToken();
                        return h1;
                    }
                }
                else
                {
                    AgregarError("Error en el else (Produccion ExprPrime)",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    return h1;
                }
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error AndExpr, token inesperado despues del if (Produccion ExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return h1;
        }
    }
    //{:,NEWLINE,=,),],,}
    if (token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion ExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::orExpr()
{
    AST* h1 = new AST("orExpr");
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
    {
        if(AST* h2 = andExpr())
        {
            root->agregar(h2);
            if(AST* h3 = orExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::orExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("orExprPrime");
    if (token.valor == "or")
    {
        root->agregar(new AST("or", true));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de andExpr
        {
            if(AST* h2 = andExpr())
            {
                root->agregar(h2);
                if(AST* h3 = orExprPrime())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error AndExpr, error despues del or (Produccion OrExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion orExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::andExpr()
{
    AST* h1 = new AST("andExpr");
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de notExpr
    {
        if(AST* h2 = notExpr())
        {
            root->agregar(h2);// BIEN
            if(AST* h3 = andExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::andExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("andExprPrime");
    if (token.valor == "and")
    {
        root->agregar(new AST("and", true));
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de notExpr
        {
            if(AST* h2 = notExpr())
            {
                root->agregar(h2);
                if(AST* h3 = andExprPrime())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error NotExpr, error despues del and (Produccion AndExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;

        }
    }
    //Follow
    if (token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion andExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::notExpr()
{
    AST* h1 = new AST("notExpr");
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de CompExpr
    {
        if(AST* h2 = CompExpr())
        {
            root->agregar(h2); // BIEN
            if(AST* h3 = notExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::notExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("notExprPrime");
    if (token.valor == "not")
    {
        root->agregar(new AST("not", true));
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de CompExpr
        {
            if(AST* h2 = CompExpr())
            {
                root->agregar(h2);
                if(AST* h3 = notExprPrime())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error CompExpr, error despues del not (Produccion notExprPrime)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follow
    if (token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion notExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::CompExpr()
{
    AST* h1 = new AST("CompExpr");
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de intExpr
    {
        if(AST* h2 = intExpr())
        {
            root->agregar(h2); // HASTA AQUI BIEN
            if(AST* h3 = CompExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::CompExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("CompExprPrime");
    if (token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is") //First de CompOp
    {
        if(AST* h2 = CompOp())
        {
            root->agregar(h2);
            if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de intExpr
            {
                if(AST* h3 = intExpr())
                {
                    root->agregar(h3);
                    if(AST* h4 = CompExprPrime())
                    {
                        root->agregar(h4);
                        return h1;
                    }
                    else
                        return 0;
                }
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
    }
    //Follow
    if (token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion CompExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::intExpr()
{
    AST* h1 = new AST("intExpr");
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Term
    {
        if(AST* h2 = Term())
        {
            root->agregar(h2);
            if(AST* h3 = intExprPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::intExprPrime() //Funcion con vacio
{
    AST* h1 = new AST("intExprPrime");
    if (token.valor == "+" || token.valor == "-")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Term
        {
            if(AST* h2 = Term())
            {
                root->agregar(h2);
                if(AST* h3 = intExprPrime())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
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
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion intExprPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Term()
{
    AST* h1 = new AST("Term");
    if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
    {
        if(AST* h2 = Factor())
        {
            root->agregar(h2);
            if(AST* h3 = TermPrime())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::TermPrime() //Funcion con vacio
{
    AST* h1 = new AST("TermPrime");
    if (token.valor == "*" || token.valor == "//" || token.valor == "%")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
        {
            if(AST* h2 = Factor())
            {
                root->agregar(h2);
                if (AST* h3 = TermPrime())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
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
        return h1;
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion TermPrime",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Factor()
{
    AST* h1 = new AST("Factor");
    if (token.valor == "-")
    {
        root->agregar(new AST(token.valor));
        token = nextToken();
        if(token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First de Factor
        {
            if(AST* h2 = Factor())
            {
                root->agregar(h2);
                return h1;
            }
            else
                return 0;
        }
    }
    if (token.tipo == "ID")//First de Name
    {
        if(AST* h3 = Name())
        {
            root->agregar(h3); // TODO EL ARBOL ANTES DE $
            return h1;
        }
        else
            return 0;
    }
    if (token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING") //First de Literal
    {
        if (AST* h4 = Literal())
        {
            root->agregar(h4);
            return h1;
        }
        else
            return 0;
    }
    if (token.valor == "[") //First List
    {
        if (AST* h5 = List())
        {
            root->agregar(h5);
            return h1;
        }
        else
            return 0;
    }
    if (token.valor == "(")
    {
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[")//First Expr
        {
            if(AST* h6 = Expr())
            {
                root->agregar(h6);
                if (token.valor == ")")
                {
                    root->agregar(new AST(token.valor, false, ")"));
                    token = nextToken();
                    return h1;
                }
                else
                {
                    AgregarError("Error, se esperaba un ) (Produccion Factor)",token.fila);
                    SaltarError();
                    if(token.tipo != "NEWLINE")
                        token = nextToken();
                    return 0;
                }
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error Expr, token inesperado no se termino el contenido de los () (Produccion Factor)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    return 0;
}
AST* Parser::Name()
{
    AST* h1 = new AST("Name");
    if (token.tipo == "ID")
    {
        AST* idi = new AST("ID");
        idi->agregar(new AST(token.valor, true, "ID"));
        root->agregar(idi);
        token = nextToken();
        if(AST* h2 = NameTail())
        {
            root->agregar(h2);
            return h1;
        }
        else
            return 0;
    }
    return 0;
}
AST* Parser::NameTail() //Funcion con vacio
{
    AST* h1 = new AST("NameTail");
    if (token.valor == "(")
    {
        root->agregar(new AST("(", false, "("));
        token = nextToken();
        if(AST* h2 = ExprList())
        {
            root->agregar(h2);
            if (token.valor == ")")
            {
                root->agregar(new AST(")", false, ")"));
                token = nextToken();
                return h1;
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
        else
            return 0;
    }
    if (token.valor == "[")
    {
        if (AST* h3 = List())
        {
            root->agregar(h3);
            return h1;
        }
        else
            return 0;
    }
    //Follow
    if (token.valor == "*" || token.valor == "//" || token.valor == "%" || token.valor == "+" || token.valor == "-" || token.valor == "not" || token.valor == "and" || token.valor == "else" || token.valor == "if" || token.valor == ":" || token.tipo == "NEWLINE" || token.valor == "=" || token.valor == "or" || token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is" || token.valor == ")" || token.valor == "]" || token.valor == ",")
        return h1;
    //Error
    AgregarError("Error, token inesperado: "+token.valor+", en la produccion NameTail",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::Literal()
{
    AST* h1 = new AST("Literal");
    //First
    if (token.valor == "None" || token.valor == "True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        return h1;
    }
    return 0;
}
AST* Parser::List()
{
    AST* h1 = new AST("List");
    if (token.valor == "[")
    {
        root->agregar(new AST("["));
        token = nextToken();
        if(AST* h2 = ExprList())
        {
            root->agregar(h2);
            if (token.valor == "]")
            {
                root->agregar(new AST("]"));
                token = nextToken();
                return h1;
            }
            else
            {
                AgregarError("Error se esperaba un ] (Produccion List)",token.fila);
                SaltarError();
                if(token.tipo != "NEWLINE")
                    token = nextToken();
                return 0;
            }
        }
    }
    return 0;
}
AST* Parser::ExprList() //Funcion con vacio
{
    AST* h1 = new AST("ExprList");
    if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First Expr
    {
        if(AST* h2 = Expr())
        {
            root->agregar(h2);
            printAST(h1);
            if(AST* h3 = ExprListTail())
            {
                root->agregar(h3);
                return h1;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    //Follows
    if (token.valor == ")" || token.valor == "]")
    {
        return h1;
    }
    //Colocar Error
    AgregarError("Error en contenido de la lista (Produccion ExprList)",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::ExprListTail() //Funcion con vacio
{
    AST* h1 = new AST("ExprListTail");
    if (token.valor == ",")
    {
        root->agregar(new AST(","));
        token = nextToken();
        if (token.valor == "-" || token.valor == "(" || token.tipo == "ID" || token.valor == "None" || token.valor =="True" || token.valor == "False" || token.tipo == "INTEGER" || token.tipo == "STRING" || token.valor == "[") //First Expr
        {
            if(AST* h2 = Expr())
            {
                root->agregar(h2);
                if(AST* h3 = ExprListTail())
                {
                    root->agregar(h3);
                    return h1;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            AgregarError("Error, token inesperado despues de la coma(,) (Produccion ExprListTail)",token.fila);
            SaltarError();
            if(token.tipo != "NEWLINE")
                token = nextToken();
            return 0;
        }
    }
    //Follows
    if (token.valor == ")" || token.valor == "]")
    {
        return h1;
    }
    //Colocar error
    AgregarError("Error, token inesperado se esperaba  alguno de los siguientes simbolos (, _ ) _ ]) (Produccion ExprListTail)",token.fila);
    SaltarError();
    if(token.tipo != "NEWLINE")
        token = nextToken();
    return 0;
}
AST* Parser::CompOp()
{
    AST* h1 = new AST("CompOp");
    //First's
    if(token.valor == "==" || token.valor == "!=" || token.valor == "<" || token.valor == ">" || token.valor == "<=" || token.valor == ">=" || token.valor == "is")
    {
        root->agregar(new AST(token.valor, true));
        token = nextToken();
        return h1;
    }
    return 0;
}
