#include "head.h"
int main() 
{
    ini_ran(12345);
    int nodos = 20;
    float probabilidad = 0.2;
    generarErdosRenyi(nodos, probabilidad);

    return 0;
}