#include "../Include/Cam.h"

const	XVECTOR3	Cam::LookConstCameraSpace = XVECTOR3(0.0f,0.0f,1.0f);
const	XVECTOR3	Cam::RightConstCameraSpace = XVECTOR3(1.0f, 0.0f, 0.0f);
const	XVECTOR3	Cam::UpConstCameraSpace = XVECTOR3(0.0f, 1.0f, 0.0f);

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
	OnUpdate(0.0f);
	CamAcceleration = 50.0f;
	acceleration = XVECTOR3(150.0f,150.0f,150.0f);

	angRoll = 0.0f;
	angPitch = 0.0f;
	angYaw = 0.0f;
	maxspeed = 100.0f;
	maxacceleration = 0.0f;
	maxRoll = 0.0f;
	maxPitch = Deg2Rad(89.0f);
	maxYaw = 0.0f;
}

void Cam::CreateProjectionMatrix( float fov, float aspect, float nearPlane, float farPlane )
{
	FieldOfView       = fov;
	AspectRatio    = aspect;
	NearPlane = nearPlane;
	FarPlane  = farPlane;

	//if(LeftHanded)
		MatPerspectiveLH( &Projection, FieldOfView, AspectRatio, NearPlane, FarPlane );
	//else
	//	MatPerspectiveRH( &Projection, FieldOfView, AspectRatio, NearPlane, FarPlane );
/*
	XMATRIX44 proj;
#if USE_LEFT_HANDED
	XMatPerspectiveLH(proj,FieldOfView,aspect,NearPlane,FarPlane);
#else
	XMatPerspectiveRH(proj,FieldOfView,aspect,NearPlane,FarPlane);
#endif

	for (int i = 0; i < 16 ; i++){
		Projection.mat[i] = proj.mat[i];
	}
*/
}

#include <iostream>
using namespace std;
void Cam::MoveForward( float dt )
{
	velocity.z += acceleration.z*dt;
	
	//if ( EnableYMovement )
	{

	//	Look.Normalize();
	//	CamVelocity += Look*CamAcceleration*dt;
	//	cout << "before " << CamVelocity.Length();
	//	CamVelocity *= 0.945f;
	//	cout << "   after " << CamVelocity.Length() << endl;

		

	//	if(CamVelocity.Length() < MaxCamVelocity)
	//		CamVelocity += Look * units;
	//	else
		//	CamVelocity = Look * units;
	}
	//else
	//{
	//	STDVECTOR3 moveVector( Look.x, 0.0f, Look.z );
	//	moveVector.Normalize();
//		moveVector *= units;
	//	CamVelocity += moveVector;
	//}
}

void Cam::MoveForward( float units , float elapsedTime )
{
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
}


void Cam::Strafe( float dt )
{
//	CamVelocity += Right* CamAcceleration * dt;
//	CamVelocity *= 0.945f;
//	velocity.z += acceleration.z*dt;

	velocity.x += acceleration.x*dt;
}


void Cam::MoveUp( float dt )
{
		velocity.y += acceleration.y*dt;
//	if ( EnableYMovement )
	//{
	//	CamVelocity.y += units;
	//}
}


void Cam::Yaw( float diff )
{
#if USE_LEFT_HANDED
	diff = -diff;
#endif
	if (maxYaw != 0.0) {
		if ((angYaw - diff) > maxYaw || (angYaw - diff) < -maxYaw)
			return;
	}
	angYaw -= diff;
	
// 	if ( radians == 0.0f )
// 	{
// 		return;
// 	}
// 	
// 	if(LeftHanded)
// 		radians*=-1;
// 
// 	STDMATRIX rotation;
// 	MatRotationAxis( &rotation, Up, -radians );
// 	VecTransformNormal( &Right, Right, rotation );
// 	VecTransformNormal( &Look, Look, rotation );

}

void Cam::Pitch( float diff )
{
#if USE_LEFT_HANDED
	diff = -diff;
#endif

	if (maxPitch != 0.0f) {
		if ((angPitch - diff) > maxPitch || (angPitch - diff) < -maxPitch)
			return;
	}

	if(diff==0.0f)
		return;

	angPitch -= diff;

	/*
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
*/
}


void Cam::Roll( float diff )
{
#if USE_LEFT_HANDED
	diff = -diff;
#endif

	if (maxRoll != 0.0) {
		if ((angRoll - diff) > maxRoll || (angRoll + diff) < -maxRoll)
			return;
	}
	angRoll -= diff;
// 	if ( radians == 0.0f )
// 	{
// 		return;
// 	}
// 	STDMATRIX rotation;
// 	MatRotationAxis( &rotation, Look, radians );
// 	VecTransformNormal( &Right, Right, rotation );
// 	VecTransformNormal( &Up, Up, rotation );
}


void Cam::OnUpdate(float dt)
{

	XMATRIX44	X_, Y_, Z_, T_;
	XMatRotationX(X_, angPitch);
	XMatRotationY(Y_, angYaw);
	XMatRotationZ(Z_, angRoll);
	transform = Z_*Y_*X_;

#if USE_LEFT_HANDED
	XMATRIX44 transpose;
	XMatTranspose(transpose,transform);
	XVecTransformNormal(look, LookConstCameraSpace, transpose);
	XVecTransformNormal(up, UpConstCameraSpace, transpose);
	XVecTransformNormal(right, RightConstCameraSpace, transpose);
#else
	XVecTransformNormal(look, LookConstCameraSpace, transform);
	XVecTransformNormal(up, UpConstCameraSpace, transform);
	XVecTransformNormal(right, RightConstCameraSpace, transform);
#endif

	look.Normalize();
	right.Normalize();
	up.Normalize();

	XVECTOR3 currentvelocity = velocity.x*right + velocity.y*up + velocity.z*look;
	velocity -= velocity*0.01f;
	position += currentvelocity*dt;

	XMatTranslation(T_, (-position));
	transform = T_*transform;

	for (int i = 0; i < 16 ; i++){
		View.mat[i] = transform.mat[i];
	}
	

}

void Cam::SetPosition( STDVECTOR3* pPosition )
{
	position.x = Position.x = pPosition->x;
	position.y = Position.y = pPosition->y;
	position.z = Position.z = pPosition->z;
}


void Cam::SetLookAt( STDVECTOR3* pLookAt )
{
	
#if USE_LEFT_HANDED
	look = position - XVECTOR3(pLookAt->x,pLookAt->y,pLookAt->z);
	look.Normalize();
	angPitch = atan2f(-look.y, look.z);
	angYaw = atan2f(look.x, -look.z);
#else
	look = XVECTOR3(pLookAt->x,pLookAt->y,pLookAt->z) - position;
	look.Normalize();
	angPitch = atan2f(-look.y, -look.z);
	angYaw = atan2f(-look.x, look.z);
#endif
	OnUpdate(0.0f);

/*	LookAt.x = pLookAt->x;
	LookAt.y = pLookAt->y;
	LookAt.z = pLookAt->z;
	Look = STDVECTOR3(LookAt - Position);
	Look.Normalize();*/
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
