/*
cbuffer matrix_WVP_cb : register( b0 )
{
	matrix m_mesh;
	matrix m_view;
	matrix m_projection;
};
*/
cbuffer world : register( b0 )// VS_BUFFER_WORLD
{
	matrix world;
};

cbuffer view : register( b1 )// VS_BUFFER_VIEW
{
	matrix view;
};

cbuffer projection : register( b2 ) // VS_BUFFER_PROJECTION
{
    matrix projection;
};

struct input_VS //VS_XYZ_RGBA_UV
{
	float4 position	: POSITION;
    float4 colour	: COLOR;
    float2 uv		: TEXCOORD;
};

// struct XYZ_UV

struct output_VS // Pixel shader input
{
	float4 position	: SV_POSITION;
    float4 colour	: COLOR;
    float2 uv		: TEXCOORD;
};

// Vertex shader outputs data to Pixel shader using 'output_VS' structure
output_VS main_VS( input_VS input )
{
	output_VS output = ( output_VS ) 0;

	//output.position = vertex.position;
	//output.position = mul( vertex.position , matrix_WVP );

	output.position = mul( input.position , world );		// float4( vertex.position , 1.0f );

	output.position = mul( output.position , view );
	output.position = mul( output.position , projection );

    output.uv = input.uv;

    output.colour = input.colour;

	return output;
}