#version 110

varying vec2 t_coord;
uniform sampler2D t_color;

void main()
{
   gl_FragColor = gl_Color * texture2D(t_color, t_coord);
}