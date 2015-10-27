#ifndef CA_CAMERA_H
#define CA_CAMERA_H

#include <Scene/Node.h>

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
				maxacceleration(0.0f),
				maxRoll(0.0f),
				maxPitch(Deg2Rad(89.0f)),
				maxYaw(0.0f),
				type(type_::FLYING),
				orientproj(orientproj_::LANDSCAPE),
				projtype(projtype_::PERSPECTIVE)
			{
				SetProjectionPerspective(fov, ratio, zn, zf);
			}

			void	SetProjectionPerspective(float &f, float &r, float &zn, float &zf);
			void	SetProjectionOrtho(float &w, float &h, float &zn, float &zf);
			void	SetProjOrientation(orientproj_ &o);

			void	SetPosition(XVECTOR3 &v);
			void	SetLookAt(XVECTOR3 &v);
			void	SetFov(float &f);
			void	SetRatio(float &f);
			void	SetZn(float &f);
			void	SetZf(float &f);

			void	RollLimit(float &f);
			void	PitchLimit(float &f);
			void	YawLimit(float &f);

			void	Roll(float &diff);
			void	Pitch(float &diff);
			void	Yaw(float &diff);

			void	SetMaxVelocity(float &f);
			void	SetMaxAcceleration(float &f);

			void	Update(float &f);

			XMATRIX44		matProj;

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
			float			maxacceleration;
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