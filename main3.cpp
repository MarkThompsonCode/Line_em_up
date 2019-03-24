//#define _USE_MATH_DEFINES

#include <map>
#include <list>

#include "DX11.h"
#include "Camera.h"
#include "Quad.h"
//#include "Command.h"
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
	Tetri_centre( uint in_block, uint in_vertex0 , uint in_vertex1 )
		: block(in_block), vertex0( in_vertex0 ), vertex1( in_vertex1 )
	{}

	Tetri_centre() {};

	uint block	= 0u;
	uint vertex0 = 0u;
	uint vertex1 = 1u;

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

//class Game_actor
//{
//	public:
//
//		virtual void rotate( Rotation in_rotation ) = 0;
//};
class State;
class Falling;

class Tetrimino //: public State< Tetrimino > //: public Game_actor
{
	public:		
		
		Tetrimino() {}

		void create( const vector< XMFLOAT2 >	block_positions_relative ,
					 Tetri_centre				in_centre,
					 const wstring				texture_diffuse ,
					 rectangle					in_playfield )  // const * playfield
		{			
			centre			= in_centre;
			block_offsets	= block_positions_relative;
			playfield		= in_playfield;

			directions.insert( make_pair( Direction::left ,  XMFLOAT2( -1.0f ,  0.0f ) ) );
			directions.insert( make_pair( Direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
			directions.insert( make_pair( Direction::down ,  XMFLOAT2(  0.0f , -1.0f ) ) );

			rotations.insert( make_pair( Rotation::clock_wise , 1.570796326794f ) );// M_PI_2 ) );
			rotations.insert( make_pair( Rotation::counter_clock_wise , -1.570796326794f ) );// M_PI_2 ) );

			add_blocks( texture_diffuse );

			//set_position();

			state = new Falling();
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

				// each block + offset
				float x = position_initial.x + ( offset.x * new_block.get_width() );// -½width );
				float y = position_initial.y + ( offset.y * new_block.get_height() );// -½height );

				new_block.set_position( XMFLOAT3( x, y, graphic_layers.block) );

				blocks.push_back( new_block );
			}
		}

		XMFLOAT3 get_centre()
		{
			// centre = ( vertex0 + vertex1 ) / 2

			const Quad & block = blocks.at( centre.block );

			vector<vertex_rgba_uv> world_vertices = block.get_world_vertices();

			XMFLOAT3 point;

			point.x = ( world_vertices.at( centre.vertex0 ).point.x + world_vertices.at( centre.vertex1 ).point.x ) / 2.0f;
			point.y = ( world_vertices.at( centre.vertex0 ).point.y + world_vertices.at( centre.vertex1 ).point.y ) / 2.0f;
			point.z = graphic_layers.block;
						
			return point;

			//x = cx + r * cos( a )
			//y = cy + r * sin( a )
			//float θz = atan2( r21 , r11 );
		}
		
		void update( double time_delta ) 
		{
			//State * new_state = state->update( *this , time_delta );
			state->update( * this , time_delta );


			// if ( position => m_playfield->boarder().bottom ) m_current_state = state::locked    // && slide_time_elapsed <= 0.0f;

			//for( auto & block : blocks ) { block.update( time_delta ); }

			//switch( state_current )
			//{
			//	case states::falling :

			//		break;
			//}

			float fall_rate = 25 * time_delta;

			//move( Direction::down )
			//   move( 0, -y = fall_rate * time_delta ( = 10 pixels per second = ? locked at 60fps )
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
					block.translate_x( directions.at( in_direction ).x );// *block.get_width() );
					block.translate_y( directions.at( in_direction ).y );// *block.get_height() ); // * delta_time * fall_rate
				}
			}
		}

		//bool can_move( const string in_direction ) { }
		//bool can_fit

		bool within_playfield( const Direction in_direction )
		{
			XMFLOAT2 test_move(0,0);
			rectangle border; // playfield->get_boarder();
			
			test_move.x = directions.at( in_direction ).x;// * blocks.front().get_width();
			test_move.y = directions.at( in_direction ).y;// * blocks.front().get_height();

			for( const auto & block : blocks )
			{
				vector<vertex_rgba_uv> verticies = block.get_world_vertices();

				for( const auto & vertex : verticies )
				{
					if( ( vertex.point.x + test_move.x ) < playfield.left ||
						( vertex.point.x + test_move.x ) > playfield.right ||
						( vertex.point.y + test_move.y ) < playfield.bottom )
					{
						return false; // block would be outside playfield bounds
					}
				}
			}

			return true;
		}

		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction){}
		
		bool can_rotate( const Rotation in_rotation )
		{
			// test quad

			// for each block 
				// for 
			return true;
		}

		void rotate( Rotation in_rotation )
		{
			if( can_rotate( in_rotation ) )
			{
				XMFLOAT3 centre = get_centre();

				for( auto & block : blocks )
				{
					block.rotate_point_z( centre , 0.2f );// rotations.at( in_rotation ) );
				}
			}

			// debug_out( "\ncentre.x = %.2f , y = %.2f" , centre.x , centre.y );
			// L"centre.x = " += to_wstring( centre.x )
		}

		private:

			vector< Quad >				blocks;
			//XMFLOAT3					position{};
			//XMFLOAT3					position_initial{};
			Tetri_centre				centre;
			vector< XMFLOAT2 >			block_offsets;
			double						drop_velocity{};
			//Grid_size					grid_size {};
			enum class					states { next , falling , fast_drop, hard_drop , moving , rotating , sliding , locked , };  // row_made , delete_row_blocks
			//states					state_current{ states::next };
			State * state;

			map< wstring , XMINT2 >		translate;
			//rectangle					border{};
			rectangle					playfield{};

			map< Direction , XMFLOAT2 >	directions;
			map< Rotation , float >		rotations;
};



