//#define _USE_MATH_DEFINES

#include <map>
#include <list>

#include "DX11.h"
#include "Camera.h"
#include "Quad.h"

using std::unique_ptr;
using std::make_pair;
using std::wstring;
using std::map;
using std::vector;
using std::list;

// namespace Line_em_up {}

enum class direction { left , right , down };
enum class rotation { clock_wise , counter_clock_wise };

//typedef struct Graphic_layers
//{
//	const float playfeild	= 0.0f;
//	const float block		= -1.0f;
//} graphic_layers;

class Graphic_layers
{
	public:
		const float playfield	= 0.0f;
		const float block		= -1.0f;
};

Graphic_layers graphic_layers;

class Tetrimino_state
{
	public:
		virtual void input() {};
		virtual void update() {};
};

class Tetrimino
{
	private:

		vector< Quad >				blocks;
		XMFLOAT3					position {};  //3
		XMFLOAT3					position_initial {}; // 3
		XMFLOAT3					centre {};
		vector< XMFLOAT2 >			block_offsets;
		double						drop_velocity {};
		//Grid_size					grid_size {};
		enum class					state { moving , falling , sliding , locked , rotating }; 
		state						state_current{ state::falling };
		map< wstring , XMINT2 >		translate;
		rectangle					border {};
		rectangle					playfield {};
		map< direction , XMFLOAT2 > directions;
		map< rotation , float >		rotations;

	public:		
		
		Tetrimino() {}

		void create( const vector< XMFLOAT2 >	block_positions_relative ,
					 const wstring				texture_diffuse ,
					 rectangle					in_playfield )
		{			
			block_offsets	= block_positions_relative;
			playfield		= in_playfield;

			directions.insert( make_pair( direction::left ,  XMFLOAT2( -1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::down ,  XMFLOAT2(  0.0f , -1.0f ) ) );

			rotations.insert( make_pair( rotation::clock_wise , 1.570796326794f ) );// M_PI_2 ) );
			rotations.insert( make_pair( rotation::counter_clock_wise , -1.570796326794f ) );// M_PI_2 ) );

			add_blocks( texture_diffuse );
		}

		void add_blocks( wstring texture_diffuse )
		{

			for( auto & offset : block_offsets )
			{
				Quad new_block( texture_diffuse );

				// 0 (screen center) - 1/2 block width
				position_initial.x = -0.5 * new_block.get_width(); 
				position_initial.y = playfield.top - ( 0.5f * new_block.get_height() );

				// each block + offset
				float x = position_initial.x + ( offset.x * new_block.get_width() );
				float y = position_initial.y + ( offset.y * new_block.get_height() );

				//float x = offset.x * new_block.get_width();
				//float y = offset.y * new_block.get_height();

				new_block.set_position( XMFLOAT3( x, y, graphic_layers.block) );

				blocks.push_back( new_block );

				update_centre();

				//XMVECTOR position = XMVectorSet( x , y , graphic_layers.block, 0.0f );				
			}
		}

		void update_centre()
		{
			centre.x = 0.0f;
			centre.y = 0.0f;

			for( auto & block : blocks )
			{
				centre.x += block.get_position().x;
				centre.y += block.get_position().y;
			}

			centre.x /= 4.0f;
			centre.y /= 4.0f;
		}

		void update() // const long double
		{
			// if ( position => m_playfield->boarder().bottom ) m_current_state = state::locked    // && slide_time_elapsed <= 0.0f;

			for( auto & block : blocks ) { block.update(); }

			// if ( falling )
			//   move( 0, -y = fall_rate * delta_time ( = 10 pixels per second = ? locked at 60fps )
		}

		void render()
		{
			for( auto & block : blocks ) { block.render(); }
		}

		void move( const direction in_direction )
		{
			if( within_playfield( in_direction ) ) //&& ! block_obstructing )
			{
				for( auto & block : blocks )
				{
					float current_x = block.get_position().x;
					float current_y = block.get_position().y;

					float translate_x = directions.at( in_direction ).x * block.get_width();
					float translate_y = directions.at( in_direction ).y * block.get_height(); // * delta_time * fall_rate

					XMFLOAT3 translate = XMFLOAT3( current_x + translate_x , current_y + translate_y , graphic_layers.block );

					block.set_position( translate );

					centre.x += translate.x;
					centre.y += translate.y;
				}

				update_centre();
			}
		}

		//bool can_move( const string in_direction ) { }
		//bool can_fit

		bool within_playfield( const direction in_direction )
		{
			XMFLOAT2 test_move {};

			test_move.x = directions.at( in_direction ).x * blocks.front().get_width();
			test_move.y = directions.at( in_direction ).y * blocks.front().get_height();

			border = get_border();

			if ( ( border.left   + test_move.x ) < playfield.left  ||
				 ( border.right  + test_move.x ) > playfield.right ||
				 ( border.bottom + test_move.y ) < playfield.bottom )
			{
				return false; // block would be outside playfield bounds
			}
			return true;
		}

		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction){}

