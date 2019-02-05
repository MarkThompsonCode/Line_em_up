#include <map>

#include "DX11.h"
#include "Camera.h"
#include "Quad.h"
//#include "..\DX11\linked_list.h"

// sounds.add_sound( "rotate", "sounds/rotate.wav" );

//#pragma
//undef _MBCS //#define _UNICODE //#define UNICODE

//using namespace std;
using std::unique_ptr;

//enum class layer { grid, terimino, osd };
//struct layer { const float osd = 1.0f; const float tetrimino = 2.0f; const float grid = 3.0f; };

struct Grid_size
{
	uint width;
	uint height;
};

//class Tetris;

class Tetrimino
{
	private:
		void add_blocks()
		{
			for( auto & block : block_positions_relative )
			{
				blocks.push_back( Quad_from_image( image_filename.data() ) ); // rvalue move constructor
			}
		}

		vector< Quad_from_image > blocks;
		std::string				image_filename {};
		vector< XMFLOAT2 >		block_positions_relative;
		map< string , XMINT2 >	translate;
		//float					angle {};
		double					drop_rate {};
		XMFLOAT2				position_initial {};
		XMFLOAT2				position;
		Grid_size				grid_size {};
		XMINT2					block_size {};
		RECT					grid_rectangle {};
		enum class				state { falling , sliding , in_place }; // rotate and falling
		state					current_state = state::falling;

	public:
		Tetrimino() {};// : m_ptr_game( nullptr ) {}
		// copy constructor
		//Tetrimino( const Tetrimino & in_copy_from ){ int i = 0;}

		// events.add_listener(this); : public listen_input
		// on_key_left();

		Tetrimino( //Tetris * in_ptr_tetris,
				   const vector< XMFLOAT2 >  in_block_positions_relative,/*&*/
				   const char * const in_filename,
				   const double in_drop_rate,
				   const Grid_size in_grid_size_world,
				   const XMINT2 in_block_size )

			: //tetris( in_ptr_tetris ),
			  block_positions_relative( in_block_positions_relative ),
			  image_filename( in_filename ),
			  drop_rate( in_drop_rate ), 			  
			  grid_size( in_grid_size_world ),
			  block_size( in_block_size )
		{			
			add_blocks();			
						
			grid_rectangle.left	  = -( 0.5f * grid_size.width );
			grid_rectangle.right  =    0.5f * grid_size.width;
			grid_rectangle.top    =	   0.5f * grid_size.height;
			grid_rectangle.bottom = -( 0.5f * grid_size.height );

			for( auto & block : blocks )
			{
				block.set_rotation( XMFLOAT3( 0.0f , 0.0f , 0.0f ) );
			}

			XMFLOAT2 position_initial;
			position_initial.x = -0.5 * block_size.x;
			position_initial.y = grid_rectangle.top - ( 0.5f * block_size.y);

			position = position_initial;
			//move_to( position_initial );
			move_to( XMFLOAT2( 0 , 0 ) );

			translate.insert( make_pair( string( "left" ) , XMINT2( -block_size.x ,  0 ) ) );
			translate.insert( make_pair( string( "right" ), XMINT2(  block_size.x ,  0 ) ) );
			translate.insert( make_pair( string( "down" ) , XMINT2(  0			   ,  -block_size.y ) ) );

			//rotation_directions.insert( make_pair( string( "clockwise" ), 1 ) );
			//rotation_directions.insert( make_pair( string( "anticlockwise" ), -1 ) );
		}

		enum class rotate_direction { clock_wise , anti_clock_wise };
		//enum class action { move, rotate, remove_blocks, drop };
		//enum class move_direction { left , right , down };

		//void action(){//print_positions();}	
		//bool move_to_grid_location( float to, float from, move_direction in_direction ) { }
		//bool block_to_left( ){for( auto & block : blocks ){}}
		
		bool block_obstructing_move() // vector< Tetriminos > * in_tetriminos
		{
			//for each block
			  // for each tetrimino : in_tetriminos
				// if block.pos == tertrimino  return true
			// return false
		}

		bool within_grid( const string test_translation )
		{
			XMINT2 move_delta;
			move_delta.x = translate.at( test_translation ).x;
			move_delta.y = translate.at( test_translation ).y;
			// test_move

			for( auto & block : blocks )
			{	
				if( ( block.x_world() + move_delta.x ) < grid_world_rectangle.left  ||
					( block.x_world() + move_delta.x ) > grid_world_rectangle.right ||
					// if -25 < -250
					( block.y_world() + move_delta.y ) < grid_world_rectangle.bottom )
				{
					return false; // block would be outside grid bounds
				}					
			}
			return true;
			//in_bounding_rectangle.left > grid_world_rectangle.left;
		}

