//
//  Scanner2.h
//  Parser
//
//  Created by Lucho on 6/30/23.
//

#ifndef Scanner2_h
#define Scanner2_h

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

vector<string> keywords = { "and", "as", "assert", "async", "await", "bool", "break",
"class","continue", "def", "del", "elif", "else", "except", "False", "finally","float",
"for", "from", "global", "if", "import", "in", "int", "is","lambda", "None", "nonlocal",
"not", "or", "pass", "raise", "return","str", "True", "try", "while", "with", "yield" };

vector<string> operadores = { "+", "-", "*", "/","//", "%", "<", ">", "<=",
">=", "==", "!=", "=", ",", ":", ".", "->" ,"!=", "!" };

vector<string> delimitadores = { "(", ")", "[", "]" };

struct Token
{
    string valor, tipo, error;
    int fila, columna;
    Token(string valor_, string tipo_, int fila_, int columna_, string error_ = "")
    {
        valor = valor_;
        tipo = tipo_;
        fila = fila_;
        columna = columna_;
        error = error_;
    }
};

struct Scanner
{
    vector<Token> tokens;
    vector<string> lineas;
    vector<Token> errores;
    string nombreArchivo;
    int fila, columna;
    stack<int> indent;

    Scanner(string nombreArchivo_)
    {
        nombreArchivo = nombreArchivo_;
        fila = columna = 0;
        indent.push(0);
    }
    void leerArchivo();
    void printLinea();
    void escaner();
    char getchar();
    char peekchar();
    void imprimirTokens();
    void imprimirErrores();
    bool verificarIndentacion(int n);
    vector<pair<string, string>> GetTokens();
    //void verificarEspacios(); // se utiliza para evitar colocar newline luego de que ocurra un error y no se detecte un token. PREGUNTAR, creo que esta bien asi
};
vector<pair<string, string>> Scanner::GetTokens()
{
    vector<pair<string, string>> v;
    for (int i = 0; i < tokens.size(); i++)
    {
        v.push_back(make_pair(tokens[i].tipo, tokens[i].valor));
    }
    return v;
}
char Scanner::getchar()
{
    char c = lineas[fila][columna];
    columna++;
    return c;
}

char Scanner::peekchar()
{
    char c = lineas[fila][columna];
    return c;
}

