#include "line_em_up.h"

Line_em_up::Line_em_up( const HWND in_window , const unsigned int in_resolution_width , const unsigned int in_resolution_height )
	: Game( in_window , in_resolution_width , in_resolution_height )
{
	camera.set_projection( Projection::orthographic );
	camera.set_position( XMVectorSet( ::origin_x , ::origin_y , -100.0f , 0.0f ) ); // vector w = 1, point w = 0

	playfield.load_diffuse( L"graphics/grid.bmp" );
	playfield.set_position( XMFLOAT3( ::origin_x , ::origin_y , ::layer_playfield ) );

	tetrimino_J.create( block_offsets_J , Tetri_centre(2,0,1) , L"graphics/blue.png" , & playfield );

	tetrimino_types.emplace_back( tetrimino_J );

	timer.start();
	timer.tick();

	input_handler.add_command( DirectX::Keyboard::Keys::Z ,		new Rotate( Rotation::clock_wise ) );
	input_handler.add_command( DirectX::Keyboard::Keys::X ,		new Rotate( Rotation::counter_clock_wise ) );
	input_handler.add_command( DirectX::Keyboard::Keys::Left ,	new Move( Direction::left ) );
	input_handler.add_command( DirectX::Keyboard::Keys::Right ,	new Move( Direction::right ) );
	input_handler.add_command( DirectX::Keyboard::Keys::Down ,	new Move( Direction::down ) );
}

void Line_em_up::reset()
{
	//camera.set_position( XMVectorSet( 0.0f , 0.0f , -100.0f , 0.0f ) );			
	//playfield.set_position( XMFLOAT3( 0.0f , 0.0f , ::layer_playfield ) );

	// vector< Tetrimino > tetriminos_static
	// drop_velocity = pixel / sec
	// velocity_rotation
	// velocity_fall
	// velocity_hard_drop
}

void Line_em_up::update( )
{
	timer.tick();

	wstring title = L"Line'em'up";

	title.append( L" - " );
	title.append( std::to_wstring( timer.get_total() ) );

	SetWindowTextW( window , title.data() );
	
	camera.update(); //timer.get_delta()

	playfield.update( timer.get_delta() );

	tetrimino_J.update( timer.get_delta() );
}

void Line_em_up::render()
{
	//DirectX::Colors::DarkBlue
	graphics.clear_render_target( DirectX::XMVECTORF32{ 0.20f , 0.38f , 0.64f , 1.0f } ); // clear backbuffer

	playfield.render();

	tetrimino_J.render();

	graphics.present();
}

void Line_em_up::input( const int in_key )
{
	command = input_handler.handle_input( in_key );

	if( command ) command->execute( tetrimino_J );
}

//DirectX::Keyboard::State kb = keyboard.GetState();


// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
// VK_A - VK_Z are the same as ASCII 'A' - 'Z' ( 0x41 - 0x5A )			

/*
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
*/
//if( kb.PageUp || kb.Space )
//if( kb.PageDown || kb.X )