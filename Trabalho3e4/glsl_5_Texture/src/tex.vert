
//Autor: Cesar Tadeu Pozzer

//uniform sampler2D texture;
varying float  x_coord;
varying vec4 v_texCoord;

void main()
{
  gl_Position = ftransform();
  v_texCoord = gl_MultiTexCoord0;
  //x_coord = (gl_Position.x / gl_Position.w + 1.0) / 2.0;
}
