#include <Scene/Camera.h>



namespace hyperspace {
	namespace scene {

		const	XVECTOR3	Camera::LookConstCameraSpace = XVECTOR3(0.0f,0.0f,1.0f);
		const	XVECTOR3	Camera::RightConstCameraSpace = XVECTOR3(1.0f, 0.0f, 0.0f);
		const	XVECTOR3	Camera::UpConstCameraSpace = XVECTOR3(0.0f, 1.0f, 0.0f);

		void	Camera::SetProjectionPerspective(float &f, float &r, float &n, float &fn) {
			fov = f;	ratio = r;	zn = n;	  zf = fn;
			projtype = projtype_::PERSPECTIVE;
			UpdateProjection();
		}

		void	Camera::SetProjectionOrtho(float &w_, float &h_, float &zn_, float &zf_) {
			w = w_;	h = h_;	zn = zn_;  zf = zf_;
			projtype = projtype_::ORTHOGONAL;
			UpdateProjection();
		}

		void	Camera::SetProjOrientation(orientproj_ &o) {
			if (orientproj != o) {
				orientproj = o;
				float rads = 0.0f;
				switch (orientproj){
					case hyperspace::scene::Camera::LANDSCAPE: {
						rads = 0.0f;
					}break;
					case hyperspace::scene::Camera::PORTRAIT: {
						rads = Deg2Rad(-90.0f);
					}break;
					case hyperspace::scene::Camera::INVERTED_LANDSCAPE: {
						rads = Deg2Rad(180.0f);
					}break;
					case hyperspace::scene::Camera::INVERTED_PORTRAIT: {
						rads = Deg2Rad(90.0f);
					}break;
				}

				XMATRIX44 rot;
				XMatRotationZ(rot,rads);
				UpdateProjection();
				matProj = rot*matProj;
			}
		}

		void	Camera::SetPosition(XVECTOR3 &v) {
			position = v;
			Update();
		}

		void	Camera::MoveOnLookDir(float &dt) {
			velocity.z += acceleration.z*dt;
		}

		void	Camera::MoveOnRight(float &dt) {
			velocity.x += acceleration.x*dt;
		}

		void	Camera::MoveOnUp(float &dt) {
			velocity.y += acceleration.y*dt;
		}

		void	Camera::Update(float dt) {
			XMATRIX44	X_, Y_, Z_, T_;
			XMatRotationX(X_, angPitch);
			XMatRotationY(Y_, angYaw);
			XMatRotationZ(Z_, angRoll);
			transform = Z_*Y_*X_;

#if USE_LEFT_HANDED
			XMATRIX44 transpose;
			XMatTranspose(transpose, transform);
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
			velocity -= velocity*friction;
			position += currentvelocity*dt;

			XMatTranslation(T_, (-position));
			transform = T_*transform;

		}

		void	Camera::UpdateProjection() {
			if (projtype = projtype_::ORTHOGONAL) {
				XMatOrtho(matProj, w, h, zn, zf);
			}
			else {
				XMatPerspective(matProj, fov, ratio, zn, zf);
			}
		}

		void	Camera::SetLookAt(XVECTOR3 &v) {
#if USE_LEFT_HANDED
			look = position - v;
			look.Normalize();
			angPitch = atan2f(-look.y, look.z);
			angYaw = atan2f(look.x, -look.z);
#else
			look = v - position;
			look.Normalize();
			angPitch = atan2f(-look.y, -look.z);
			angYaw = atan2f(-look.x, look.z);
#endif
			Update();
		}

		void	Camera::SetFov(float &f) {
			fov = f;
			UpdateProjection();
		}

		void	Camera::SetRatio(float &f) {
			ratio = f;
			UpdateProjection();
		}

		void	Camera::SetZn(float &f) {
			zn = f;
			UpdateProjection();
		}

		void	Camera::SetZf(float &f) {
			zf = f;
			UpdateProjection();
		}

		void	Camera::RollLimit(float &f) {
			maxRoll = f;
			if (maxRoll == 0.0f) {
				return;
			}
			SetLimit(angRoll, maxRoll);
			Update();
		}

		void	Camera::PitchLimit(float &f) {
			maxPitch = f;
			if (maxPitch == 0.0f) {
				return;
			}
			SetLimit(angPitch, maxPitch);
			Update();
		}

		void	Camera::YawLimit(float &f) {
			maxYaw = f;
			if (maxYaw == 0.0f) {
				return;
			}
			SetLimit(angYaw, maxYaw);
			Update();
		}

		void	Camera::SetLimit(float &current, float limit){
			if (current > limit) {
				current = limit - Deg2Rad(1.0f);
			}
			else if (current < -limit) {
				current = -limit + Deg2Rad(1.0f);
			}
		}
		// Z
		void	Camera::Roll(float diff) {
#if USE_LEFT_HANDED
			diff = -diff;
#endif
			if (maxRoll != 0.0) {
				if ((angRoll - diff) > maxRoll || (angRoll - diff) < -maxRoll)
					return;
			}
			angRoll -= diff;
		}
		// X
		void	Camera::Pitch(float diff) {
#if USE_LEFT_HANDED
			diff = -diff;
#endif
			if (maxPitch != 0.0) {
				if ((angPitch - diff) > maxPitch || (angPitch - diff) < -maxPitch)
					return;
			}
			angPitch -= diff;
		}
		// Y
		void	Camera::Yaw(float diff) {
#if USE_LEFT_HANDED
			diff = -diff;
#endif
			if (maxYaw != 0.0) {
				if ((angYaw - diff) > maxYaw || (angYaw - diff) < -maxYaw)
					return;
			}
			angYaw -= diff;
		}

		void	Camera::ResetCamera() {
			position = XVECTOR3(0.0f, 0.0f, 0.0f);
			look = XVECTOR3(0.0f, 0.0f, 1.0f);
			right = XVECTOR3(1.0f, 0.0f, 0.0f);
			up = XVECTOR3(0.0f, 1.0f, 0.0f);
			velocity = XVECTOR3(0.0f, 0.0f, 0.0f);
			acceleration = XVECTOR3(0.0f, 0.0f, 0.0f);
			fov = Deg2Rad(90.0f);
			ratio = 1.77777777f;
			zn = 1.0f;
			zf = 100000.0f;
			w = 1280.0f;
			h = 720.0f;
			angRoll = 0.0f;
			angPitch = 0.0f;
			angYaw = 0.0f;
			maxspeed = 100.0f;
			maxacceleration = 0.0f;
			friction = 0.01f;
			maxRoll = 0.0f;
			maxPitch = Deg2Rad(89.0f);
			maxYaw = 0.0f;
			type = type_::FLYING;
			orientproj = orientproj_::LANDSCAPE;
			projtype = projtype_::PERSPECTIVE;
			UpdateProjection();
			transform.Identity();
		}

		void	Camera::SetMaxVelocity(float &f) {

		}

		void	Camera::SetMaxAcceleration(float &f) {

		}
	}
}