#pragma once

class Line_em_up : public DX11
{
	private:

	//Input_handler keyboard_handler;

	Camera					camera;
	Quad					playfield;

	double					drop_velocity{ -1.0 }; // playfield square per second

	vector< Tetrimino >		tetrimino_types;
	//list< Tetrimino >		tetrimino_bucket;

	Tetrimino				tetrimino_J;
	//	   [3]
	//	   +2]
	//	[0][1]

	vector< XMFLOAT2 >		block_offsets_J{ XMFLOAT2( -0.5f , -1.0f ) , XMFLOAT2( 0.5f , -1.0f ) , XMFLOAT2( 0.5f , 0.0f ) , XMFLOAT2( 0.5f , 1.0f ) };
	//vector< Tetrimino >::const_iterator active_tetrimino;  // using vec<>::itr active_tetri // active_tetri = blocks.front();

	Timer					timer;

	// stage/difficulty -> pixel per second
	const map< uint , float >		level_fall_rates{ { 1u , 1.0f } };	// 1 block per second

																		// 1 block.y = 25 pixels
																		// 25 pixels / 1 second = 25 pixels/sec

																		// speed = distance / time
																		// distance = speed * time

																		// tetri : public state

																		// block.update() 
																		//   block.y += 25 * time (

	public:

	Line_em_up( HINSTANCE h_instance , UINT window_width , UINT window_height ) : DX11( h_instance , window_width , window_height )
	{
		camera.set_projection( Projection::orthographic );
		camera.set_position( XMVectorSet( origin_x , origin_y , -100.0f , 0.0f ) );

		playfield.load_diffuse( L"graphics/grid.bmp" );
		playfield.set_position( XMFLOAT3( origin_x , origin_y , graphic_layers.playfield ) );

		//top, left, right, bottom
		rectangle playfield_boarder( 250 , -125 , 125 , -250 );

		Tetri_centre centre;//( 2, XMFLOAT3(-0.5f, 0,0) );
		centre.block = 2;
		centre.vertex0 = 0;
		centre.vertex1 = 1;

		tetrimino_J.create( block_offsets_J , centre , L"graphics/blue.png" , playfield_boarder );//playfield.get_border() );

		timer.start();
		timer.tick();
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
		timer.tick();

		wstring title = L"Line'em'up";

		title.append( L" - " );
		title.append( std::to_wstring( time_delta ) );

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
			tetrimino_J.rotate( Rotation::counter_clock_wise );	// rotation speed
		}

		if( kb.Down || kb.S )
			tetrimino_J.move( Direction::down ); // fast_drop

		if( kb.Left || kb.A )
			tetrimino_J.move( Direction::left );
		// events.add( Direction::left );

		if( kb.Right || kb.D )
		{
			tetrimino_J.move( Direction::right );
		}

		//if( kb.PageUp || kb.Space )
		//if( kb.PageDown || kb.X )

		camera.update(); //timer.get_delta()

		playfield.update( timer.get_delta() );

		tetrimino_J.update( timer.get_delta() );
	}

	void render()
	{
		clear( XMFLOAT4( 0.20f , 0.38f , 0.64f , 1.0f ) ); // clear backbuffer

		playfield.render();

		tetrimino_J.render();

		present();
	}

};