		rectangle get_border()
		{
			// for each vertex in apply SRT matrix multiplication
			// find min and max vertices

			rectangle tetri_border = blocks.front().get_border();

			for( auto & block : blocks )
			{
				rectangle block_boarder = block.get_border();

				if( block_boarder.left < tetri_border.left )
					tetri_border.left = block_boarder.left;

				if( block_boarder.right > tetri_border.right )
					tetri_border.right = block_boarder.right;

				if( block_boarder.top > tetri_border.top )
					tetri_border.top = block_boarder.top;

				if( block_boarder.bottom < tetri_border.bottom )
					tetri_border.bottom = block_boarder.bottom;
			}
			
			return tetri_border;
		}

		//bool can_rotate( const rotation in_rotation ) {}

		void rotate( const rotation in_rotation )
		{
			//if( can_rotate )
						
			rectangle border = get_border();

			centre.x = border.left + abs( ( border.right - border.left ) / 2.0 );
			centre.y = border.top + abs( ( border.bottom - border.top ) / 2.0 );
			centre.z = graphic_layers.block;

			for( auto & block : blocks )
			{
				block.rotate_increment_z( 0.02f );// rotations.at( in_rotation ) );
				block.rotate_point_increment_z( centre , 0.02f );// rotations.at( in_rotation ) );
			}
		}
};

class Line_em_up : public DX11
{
	private:

		//InputHandler input_handler;

		Camera					camera;
		Quad					playfield;

		double					drop_velocity { -1.0 }; // playfield square per second

		vector< Tetrimino >		tetrimino_types;

		//list< Tetrimino >		tetrimino_bucket;

		Tetrimino				tetrimino_J;
		vector< XMFLOAT2 >		block_offsets_J{ XMFLOAT2( -1,  0 ) , XMFLOAT2( -1, -1 ) , XMFLOAT2( 0, -1 ) , XMFLOAT2( 1, -1 ) };

		//vector< Tetrimino >::const_iterator active_tetrimino;

		// stage/difficulty -> pixel per second
		map< int , float >		level_fall_rates{ { 1 , 1.0f } };
	
	public:

		Line_em_up( HINSTANCE h_instance , UINT window_width , UINT window_height ) : DX11( h_instance , window_width , window_height )
		{
			camera.set_projection( Projection::orthographic );
			camera.set_position( XMVectorSet( 0.0f , 0.0f , -100.0f , 0.0f ) );
						
			playfield.load_diffuse( L"graphics/grid.bmp" );
			playfield.set_position( XMFLOAT3( 0.0f , 0.0f , graphic_layers.playfield ) );
			
			tetrimino_J.create( block_offsets_J , L"graphics/blue.png", playfield.get_border() );

			// vector< Tetrimino > tetriminos_static
			// drop_velocity = pixel / sec
			// velocity_rotation
			// velocity_fall
			// velocity_hard_drop
		}

		void update( const long double time_delta )
		{
			Keyboard::State kb = keyboard->GetState();

			//input_handler.handle_input();
			
				if( kb.Escape )
					PostQuitMessage( 0 );

				//auto mouse = m_mouse->GetState();
				if( kb.Space )
				{
					reset();
				}

				if( kb.Z )
				{
					tetrimino_J.rotate( rotation::counter_clock_wise );

					// rotation speed
				}

				if( kb.Down || kb.S ) tetrimino_J.move( direction::down );

				if( kb.Left || kb.A )
					tetrimino_J.move( direction::left );
					// events.add( KEY_LEFT, param1, param2 );

				if( kb.Right || kb.D )
				{
					tetrimino_J.move( direction::right );
				}

				//if( kb.PageUp || kb.Space )
				//if( kb.PageDown || kb.X )
			
			
			playfield.update();

			tetrimino_J.update();
		}

		void render()
		{
			clear( XMFLOAT4( 0.20f, 0.38f, 0.64f, 1.0f ) );
			
			camera.update();

			playfield.render();

			tetrimino_J.render();
						
			present();
		}

};

int WINAPI wWinMain( HINSTANCE h_instance , HINSTANCE prev_instance , LPWSTR command_line , int window_display_options )
{
	UNREFERENCED_PARAMETER( prev_instance );
	UNREFERENCED_PARAMETER( command_line );  // [ /editor ]
	UNREFERENCED_PARAMETER( window_display_options );

	Line_em_up game( h_instance , 600u , 800u );
	game.message_loop();

	return 0;// win_message;
}