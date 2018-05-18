#include <map>

#include "DX11_base.h"
#include "Camera.h"
#include "Quad.h"

using std::unique_ptr;
using std::make_pair;
using std::wstring;
using std::map;

// namespace line_em_up {}

enum class direction { left , right , down , cw , ccw };

const struct
{
	float block = 0.0f;
} layers;

struct rectangle
{
	float left {};
	float top {};
	float right {};
	float bottom {};
};

class Block
{
	private:

		//string			image_filename {};
		unique_ptr< Quad >	m_quad; // map_diffuse

		//XMFLOAT2			m_position {};
		float				m_width {};
		float				m_height {};
		rectangle			m_border { };

	public:
		
		// default constructor
		Block( const wstring in_image_filename)//, XMFLOAT3 in_position )
		{
			m_quad = make_unique< Quad >( in_image_filename );

			//m_quad->position( in_position );

			m_width  = m_quad->width(); //textures.by_name("block_blue")->width();
			m_height = m_quad->height();
		}

		// copy constructor
		Block( const Block & ) = delete; // unique_pointer cannot be copied

		// copy assignment constructor
		Block & operator = ( const Block & ) = delete; // unique_pointer cannot be copied

		// move assignment operator
		Block & operator = ( Block && in_block )
		{
			if( this != & in_block ) // copy self gaurd?
			{
				m_quad = move( in_block.m_quad );

				m_width = in_block.m_width;
				m_height = in_block.m_height;
				m_border = in_block.m_border;
			}

			return * this;
		}

		// move constructor
		Block( Block && in_block )
		{
			m_quad = std::move( in_block.m_quad );

			m_width = in_block.m_width;
			m_height = in_block.m_height;
			m_border = in_block.m_border;
		}

		const float width() const { return m_width; }
		const float height() const { return m_height; }

		void position( const float in_x , const float in_y ) 
		{ 
			m_quad->position( XMFLOAT3( in_x , in_y , layers.block ) );

			border();
		}

		const XMFLOAT3 position() const { return m_quad->position(); }

		void update() { m_quad->update(); }
		void render() { m_quad->render(); }

		//const float left_side() const { return ( m_quad->position().x - ( m_width + 0.5f ) ); }

		rectangle border()
		{
			m_border.top	= m_quad->position().y + ( m_height * 0.5f );
			m_border.bottom = m_quad->position().y - ( m_height * 0.5f );
			m_border.left	= m_quad->position().x - ( m_width  * 0.5f );
			m_border.right	= m_quad->position().x + ( m_width  * 0.5f );			

			return m_border;
		}
};

//class Playfield;

class Tetrimino
{
	private:

		//wstring					image_filename {};
		vector< Block >				blocks;
		XMFLOAT2					position {};
		XMFLOAT2					position_initial {};
		XMFLOAT2					center {};
		vector< XMFLOAT2 >			block_offsets; // &
		double						drop_velocity {};
		//Grid_size					grid_size {};
		enum class					state { moving , falling , sliding , in_place , rotating }; 
		state						current_state { state::falling };
		map< wstring , XMINT2 >		translate;
		rectangle						m_border {};
		rectangle						playfield {};
		//Playfield * m_playfield = nullptr; // shared_pointer<>

		map< direction , XMFLOAT2 > directions;

	public:		
		
		Tetrimino() {}
		Tetrimino( const vector< XMFLOAT2 >	in_block_positions_relative ,/*&*/
				   const wstring			in_texture_diffuse ,
				   rectangle						in_playfield )
				   //const Playfield * const in_playfield )

