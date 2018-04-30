#ifndef T1000_PRIMITIVE_INSTANCE_H
#define T1000_PRIMITIVE_INSTANCE_H

#include <Config.h>

#include <scene/PrimitiveBase.h>
#include <video/BaseDriver.h>
#include <utils/xMaths.h>

namespace t1000 {
  class PrimitiveInst {
  public:
    void	CreateInstance(PrimitiveBase *pPrim, XMATRIX44 *pVP) {
      gSig = 0; 
      for (int i = 0; i < 8; i++) {
        Textures[i] = 0;
      }
      EnvMap = 0;
      
      pBase = pPrim;
      pViewProj = pVP;
      XMatIdentity(Position);
      XMatIdentity(Scale);
      XMatIdentity(RotationX);
      XMatIdentity(RotationY);
      XMatIdentity(RotationZ);
      XMatIdentity(RotationZ);
      XMatIdentity(Final);
      Visible = true;
      m_brightness = 1.0f;
    }

    void	TranslateAbsolute(float x, float y, float z);
    void	RotateXAbsolute(float ang);
    void	RotateYAbsolute(float ang);
    void	RotateZAbsolute(float ang);
    void	ScaleAbsolute(float scX);
    void	ScaleAbsolute(float scX, float scY, float scZ);

    void	TranslateRelative(float x, float y, float z);
    void	RotateXRelative(float ang);
    void	RotateYRelative(float ang);
    void	RotateZRelative(float ang);
    void	ScaleRelative(float sc);


    void	Update();
    void	Draw();

    void ToogleVisible() { Visible = !Visible; }
    void SetVisible(bool f) { Visible = f; }

    //
    void SetGlobalSignature(unsigned long long f) { gSig = f; }
    void SetTexture(Texture* tex, int index) {
      Textures[index] = tex;
    }

    void SetEnvironmentMap(Texture* tex) {
      EnvMap = tex;
    }
    void SetBrightness(float brightness) {
      m_brightness = brightness;
    }
	void SetParallaxSettings(float lsamples, float hsamples, float height) {
		m_fParallaxLowSamples = lsamples;
		m_fParallaxHighSamples = hsamples;
		m_fParallaxHeight = height;
	}
    Texture*				 Textures[8];
    Texture*			     EnvMap;
    unsigned long long gSig;
    float m_brightness;
    //

    bool Visible;

    XMATRIX44		Position;
    XMATRIX44		Scale;
    XMATRIX44		RotationX;
    XMATRIX44		RotationY;
    XMATRIX44		RotationZ;
    XMATRIX44		Final;

	float m_fParallaxLowSamples;
	float m_fParallaxHighSamples;
	float m_fParallaxHeight;

    XMATRIX44		*pViewProj;
    PrimitiveBase	*pBase;
  };
}

#endif
