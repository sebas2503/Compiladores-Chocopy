//
//  main.cpp
//  Parser
//
//  Created by Lucho on 5/15/23.
//

#include "Parser.h"
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
