#include <Utils/xFile/xMesh.h>
#include <Utils/Time.h>

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <Driver/EffectGL.h>


using namespace hyperspace;
namespace xF {

	AnimationController::AnimationController(xF::xAnimationInfo* aInfo, xF::xSkeleton *skInfoBind, xF::xSkeleton *skInfoOut) :m_pAInfo(aInfo),/* m_pSkeletonBind(skInfoBind),*/ m_pSkeletonOut(skInfoOut) {
		Ticks = 4800;
		msPerTick = 4800.0;
		m_Speed = 1.0;

		m_ActualAnimationSet = 0;
		m_LoopingAnimations = false;
		m_LerpAnimations = false;
		m_TotalAnimationsSets = m_pAInfo->Animations.size();
		
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
		
#if !BYPASS_TIMER
		m_Timer.Start();
#endif
	}

	void AnimationController::ResetAnimationSet() {
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
	}

	void AnimationController::ToogleLoop() {
		m_LoopingAnimations = !m_LoopingAnimations;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
	}

	void AnimationController::ToogleLerp() {
		m_LerpAnimations = !m_LerpAnimations;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
	}

	void AnimationController::IncreaseSpeed() {
		msPerTick *= 2.0f;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
	}

	void AnimationController::ReduceeSpeed() {
		msPerTick *= 0.5f;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
	}

	void AnimationController::ResetLocals(int bitflag) {
		if (bitflag == 0)
			return;

		for (unsigned int i = 0; i < m_pAInfo->Animations.size(); i++) {
			xF::xAnimationSet *pActualAS = &m_pAInfo->Animations[i];
			for (unsigned int j = 0; j < pActualAS->BonesRef.size(); j++) {
				xF::xAnimationBone *pActualBone = &pActualAS->BonesRef[j];

				if (bitflag&AnimationController::POSITION_KEY) {
					pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::START;
					pActualBone->ActualKey.LocalIndexPos = 0;
					pActualBone->ActualKey.LocaltimePos = 0.0;
					pActualBone->ActualKey.LocaltimePosLerp = 0.0;
					pActualBone->ActualKey.MaxIndexPos = pActualBone->PositionKeys.size();
				}
				if (bitflag&AnimationController::ROTATION_KEY) {
					pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::START;
					pActualBone->ActualKey.LocalIndexRot = 0;
					pActualBone->ActualKey.LocaltimeRot = 0.0;
					pActualBone->ActualKey.LocaltimeRotLerp = 0.0;
					pActualBone->ActualKey.MaxIndexRot = pActualBone->RotationKeys.size();
				}
				if (bitflag&AnimationController::SCALE_KEY) {
					pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::START;
					pActualBone->ActualKey.LocalIndexSc = 0;
					pActualBone->ActualKey.LocaltimeSc = 0.0;
					pActualBone->ActualKey.LocaltimeScLerp = 0.0;
					pActualBone->ActualKey.MaxIndexSc = pActualBone->ScaleKeys.size();
				}

			}
		}
	}

	void AnimationController::NextAnimationSet() {
		m_ActualAnimationSet++;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
		if (m_ActualAnimationSet >= m_TotalAnimationsSets) {
			m_ActualAnimationSet = 0;
		}
	}

	void AnimationController::PrevAnimationSet() {
		m_ActualAnimationSet--;
		ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
		if (m_ActualAnimationSet <= 0)
			m_ActualAnimationSet = m_TotalAnimationsSets - 1;
	}

	void AnimationController::Update() {
#if !BYPASS_TIMER
		m_Timer.Update();
#endif
		if (m_LerpAnimations)
			UpdateLerp();
		else
			UpdateNoLerp();
	}