		void move_direction( const string translation )
		{
			for( auto & block : blocks )
			{
				float x = block.x_world() + translate.at( translation ).x;
				float y = block.y_world() + translate.at( translation ).y;
				float z = 0.0f;

				block.position( XMFLOAT3( x , y , z ) );
			}
		}

		void key_move( const string in_direction )// , vector< Tetrimino > & in_placed_blocks )
		{
			//if( within_grid( in_direction ) )
			//{
				// && ! block_obstructing_move( in_direction )
				move_direction( in_direction );
			//}
		}

		// change update to a function pointer
		void update( const double in_time_delta  )
		{
			switch( current_state )
			{
				case state::falling : 
					//fall( in_time_delta ); 
				break;

				case state::in_place :
					//m_ptr_game->in_place( this );
				break;
			}			
		}		

		// PRIVATE:
		// void transition_world
		void move_to( XMFLOAT2 in_world_position )
		{
			//position_world = in_world_position;

			vector< XMFLOAT2 >::iterator itr_positions = block_positions_relative.begin();

			// 0..4 blocks
			// 4 positions

			for( auto & block : blocks )
			{
				float x = in_world_position.x + ( itr_positions->x * block_size.x );
				float y = in_world_position.y + ( itr_positions->y * block_size.y );
				float z = 0.0f;

				block.position( XMFLOAT3( x , y , z ) );

				itr_positions++;
			}

		}

		// PRIVATE:
		//void update_rotation()
		//{				
		//	move_to( XMFLOAT2( 0 , 0 ) );			

		//	for( auto & block : blocks )
		//	{
		//		//block.rotate_z( XMConvertToRadians( angle ) );
		//		block.rotate_axis( tetrimino_centre + position_world , angle );
		//	}	

		//	//move_to( position_world );
		//}

		// m_double_slide_time_remaining;				
		//void drop();
		void fall( const double & in_time_delta )
		{ 
			for( auto & block : blocks )
			{
				//if( can_move( move_direction::down) ) {	}
				block.translate_y( drop_rate * in_time_delta );

				// check collision against all other tetriminos blocks / check against filled grid array
				//if( lowest_position() <= m_r_grid.bottom )
				//	m_e_current_state = m_e_state::sliding;
					//update_state( state::sliding );
			}
		}	

		//void key_rotate( const rotate_direction in_rotate )// , const int in_placed_blocks[ 10 ][ 20 ] )
		//{
		//	// if against_wall 
		//		// if can_move_away_from_wall
		//			// rotate_with_move
		//	
		//	switch( in_rotate )
		//	{
		//		case rotate_direction::clock_wise :
		//			angle += 90;
		//		break;
		//		
		//		case rotate_direction::anti_clock_wise :
		//			angle += -90;
		//		break;
		//		
		//	}
		//	update_rotation();
		//	//place_at( position_world.x , position_world.y );	
		//}
	
		void render() {	for( auto & block : blocks ) block.render(); }


};



// Game name: Line'em up
class Tetris : public DX11
{
	public:

		Tetris( HINSTANCE h_instance , UINT window_width , UINT window_height )	: DX11( h_instance , window_width , window_height )
		{
			camera = make_unique< Camera >( "cam1" );
			camera->set_projection_method( projection::orthographic );
			camera->z( -100.0f );

			grid_image = make_unique< Quad_from_image >( "graphics/grid.bmp" );
			//grid_image->z_world( 3.0f );

			grid_size_world.width	= grid_image->width();
			grid_size_world.height	= grid_image->height();
								
			vector< XMFLOAT2 > positions_J { XMFLOAT2( -1,  0 ) , XMFLOAT2( -1, -1 ) , XMFLOAT2( 0, -1 ) , XMFLOAT2( 1, -1 ) };
			// 0 = []      1 = [][]  2 = [][][]  3 =   []
			//     [][][]      []            []        []
			//                 []                    [][]

			// 
			
			//XMINT2 initial_grid_position {};
			const XMINT2 block_size { 25, 25 };
			
			// + block_size
			Tetrimino tetrimino_J( positions_J , "graphics/blue.png" , drop_velocity , grid_size_world , block_size );
			
			tetrimino_types.push_back( move( tetrimino_J ) );

			//tetriminos_in_play.push_back( Tetrimino( tetrimino_types.back() ) ); 
			// copy constructor not copying iterator because it is a dynamically allocated variable
			
			//active_tetrimino = & tetrimino_types.front();
			active_tetrimino = tetrimino_types.begin();

			//vector<double> level_drop_rates
			//const uint current_drop_rate;

			//void in_place( Tetrimino * in_tetrimino );

			//tetrimino_in_play->set_position_initial( )
			

			//grid_placed_blocks.resize( grid_size_blocks.width , vector< unsigned char >( grid_size_blocks.height ) );
			// grid.set_placed_blocks()
			//grid_placed_blocks
									
			// m_p_graphics_manager = std::make_unique< Graphics_manager >( );
			// graphics_manager->add_quad_from_image( "green_block", "image.png" );
			// graphics_manager->get_quad( "green_block" );		

			//game_state menu();
			//game_state game();
			//game_state hi_scores();
		}

