//#define _USE_MATH_DEFINES

#include <map>
#include <list>

#include "DX11.h"
#include "Camera.h"
#include "Quad.h"
#include "Command.h"
#include "DX11_math.h"

using std::unique_ptr;
using std::make_pair;
using std::wstring;
using std::map;
using std::vector;
using std::list;

using DirectX::XMINT2;

// namespace Line_em_up {}

enum class Direction { left , right , down };
enum class Rotation { clock_wise , counter_clock_wise };

//typedef struct Graphic_layers
//{
//	const float playfeild	= 0.0f;
//	const float block		= -1.0f;
//} graphic_layers;

const float origin_x = 0.0f;
const float origin_y = 0.0f;

class Graphic_layers
{
	public:
		const float playfield	= 0.0f;
		const float block		= -1.0f;
};

Graphic_layers graphic_layers;

typedef struct Tetri_centre
{
	uint block		= 0u;
	float offset_x	= 0.0f;;
	float offset_y	= 0.0f;;
	float offset_z	= 0.0f;
} Tetri_centre;

//class Tetrimino_state
//{
//	public:
//
//		virtual void input() {};
//		virtual void update() {};
//};

/*
class Tetrimino;

class Rotate : public Command< Tetrimino >
{
	public:

		Rotate( const Rotation in_rotation ) : rotation_direction( in_rotation )
		{}

		virtual void execute( Tetrimino & in_tetrimino ) override
		{
			in_tetrimino.rotate( rotation_direction );
		}

	private:

		Rotation rotation_direction;
};

class Hard_drop : public Command< Tetrimino >
{
	public:

	virtual void execute( Tetrimino & in_tetrimino ) override
	{
		//cout << "\nDrop";
	}
};
*/


class Game_actor
{
	public:

		virtual void rotate( Rotation in_rotation ) = 0;
};

class Tetrimino : public Game_actor
{
	public:		
		
		Tetrimino() {}

