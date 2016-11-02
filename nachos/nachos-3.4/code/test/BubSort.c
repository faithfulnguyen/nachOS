#include"syscall.h"

void main()
{
	int n = 0;
	int Arr[100];
	int a = 0, b = 0, c = 0, d = 0;
	int tmp = 0;
	n = ReadInt(); // doc vao kich thuoc mang
	for ( ; a < n; a++ )
		Arr[ a ] = ReadInt( );
// bat dau sap xep
// sap xep tang dan
	for ( ; b < n - 1; b++ )
	{
	    for ( c = n - 1; c > b; c-- )
	    {
		if ( Arr[c] < Arr[c - 1] )
		{
			tmp = Arr[c];
			Arr[c] = Arr[c - 1];
			Arr[c- 1] = tmp;
		} 
	    }
	}
	
	for ( ; d < n; d++ )
		PrintInt( Arr[d] );

}

