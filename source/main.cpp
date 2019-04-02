#include <map>

#include "..\DX11\DX11_base.h"
#include "..\DX11\Camera.h"
#include "..\DX11\Quad.h"
#include "..\DX11\linked_list.h"

// sounds.add_sound( "rotate", "sounds/rotate.wav" );

//#pragma
//undef _MBCS //#define _UNICODE //#define UNICODE

using namespace std;

//enum class layer { grid, terimino, osd };
//struct layer { const float osd = 1.0f; const float tetrimino = 2.0f; const float grid = 3.0f; };

struct grid_size
{
	unsigned int width;
	unsigned int height;
};

class Tetris;

class Tetrimino
{
	public:

		Tetrimino() {};// : m_ptr_game( nullptr ) {}

		// copy constructor
		Tetrimino( const Tetrimino & in_copy_from )	
		{ int i = 0;}

		Tetrimino( //Tetris * in_ptr_tetris,
				   const vector< vector< XMFLOAT2 > > /*&*/ in_block_positions_relative,
				   const char * const in_filename,
				   const double in_drop_rate,
				   const grid_size in_grid_size_world,
				   const XMINT2 in_block_size )

			: //tetris( in_ptr_tetris ),
			  block_positions_relative( in_block_positions_relative ),
			  image_filename( in_filename ),
			  drop_rate( in_drop_rate ), 			  
			  grid_size_world( in_grid_size_world ),
			  block_size( in_block_size )
		{			
			add_blocks();			
						
			grid_world_rectangle.left	= -( 0.5f * grid_size_world.width );
			grid_world_rectangle.right	=    0.5f * grid_size_world.width;
			grid_world_rectangle.top	=	 0.5f * grid_size_world.height;
			grid_world_rectangle.bottom = -( 0.5f * grid_size_world.height );

			position_initial.x = -0.5 * block_size.x ;
			position_initial.y = grid_world_rectangle.top - ( 0.5f * block_size.y);

			//itr_current_rotation = block_positions_relative.begin();


			move_to( position_initial.x , position_initial.y );

			//rotate();
			
			//position_world = position_grid_initial;	

			directions.insert( make_pair( string( "left" ) , XMINT2( -block_size.x ,  0 ) ) );
			directions.insert( make_pair( string( "right" ), XMINT2(  block_size.x ,  0 ) ) );
			directions.insert( make_pair( string( "down" ) , XMINT2(  0			   ,  -block_size.y ) ) );

			//rotation_directions.insert( make_pair( string( "clockwise" ), 1 ) );
			//rotation_directions.insert( make_pair( string( "anticlockwise" ), -1 ) );
			/*
			rotations.add( 1 );
			rotations.add( 2 );
			rotations.add( 3 );
			rotations.add( 4 );
			*/
		}

		enum class rotate_direction { clock_wise , anti_clock_wise };
		//enum class action { move, rotate, remove_blocks, drop };
		//enum class move_direction { left , right , down };

		//const vector< XMINT2 >
		/*void update_grid_positions() 
		{
			block_positions_grid.clear();

			for( auto & block : blocks )
			{
				XMINT2 block_grid_position;

				XMFLOAT3 block_world_position;
				block_world_position.x = grid_world_rectangle.left - ( block.get_position().x + ( 0.5f * block_size_world.x ));
				
				block_grid_position.x = block_world_position.x / block_size_world.x;
				block_grid_position.y = 20 - ( grid_size_world.height / block_size_world.y );

				block_positions_grid.push_back( block_grid_position );
			}			
		}*/

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


		bool within_grid( const string in_direction )
		{
			XMINT2 move_delta;
			move_delta.x = directions.at( in_direction ).x;
			move_delta.y = directions.at( in_direction ).y;

			for( auto & block : blocks )
			{	
				if( ( block.get_position().x + move_delta.x ) < grid_world_rectangle.left  ||
					( block.get_position().x + move_delta.x ) > grid_world_rectangle.right ||
					// if -25 < -250
					( block.get_position().y + move_delta.y ) < grid_world_rectangle.bottom )
				{
					return false; // block would be outside grid bounds
				}					
			}
			return true;
			//in_bounding_rectangle.left > grid_world_rectangle.left;
		}