bool Scanner::verificarIndentacion(int n)
{
    if (n < indent.top()) // hay indentacion
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Scanner::escaner()
{
    string token = ""; // va a guardar el valor del token

    // voy a leer linea por linea del vector de strings lineas
    for (; fila < lineas.size(); fila++)
    {
        bool primera_tabulacion = false; // se utiliza para solo reconocer tabs al inicio de la linea
        bool primer_espacio = false;
        int cant_indent = 0;
        cout << "[";
        cout << fila << "";

        // indentaciones por linea
        string revision = lineas[fila];
        bool flag = false; // para hallar identacion en cada fila

        bool validIndent = false;


        for (int i = 0; i < revision.length(); i++)
        {
            if (flag == false && revision[i] == 32) // espacio
            {
                cant_indent++;
            }
            else if (flag == false && revision[i] == 9) // tab
            {
                cant_indent += 8;
            }
            else if (revision[i] != 9 || revision[i] != 32)
            {
                flag = true;
            }
        }
        // ya tengo el numero de espacios para indent

        for (int i = 0; i < revision.length(); i++)
        {
            if (revision[i] != 9 || revision[i] != 32)
            {
                validIndent = true;
            }
        }

        if (validIndent)
        {
            if (cant_indent > indent.top()) // 9 > 0
            {
                tokens.push_back(Token("INDENT", "INDENT", fila + 1, 1));
            }
            else if (cant_indent < indent.top())
            {
                tokens.push_back(Token("DEDENT", "DEDENT", fila + 1, 1));
            }

            indent.push(cant_indent); // agrego 9, 0 9 5
        }

        for (columna = 0; columna < lineas[fila].size();)
        {
            //cout << "-> " << columna;
            char caracter = getchar();//lineas[fila][columna]; // primer caracter de la linea
            int columna_encontrado; // guarda la columna en la que se encuentra al token por primera vez
            char next_caracter; // siguiente caracter
            //bool primera_tabulacion = false; // se utiliza para solo reconocer tabs al inicio de la linea

            if (caracter >= 32 && caracter <= 126) // no acepta caracteres que no esten entre 32 y 126, solo acepta a la tabulacion
            {

                //KEYWORDS
                if (isalpha(caracter) || caracter == '_')
                {
                    int columna_encontrado = columna;// columna_encontrado es donde se encontr€ el token por primera vez
                    token += caracter; // lo a“ado al token
                    next_caracter = peekchar(); // agarro el siguiente caracter
                    while (isalpha(next_caracter) || isdigit(next_caracter) || next_caracter == '_')
                    {
                        token += getchar();
                        next_caracter = peekchar();
                    }
                    // al terminar
                    if (find(keywords.begin(), keywords.end(), token) != keywords.end()) // si el token est∑ en las keywords
                    {
                        tokens.push_back(Token(token, "KEYWORD", fila + 1, columna_encontrado));
                    }
                    else // si no es keyword, es identificador
                    {
                        tokens.push_back(Token(token, "ID", fila + 1, columna_encontrado));
                    }
                    token = ""; // reseteo el token
                    primera_tabulacion = true; // aviso al tab que ya encontr€ otro caracter
                    primer_espacio = true;

                }

                // NUMEROS
                else if (isdigit(caracter))
                {
                    columna_encontrado = columna;
                    token += caracter; // se a“ade el numero al token
                    next_caracter = peekchar();
                    while (isdigit(next_caracter) || next_caracter == '.')
                    {
                        token += getchar();
                        next_caracter = peekchar();
                    }
                    if (token.size() > 1 && token[0] == '0') // si es un numero que tiene m∑s de un digito y empieza en 0, no es v∑lido
                    {
                        errores.push_back(Token(token, "ERROR", fila + 1, columna_encontrado, "NUMERO INVALIDO"));
                    }
                    else if (token.find('.') != -1) // si encuentra un punto en el numero es float
                    {
                        tokens.push_back(Token(token, "FLOAT", fila + 1, columna_encontrado));
                    }
                    else // es un numero entero normal
                    {
                        long long max_entero = stoll(token); // verifica el limite de un entero
                        if (max_entero > INT_MAX)
                        {
                            errores.push_back(Token(token, "ERROR", fila + 1, columna_encontrado, "ENTERO SOBREPASA EL LIMITE"));
                        }
                        else // acepta el entero
                        {
                            tokens.push_back(Token(token, "INTEGER", fila + 1, columna_encontrado));
                        }
                    }
                    token = ""; // reseteo el token
                    primera_tabulacion = true; // aviso al tab que ya encontr€ otro caracter
                    primer_espacio = true;
                }

                //OPERADORES
                else if (find(operadores.begin(), operadores.end(), string(1, caracter)) != operadores.end()) // si el caracter es un operador
                {

                    columna_encontrado = columna;
                    token += caracter;
                    next_caracter = peekchar(); // evaluo el siguiente caracter
                    if ((caracter == '+' && next_caracter == '+') || (caracter == '-' && next_caracter == '-') || (caracter == '-' && next_caracter == '>') || (caracter == '=' && next_caracter == '=') || (caracter == '!' && next_caracter == '=')
                        || (caracter == '<' && next_caracter == '=') || (caracter == '>' && next_caracter == '=') || (caracter == '/' && next_caracter == '/'))
                    {
                        token += getchar();
                    }
                    tokens.push_back(Token(token, "OP", fila + 1, columna_encontrado)); // a“ade operador
                    token = "";
                    primera_tabulacion = true; // aviso al tab que ya encontr€ otro caracter
                    primer_espacio = true;

                }

                // DELIMITADORES
                else if (find(delimitadores.begin(), delimitadores.end(), string(1, caracter)) != delimitadores.end())
                {
                    columna_encontrado = columna;
                    token += caracter;
                    if (caracter == '(') tokens.push_back(Token(token, "OPEN_PAR", fila + 1, columna_encontrado));
                    if (caracter == ')') tokens.push_back(Token(token, "CLO_PAR", fila + 1, columna_encontrado));
                    if (caracter == '[') tokens.push_back(Token(token, "OPEN_COR", fila + 1, columna_encontrado));
                    if (caracter == ']') tokens.push_back(Token(token, "CLO_COR", fila + 1, columna_encontrado));
                    token = "";
                    primera_tabulacion = true; // aviso al tab que ya encontr€ otro caracter
                    primer_espacio = true;
                }

                // STRINGS
                else if (caracter == '"')
                {
                    token += caracter;
                    next_caracter = peekchar();
                    columna_encontrado = columna;
                    string error_string = "";
                    bool error = false; // indicador si hubo error o no
                    bool segunda_comilla = false; // verifica si la cadena se cierra luego de encontrar \"
                    while (next_caracter != '"' && columna < lineas[fila].size())
                    {

                        token += getchar();
                        next_caracter = peekchar();
                    }

                    ////
                    // caso \"
                    int pos = lineas[fila].find('\\');
                    if (pos != -1) // sÃ encontr€ un \\a
                    {
                        if (lineas[fila][pos + 1] != '"') // error
                        {
                            // asigno el valor invalido a error_string
                            error_string = token.substr(pos, token.size());
                            errores.push_back(Token(error_string, "ERROR", fila + 1, columna, "STRING INVALIDO"));
                            error = true;
                        }
                        else // si existe un \", verificar que hay una comilla que cierra
                        {// seguimos recorriendo hasta que encuentre otra comilla
                            while (next_caracter != '"' && columna < lineas[fila].size())
                            {

                                token += getchar();
                                next_caracter = peekchar();
                                if (next_caracter == '"')
                                {
                                    segunda_comilla = true;// encontr€ comilla de cierre
                                    error = false;
                                }
                            }
                        }
                    }
                    token += getchar();
                    if (!error) // si no hubo error, y si se cierra correctamente, entonces sÃ se debe crear el token
                    {
                        tokens.push_back(Token(token, "STRING", fila + 1, columna_encontrado));
                    }
                    ///
                    //token += getchar();
                    token = "";
                    primera_tabulacion = true; // aviso al tab que ya encontr€ otro caracter
                    primer_espacio = true;
                }

                else
                {
                    //cout << "HOLA-----------------------------------------------";
                    //cout << "gaaaaaaaaaaaaa
                    if (caracter != 32 && caracter != 9)
                        errores.push_back(Token(string(1, caracter), "ERROR", fila + 1, columna, "TOKEN NO RECONOCIDO"));
                    //columna++; // avanza columna, no estoy seguro
                    //cout << "HOLA----------------------------------------------->              "<<columna;
                }



                //cout << caracter;
                if (columna == lineas[fila].size()) //cout << "salto";
                {
                    tokens.push_back(Token("", "NEWLINE", fila + 1, columna + 1)); // REVISAR COLUMNA QUE SE IMPRIME EN TOKEN, AUMENTAR +1
                }
            }
            else
            {
                if(caracter != 9)
                    errores.push_back(Token(string(1, caracter), "ERROR", fila + 1, columna, "CARACTER INVALIDO"));
            }




        }cout << "]" << endl;

    }
}

void Scanner::leerArchivo() {

    ifstream archivo(nombreArchivo);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {

        int cant_espacios = 0; // cantidad de espacios se comparar∑ con el size para saber si todos son espacios
        int cant_tabs = 0; // cantidad de tabs
        int comentario = linea.find('#'); // busco un comentario
        int suma_chars = 0; // variable de prueba
        if (comentario != -1)// si hay un comentario, reduzco la linea hasta antes de el comentario
        {
            linea = linea.substr(0, comentario);
        }
        string::iterator it;
        for (it = linea.begin(); it != linea.end(); ++it) //recorro la linea sumando sus caracteres, para saber si solo tiene espacios en blanco
        {
            suma_chars += (*it);
            if (*it == ' ')
                cant_espacios++;
            if (*it == 9)
                cant_tabs++;
        }
        cout << "SUMACHARS: " << suma_chars << endl;
        // newline
        if (cant_espacios == linea.size()) // si es igual hay puro espacio y se reduce la linea
        {
            linea = "";
        }
        if (cant_tabs == linea.size()) // si es igual hay puro espacio y se reduce la linea
        {
            linea = "";
        }
        //cout << cant_espacios<<endl;
        lineas.push_back(linea);
    }

    archivo.close();
}

void Scanner::printLinea()
{
    for (int i = 0; i < lineas.size(); i++)
    {
        cout << "[";
        for (int j = 0; j < lineas[i].size(); j++)
        {
            //if (lineas[i][j] == '\n') cout << "ENDL";
            cout << lineas[i][j];

        }cout << "]" << lineas[i].size() << endl;
    }
}

void Scanner::imprimirTokens()
{
    vector<Token>::iterator it;
    for (it = tokens.begin(); it < tokens.end(); it++)
    {
        cout << "DEBUG SCAN - " << (*it).tipo << "  [" << (*it).valor << "] found at (" << (*it).fila << ":" << (*it).columna << ")" << endl;
    }
}

void Scanner::imprimirErrores()
{
    cout << "------------------------------------------" << endl;
    vector<Token>::iterator it;
    cout << errores.size();
    for (it = errores.begin(); it < errores.end(); it++)
    {
        cout << "INFO SCAN - " << (*it).tipo << "  [" << (*it).valor << "] found at (" << (*it).fila << ":" << (*it).columna << ") " << (*it).error << endl;
    }
}
vector<pair<string, string>> Realizar()
{
    Scanner scan("/Users/lucho/Desktop/CursosU_2023-1/Compiladores/CasosPrueba/testFinal.txt");
    scan.leerArchivo();
    scan.escaner();
    scan.imprimirTokens();
    scan.imprimirErrores();
    vector<pair<string, string>> v;
    v = scan.GetTokens();
    //v.erase(v.end()-1);
    if(v.size() != 0)
    {
        //v.push_back(make_pair("DEDENT","DEDENT"));
    }
    v.push_back(make_pair("$", "$"));
    return v;
}
#endif /* Scanner2_h */
