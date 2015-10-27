#include "../Include/Cam.h"

#include <iostream>
using namespace std;
#define OTHER 1

Cam::Cam()
{
	// MaxPitch        = D3DXToRadian( 89.0f );
	MaxCamVelocity     = .5f;
	LeftHanded			= true;
	InvertYaxis         = false;
	Rotated		  = false;
	EnableYMovement = true;
	Position        = STDVECTOR3( 0.0f, 0.0f, 0.0f );
	CamVelocity        = STDVECTOR3( 0.0f, 0.0f, 0.0f );
	Look            = STDVECTOR3( 0.0f, 0.0f, 1.0f );
	CreateProjectionMatrix( STD_PI / 3.0f, 1.3f, 1.0f, 1000.0f );
	Direction = NOT_FIXED;


	angx = 0.0f;
	angy = 0.0f;
	angz = 0.0f;

	VecRight = STDVECTOR3( 1.0f, 0.0f, 0.0f );
	VecUp    = STDVECTOR3( 0.0f, 1.0f, 0.0f );
	VecLook  = STDVECTOR3( 0.0f, 0.0f, 1.0f );

	
	OnUpdate(0.0f);
}

void Cam::CreateProjectionMatrix( float fov, float aspect, float nearPlane, float farPlane )
{
	FieldOfView       = fov;
	AspectRatio    = aspect;
	NearPlane = nearPlane;
	FarPlane  = farPlane;

	if(LeftHanded)
		MatPerspectiveLH( &Projection, FieldOfView, AspectRatio, NearPlane, FarPlane );
	else
		MatPerspectiveRH( &Projection, FieldOfView, AspectRatio, NearPlane, FarPlane );
}


void Cam::MoveForward( float units )
{
#if OTHER
		STDVECTOR3 vec_look = STDVECTOR3( 0.0f, 0.0f, 1.0f );
		VecTransformNormal(&vec_look,vec_look,View);
		Position += vec_look*units;
#else
	if ( EnableYMovement )
	{
		if(CamVelocity.Length() < MaxCamVelocity)
			CamVelocity += Look * units;
		else
			CamVelocity = Look * units;
	}
	else
	{
		STDVECTOR3 moveVector( Look.x, 0.0f, Look.z );
		moveVector.Normalize();
		moveVector *= units;
		CamVelocity += moveVector;
	}
#endif
}

void Cam::MoveForward( float units , float elapsedTime )
{
#if OTHER
#else
	static float a = 0.0;

	a+= elapsedTime*10;

	if ( EnableYMovement )
	{
		CamVelocity += Look * units;
	}
	else
	{
		STDVECTOR3 moveVector( Look.x, .05f*sin(a), Look.z );
		moveVector.Normalize();
		moveVector *= units;
		CamVelocity += moveVector;
	}
#endif
}


void Cam::Strafe( float units )
{
#if OTHER
	STDVECTOR3 vec_look = STDVECTOR3( 1.0f, 0.0f, 0.0f );
	VecTransformNormal(&vec_look,vec_look,View);
	Position += vec_look*units;
#else
	CamVelocity += Right * units;
#endif
}


void Cam::MoveUp( float units )
{
#if OTHER
	Position.y += units;
#else
	if ( EnableYMovement )
	{
		CamVelocity.y += units;
	}
#endif
}


void Cam::Yaw( float radians )
{
#if OTHER
	STDMATRIX rotation;
	MatRotationAxis(&rotation,VecUp,radians);
	VecTransformNormal( &VecLook, VecLook, rotation );
	angy -= radians;
#else
	if ( radians == 0.0f )
	{
		return;
	}
	
	if(LeftHanded)
		radians*=-1;

	STDMATRIX rotation;
	MatRotationAxis( &rotation, Up, -radians );
	VecTransformNormal( &Right, Right, rotation );
	VecTransformNormal( &Look, Look, rotation );
#endif
}

void Cam::Pitch( float radians )
{
#if OTHER
	if( (angx-radians) < -MaxPitch || (angx-radians) > MaxPitch)
		return;

	STDMATRIX rotation;
	MatRotationAxis(&rotation,VecRight,radians);
	VecTransformNormal( &VecLook, VecLook, rotation );
	angx -= radians;
#else
	if ( radians == 0.0f )
	{
		return;
	}

	if(LeftHanded)
		radians*=-1;

	radians = (InvertYaxis) ? radians : -radians;
	PitchRad -= radians;
	if ( PitchRad > MaxPitch )
	{
		radians += PitchRad - MaxPitch;
	}
	else if ( PitchRad < -MaxPitch )
	{
		radians += PitchRad + MaxPitch;
	}

	STDMATRIX rotation;
	MatRotationAxis( &rotation, Right, radians );
	VecTransformNormal( &Up, Up, rotation );
	VecTransformNormal( &Look, Look, rotation );
#endif
}


void Cam::Roll( float radians )
{
#if OTHER

#else
	if ( radians == 0.0f )
	{
		return;
	}
	STDMATRIX rotation;
	MatRotationAxis( &rotation, Look, radians );
	VecTransformNormal( &Right, Right, rotation );
	VecTransformNormal( &Up, Up, rotation );
#endif
}

