#ifndef CA_CAMERA_H
#define CA_CAMERA_H

#include <Scene/Node.h>
#include <Utils/MemoryTracker.h>

namespace hyperspace {
	namespace scene {
		class Camera : public CNode_ {
		public:
			enum type_ {
				FLYING,
				FPS,
				SPACESHIP,
				TARGET,
				TARGET_ORTHO
			};

			enum orientproj_ {
				LANDSCAPE = 0,
				PORTRAIT,
				INVERTED_LANDSCAPE,
				INVERTED_PORTRAIT
			};
			
			enum projtype_ {
				ORTHOGONAL = 0,
				PERSPECTIVE
			};

			Camera() : 
				CNode_(NodeType_::CAMERA),
				position(XVECTOR3(0.0f,0.0f,0.0f)),
				look(XVECTOR3(0.0f,0.0f,1.0f)),
				right(XVECTOR3(1.0f,0.0f,0.0f)),
				up(XVECTOR3(0.0f,1.0f,0.0f)),
				velocity(XVECTOR3(0.0f,0.0f,0.0f)),
				acceleration(XVECTOR3(0.0f,0.0f,0.0f)),
				fov(Deg2Rad(90.0f)),
				ratio(1.77777777f),
				zn(1.0f),
				zf(100000.0f),
				w(1280.0f),
				h(720.0f),
				angRoll(0.0f),
				angPitch(0.0f),
				angYaw(0.0f),
				maxspeed(100.0f),
				friction(0.01f),
				maxRoll(0.0f),
				maxPitch(Deg2Rad(89.0f)),
				maxYaw(0.0f),
				type(type_::FLYING),
				orientproj(orientproj_::LANDSCAPE),
				projtype(projtype_::PERSPECTIVE)
			{
				UpdateProjection();
				MemAppendHeap(Camera);
			}

			void	SetProjectionPerspective(float &f, float &r, float &zn, float &zf);
			void	SetProjectionOrtho(float &w, float &h, float &zn, float &zf);
			void	SetProjOrientation(orientproj_ &o);
			void	UpdateProjection();

			void	SetPosition(XVECTOR3 &v);
			void	SetLookAt(XVECTOR3 &v);
			void	SetFov(float &f);
			void	SetRatio(float &f);
			void	SetZn(float &f);
			void	SetZf(float &f);

			void	RollLimit(float &f);
			void	PitchLimit(float &f);
			void	YawLimit(float &f);
			void	SetLimit(float &current,float limit);

			void	Roll(float diff);
			void	Pitch(float diff);
			void	Yaw(float diff);

			void	MoveOnLookDir(float &dt);
			void	MoveOnRight(float &dt);
			void	MoveOnUp(float &dt);

			void	SetMaxVelocity(float &f);

			void	ResetCamera();

			void	Update(float dt=0.0f);
			
			inline XMATRIX44& GetViewMat() {
				return transform;
			}

			inline XMATRIX44& GetMatViewProj() {
				return matViewProj;
			}

			XMATRIX44		matProj;
			XMATRIX44		matViewProj;

			XVECTOR3		position;
			XVECTOR3		look;
			XVECTOR3		right;
			XVECTOR3		up;

			XVECTOR3		velocity;
			XVECTOR3		acceleration;

			static const	XVECTOR3	LookConstCameraSpace;
			static const	XVECTOR3	RightConstCameraSpace;
			static const	XVECTOR3	UpConstCameraSpace;

			float			fov;
			float			ratio;
			float			zn;
			float			zf;
			float			w;
			float			h;

			float			angRoll;
			float			angPitch;
			float			angYaw;

			float			maxspeed;
			float			friction;
			float			maxRoll;
			float			maxPitch;
			float			maxYaw;

			type_			type;
			orientproj_		orientproj;
			projtype_		projtype;

			


		};
	}
}


#endif