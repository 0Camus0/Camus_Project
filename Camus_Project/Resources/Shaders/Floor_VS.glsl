uniform mediump mat4    WorldViewProjection;
uniform highp   mat4    LightWorldViewProjection;
uniform mediump mat4	World;
uniform mediump vec3	myLightDirection;
uniform mediump	 vec3	CamPos;

varying mediump vec2	varCoord;
varying lowp	vec3	varNormal;
varying lowp	vec3	varTangent;
varying lowp	vec3	varBinormal;
varying mediump vec3	WorldPos;
varying mediump	vec3	camPos;

varying lowp	vec4	RasterPos;
varying highp	vec4	RasterPosbyLight;

attribute highp  vec4	myVertex;
attribute lowp	  vec3	myNormal;
attribute lowp    vec3  myTanget;
attribute lowp    vec3  myBinormal;
attribute mediump vec2	myUV;

void main(void)
{


RasterPos =  ( WorldViewProjection )*myVertex;

RasterPosbyLight =  ( LightWorldViewProjection )*myVertex;

gl_Position =  RasterPos;

lowp mat3 world = mat3(World);

WorldPos = vec3(World * myVertex);

varNormal = normalize(world * myNormal);
varTangent = normalize(world * myTanget);
varBinormal = normalize(world * myBinormal);


camPos = CamPos;

varCoord = myUV.st;

}