void Cam::OnUpdate(float dt)
{

#if OTHER
	STDMATRIX MatX,MatY,MatTrans,Sc;
	MatRotationX(&MatX,angx);
	MatRotationY(&MatY,angy);
	MatTranslation(&MatTrans,-Position.x,-Position.y,-Position.z);
	MatMultiply(&View,&MatY,&MatX);
	MatMultiply(&View,&MatTrans,&View);
#else
	Position += CamVelocity;
	CamVelocity = STDVECTOR3( 0.0f, 0.0f, 0.0f );
	LookAt = Position + Look;


	STDVECTOR3 up = STDVECTOR3( 0.0f, 1.0f, 0.0f );

	if(LeftHanded){
		MatViewLookAtLH( &View, Position, LookAt, up );

		Right.x = View.a1; 
		Right.y = View.b1;
		Right.z = View.c1;  
		Up.x = View.a2;
		Up.y = View.b2;
		Up.z = View.c2;
		Look.x = View.a3;
		Look.y = View.b3;
		Look.z = View.c3;
	}else{
		MatViewLookAtRH( &View, Position, LookAt, up );

	 	Right.x = View.a1;
	 	Right.y = View.b1;
	 	Right.z = View.c1;
	 	Up.x = View.a2;
	 	Up.y = View.b2;
	 	Up.z = View.c2;
	 	Look.x = -View.a3;
	 	Look.y = -View.b3;
	 	Look.z = -View.c3;
	}
	
	float lookLengthOnXZ = sqrtf( Look.z * Look.z + Look.x * Look.x );
	PitchRad = atan2f( Look.y, lookLengthOnXZ );
	YawRad   = atan2f( Look.x, Look.z );
#endif
}

void Cam::SetPosition( STDVECTOR3* pPosition )
{
	Position.x = pPosition->x;
	Position.y = pPosition->y;
	Position.z = pPosition->z;
}

#include <math.h> 
void Cam::SetLookAt( STDVECTOR3* pLookAt )
{
#if OTHER
	Look = Position - *pLookAt;
	angx = atan2f(Look.y,Look.z);
	angy = atan2f(Look.x,-(Look.z));
	OnUpdate(0.0f);
#else
	LookAt.x = pLookAt->x;
	LookAt.y = pLookAt->y;
	LookAt.z = pLookAt->z;
	Look = STDVECTOR3(LookAt - Position);
	Look.Normalize();
#endif
}

void Cam::SetLook( STDVECTOR3* pLook )
{
	Look.x = pLook->x;
	Look.y = pLook->y;
	Look.z = pLook->z;
	Look.Normalize();
}

void Cam::SetDirection(int d)
{
	if(d == NOT_FIXED){return;}

	STDVECTOR3 vEyePt = STDVECTOR3( 0.0f, 0.0f, 0.0f );
	STDVECTOR3 vLookDir;
	STDVECTOR3 vUpDir;

	switch( d ) //enum Dir { X_POS = 0, X_NEG, Y_POS, Y_NEG, Z_POS, Z_NEG, NOT_FIXED };
	{
	case X_POS:
		vLookDir = STDVECTOR3( 1.0f, 0.0f, 0.0f );
		vUpDir = STDVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case X_NEG:
		vLookDir = STDVECTOR3( -1.0f, 0.0f, 0.0f );
		vUpDir = STDVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case Y_POS:
		vLookDir = STDVECTOR3( 0.0f, 1.0f, 0.0f );
		vUpDir = STDVECTOR3( 0.0f, 0.0f, -1.0f );
		break;
	case Y_NEG:
		vLookDir = STDVECTOR3( 0.0f, -1.0f, 0.0f );
		vUpDir = STDVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case Z_POS:
		vLookDir = STDVECTOR3( 0.0f, 0.0f, 1.0f );
		vUpDir = STDVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case Z_NEG:
		vLookDir = STDVECTOR3( 0.0f, 0.0f, -1.0f );
		vUpDir = STDVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	}

	MatViewLookAtRH( &View, vEyePt, vLookDir, vUpDir );

	switch( d ) //enum Dir { X_POS = 0, X_NEG, Y_POS, Y_NEG, Z_POS, Z_NEG, NOT_FIXED };
	{
	case X_POS:
		View.d1 = Position.z;
		View.d2 = -Position.y;
		View.d3 = Position.x;
		break;
	case X_NEG:
		View.d1 = -Position.z;
		View.d2 = -Position.y;
		View.d3 = Position.x;
		break;
	case Y_POS:
		View.d1 = -Position.x;
		View.d2 = Position.z;
		View.d3 = -Position.y;
		break;
	case Y_NEG:
		View.d1 = -Position.x;
		View.d2 = -Position.z;
		View.d3 = Position.y;
		break;
	case Z_POS:
		View.d1 = -Position.x;
		View.d2 = -Position.y;
		View.d3 = -Position.z;
		break;
	case Z_NEG:
		View.d1 = -Position.x;
		View.d2 = -Position.y;
		View.d3 = Position.z;
		break;
	}
}
