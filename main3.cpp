#include <map>

#include "..\DX11\DX11_base.h"
#include "..\DX11\Camera.h"
#include "..\DX11\Quad.h"

using std::unique_ptr;
using std::make_pair;
using std::wstring;


// namespace line_em_up {}

enum class direction { left , right , down , cw , ccw };

const float block_layer = 0.0f;

class Block
{
	private:

		//string			image_filename {};
		unique_ptr< Quad >	m_quad; // map_diffuse

		//XMFLOAT2			m_position {};
		float				m_width {};
		float				m_height {};
		RECT				m_border { };

	public:
		
		// default constructor
		Block( const wstring in_image_filename )
		{
			m_quad = make_unique< Quad >( in_image_filename );

			m_width  = m_quad->width(); //textures.by_name("block_blue")->width();
			m_height = m_quad->height();
		}

		// copy constructor
		Block( const Block & in_block ) = delete;
		// copy assignment constructor
		Block & operator = ( const Block & in_block ) = delete;

		// move assignment operator
		Block & operator = ( Block && in_block )
		{
			if( this != & in_block ) // copy self gaurd?
			{
				m_quad = move( in_block.m_quad ); // quad contains a unique pointer

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
			m_quad->position( XMFLOAT3( in_x , in_y , block_layer ) ); 
		}
		const XMFLOAT3 position() const { return m_quad->position(); }

		void update() { m_quad->update(); }
		void render() { m_quad->render(); }

		const float left_side() const { return ( m_quad->position().x - ( m_width + 0.5f ) ); }

		RECT border()
		{
			m_border.top		= m_quad->position().y - ( m_height * 0.5f );
			m_border.left		= m_quad->position().x - ( m_width * 0.5f );
			m_border.right		= m_quad->position().x + ( m_width * 0.5f );
			m_border.bottom	= m_quad->position().y + ( m_height * 0.5f );

			return m_border;
		}
};


class Tetrimino
{
	private:

		wstring						image_filename {};
		vector< Block >				m_blocks;
		XMFLOAT2					position {};
		XMFLOAT2					position_initial {};
		XMFLOAT2					center { };
		vector< XMFLOAT2 >			block_positions;
		double						m_drop_velocity {};
		//Grid_size					grid_size {};
		enum class					state { moving , falling , sliding , in_place , rotating }; 
		state						current_state { state::falling };
		std::map< wstring , XMINT2 >		translate;
		//Playfield * m_playfield = nullptr; // shared_pointer<>
		RECT						m_playfield {};
			
		std::map < direction , XMFLOAT2 > directions;
		//map< string , XMFLOAT2 >		move_direction;

	public:

		
		
		Tetrimino() {}
		Tetrimino( const vector< XMFLOAT2 >	in_block_positions_relative ,/*&*/
				   const wstring				in_image_filename ,
				   const double				in_drop_velocity ,
				   const RECT				in_playfield )

			:	block_positions( in_block_positions_relative ) ,
				image_filename( in_image_filename ) ,
				m_drop_velocity( in_drop_velocity ) ,
				m_playfield( in_playfield )
		{			
			directions.insert( make_pair( direction::left ,  XMFLOAT2( -1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
			directions.insert( make_pair( direction::down ,  XMFLOAT2(  0.0f , -1.0f ) ) );

			add_blocks();			
		}

		void add_blocks()
		{
			for( auto & block : block_positions )
			{
				Block new_block( image_filename.data() );

				float width  = new_block.width();
				float height = new_block.height();

				position_initial.x = -0.5 * width; // screen center - 1/2 block width
				position_initial.y = m_playfield.top - ( 0.5f * height ); // 

				float x = position_initial.x + ( block.x * width );
				float y = position_initial.y + ( block.y * height );
				float z = block_layer;

				new_block.position( x , y );
				
				m_blocks.push_back( std::move(new_block) ); // block has no copy constructor? // rvalue move constructor
			}
		}

		void update()
		{
			for( auto & block : m_blocks ) { block.update(); }
		}

		void render()
		{
			for( auto & block : m_blocks ) { block.render(); }
		}

		void move( const direction in_direction )
		{
			if( within_playfield( in_direction ) ) //&& ! block_obstructing )
			{
				for( auto & block : m_blocks )
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

		bool within_playfield( const direction in_direction )
		{
			vector<Block>::const_iterator citr_block = m_blocks.cbegin();

			XMFLOAT2 test_move;
			test_move.x = directions.at( in_direction ).x * citr_block->width();
			test_move.y = directions.at( in_direction ).y * citr_block->height();

			for( const auto & block : m_blocks )
			{
				if( ( block.position().x + test_move.x ) < m_playfield.left ||
					( block.position().x + test_move.x ) > m_playfield.right ||
					// if -25 < -250
					( block.position().y + test_move.y ) < m_playfield.bottom )
				{
					return false; // block would be outside grid bounds
				}
			}
			return true;
		}

		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction)		{		}

		RECT border()
		{
			//if( ! m_blocks.empty() )
			RECT border = m_blocks.front().border();

			for( auto & block : m_blocks )
			{
				if( block.border().left < border.left )
					border.left = block.border().left;

				if( block.border().right > border.right )
					border.right = block.border().right;

				if( block.border().top < border.top )
					border.top = block.border().top;

				if( block.border().bottom > border.bottom )
					border.bottom = block.border().bottom;
			}

			return border;
		}

		void rotate( const direction in_direction )
		{
			RECT temp_border = border();
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
			m_boarders.left		= static_cast<LONG>( m_background->width() * -0.5 );
			m_boarders.right	= m_background->width() / 2;
			m_boarders.top		= m_background->height() / 2;
			m_boarders.bottom	= static_cast<LONG>( m_background->height() * -0.5 );
		}

		const RECT boarders( void ) const { return m_boarders; }

		void update() { m_background->update(); }
		void render() { m_background->render(); }

	private:

		RECT m_boarders {};
		unique_ptr< Quad > m_background;

};


// Game name: Line'em up
class Tetris : public DX11
{
	private:

		unique_ptr< Camera >	m_camera;
		unique_ptr< Playfield >	m_playfield;

		double					m_drop_velocity { -1.0 }; // pixels/time

		unique_ptr<Tetrimino>	tetrimino_J;

		vector< Tetrimino >::iterator active_tetrimino;
	
	public:

		Tetris( HINSTANCE h_instance , UINT window_width , UINT window_height ) : DX11( h_instance , window_width , window_height )
		{
			m_camera = make_unique< Camera >( L"cam1" );
			m_camera->projection( Projection::orthographic );
						
			m_playfield = make_unique< Playfield >( L"graphics/grid.bmp" );

			vector< XMFLOAT2 > positions_J { XMFLOAT2( -1,  0 ) , XMFLOAT2( -1, -1 ) , XMFLOAT2( 0, -1 ) , XMFLOAT2( 1, -1 ) };
			
			tetrimino_J = make_unique<Tetrimino>( positions_J , L"graphics/blue.png" , m_drop_velocity , m_playfield->boarders() );
			// make_shared<>()
			// drop_velocity m/s
			// rotation_velocity
			// move_velocity

			//active_tetrimino = 
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
			
			m_playfield->update();

			tetrimino_J->update();
		}

		void render()
		{
			clear();
			
			m_camera->render();

			m_playfield->render();
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