//template < typename type >
class State //abstract
{
	public:
		virtual State * update( Tetrimino & actor , double time_delta ) {};
};

class Falling : public State//< Tetrimino >
{
	public:
		virtual State * update( Tetrimino & actor , double time_delta )
		{
			actor.move( Direction::down );
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

//template< typename type >
class Command
{
	public:

	virtual void execute( Tetrimino & actor ) = 0;
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

	Command * handle_input( const uchar in_char )
	{
		const auto result = command_map.find( in_char );

		if( result != command_map.end() ) return result->second;
		else return nullptr;
	}

	// add_command()

	private:

	map< uchar , Command * > command_map;
};

//Bounding_box get_bounding_box()
//{
//	Bounding_box box;
//	return box;
//}
//
//Bounding_box get_bounding_oriented_box()
//{
//	Bounding_box box = get_bounding_box();
//
//	box.min.x = blocks.front().get_bounding_box().min.x;
//	box.min.y = blocks.front().get_bounding_box().min.y;
//	box.min.z = blocks.front().get_bounding_box().min.z;
//
//	//for( auto & block : blocks )
//	//{
//
//	//	if( block.point.x < box.min.x ) box.min.x = vertex.point.x;
//	//	if( vertex.point.x > box.max.x ) box.max.x = vertex.point.x;
//
//	//	if( vertex.point.y < box.min.y ) box.min.y = vertex.point.y;
//	//	if( vertex.point.y > box.max.y ) box.max.y = vertex.point.y;
//
//	//	if( vertex.point.z < box.min.z ) box.min.z = vertex.point.z;
//	//	if( vertex.point.z > box.max.z ) box.max.z = vertex.point.z;
//	//}
//
//	return box;
//	// for each vertex in apply SRT matrix multiplication
//	// find min and max vertices
//
//	/*	rectangle tetri_border = blocks.front().get_border();
//
//	for( auto & block : blocks )
//	{
//	rectangle block_boarder = block.get_border();
//
//	if( block_boarder.left < tetri_border.left )
//	tetri_border.left = block_boarder.left;
//
//	if( block_boarder.right > tetri_border.right )
//	tetri_border.right = block_boarder.right;
//
//	if( block_boarder.top > tetri_border.top )
//	tetri_border.top = block_boarder.top;
//
//	if( block_boarder.bottom < tetri_border.bottom )
//	tetri_border.bottom = block_boarder.bottom;
//	}
//
//	return tetri_border;*/
//	//rectangle temp_boarder{};
//
//	//return temp_boarder;
//}