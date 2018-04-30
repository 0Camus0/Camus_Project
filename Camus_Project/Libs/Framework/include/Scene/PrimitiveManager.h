#ifndef T1000_PRIMITIVEMANAGER_H
#define T1000_PRIMITIVEMANAGER_H

#include <Config.h>

#include <vector>
#include <utils/xMaths.h>
#include <scene/PrimitiveBase.h>
#include <scene/SceneProp.h>

namespace t1000 {
  class Spline;
  class PrimitiveManager {
  private:
    int  CreateQuad();
  public:
    enum PRIMITIVES {
      QUAD = 0,
      COUNT
    };
    void SetVP(XMATRIX44 *vp) {
      pVP = vp;
    }
    int  CreateTriangle();
    int	 CreateCube();
    int	 CreateMesh(char *fname);
    int  CreateSpline(Spline& spline);

    void SetSceneProps(SceneProps *p);
    void Init();

    void DrawPrimitives();
    void DestroyPrimitives();
    PrimitiveBase*	GetPrimitive(unsigned int) const;

    std::vector<PrimitiveBase*> primitives;

    XMATRIX44 *pVP;
  };
}

#endif
