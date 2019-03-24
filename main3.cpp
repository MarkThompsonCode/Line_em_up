#include "Line_em_up.h"

int WINAPI wWinMain( HINSTANCE h_instance , HINSTANCE prev_instance , LPWSTR command_line , int window_display_options )
{
	UNREFERENCED_PARAMETER( prev_instance );
	UNREFERENCED_PARAMETER( command_line );  // [ /editor ]
	UNREFERENCED_PARAMETER( window_display_options );

	Line_em_up game( h_instance , 600u , 800u );
	game.message_loop();

	return 0;// win_message;
}