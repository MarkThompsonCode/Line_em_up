#pragma once

const float origin_x = 0.0f;
const float origin_y = 0.0f;

enum class Direction { left , right , down };
enum class Rotation { clock_wise , counter_clock_wise };

const float layer_playfield = 0.0f;
const float layer_block = -1.0f;

//enum class Graphic_layers { playfield = 0 , block };

//typedef struct Graphic_layers
//{
//	const float playfield	= 0.0f;
//	const float block		= -1.0f;
//} Graphic_layers;

//class Graphic_layers
//{
//	public:
//	const float playfield = 0.0f;
//	const float block = -1.0f;
//};
//
//Graphic_layers graphic_layers;