		~Tetris() {	
			//delete tetrimino_in_play;	
		}

		void update( const double time_delta )
		{			
			// for( tetri : 

			//game_states.update()
			//game.update(){}

			//tetriminos.update( time_delta );
		}

		void render()
		{
			clear();			
			
			grid_image->render();

			active_tetrimino->render();
			//tetriminos.render();

			camera->render();
						
			//m_font_fps->render();

			present();
		}

		void on_key_down( const WPARAM wParam , const LPARAM lParam )
		{
			switch( wParam )
			{
				case VK_SPACE:
					//tetriminos.add( tetrimino_J ); // add( available_tetriminos )
				break;

				case VK_LEFT:					
					active_tetrimino->key_move( "left" );
					// events.add( KEY_LEFT, param1, param2 );
				break;

				case VK_RIGHT:
					active_tetrimino->key_move( "right" );
				break;

				case VK_DOWN:
					active_tetrimino->key_move( "down" );
				break;

				case 0x5A: // Z  // keyboard::z = 0x5A
				//case VK_MENU:
					//active_tetrimino->key_rotate( Tetrimino::rotate_direction::anti_clock_wise );
				break;

				case 0x58: // X
				//case VK_CONTROL :
					//active_tetrimino->key_rotate( rotate::clock_wise );
				break;
			}	
		}

	private:
		unique_ptr< Camera > camera;
		unique_ptr< Quad_from_image > grid_image;
		//std::unique_ptr< Font > m_font_fps;		
		//std::vector< game_state > m_v_game_states;		
		//Tetriminos tetriminos;
		vector< Tetrimino > tetrimino_types;
		vector< Tetrimino > tetriminos_in_play;
		vector< Tetrimino >::iterator active_tetrimino;
		//Tetrimino * active_tetrimino;
		//grid_size grid_size_blocks { 10 , 20 };
		//int grid_placed_blocks[ 10 ][ 20 ] {};
		//vector< vector< unsigned char > > grid_placed_blocks;	
		grid_size grid_size_world {};
		// grid = textured_quad< runtime_generated_texture >
		//RECT client_area = get_client_area();
		double	drop_velocity = -1.0f;
		double  slide_time = 1.0f;
		//double  next_tetrimino_durration = 

		// level = 0;
		// lines_cleared
		// vectot<double> level_drop_velocity
		
		// update -  

		// input - rotate right, rotate left, drop imediate, increase drop velocity
		//			- test for wall kick
};

int WINAPI wWinMain( HINSTANCE h_instance , HINSTANCE prev_instance , LPWSTR command_line , int window_display_options )
{
	UNREFERENCED_PARAMETER( prev_instance );
	UNREFERENCED_PARAMETER( command_line );  // [ /editor ]
	UNREFERENCED_PARAMETER( window_display_options );

	Tetris game( h_instance , 600u , 800u );
	game.message_loop();

	return 0;// win_message;
}

/*class Tetriminos
{
	public:
		Tetriminos() {}
		void add(  ) {
		tetriminos.push_back( in_tetrimino ); // object copy
		//m_v_tetriminos.at( 0 ).print_positions();
		m_current_tetrimino = m_v_tetriminos.begin(); }
		void render() {	for( auto & tetrimino : m_v_tetriminos ) tetrimino.render(); }
		void update( const double in_time_delta ) { for( auto & tetrimino : m_v_tetriminos ) tetrimino.update( in_time_delta );	}
		void move_current(const Tetrimino::move_direction in_direction ) { m_current_tetrimino->move( in_direction ); }
		void rotate_current( const Tetrimino::rotate_direction in_rotate ) { m_current_tetrimino->rotate( in_rotate ); }
	private:
		vector< Tetrimino > tetriminos;
		vector< Tetrimino >::iterator falling_tetrimino;
};
*/

/*
class game_state
{
	public:
		game_state();
	private:
};
*/

/*
// user defined destructor
~block() {}

// copy constructor
block( const block & in_copy_from )
{
	m_f2_position = in_copy_from.m_f2_position;
	m_f_rotation = in_copy_from.m_f_rotation;

	m_p_quad_image = std::move( in_copy_from.m_p_quad_image );
}

// move constructor
block( block && in_move_from )
{
	m_f2_position = XMFLOAT2( 0.0f , 0.0f );
	m_f_rotation = 0.0f;
}

// copy assignment operator
block operator = ( const block & in_copy_from )
{
	m_p_quad_image.reset( Quad_from_image( *in_copy_from.m_p_quad_image ) );

}

// move assignment operator
block operator = ( block && in_move_from )
{
	if( this != &in_move_from )
	{
		m_p_quad_image = std::move( in_move_from.m_p_quad_image );
	}
	return *this
}
*/
