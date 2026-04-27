#include "head.h"
double alpha = 0.3;
double beta  = 0.5;
double mu    = 0.1;
int main() 
{
    srand(time(NULL));
    int SEED = (int)rand();
    ini_ran(SEED);
    int nodos = 100;
    float probabilidad = 0.2;
    float p_Hueco, p_Vampiro;
    p_Hueco = 0.5;
    p_Vampiro = 0.05;

    //generarErdosRenyi(nodos, probabilidad);
    generaRedInicial(nodos,p_Hueco,p_Vampiro);
    return 0;
}