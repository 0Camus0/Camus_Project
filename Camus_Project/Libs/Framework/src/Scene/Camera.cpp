#include <Scene/Camera.h>

#define USE_LEFT_HANDED 1

namespace hyperspace {
	namespace scene {
		void	Camera::SetProjectionPerspective(float &f, float &r, float &n, float &f) {
			fov = f;	ratio = r;	zn = n;	  zf = f;
#if USE_LEFT_HANDED
			XMatPerspectiveLH(matProj, fov, ratio, zn, zf);
#else
			XMatPerspectiveRH(matProj, fov, ratio, zn, zf);
#endif
		}

		void	Camera::SetProjectionOrtho(float &w, float &h, float &zn, float &zf) {

		}

		void	Camera::SetProjOrientation(orientproj_ &o) {

		}

		void	Camera::SetPosition(XVECTOR3 &v) {

		}

		void	Camera::Update(float &f) {

		}

		void	Camera::SetLookAt(XVECTOR3 &v) {

		}

		void	Camera::SetFov(float &f) {

		}

		void	Camera::SetRatio(float &f) {

		}

		void	Camera::SetZn(float &f) {

		}

		void	Camera::SetZf(float &f) {

		}

		void	Camera::RollLimit(float &f) {

		}

		void	Camera::PitchLimit(float &f) {

		}

		void	Camera::YawLimit(float &f) {

		}

		void	Camera::Roll(float &diff) {

		}

		void	Camera::Pitch(float &diff) {

		}

		void	Camera::Yaw(float &diff) {

		}

		void	Camera::SetMaxVelocity(float &f) {

		}

		void	Camera::SetMaxAcceleration(float &f) {

		}
	}
}