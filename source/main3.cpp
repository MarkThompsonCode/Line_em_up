#include "window.h"
#include "game.h"
#include "line_em_up.h"

WPARAM message_loop( Game & in_game );

int WINAPI wWinMain( HINSTANCE h_instance , HINSTANCE prev_instance , PWSTR command_line , int window_display_options )
{
	UNREFERENCED_PARAMETER( prev_instance );
	UNREFERENCED_PARAMETER( command_line );  // [ /editor ]
	UNREFERENCED_PARAMETER( window_display_options );
		
	HWND window_handle = initialise( h_instance , 600u , 800u );

	Line_em_up game( window_handle , 600u , 800u );

	SetWindowLongPtr( window_handle , GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&game) ); 
	// to pass pointer to window_procedure() as that function must conform to what classex.lpfnWndProc{ window_procedure } expects

	message_loop( game );
		
	return 0;
}

WPARAM message_loop( Game & in_game )
{
	MSG win_msg{};

	while( win_msg.message != WM_QUIT )
	{
		if( PeekMessage( &win_msg , nullptr , 0 , 0 , PM_REMOVE ) )
		{
			TranslateMessage( &win_msg );	// receive character input from the keyboard.
			DispatchMessage( &win_msg );
		}
		else
		{
			in_game.update();
			in_game.render();
			// add_event(game_update);
		}
	}
	
	return win_msg.wParam;
}