#version 110

// comes from game during init
uniform mat4 dynamic_projection;
uniform mat4 static_projection;

// goes to fragment shader
varying vec2 t_coord;

// data from the VBO
attribute vec3 vert_data;
attribute vec2 tex_data;
attribute vec4 color_data;
attribute vec4 xform_data;

void main()
{	
	float x = vert_data[0];
	float y = vert_data[1];
	float z = vert_data[2];
	
	float trans_x = xform_data[0];
	float trans_y = xform_data[1];
	float angle = xform_data[2];
	float scale = xform_data[3];
	
	mat4 scale_matrix = mat4(
		vec4(scale, 0, 0, 0),
		vec4(0, scale, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);
	
	mat4 rotation_matrix = mat4(
		vec4(cos(angle), sin(angle), 0, 0),
		vec4(-(sin(angle)), cos(angle), 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);
	
	gl_FrontColor = color_data;	
	
	// scale and rotate
	vec4 newPosition = scale_matrix * rotation_matrix * vec4(x, y, z, 1);
	// translate
	newPosition[0] += trans_x;
	newPosition[1] += trans_y;
	// projection
	if(z <= 0.0) // cameras are cool!
	{
		gl_Position = dynamic_projection * newPosition;
	}
	else // stick it to the screen (UI etc.)
	{
		gl_Position = static_projection * newPosition;
	}
	
	t_coord = tex_data;
}
