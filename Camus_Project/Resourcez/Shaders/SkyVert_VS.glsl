uniform mediump mat4    WorldViewProjection;

varying mediump vec2	varCoord;

attribute mediump vec4	myVertex;
attribute mediump vec2	myUV;

void main(void)
{
gl_Position =  ( WorldViewProjection )*myVertex;
varCoord = myUV.st;
}