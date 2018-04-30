#ifndef T1000_TQUAD_H
#define T1000_TQUAD_H

#include <video/BaseDriver.h>

namespace t1000 {
  struct Quad {
    struct Vertex {
      float x, y,z,w, s, t;
    };
    void Init();
    void Destroy();
    void Set();

    IndexBuffer*		m_IB;
    VertexBuffer*		m_VB;
    Vertex m_vertex[4];
    unsigned short	m_index[6];
  };
}

#endif