		//XMINT2 move_to_position { grid_postion.x + map_directions.at( "left" ).x,
		//grid_postion.y + map_directions.at( "left" ).y };
		
		void move_direction( const string in_direction )
		{
			for( auto & block : blocks )
			{
				float x = block.get_position().x + directions.at( in_direction ).x;
				float y = block.get_position().y + directions.at( in_direction ).y;
				float z = 0.0f;

				block.set_position( XMFLOAT3( x , y , z ) );
			}
		}

		void key_move( const string in_direction )// , vector< Tetrimino > & in_placed_blocks )
		{
			if( within_grid( in_direction ) )
			{
				// && ! block_obstructing_move( in_direction )
				move_direction( in_direction );
			}
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
		void move_to( const float in_world_x , const float in_world_y )
		{
			// for each block
			//		xy + current_rotation.position(block)

			for( const auto & block : blocks )
			{
				//float x = block_positions_relative.at( current_rotation ).
				block.next_rotation();
			}

			vector< Quad_from_image >::iterator itr_block = blocks.begin();
			//vector< XMFLOAT2 >::iterator itr_positions = itr_current_rotation->begin();

			vector< XMFLOAT2 > & ref_current_rotation = block_positions_relative.at( current_rotation );

			for( ; itr_positions != itr_current_rotation->end(); ++itr_block , ++itr_positions )
			{
				float x = in_world_x + ( itr_positions->x * block_size.x );
				float y = in_world_y + ( itr_positions->y * block_size.y );
				float z = 0.0f;

				itr_block->set_position( XMFLOAT3( x , y , z ) );
			}
		}

		// PRIVATE:
		void update_rotation()
		{
			vector< Quad_from_image >::iterator itr_block = blocks.begin();
			
			//vector< XMFLOAT2 > & current_rotation_positions = block_positions_relative.at( 0 );// current_rotation );

			vector< XMFLOAT2 >::iterator itr_positions = itr_current_rotation->begin();

			for( ; itr_positions != itr_current_rotation->end(); ++itr_block, ++itr_positions )
			{
				//XMFLOAT2 block_size				{ static_cast< float >( itr_block->get_width() ), static_cast< float >( itr_block->get_height() ) };
				XMFLOAT2 block_size_half		{ 0.5f * block_size.x , 0.5f * block_size.y };
				//XMFLOAT2 grid_size_world_half	{ 0.5f * grid_size_world.width,  0.5f * grid_size_world.height };
				
				//float position_block_relative_x = itr_positions_relative->x * block_size.x;
				//float position_block_relative_y = itr_positions_relative->y * block_size.y;

				//float x = itr_block->get_position().x + position_block_relative_x;// -block_size_half.x;
				//float y = itr_block->get_position().y + position_block_relative_y;// -block_size_half.y;
				float z = 0.0f;
	
				//itr_block->set_position( XMFLOAT3( x , y , z ) );
			}	
			//update_grid_positions();
		}

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

		void key_rotate( const rotate_direction in_rotate )// , const int in_placed_blocks[ 10 ][ 20 ] )
		{
			// if against_wall 
				// if can_move_away_from_wall
					// rotate_with_move

			//vector< vector< XMFLOAT2 > > block_positions_relative;
			//vector< vector< XMFLOAT2 > >::iterator itr_current_rotation;

			//vector< vector< XMFLOAT2 > >::iterator itr_positions = block_positions_relative.begin();
			//itr_current_rotation = block_positions_relative.begin();

			//vector< vector< XMFLOAT2 > > vv_xmf2 { { XMFLOAT2( 1,2 ) }, { XMFLOAT2( 3 , 4 ) } };
			//vector< XMFLOAT2 > & itr = vv_xmf2.back();

			//if( itr == vv_xmf2.back() )			{	}
			
			switch( in_rotate )
			{
				case rotate_direction::clock_wise :
					if( itr_current_rotation == block_positions_relative.end() )
						itr_current_rotation = block_positions_relative.begin();
					else
						++itr_current_rotation;

					//current_rotation = rotations.current();
					//rotations.increment(); // rotations++;

					// block.add_rotation_positions( XMFLOAT2( -1 , 0 ), XMFLOAT2( -1 , -1 ), XMFLOAT2( -1 , 1 ), XMFLOAT2( 1 , 1 ) );


					//if( current_rotation == 3 ) current_rotation = -1;
					//++current_rotation;
				break;
				
				case rotate_direction::anti_clock_wise :
					//if( current_rotation == 0 ) current_rotation = 4;
					//--current_rotation;
				break;
				
			}
			update_rotation();
			//place_at( position_world.x , position_world.y );	
		}
		
		/*RECT get_block_sides_world( vector< Quad_from_image >::iterator in_itr_block )
		{
			RECT sides_world {};

			float block_width_half	= 0.5f * in_itr_block->get_width();
			float block_height_half	= 0.5f * in_itr_block->get_height();

			sides_world.left	= in_itr_block->get_position_x() - block_width_half;
			sides_world.right	= in_itr_block->get_position_x() + block_width_half;
			sides_world.top		= in_itr_block->get_position_y() + block_height_half;
			sides_world.bottom	= in_itr_block->get_position_y() - block_height_half;

			return sides_world;
		}*/

		/*const RECT get_bounding_rectangle( )
		{
			RECT bounding_block_side_positions {};
			RECT current_block_side_positions {};
			
			vector< Quad_from_image >::iterator itr_block = blocks.begin();

			bounding_block_side_positions = get_block_sides_world( itr_block );

			++itr_block;
			
			for( ; itr_block != blocks.end(); ++itr_block )
			{
				current_block_side_positions = get_block_sides_world( itr_block );
				
				if( current_block_side_positions.left < bounding_block_side_positions.left )
					bounding_block_side_positions.left = current_block_side_positions.left;

				if( current_block_side_positions.right > bounding_block_side_positions.right )
					bounding_block_side_positions.right = current_block_side_positions.right;

				if( current_block_side_positions.top > bounding_block_side_positions.top )
					bounding_block_side_positions.top = current_block_side_positions.top;

				if( current_block_side_positions.bottom < bounding_block_side_positions.bottom )
					bounding_block_side_positions.bottom = current_block_side_positions.bottom;
			}

			//debug_out( "bounding left = %i \n" , bounding_block_side_positions.left );

			return bounding_block_side_positions;
		}*/

		void print_positions()
		{					
			for( const auto & block : blocks )
				debug_out( "print positions x %f y %f z %f \n" ,
						   block.get_position().x ,
						   block.get_position().y ,
						   block.get_position().z );
		}

		void render() {	for( auto & block : blocks ) block.render(); }

	private:

		// cyclic reference
		// Tetris & m_ptr_game; // investigate model-view-document architecture
		// * could be another object type, & will always be of T& type

		vector< Quad_from_image > blocks;
		
		void add_blocks( )
		{	
			for( auto & block : block_positions_relative )
			{
				// object copied
				//Quad_from_image new_block( image_filename.data() );
				//blocks.push_back( new_block ); 

				blocks.push_back( Quad_from_image( image_filename.data() ) ); // rvalue move consttructor
			}			
		}		

		std::string image_filename {};

		vector< vector< XMFLOAT2 > > block_positions_relative;
		//vector< vector< XMFLOAT2 > >::iterator itr_current_rotation = block_positions_relative.begin();

		map< string , XMINT2 > directions;

		//vector< XMINT2 > block_positions_grid;

		int			current_rotation = 0;
		//linked_list<int> rotations;
		//int			total_rotations = 3;

		double		drop_rate {};
		//XMFLOAT2	m_f2_block_size {};
		
		XMFLOAT2	position_initial {};
		//XMFLOAT2	position_world {};

		grid_size	grid_size_world {};
		XMINT2		block_size {};

		//XMFLOAT2	block_size_world {};

		RECT		grid_world_rectangle {};
		
		enum class  state { falling , sliding , in_place }; // rotate and falling
		state		current_state = state::falling;
};

/*class Tetriminos
{
	public:
		Tetriminos() {}

		void add(  ) 
		{ 
			tetriminos.push_back( in_tetrimino ); // object copy
					
			//m_v_tetriminos.at( 0 ).print_positions();
			m_current_tetrimino = m_v_tetriminos.begin();
		}

		void render() 
		{	
			for( auto & tetrimino : m_v_tetriminos ) tetrimino.render(); 
		}

		void update( const double in_time_delta )
		{
			for( auto & tetrimino : m_v_tetriminos ) tetrimino.update( in_time_delta );
		}

		void move_current(const Tetrimino::move_direction in_direction )
		{
			m_current_tetrimino->move( in_direction );
		}

		void rotate_current( const Tetrimino::rotate_direction in_rotate )
		{
			m_current_tetrimino->rotate( in_rotate );
		}

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

// Game name: Line'em up
class Tetris : public DX11
{
	public:
		Tetris( HINSTANCE h_instance , UINT window_width , UINT window_height )
			: DX11( h_instance , window_width , window_height )
		{
			camera = make_unique< Camera >( "cam1" );
			camera->set_projection_method( camera_projection::ORTHOGRAPHIC );
			camera->set_z( -100.0f );

			grid_image = make_unique< Quad_from_image >( "graphics/grid.bmp" );
			grid_image->set_z( 3.0f );

			grid_size_world.width	= grid_image->get_width();
			grid_size_world.height	= grid_image->get_height();
								
			vector< vector< XMFLOAT2 > > positions_J {	{ XMFLOAT2( -1,  0 ), XMFLOAT2( -1, -1 ), XMFLOAT2(  0, -1 ), XMFLOAT2(  1, -1 ) },  // 0
														{ XMFLOAT2( -1, -1 ), XMFLOAT2( -1,  0 ), XMFLOAT2( -1,  1 ), XMFLOAT2(  0,  1 ) },	 // 1
														{ XMFLOAT2( -1,  1 ), XMFLOAT2(  0,  1 ), XMFLOAT2(  1,  1 ), XMFLOAT2(  1,  0 ) },  // 2
														{ XMFLOAT2(  1,  1 ), XMFLOAT2(  1,  0 ), XMFLOAT2(  1, -1 ), XMFLOAT2(  0, -1 ) } };// 3
			// 0 = []      1 = [][]  2 = [][][]  3 =   []
			//     [][][]      []            []        []
			//                 []                    [][]
			
			//tetrimino
			//	private:
			//		vector< vector< XMFLOAT2 > > positions

			//tetris
			//	private:
			//		vector< Tetrimino > tetrimino_types		

			//XMINT2 initial_grid_position {};
			const XMINT2 block_size { 25, 25 };
			
			// + block_size
			Tetrimino tetrimino_J( positions_J , "graphics/blue.png" , drop_velocity , grid_size_world , block_size );
			
			tetrimino_types.push_back( move(tetrimino_J) );


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

		~Tetris()
		{
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
				break;

				case VK_RIGHT:
					active_tetrimino->key_move( "right" );
				break;

				case VK_DOWN:
					active_tetrimino->key_move( "down" );
				break;

				case VK_MENU:
					active_tetrimino->key_rotate( Tetrimino::rotate_direction::anti_clock_wise );
				break;

				case 0x5A :
				case VK_CONTROL :
					active_tetrimino->key_rotate( Tetrimino::rotate_direction::clock_wise );
				break;
			}	
		}

	private:

		std::unique_ptr< Camera > camera;
		std::unique_ptr< Quad_from_image > grid_image;

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
	UNREFERENCED_PARAMETER( command_line );  // switch [ /editor ]
	UNREFERENCED_PARAMETER( window_display_options );

	Tetris game( h_instance , 600u , 800u );
	game.message_loop();

	return 0;// win_message;
}

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
