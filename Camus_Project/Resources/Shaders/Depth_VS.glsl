uniform highp mat4    WorldViewProjection;
attribute highp vec4	myVertex;

void main(void)
{
gl_Position =  ( WorldViewProjection )*vec4(myVertex.xyz,1.0);

}