	void AnimationController::UpdateNoLerp() {
#if BYPASS_TIMER
		float CurrentElapsedTime = 0.016f;
#else
		float CurrentElapsedTime = m_Timer.GetElapsedTime();
#endif
		xF::xAnimationSet *pActualAS = &m_pAInfo->Animations[m_ActualAnimationSet];
		for (unsigned int j = 0; j < pActualAS->BonesRef.size(); j++) {
			xF::xAnimationBone *pActualBone = &pActualAS->BonesRef[j];
			pActualBone->ActualKey.LocaltimePos += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeRot += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeSc += CurrentElapsedTime;

			pActualBone->ActualKey.LocaltimePosLerp += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeRotLerp += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeScLerp += CurrentElapsedTime;

			if (pActualBone->ActualKey.MaxIndexPos == 1) {
				pActualBone->ActualKey.PositionKey.Position.x = pActualBone->PositionKeys[0].Position.x;
				pActualBone->ActualKey.PositionKey.Position.y = pActualBone->PositionKeys[0].Position.y;
				pActualBone->ActualKey.PositionKey.Position.z = pActualBone->PositionKeys[0].Position.z;
				pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexPos + 1) >= pActualBone->ActualKey.MaxIndexPos);
				if (!ShouldReset) {
					pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::RUNNING;

					xPositionKey *pActualPosKey = &pActualBone->PositionKeys[pActualBone->ActualKey.LocalIndexPos];
					xPositionKey *pNextPosKey = &pActualBone->PositionKeys[pActualBone->ActualKey.LocalIndexPos + 1];

					long NextTick = pNextPosKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimePos);
					//double Unit = static_cast<double>(pNextPosKey->t.i_atTime - pActualPosKey->t.i_atTime) / msPerTick;	// TOCHECK;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexPos++;
						pActualBone->ActualKey.PositionKey.Position.x = pNextPosKey->Position.x;
						pActualBone->ActualKey.PositionKey.Position.y = pNextPosKey->Position.y;
						pActualBone->ActualKey.PositionKey.Position.z = pNextPosKey->Position.z;
						pActualBone->ActualKey.LocaltimePosLerp = 0.0;
					}
					else {
						float weight = 0.0; //pActualBone->ActualKey.LocaltimePosLerp/Unit;
						pActualBone->ActualKey.PositionKey.Position.x = pActualPosKey->Position.x + (pNextPosKey->Position.x - pActualPosKey->Position.x)*weight;
						pActualBone->ActualKey.PositionKey.Position.y = pActualPosKey->Position.y + (pNextPosKey->Position.y - pActualPosKey->Position.y)*weight;
						pActualBone->ActualKey.PositionKey.Position.z = pActualPosKey->Position.z + (pNextPosKey->Position.z - pActualPosKey->Position.z)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.PositionKey.Position.x = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.x;
					pActualBone->ActualKey.PositionKey.Position.y = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.y;
					pActualBone->ActualKey.PositionKey.Position.z = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.z;
				}
			}

			if (pActualBone->ActualKey.MaxIndexRot == 1) {
				pActualBone->ActualKey.RotationKey.Rot.x = pActualBone->RotationKeys[0].Rot.x;
				pActualBone->ActualKey.RotationKey.Rot.y = pActualBone->RotationKeys[0].Rot.y;
				pActualBone->ActualKey.RotationKey.Rot.z = pActualBone->RotationKeys[0].Rot.z;
				pActualBone->ActualKey.RotationKey.Rot.w = pActualBone->RotationKeys[0].Rot.w;
				pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexRot + 1) >= pActualBone->ActualKey.MaxIndexRot);
				if (!ShouldReset) {
					pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::RUNNING;
					xRotationKey *pActualRotKey = &pActualBone->RotationKeys[pActualBone->ActualKey.LocalIndexRot];
					xRotationKey *pNextRotKey = &pActualBone->RotationKeys[pActualBone->ActualKey.LocalIndexRot + 1];

					long NextTick = pNextRotKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimeRot);
				//	float Unit = static_cast<float>((pNextRotKey->t.i_atTime - pActualRotKey->t.i_atTime) / msPerTick); // TOCHECK;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexRot++;
						pActualBone->ActualKey.RotationKey.Rot.x = pNextRotKey->Rot.x;
						pActualBone->ActualKey.RotationKey.Rot.y = pNextRotKey->Rot.y;
						pActualBone->ActualKey.RotationKey.Rot.z = pNextRotKey->Rot.z;
						pActualBone->ActualKey.RotationKey.Rot.w = pNextRotKey->Rot.w;
						pActualBone->ActualKey.LocaltimeRotLerp = 0.0;
					}
					else {
						float weight = 0.0; //pActualBone->ActualKey.LocaltimeRotLerp/Unit;
						pActualBone->ActualKey.RotationKey.Rot.x = pActualRotKey->Rot.x + (pNextRotKey->Rot.x - pActualRotKey->Rot.x)*weight;
						pActualBone->ActualKey.RotationKey.Rot.y = pActualRotKey->Rot.y + (pNextRotKey->Rot.y - pActualRotKey->Rot.y)*weight;
						pActualBone->ActualKey.RotationKey.Rot.z = pActualRotKey->Rot.z + (pNextRotKey->Rot.z - pActualRotKey->Rot.z)*weight;
						pActualBone->ActualKey.RotationKey.Rot.w = pActualRotKey->Rot.w + (pNextRotKey->Rot.w - pActualRotKey->Rot.w)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.RotationKey.Rot.x = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.x;
					pActualBone->ActualKey.RotationKey.Rot.y = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.y;
					pActualBone->ActualKey.RotationKey.Rot.z = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.z;
					pActualBone->ActualKey.RotationKey.Rot.w = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.w;
				}
			}

			if (pActualBone->ActualKey.MaxIndexSc == 1) {
				pActualBone->ActualKey.ScaleKey.Scale.x = pActualBone->ScaleKeys[0].Scale.x;
				pActualBone->ActualKey.ScaleKey.Scale.y = pActualBone->ScaleKeys[0].Scale.y;
				pActualBone->ActualKey.ScaleKey.Scale.z = pActualBone->ScaleKeys[0].Scale.z;
				pActualBone->ActualKey.ScaleKey.Scale.w = pActualBone->ScaleKeys[0].Scale.w;
				pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexSc + 1) >= pActualBone->ActualKey.MaxIndexSc);
				if (!ShouldReset) {
					pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::RUNNING;
					xScaleKey *pActualScaleKey = &pActualBone->ScaleKeys[pActualBone->ActualKey.LocalIndexSc];
					xScaleKey *pNextScaleKey = &pActualBone->ScaleKeys[pActualBone->ActualKey.LocalIndexSc + 1];

					long NextTick = pNextScaleKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimeSc);
				//	float Unit = static_cast<float>(pNextScaleKey->t.i_atTime - pActualScaleKey->t.i_atTime) / msPerTick; // TOCHECK;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexRot++;
						pActualBone->ActualKey.ScaleKey.Scale.x = pNextScaleKey->Scale.x;
						pActualBone->ActualKey.ScaleKey.Scale.y = pNextScaleKey->Scale.y;
						pActualBone->ActualKey.ScaleKey.Scale.z = pNextScaleKey->Scale.z;
						pActualBone->ActualKey.LocaltimeScLerp = 0.0;
					}
					else {
						float weight = 0.0;//pActualBone->ActualKey.LocaltimeScLerp/Unit;
						pActualBone->ActualKey.ScaleKey.Scale.x = pActualScaleKey->Scale.x + (pNextScaleKey->Scale.x - pActualScaleKey->Scale.x)*weight;
						pActualBone->ActualKey.ScaleKey.Scale.y = pActualScaleKey->Scale.y + (pNextScaleKey->Scale.y - pActualScaleKey->Scale.y)*weight;
						pActualBone->ActualKey.ScaleKey.Scale.z = pActualScaleKey->Scale.z + (pNextScaleKey->Scale.z - pActualScaleKey->Scale.z)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.ScaleKey.Scale.x = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.x;
					pActualBone->ActualKey.ScaleKey.Scale.y = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.y;
					pActualBone->ActualKey.ScaleKey.Scale.z = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.z;

				}
			}

			XMATRIX44	Rotation;
			{
				XQUATERNION q = pActualBone->ActualKey.RotationKey.Rot;
				float x2 = q.x * q.x;
				float y2 = q.y * q.y;
				float z2 = q.z * q.z;
				float xy = q.x * q.y;
				float xz = q.x * q.z;
				float yz = q.y * q.z;
				float wx = q.w * q.x;
				float wy = q.w * q.y;
				float wz = q.w * q.z;

				XMATRIX44	Temp(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
					2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
					2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);


				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Rotation.m[i][j] = Temp.m[i][j];
					}
				}
			}

			XMATRIX44 Position;
			{
				XVECTOR3 p = pActualBone->ActualKey.PositionKey.Position;
				XMatTranslation(Position, p.x, p.y, p.z);
			}

			XMATRIX44 Scale;
			{
				XVECTOR3 sc = pActualBone->ActualKey.ScaleKey.Scale;
				XMatScaling(Scale, sc.x, sc.y, sc.z);
			}
			pActualBone->MatrixfromKeys = Scale*Rotation*Position;


			m_pSkeletonOut->Bones[pActualBone->BoneID].Bone = pActualBone->MatrixfromKeys;

		}

		if (m_LoopingAnimations) {
			bool ShouldRestart = true;
			for (unsigned int j = 0; j < pActualAS->BonesRef.size(); j++) {
				xF::xAnimationBone *pActualBone = &pActualAS->BonesRef[j];
				if (pActualBone->ActualKey.StatePos != xF::xAnimationSingleKey::FINISHED ||
					pActualBone->ActualKey.StateRot != xF::xAnimationSingleKey::FINISHED ||
					pActualBone->ActualKey.StateSc != xF::xAnimationSingleKey::FINISHED)
					ShouldRestart = false;
			}

			if (ShouldRestart)
				ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
		}
	}

	void AnimationController::UpdateLerp() {
#if BYPASS_TIMER
		float CurrentElapsedTime = 0.016f;
#else
		float CurrentElapsedTime = m_Timer.GetElapsedTime();
#endif
		xF::xAnimationSet *pActualAS = &m_pAInfo->Animations[m_ActualAnimationSet];
		for (unsigned int j = 0; j < pActualAS->BonesRef.size(); j++) {
			xF::xAnimationBone *pActualBone = &pActualAS->BonesRef[j];
			pActualBone->ActualKey.LocaltimePos += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeRot += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeSc += CurrentElapsedTime;

			pActualBone->ActualKey.LocaltimePosLerp += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeRotLerp += CurrentElapsedTime;
			pActualBone->ActualKey.LocaltimeScLerp += CurrentElapsedTime;

			if (pActualBone->ActualKey.MaxIndexPos == 1) {
				pActualBone->ActualKey.PositionKey.Position.x = pActualBone->PositionKeys[0].Position.x;
				pActualBone->ActualKey.PositionKey.Position.y = pActualBone->PositionKeys[0].Position.y;
				pActualBone->ActualKey.PositionKey.Position.z = pActualBone->PositionKeys[0].Position.z;
				pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexPos + 1) >= pActualBone->ActualKey.MaxIndexPos);
				if (!ShouldReset) {
					pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::RUNNING;
					xPositionKey *pActualPosKey = &pActualBone->PositionKeys[pActualBone->ActualKey.LocalIndexPos];
					xPositionKey *pNextPosKey = &pActualBone->PositionKeys[pActualBone->ActualKey.LocalIndexPos + 1];

					long NextTick = pNextPosKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimePos);
					float Unit = static_cast<float>(pNextPosKey->t.i_atTime - pActualPosKey->t.i_atTime) / msPerTick;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexPos++;
						pActualBone->ActualKey.PositionKey.Position.x = pNextPosKey->Position.x;
						pActualBone->ActualKey.PositionKey.Position.y = pNextPosKey->Position.y;
						pActualBone->ActualKey.PositionKey.Position.z = pNextPosKey->Position.z;
						pActualBone->ActualKey.LocaltimePosLerp = 0.0;
					}
					else {
						float weight = pActualBone->ActualKey.LocaltimePosLerp / Unit;
						pActualBone->ActualKey.PositionKey.Position.x = pActualPosKey->Position.x + (pNextPosKey->Position.x - pActualPosKey->Position.x)*weight;
						pActualBone->ActualKey.PositionKey.Position.y = pActualPosKey->Position.y + (pNextPosKey->Position.y - pActualPosKey->Position.y)*weight;
						pActualBone->ActualKey.PositionKey.Position.z = pActualPosKey->Position.z + (pNextPosKey->Position.z - pActualPosKey->Position.z)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StatePos = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.PositionKey.Position.x = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.x;
					pActualBone->ActualKey.PositionKey.Position.y = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.y;
					pActualBone->ActualKey.PositionKey.Position.z = pActualBone->PositionKeys[pActualBone->ActualKey.MaxIndexPos - 1].Position.z;
				}
			}

			if (pActualBone->ActualKey.MaxIndexRot == 1) {
				pActualBone->ActualKey.RotationKey.Rot.x = pActualBone->RotationKeys[0].Rot.x;
				pActualBone->ActualKey.RotationKey.Rot.y = pActualBone->RotationKeys[0].Rot.y;
				pActualBone->ActualKey.RotationKey.Rot.z = pActualBone->RotationKeys[0].Rot.z;
				pActualBone->ActualKey.RotationKey.Rot.w = pActualBone->RotationKeys[0].Rot.w;
				pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexRot + 1) >= pActualBone->ActualKey.MaxIndexRot);
				if (!ShouldReset) {
					pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::RUNNING;
					xRotationKey *pActualRotKey = &pActualBone->RotationKeys[pActualBone->ActualKey.LocalIndexRot];
					xRotationKey *pNextRotKey = &pActualBone->RotationKeys[pActualBone->ActualKey.LocalIndexRot + 1];

					long NextTick = pNextRotKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimeRot);
					float Unit = static_cast<float>(pNextRotKey->t.i_atTime - pActualRotKey->t.i_atTime) / msPerTick;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexRot++;
						pActualBone->ActualKey.RotationKey.Rot.x = pNextRotKey->Rot.x;
						pActualBone->ActualKey.RotationKey.Rot.y = pNextRotKey->Rot.y;
						pActualBone->ActualKey.RotationKey.Rot.z = pNextRotKey->Rot.z;
						pActualBone->ActualKey.RotationKey.Rot.w = pNextRotKey->Rot.w;
						pActualBone->ActualKey.LocaltimeRotLerp = 0.0;
					}
					else {
						float weight = pActualBone->ActualKey.LocaltimeRotLerp / Unit;
						pActualBone->ActualKey.RotationKey.Rot.x = pActualRotKey->Rot.x + (pNextRotKey->Rot.x - pActualRotKey->Rot.x)*weight;
						pActualBone->ActualKey.RotationKey.Rot.y = pActualRotKey->Rot.y + (pNextRotKey->Rot.y - pActualRotKey->Rot.y)*weight;
						pActualBone->ActualKey.RotationKey.Rot.z = pActualRotKey->Rot.z + (pNextRotKey->Rot.z - pActualRotKey->Rot.z)*weight;
						pActualBone->ActualKey.RotationKey.Rot.w = pActualRotKey->Rot.w + (pNextRotKey->Rot.w - pActualRotKey->Rot.w)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StateRot = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.RotationKey.Rot.x = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.x;
					pActualBone->ActualKey.RotationKey.Rot.y = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.y;
					pActualBone->ActualKey.RotationKey.Rot.z = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.z;
					pActualBone->ActualKey.RotationKey.Rot.w = pActualBone->RotationKeys[pActualBone->ActualKey.MaxIndexRot - 1].Rot.w;
				}
			}

			if (pActualBone->ActualKey.MaxIndexSc == 1) {
				pActualBone->ActualKey.ScaleKey.Scale.x = pActualBone->ScaleKeys[0].Scale.x;
				pActualBone->ActualKey.ScaleKey.Scale.y = pActualBone->ScaleKeys[0].Scale.y;
				pActualBone->ActualKey.ScaleKey.Scale.z = pActualBone->ScaleKeys[0].Scale.z;
				pActualBone->ActualKey.ScaleKey.Scale.w = pActualBone->ScaleKeys[0].Scale.w;
				pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::FINISHED;
			}
			else {
				bool ShouldReset = ((pActualBone->ActualKey.LocalIndexSc + 1) >= pActualBone->ActualKey.MaxIndexSc);
				if (!ShouldReset) {
					pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::RUNNING;
					xScaleKey *pActualScaleKey = &pActualBone->ScaleKeys[pActualBone->ActualKey.LocalIndexSc];
					xScaleKey *pNextScaleKey = &pActualBone->ScaleKeys[pActualBone->ActualKey.LocalIndexSc + 1];

					long NextTick = pNextScaleKey->t.i_atTime;
					long ActualTick = static_cast<long>(msPerTick*pActualBone->ActualKey.LocaltimeSc);
					float Unit = static_cast<float>(pNextScaleKey->t.i_atTime - pActualScaleKey->t.i_atTime) / msPerTick;

					if (ActualTick >= NextTick) {
						pActualBone->ActualKey.LocalIndexRot++;
						pActualBone->ActualKey.ScaleKey.Scale.x = pNextScaleKey->Scale.x;
						pActualBone->ActualKey.ScaleKey.Scale.y = pNextScaleKey->Scale.y;
						pActualBone->ActualKey.ScaleKey.Scale.z = pNextScaleKey->Scale.z;
						pActualBone->ActualKey.LocaltimeScLerp = 0.0;
					}
					else {
						float weight = pActualBone->ActualKey.LocaltimeScLerp / Unit;
						pActualBone->ActualKey.ScaleKey.Scale.x = pActualScaleKey->Scale.x + (pNextScaleKey->Scale.x - pActualScaleKey->Scale.x)*weight;
						pActualBone->ActualKey.ScaleKey.Scale.y = pActualScaleKey->Scale.y + (pNextScaleKey->Scale.y - pActualScaleKey->Scale.y)*weight;
						pActualBone->ActualKey.ScaleKey.Scale.z = pActualScaleKey->Scale.z + (pNextScaleKey->Scale.z - pActualScaleKey->Scale.z)*weight;
					}
				}
				else {
					pActualBone->ActualKey.StateSc = xF::xAnimationSingleKey::FINISHED;
					pActualBone->ActualKey.ScaleKey.Scale.x = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.x;
					pActualBone->ActualKey.ScaleKey.Scale.y = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.y;
					pActualBone->ActualKey.ScaleKey.Scale.z = pActualBone->ScaleKeys[pActualBone->ActualKey.MaxIndexSc - 1].Scale.z;
				}
			}

			XMATRIX44	Rotation;
			{
				XQUATERNION q = pActualBone->ActualKey.RotationKey.Rot;
				float x2 = q.x * q.x;
				float y2 = q.y * q.y;
				float z2 = q.z * q.z;
				float xy = q.x * q.y;
				float xz = q.x * q.z;
				float yz = q.y * q.z;
				float wx = q.w * q.x;
				float wy = q.w * q.y;
				float wz = q.w * q.z;

				XMATRIX44	Temp(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
					2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
					2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						Rotation.m[i][j] = Temp.m[i][j];
					}
				}
			}

			XMATRIX44 Position;
			{
				XVECTOR3 p = pActualBone->ActualKey.PositionKey.Position;
				XMatTranslation(Position, p.x, p.y, p.z);
			}

			XMATRIX44 Scale;
			{
				XVECTOR3 sc = pActualBone->ActualKey.ScaleKey.Scale;
				XMatScaling(Scale, sc.x, sc.y, sc.z);
			}
			pActualBone->MatrixfromKeys = Scale*Rotation*Position;


			m_pSkeletonOut->Bones[pActualBone->BoneID].Bone = pActualBone->MatrixfromKeys;
		}

		if (m_LoopingAnimations) {
			bool ShouldRestart = true;
			for (unsigned int j = 0; j < pActualAS->BonesRef.size(); j++) {
				xF::xAnimationBone *pActualBone = &pActualAS->BonesRef[j];
				if (pActualBone->ActualKey.StatePos != xF::xAnimationSingleKey::FINISHED ||
					pActualBone->ActualKey.StateRot != xF::xAnimationSingleKey::FINISHED ||
					pActualBone->ActualKey.StateSc != xF::xAnimationSingleKey::FINISHED)
					ShouldRestart = false;
			}

			if (ShouldRestart)
				ResetLocals(AnimationController::POSITION_KEY | AnimationController::ROTATION_KEY | AnimationController::SCALE_KEY);
		}
	}

	void xMesh::CreateBuffers() {
		Desc.bAlignMemory = true;
		Desc.bHardwareSkinning = false;
		BuildSubsets();
	}

	void xMesh::BuildSubsets() {
		const std::size_t size_geometry = Geometry.size();
		
		for (unsigned int i = 0; i < size_geometry; i++) {
			unsigned int VertexSizeinBytes = 0;
			unsigned int VectorSizeinBytes = 0;

			if (Desc.bAlignMemory)
				VectorSizeinBytes = 16;
			else
				VectorSizeinBytes = 12;

			xMeshGeometry *pActual = &Geometry[i];

			video::CTechnique_ *technique = new video::TechniqueGL();

			if(pActual->Info.SkinMeshHeader.NumBones>0){
				technique->Initialize("Animated", "Anim");
			}else{
				technique->Initialize("Static", "All");
			}
	
			if (pActual->VertexAttributes&xMeshGeometry::HAS_POSITION) {
				VertexSizeinBytes += VectorSizeinBytes;
				technique->BindAttribute("Position_", 0, hyperspace::video::shader::bind_::POSITION);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_NORMAL) {
				VertexSizeinBytes += VectorSizeinBytes;
				technique->BindAttribute("Normal_", 0, hyperspace::video::shader::bind_::NORMAL);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
				VertexSizeinBytes += VectorSizeinBytes;
				technique->BindAttribute("Tangent_", 0, hyperspace::video::shader::bind_::TANGENT);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_BINORMAL) {
				VertexSizeinBytes += VectorSizeinBytes;
				technique->BindAttribute("Binormal_", 0, hyperspace::video::shader::bind_::BINORMAL);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD0) {
				VertexSizeinBytes += 8;
				technique->BindAttribute("Texcoord0_", 0, hyperspace::video::shader::bind_::TEXCOORD_0);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD1) {
				VertexSizeinBytes += 8;
				technique->BindAttribute("Texcoord1_", 0, hyperspace::video::shader::bind_::TEXCOORD_1);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD2) {
				VertexSizeinBytes += 8;
				technique->BindAttribute("Texcoord2_", 0, hyperspace::video::shader::bind_::TEXCOORD_2);
			}

			if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD3) {
				VertexSizeinBytes += 8;
				technique->BindAttribute("Texcoord3_", 0, hyperspace::video::shader::bind_::TEXCOORD_3);
			}

			xFinalGeometry tmpGeo;
			pActual->VertexSize = tmpGeo.VertexSize = VertexSizeinBytes;
			unsigned int NumFloatsPerVertex = VertexSizeinBytes / 4;
			unsigned int NumFloatsPerGeometry = NumFloatsPerVertex*pActual->NumVertices;
			unsigned int NumComponents = VectorSizeinBytes / 4;

			tmpGeo.NumVertex = pActual->NumVertices;
			tmpGeo.pData = new float[NumFloatsPerGeometry];
			tmpGeo.pDataDest = new float[NumFloatsPerGeometry];

			unsigned int counter = 0;
			for (unsigned int j = 0; j < pActual->NumVertices; j++) {

				if (pActual->VertexAttributes&xMeshGeometry::HAS_POSITION) {
					for (unsigned int k = 0; k < NumComponents; k++) {
						tmpGeo.pData[counter++] = pActual->Positions[j].v[k];
					}
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_NORMAL) {
					for (unsigned int k = 0; k < NumComponents; k++) {
						tmpGeo.pData[counter++] = pActual->Normals[j].v[k];
					}
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
					for (unsigned int k = 0; k < NumComponents; k++) {
						tmpGeo.pData[counter++] = pActual->Tangents[j].v[k];
					}
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_BINORMAL) {
					for (unsigned int k = 0; k < NumComponents; k++) {
						tmpGeo.pData[counter++] = pActual->Binormals[j].v[k];
					}
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD0) {
					tmpGeo.pData[counter++] = pActual->TexCoordinates[0][j].x;
					tmpGeo.pData[counter++] = pActual->TexCoordinates[0][j].y;
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD1) {
					tmpGeo.pData[counter++] = pActual->TexCoordinates[1][j].x;
					tmpGeo.pData[counter++] = pActual->TexCoordinates[1][j].y;
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD2) {
					tmpGeo.pData[counter++] = pActual->TexCoordinates[2][j].x;
					tmpGeo.pData[counter++] = pActual->TexCoordinates[2][j].y;
				}

				if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD3) {
					tmpGeo.pData[counter++] = pActual->TexCoordinates[3][j].x;
					tmpGeo.pData[counter++] = pActual->TexCoordinates[3][j].y;
				}

			}

			for (unsigned int j = 0; j < counter; j++) {
				tmpGeo.pDataDest[j] = tmpGeo.pData[j];
			}

			glGenBuffers(1, &tmpGeo.Id);
			glBindBuffer(GL_ARRAY_BUFFER, tmpGeo.Id);
			glBufferData(GL_ARRAY_BUFFER, pActual->NumVertices*pActual->VertexSize, &tmpGeo.pData[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glFlush();

			glGenBuffers(1, &tmpGeo.IdIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpGeo.IdIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, pActual->Triangles.size()*sizeof(unsigned short), &pActual->Triangles[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glFlush();

			xDWORD NumMaterials = pActual->MaterialList.Materials.size();
			tmpGeo.Subsets.reserve(NumMaterials);

			xDWORD NumFaceIndices = pActual->MaterialList.FaceIndices.size();

			for (unsigned int j = 0; j < NumMaterials; j++) {

			//	pActual->MaterialList.Materials[j].
				xSubsetInfo tmpSubset;
				tmpSubset.NumTris = 0;
				for (unsigned int k = 0; k < NumFaceIndices; k++) {
					if (pActual->MaterialList.FaceIndices[k] == j) {
						tmpSubset.NumTris++;
					}
				}
				tmpSubset.NumVertex = tmpSubset.NumTris * 3;
				unsigned short *tmpIndexex = new unsigned short[tmpSubset.NumVertex];
				counter = 0;
				bool first = false;
				for (unsigned int k = 0; k < NumFaceIndices; k++) {
					if (pActual->MaterialList.FaceIndices[k] == j) {
						unsigned int index = k * 3;
						if (!first) {
							tmpSubset.TriStart = k;
							tmpSubset.VertexStart = index;
							first = true;
						}
						tmpIndexex[counter++] = pActual->Triangles[index];
						tmpIndexex[counter++] = pActual->Triangles[index + 1];
						tmpIndexex[counter++] = pActual->Triangles[index + 2];
					}
				}

				glGenBuffers(1, &tmpSubset.Id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpSubset.Id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, tmpSubset.NumTris * 3 * sizeof(unsigned short), tmpIndexex, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glFlush();


				tmpSubset.VertexAttrib = pActual->VertexAttributes;
				tmpSubset.VertexSize = pActual->VertexSize;
				tmpSubset.bAlignedVertex = Desc.bAlignMemory;
				tmpGeo.Subsets.push_back(tmpSubset);
				delete[] tmpIndexex;
			}
			techniques.push_back(technique);
			MeshInfo.push_back(tmpGeo);
		}

	}

	void xMesh::GatherMaterials(video::Material_ &mat, xF::xMaterial &xMat){
		video::Material_::MatUnit face_color;
		video::Material_::MatUnit specular_color;
		video::Material_::MatUnit specular_power;
		video::Material_::MatUnit emissive_color;

		face_color.kind  = video::Material_::Unit_Kind::COLOR;
		face_color.color.kind = video::Material_::Kind_Color::DIFFUSE_COLOR;
		face_color.color.rgba[0] = xMat.FaceColor.r;
		face_color.color.rgba[1] = xMat.FaceColor.g;
		face_color.color.rgba[2] = xMat.FaceColor.b;
		face_color.color.rgba[3] = xMat.FaceColor.a;

		specular_color.kind = video::Material_::Unit_Kind::COLOR;
		specular_color.color.kind = video::Material_::Kind_Color::SPECULAR_COLOR;
		specular_color.color.rgba[0] = xMat.Specular.r;
		specular_color.color.rgba[1] = xMat.Specular.g;
		specular_color.color.rgba[2] = xMat.Specular.b;
		specular_color.color.rgba[3] = 1.0;

		emissive_color.kind = video::Material_::Unit_Kind::COLOR;
		emissive_color.color.kind = video::Material_::Kind_Color::AMBIENT_COLOR;
		emissive_color.color.rgba[0] = xMat.Emissive.r;
		emissive_color.color.rgba[1] = xMat.Emissive.g;
		emissive_color.color.rgba[2] = xMat.Emissive.b;
		emissive_color.color.rgba[3] = 1.0;

		specular_power.kind = video::Material_::Unit_Kind::VALUE;
		specular_power.value.kind = video::Material_::Kind_Value::SPECULAR_POWER;
		specular_power.value.value = xMat.Power;

		mat.props.push_back(face_color);
		mat.props.push_back(specular_color);
		mat.props.push_back(emissive_color);
		mat.props.push_back(specular_power);
		
	}

	void xMesh::ReleaseBuffers() {
		for (unsigned int i = 0; i < MeshInfo.size(); i++) {
			glDeleteBuffers(1, &MeshInfo[i].Id);
			for (unsigned int j = 0; j < MeshInfo[i].Subsets.size(); j++) {
				glDeleteBuffers(1, &MeshInfo[i].Subsets[j].Id);
			}

		}
	}

	void xMesh::Update() {
		for (unsigned int i = 0; i < SkeletonAnimated.Bones.size(); i++) {
			SkeletonAnimated.Bones[i].Touched = 0;
		}


		XMATRIX44 Id;
		XMatIdentity(Id);

		XMATRIX44 Rot;
		static float val = 0.0f;
		XMATRIX44 Scale;
		XMatScaling(Scale, 0.5, 0.5, 0.5);
		XMatRotationXLH(Rot, val);
		Id = Scale*Rot;
		//XMatMultiply(SkeletonAnimated.Bones[7].Bone,Scale,SkeletonAnimated.Bones[7].Bone);


		//SkeletonAnimated.Bones[7].Bone *= Scale*Rot;

		UpdateAnimation();

		UpdateSkeleton(&SkeletonAnimated.Bones[0], Id);


		for (unsigned int i = 0; i < Geometry.size(); i++) {
			xMeshGeometry *pActualMesh = &Geometry[i];
			for (unsigned int j = 0; j < pActualMesh->Info.SkinMeshHeader.NumBones; j++) {
				//XMatMultiply(pActualMesh->Info.SkinWeights[j].MatrixFinal,pActualMesh->Info.SkinWeights[j].MatrixOffset,*pActualMesh->Info.SkinWeights[j].MatrixCombinedAnimation);  //// Orig working with first xmath lib
				pActualMesh->Info.SkinWeights[j].MatrixFinal = pActualMesh->Info.SkinWeights[j].MatrixOffset*(*pActualMesh->Info.SkinWeights[j].MatrixCombinedAnimation);
			}
		}

		UpdateSoftwareSkinning();
	}

	void xMesh::UpdateSkeleton(xBone *pBone, XMATRIX44 mat) {

		//XMatMultiply(pBone->Combined,pBone->Bone,mat); // Orig working with first xmath lib
		pBone->Combined = pBone->Bone*mat;

		pBone->Touched = 1;

		for (unsigned int i = 0; i < pBone->Brothers.size(); i++) {
			unsigned int BroIndex = pBone->Brothers[i];
			if (!SkeletonAnimated.Bones[BroIndex].Touched) {
				UpdateSkeleton(&SkeletonAnimated.Bones[BroIndex], mat);
			}
		}

		if (pBone->Sons.size() > 0) {
			unsigned int SonIndex = pBone->Sons[0];
			UpdateSkeleton(&SkeletonAnimated.Bones[SonIndex], pBone->Combined);
		}

	}

	void xMesh::UpdateSoftwareSkinning() {

		for (unsigned int i = 0; i < Geometry.size(); i++) {

			xMeshGeometry *pActualMesh = &Geometry[i];
			unsigned char* pSrc = (unsigned char*)MeshInfo[i].pData;
			unsigned char* pDest = (unsigned char*)MeshInfo[i].pDataDest;
			memcpy(pDest, pSrc, MeshInfo[i].NumVertex*MeshInfo[i].VertexSize);

			unsigned int   VertsIndex = 0;
			//unsigned int   NumWeights = 0; // TOCHECK;
			unsigned int   VectorSizeinBytes = 0;
			unsigned int   VertexAttrib = pActualMesh->VertexAttributes;
			float		   VertsWeight = 0;

			XVECTOR3 Pos, Normal, Tangent, Binormal;
			XVECTOR3 Pos_out, Normal_out, Tangent_out, Binormal_out;
			XVECTOR3 *pPos = 0, *pNormal = 0, *pTangent = 0, *pBinormal = 0;
			if (Desc.bAlignMemory)
				VectorSizeinBytes = 16;
			else
				VectorSizeinBytes = 12;

			unsigned int   offset = 0;

			for (unsigned int j = 0; j < pActualMesh->Info.SkinMeshHeader.NumBones; j++) {

				for (unsigned int k = 0; k < pActualMesh->Info.SkinWeights[j].Weights.size(); k++) {

					VertsIndex = pActualMesh->Info.SkinWeights[j].VertexIndices[k];
					VertsWeight = pActualMesh->Info.SkinWeights[j].Weights[k];

					offset = VertsIndex*MeshInfo[i].VertexSize;

					unsigned int AccumOffset = offset;
					Pos = XVECTOR3(*(_XVECTOR3*)(pSrc + AccumOffset));
					AccumOffset += VectorSizeinBytes;

					if (VertexAttrib&xMeshGeometry::HAS_NORMAL) {
						Normal = XVECTOR3(*(_XVECTOR3*)(pSrc + AccumOffset));
						AccumOffset += VectorSizeinBytes;
					}

					if (VertexAttrib&xMeshGeometry::HAS_TANGENT) {
						Tangent = XVECTOR3(*(_XVECTOR3*)(pSrc + AccumOffset));
						AccumOffset += VectorSizeinBytes;
					}

					if (VertexAttrib&xMeshGeometry::HAS_BINORMAL) {
						Binormal = XVECTOR3(*(_XVECTOR3*)(pSrc + AccumOffset));
						AccumOffset += VectorSizeinBytes;
					}

					XMATRIX44	pMat = pActualMesh->Info.SkinWeights[j].MatrixFinal;
#if !USE_LEFT_HANDED
					XMatTranspose(pMat, pMat);
#endif
					XVecTransform(Pos_out, Pos, pMat);
					if (VertexAttrib&xMeshGeometry::HAS_NORMAL)
						XVecTransformNormal(Normal_out, Normal, pMat);
					if (VertexAttrib&xMeshGeometry::HAS_TANGENT)
						XVecTransformNormal(Tangent_out, Tangent, pMat);
					if (VertexAttrib&xMeshGeometry::HAS_BINORMAL)
						XVecTransformNormal(Binormal_out, Binormal, pMat);

					AccumOffset = offset;
					pPos = (XVECTOR3*)(pDest + AccumOffset);
					AccumOffset += VectorSizeinBytes;

					if (VertexAttrib&xMeshGeometry::HAS_NORMAL) {
						pNormal = (XVECTOR3*)(pDest + AccumOffset);
						AccumOffset += VectorSizeinBytes;
					}

					if (VertexAttrib&xMeshGeometry::HAS_TANGENT) {
						pTangent = (XVECTOR3*)(pDest + AccumOffset);
						AccumOffset += VectorSizeinBytes;
					}

					if (VertexAttrib&xMeshGeometry::HAS_BINORMAL) {
						pBinormal = (XVECTOR3*)(pDest + AccumOffset);
						AccumOffset += VectorSizeinBytes;
					}

					XVECTOR3 dif = (Pos_out - Pos)*VertsWeight;
					pPos->x += dif.x;	pPos->y += dif.y;	pPos->z += dif.z;

					if (VertexAttrib&xMeshGeometry::HAS_NORMAL) {
						dif = (Normal_out - Normal)*VertsWeight;
						pNormal->x += dif.x;	pNormal->y += dif.y;	pNormal->z += dif.z;
					}

					if (VertexAttrib&xMeshGeometry::HAS_TANGENT) {
						dif = (Tangent_out - Tangent)*VertsWeight;
						pTangent->x += dif.x;	pTangent->y += dif.y;	pTangent->z += dif.z;
					}

					if (VertexAttrib&xMeshGeometry::HAS_BINORMAL) {
						dif = (Binormal_out - Binormal)*VertsWeight;
						pBinormal->x += dif.x;	pBinormal->y += dif.y;	pBinormal->z += dif.z;
					}

				}

			}

			glBindBuffer(GL_ARRAY_BUFFER, MeshInfo[i].Id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, pActualMesh->NumVertices*pActualMesh->VertexSize, &MeshInfo[i].pDataDest[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


		}
	}

	void xMesh::UpdateAnimation() {
		if (Animation.isAnimInfo) {
			m_AnimController->Update();
		}
	}
}