		void create( const vector< XMFLOAT2 >	block_positions_relative ,
					 const wstring				texture_diffuse ,
					 rectangle					in_playfield )  // const * playfield
		{			
			block_offsets	= block_positions_relative;
			playfield		= in_playfield;

			directions.insert( make_pair( Direction::left ,  XMFLOAT2( -1.0f ,  0.0f ) ) );
			directions.insert( make_pair( Direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
			directions.insert( make_pair( Direction::down ,  XMFLOAT2(  0.0f , -1.0f ) ) );

			rotations.insert( make_pair( Rotation::clock_wise , 1.570796326794f ) );// M_PI_2 ) );
			rotations.insert( make_pair( Rotation::counter_clock_wise , -1.570796326794f ) );// M_PI_2 ) );

			add_blocks( texture_diffuse );
		}

		void add_blocks( wstring texture_diffuse )
		{
			for( auto & offset : block_offsets )
			{
				Quad new_block( texture_diffuse );

				XMFLOAT2 position_initial;
				// 0 (screen center) - 1/2 block width
				position_initial.x = 0.0f;// -0.5 * new_block.get_width();
				position_initial.y = 0.0f;//playfield.top - ( 0.5f * new_block.get_height() );
				
				//float ½width  = new_block.get_width()  / 2.0f;
				//float ½height = new_block.get_height() / 2.0f;				

				// each block + offset
				float x = position_initial.x + ( offset.x * new_block.get_width() );// -½width );
				float y = position_initial.y + ( offset.y * new_block.get_height() );// -½height );

				//float x = offset.x * new_block.get_width();
				//float y = offset.y * new_block.get_height();

				//debug_out( "\nposition.x = %.2f , y = %.2f" , x , y );

				new_block.set_position( XMFLOAT3( x, y, graphic_layers.block) );

				blocks.push_back( new_block );

				//XMVECTOR position = XMVectorSet( x , y , graphic_layers.block, 0.0f );				
			}
		}

		XMFLOAT3 get_centre()
		{
			//Bounding_box box = get_bounding_oriented_box();

			//float x = ( box.min.x + box.max.x ) / 2.0f;
			//float y = ( box.min.y + box.max.y ) / 2.0f;
			//float z = ( box.min.z + box.max.z ) / 2.0f;

			//return XMFLOAT3( x , y , z );

			XMFLOAT3 offset;
			const auto block = blocks.at( centre.block );

			offset.x = block.get_position().x + ( centre.offset_x * block.get_width() );
			offset.y = block.get_position().y + ( centre.offset_y * block.get_height() );
			offset.z = block.get_position().z + ( centre.offset_z * block.get_height() );


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

		void move( const Direction in_direction )
		{
			if( within_playfield( in_direction ) ) //&& ! block_obstructing )
			{
				for( auto & block : blocks )
				{
					block.translate_x( directions.at( in_direction ).x * block.get_width() );
					block.translate_y( directions.at( in_direction ).y * block.get_height() ); // * delta_time * fall_rate
				}
			}
		}

		//bool can_move( const string in_direction ) { }
		//bool can_fit

		bool within_playfield( const Direction in_direction )
		{
			XMFLOAT2 test_move {};
			rectangle border;

			//test_move.x = directions.at( in_direction ).x * blocks.front().get_width();
			//test_move.y = directions.at( in_direction ).y * blocks.front().get_height();

			//border = get_bounding_box(); // Bounding_oriented_box get_bob()

			//if ( ( border.left   + test_move.x ) < playfield.left  ||
			//	 ( border.right  + test_move.x ) > playfield.right ||
			//	 ( border.bottom + test_move.y ) < playfield.bottom )
			//{
			//	return false; // block would be outside playfield bounds
			//}

			return true;
		}

		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction){}



		Bounding_box get_bounding_box()
		{
			Bounding_box box;
		}

		Bounding_box get_bounding_oriented_box()
		{
			Bounding_box box;

			box.min.x = blocks.front().get_bounding_box().min.x;
			box.min.y = blocks.front().get_bounding_box().min.y;
			box.min.z = blocks.front().get_bounding_box().min.z;


				//for( auto & block : blocks )
				//{

				//	if( block.point.x < box.min.x ) box.min.x = vertex.point.x;
				//	if( vertex.point.x > box.max.x ) box.max.x = vertex.point.x;

				//	if( vertex.point.y < box.min.y ) box.min.y = vertex.point.y;
				//	if( vertex.point.y > box.max.y ) box.max.y = vertex.point.y;

				//	if( vertex.point.z < box.min.z ) box.min.z = vertex.point.z;
				//	if( vertex.point.z > box.max.z ) box.max.z = vertex.point.z;
				//}

				return box;
			// for each vertex in apply SRT matrix multiplication
			// find min and max vertices

		/*	rectangle tetri_border = blocks.front().get_border();

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
			
			return tetri_border;*/
			//rectangle temp_boarder{};

			//return temp_boarder;
		}

		//bool can_rotate( const rotation in_rotation ) {}

		void rotate( Rotation in_rotation )
		{
			//if( can_rotate )

			XMFLOAT3 centre = get_centre();
			debug_out( "\ncentre.x = %.2f , y = %.2f" , centre.x , centre.y );

						
			for( auto & block : blocks )
			{
				//debug_out( "\nposition.x = %.2f , y = %.2f" , position.x , position.y );
				
				//block.rotate_point_z( centre , 0.08f );// rotations.at( in_rotation ) );

			}
		}

		private:

			vector< Quad >				blocks;
			//XMFLOAT3					position{};
			//XMFLOAT3					position_initial{};
			//Tetri_centre				centre{};
			vector< XMFLOAT2 >			block_offsets;
			double						drop_velocity{};
			//Grid_size					grid_size {};
			enum class					state { moving , falling , sliding , locked , rotating }; // fast_drop, hard_drop
			state						state_current{ state::falling };
			map< wstring , XMINT2 >		translate;
			//rectangle					border{};
			rectangle					playfield{};
			map< Direction , XMFLOAT2 > directions;
			map< Rotation , float >		rotations;
};

class Input_handler
{
	public:

		Input_handler()
		{
			// Methods to bind commands...
			//command_map.insert( make_pair( Keyboard::Keys::Z , new Rotate( Rotation::clock_wise ) ) );
			//command_map.insert( make_pair( Keyboard::Keys::X , new Rotate( Rotation::counter_clock_wise ) ) );
		}

		Command< Tetrimino > * handle_input( const uchar in_char )
		{
			const auto result = command_map.find( in_char );

			if( result != command_map.end() ) return result->second;
			else return nullptr;
		}

		// add_command()

	private:

		map< uchar , Command< Tetrimino > * > command_map;
};

class Line_em_up : public DX11
{
	private:

		//Input_handler keyboard_handler;

		Camera					camera;
		Quad					playfield;

		double					drop_velocity { -1.0 }; // playfield square per second

		vector< Tetrimino >		tetrimino_types;

		//list< Tetrimino >		tetrimino_bucket;

		Tetrimino				tetrimino_J;
		vector< XMFLOAT2 >		block_offsets_J{ XMFLOAT2( -0.5f , -1.0f ) , XMFLOAT2( 0.5f , -1.0f ) , XMFLOAT2( 0.5f , 0.0f ) , XMFLOAT2( 0.5f , 1.0f ) };
		//Tetri_centre			tetrimino_J_centre { 2, -0.5, 0 };
		//vector< Tetrimino >::const_iterator active_tetrimino;

		// stage/difficulty -> pixel per second

		//Timer					timer_main;

		map< uint , float >		level_fall_rates{ { 1u , 1.0f } };		
	
	public:

		Line_em_up( HINSTANCE h_instance , UINT window_width , UINT window_height ) : DX11( h_instance , window_width , window_height )
		{
			camera.set_projection( Projection::orthographic );
			camera.set_position( XMVectorSet( origin_x , origin_y , -100.0f , 0.0f ) );

			playfield.load_diffuse( L"graphics/grid.bmp" );
			playfield.set_position( XMFLOAT3( origin_x , origin_y , graphic_layers.playfield ) );

			//top, left, right, bottom
			rectangle playfield_boarder( 250 , -125 , 125 , -250 );

			tetrimino_J.create( block_offsets_J , L"graphics/blue.png" , playfield_boarder );//playfield.get_border() );
		}

		//Input_handler::update

		void reset()
		{			
			//camera.set_position( XMVectorSet( 0.0f , 0.0f , -100.0f , 0.0f ) );			
			//playfield.set_position( XMFLOAT3( 0.0f , 0.0f , graphic_layers.playfield ) );
				
			// vector< Tetrimino > tetriminos_static
			// drop_velocity = pixel / sec
			// velocity_rotation
			// velocity_fall
			// velocity_hard_drop
		}

		void update( const long double time_delta )
		{
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
					tetrimino_J.rotate( Rotation::counter_clock_wise );

					// rotation speed
				}

				if( kb.Down || kb.S ) tetrimino_J.move( Direction::down );

				if( kb.Left || kb.A )
					tetrimino_J.move( Direction::left );
					// events.add( KEY_LEFT, param1, param2 );

				if( kb.Right || kb.D )
				{
					tetrimino_J.move( Direction::right );
				}

				//if( kb.PageUp || kb.Space )
				//if( kb.PageDown || kb.X )
			
			
			playfield.update();

			tetrimino_J.update();
		}

		void render()
		{
			clear( XMFLOAT4( 0.20f, 0.38f, 0.64f, 1.0f ) ); // clear backbuffer
			
			camera.update(); // ? required

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