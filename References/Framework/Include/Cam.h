/*=============================================================================
Cam.h  : Camera class responsible for move and rotate.
Revision history:
* Created by Camus
=============================================================================*/

#ifndef Cam_H
#define Cam_H

#include "FTMaths.h"
#include "xMaths.h"

class Cam
{
public:
	enum { X_POS = 0, X_NEG, Y_POS, Y_NEG, Z_POS, Z_NEG, NOT_FIXED };

	Cam();
	void CreateProjectionMatrix( float fov, float aspect, float nearPlane, float farPlane );
	void MoveForward( float units );
	void MoveForward( float units , float elapsedTime );
	void Strafe( float units );
	void MoveUp( float units );

	void Yaw( float radians );
	void Pitch( float radians );
	void Roll( float radians );

	virtual void OnUpdate(float dt);

	void SetDirection(int d);

	void SetPosition( STDVECTOR3* pPosition );

	void SetLookAt( STDVECTOR3* pLookAt );

	void SetLook( STDVECTOR3* pLookAt );

	void SetFOV( float fov )            { CreateProjectionMatrix( fov, AspectRatio, NearPlane, FarPlane ); }

	void SetAspectRatio( float aspect ) { CreateProjectionMatrix( FieldOfView, aspect, NearPlane, FarPlane ); }

	void SetNearPlane( float plane )    { CreateProjectionMatrix( FieldOfView, AspectRatio, plane, FarPlane ); }

	void SetFarPlane( float plane )     { CreateProjectionMatrix( FieldOfView, AspectRatio, NearPlane, plane ); }

	void SetMaxVelocity( float maxVelocity ) { MaxCamVelocity = maxVelocity; }

	void SetInvertY( bool invert )           { InvertYaxis = invert; }

	void SetMaxPitch( float maxPitch )       { MaxPitch = maxPitch; }

	void SetRotateCam( bool rotar )          { Rotated   = rotar; }

	STDMATRIX* GetViewMatrix()        { return &View; }

	STDMATRIX* GetProjectionMatrix()
	{
		if(Rotated == false)
		{return &Projection;}
		else
		{
			STDMATRIX rotada;
			MatRotationZ(&rotada,Deg2Rad(180.0f));

			return &(rotada*Projection);
		}
	}


	STDVECTOR3* GetPosition()         { return &Position; }

	STDVECTOR3* GetLookAt()           { return &LookAt; }

	STDVECTOR3* GetLook()				{ return &Look; }

	STDVECTOR3* GetRight()				{return &Right; }

	float GetFOV()                     { return FieldOfView; }

	float GetAspectRatio()             { return AspectRatio; }

	float GetNearPlane()               { return NearPlane; }

	float GetFarPlane()                { return FarPlane; }

	float GetMaxVelocity()             { return MaxCamVelocity; }

	bool  GetInvertY()                 { return InvertYaxis; }

	float GetPitch()                   { return PitchRad; }

	float GetYaw()                     { return YawRad; }

	float GetMaxPitch()                { return MaxPitch; }


private:
	STDMATRIX  View;
	STDMATRIX  Projection;
	STDVECTOR3 Right;
	STDVECTOR3 Up;
	STDVECTOR3 Look;
	STDVECTOR3 Position;
	STDVECTOR3 LookAt;
	STDVECTOR3 CamVelocity;
	float		CamAcceleration;
	
	float       YawRad;
	float       PitchRad;
	float       MaxPitch;
	float       MaxCamVelocity;
	float       FieldOfView;
	float       AspectRatio;
	float       NearPlane;
	float       FarPlane;
	bool        InvertYaxis;
	bool        EnableYMovement;
	bool        Rotated;
	bool		LeftHanded;
	int			Direction;


	XVECTOR3		position;
	XVECTOR3		look;
	XVECTOR3		right;
	XVECTOR3		up;

	XVECTOR3		velocity;
	XVECTOR3		acceleration;

	static const	XVECTOR3	LookConstCameraSpace;
	static const	XVECTOR3	RightConstCameraSpace;
	static const	XVECTOR3	UpConstCameraSpace;

	float			angRoll;
	float			angPitch;
	float			angYaw;

	float			maxspeed;
	float			maxacceleration;
	float			maxRoll;
	float			maxPitch;
	float			maxYaw;

	XMATRIX44	transform;
};

#endif