			: block_offsets( in_block_positions_relative ) ,
			  playfield( in_playfield )
		{			
			directions.insert( make_pair( direction::left ,  XMFLOAT2( -1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::down ,  XMFLOAT2(  0.0f , -1.0f ) ) );

			add_blocks( in_texture_diffuse );
		}

		void add_blocks( wstring in_texture_diffuse )
		{
			for( auto & offset : block_offsets )
			{
				Block new_block( in_texture_diffuse );
				
				position_initial.x = -0.5 * new_block.width(); // screen center - 1/2 block width
				position_initial.y = playfield.top - ( 0.5f * new_block.height() ); // 

				float x = position_initial.x + ( offset.x * new_block.width() );
				float y = position_initial.y + ( offset.y * new_block.height() );

				//float x = offset.x * new_block.width();
				//float y = offset.y * new_block.height();

				new_block.position( x , y );
				
				blocks.emplace_back( std::move( new_block ) ); // block has no copy constructor // rvalue move constructor
			}
		}

		void update()
		{
			for( auto & block : blocks ) { block.update(); }
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
					float current_x = block.position().x;
					float current_y = block.position().y;

					float move_x = directions.at( in_direction ).x * block.width();
					float move_y = directions.at( in_direction ).y * block.height();

					block.position( current_x + move_x , current_y + move_y );
				}
			}
		}

		//bool can_move( const string in_direction ) { }
		//bool can_fit

		bool within_playfield( const direction in_direction )
		{
			XMFLOAT2 test_move {};
			test_move.x = directions.at( in_direction ).x * blocks.front().width();
			test_move.y = directions.at( in_direction ).y * blocks.front().height();

			m_border = border();

			if ( ( m_border.left   + test_move.x ) < playfield.left  ||
				 ( m_border.right  + test_move.x ) > playfield.right ||
				 ( m_border.bottom + test_move.y ) < playfield.bottom )
			{
				return false; // block would be outside playfield bounds
			}
			return true;
		}

		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction){}

		rectangle border()
		{
			//if( ! m_blocks.empty() )

			rectangle tetri_border = blocks.front().border();

			for( auto & block : blocks )
			{
				if( block.border().left < tetri_border.left )
					tetri_border.left = block.border().left;

				if( block.border().right > tetri_border.right )
					tetri_border.right = block.border().right;

				if( block.border().top > tetri_border.top )
					tetri_border.top = block.border().top;

				if( block.border().bottom < tetri_border.bottom )
					tetri_border.bottom = block.border().bottom;
			}

			debug_out( "\ntetri_border.bottom =  %f" , tetri_border.bottom);

			return tetri_border;
		}

		void rotate( const direction in_direction )
		{
			rectangle temp_border = border();
			XMFLOAT2 centre { };

			centre.x = 0.5 * ( temp_border.right - temp_border.left );
			centre.y = 0.5 * ( temp_border.bottom - temp_border.top );

			// block.pos - centre.pos
		}

		//bool can_rotate( const rotation in_rotation ) {}

};


class Playfield
{
	public:

		Playfield( const wstring image )
		{
			m_background = make_unique< Quad >( image.data() );
			m_background->position( XMFLOAT3( 0.0f , 0.0f , 10.0f ) ); // enum class layer { background, 

			// centered at 0,0
			m_boarder.left		= -0.5 * static_cast< LONG >( m_background->width() );
			m_boarder.right		=  0.5 * m_background->width();
			m_boarder.top		=  0.5 * m_background->height();
			m_boarder.bottom	= -0.5 * static_cast< LONG >( m_background->height() );
		}

		const rectangle boarder( void ) const { return m_boarder; }

		void update() { m_background->update(); }
		void render() { m_background->render(); }

	private:

		rectangle m_boarder {};
		//cell_size;

		unique_ptr< Quad > m_background;
};


// Game name: Line'em up
class Tetris : public DX11
{
	private:

		unique_ptr< Camera >	camera;
		unique_ptr< Playfield >	playfield;

		double					drop_velocity { -1.0 }; // pixels/time

		unique_ptr< Tetrimino >	tetrimino_J;

		vector< Tetrimino >::iterator active_tetrimino;
	
	public:

		Tetris( HINSTANCE h_instance , UINT window_width , UINT window_height ) : DX11( h_instance , window_width , window_height )
		{
			camera = make_unique< Camera >( L"cam1" );
			camera->projection( Projection::orthographic );
						
			playfield = make_unique< Playfield >( L"graphics/grid.bmp" );

			vector< XMFLOAT2 > block_offsets_J { XMFLOAT2( -1,  0 ) , XMFLOAT2( -1, -1 ) , XMFLOAT2( 0, -1 ) , XMFLOAT2( 1, -1 ) };
			
			tetrimino_J = make_unique<Tetrimino>( block_offsets_J , L"graphics/blue.png", playfield->boarder() );

			// vector< Tetrimino > tetriminos_static
			// drop_velocity m/s
			// velocity_rotation
			// velocity_fall
			// velocity_drop
		}

		void update( const double time_delta )
		{
			// auto key_state = keyboard->GetState();
			auto kb = m_keyboard->GetState();

			if( kb.Escape )
				PostQuitMessage( 0 );

			//auto mouse = m_mouse->GetState();

			if( kb.Z )
			{
				tetrimino_J->rotate( direction::ccw );
			}

			if( kb.Down || kb.S )
				tetrimino_J->move( direction::down );

			if( kb.Left || kb.A )
				tetrimino_J->move( direction::left );
				// events.add( KEY_LEFT, param1, param2 );

			if( kb.Right || kb.D )
			{
				tetrimino_J->move( direction::right );
			}

			//if( kb.PageUp || kb.Space )
			//if( kb.PageDown || kb.X )
			
			playfield->update();

			tetrimino_J->update();
		}

		void render()
		{
			clear();
			
			camera->render();

			playfield->render();
			tetrimino_J->render();
						
			present();
		}

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