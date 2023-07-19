//
//  Scanner.h
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#ifndef Scanner_h
#define Scanner_h

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;
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
    Token()
    {
        valor = " ";
        tipo = " ";
        error = " ";
        fila = 0;
        columna = 0;
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
    vector<Token> GetTokens();
    //void verificarEspacios(); // se utiliza para evitar colocar newline luego de que ocurra un error y no se detecte un token. PREGUNTAR, creo que esta bien asi
};
vector<Token> Realizar();
#endif /* Scanner_h */