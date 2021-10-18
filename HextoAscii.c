#include "LIBRERIA.H"

int HextoInt (char a)
{
   int aux = 0;
   char buf [4];

      buf [0] = ((a % 10));
	  a /= 10;				   
	  buf [1] = ((a % 10)*10);
	  a /= 10;				
	  buf [2] = (a*100);	
	  buf [3] = buf [0] + buf [1] + buf [2]; 
	  aux = buf [3];

	 return aux;

}