#include "Line_em_up.h"
#include "globals.h"

Line_em_up::Line_em_up( HINSTANCE h_instance , UINT window_width , UINT window_height ) 
: DX11( h_instance , window_width , window_height )
{
	camera.set_projection( Projection::orthographic );
	camera.set_position( XMVectorSet( ::origin_x , ::origin_y , -100.0f , 0.0f ) );

	playfield.load_diffuse( L"graphics/grid.bmp" );
	playfield.set_position( XMFLOAT3( origin_x , origin_y , ::layer_playfield ) );

	//top, left, right, bottom
	//rectangle playfield_boarder( 250 , -125 , 125 , -250 );
	
	Tetri_centre centre_J( 2, 0, 1 );

	tetrimino_J.create( block_offsets_J , centre_J , L"graphics/blue.png" , & playfield );

	timer.start();
	timer.tick();
}


void Line_em_up::reset()
{
	//camera.set_position( XMVectorSet( 0.0f , 0.0f , -100.0f , 0.0f ) );			
	//playfield.set_position( XMFLOAT3( 0.0f , 0.0f , graphic_layers.playfield ) );

	// vector< Tetrimino > tetriminos_static
	// drop_velocity = pixel / sec
	// velocity_rotation
	// velocity_fall
	// velocity_hard_drop
}

void Line_em_up::update( const long double time_delta )
{
	timer.tick();

	wstring title = L"Line'em'up";

	title.append( L" - " );
	title.append( std::to_wstring( timer.get_total() ) );

	SetWindowTextW( get_window_handle() , title.data() );

	Keyboard::State kb = keyboard->GetState();

	// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	// VK_A - VK_Z are the same as ASCII 'A' - 'Z' ( 0x41 - 0x5A )			

	if( kb.Escape || kb.Q )
		PostQuitMessage( 0 );

	//auto mouse = m_mouse->GetState();
	if( kb.Space )
	{
		//reset();
		// hard_drop
	}

	if( kb.Z )
	{
		tetrimino_J.try_rotate( Rotation::counter_clock_wise );	// rotation speed
	}

	if( kb.Down || kb.S )
		tetrimino_J.try_move( Direction::down ); // fast_drop

	if( kb.Left || kb.A )
		tetrimino_J.try_move( Direction::left );
	// events.add( Direction::left );

	if( kb.Right || kb.D )
	{
		tetrimino_J.try_move( Direction::right );
	}

	//if( kb.PageUp || kb.Space )
	//if( kb.PageDown || kb.X )

	camera.update(); //timer.get_delta()

	playfield.update( timer.get_delta() );

	tetrimino_J.update( timer.get_delta() );
}

void Line_em_up::render()
{
	clear( XMFLOAT4( 0.20f , 0.38f , 0.64f , 1.0f ) ); // clear backbuffer

	playfield.render();

	tetrimino_J.render();

	present();
}