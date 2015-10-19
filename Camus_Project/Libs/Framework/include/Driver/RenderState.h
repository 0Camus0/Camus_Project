#ifndef CA_RENDER_STATE_H
#define CA_RENDER_STATE_H

#include <Driver/BaseDriver.h>
#include <cstdint>

namespace hyperspace {
	namespace video {

		class CRenderState{
		public:
			CRenderState() {
				capabilities = CULL_FACE | DEPTH;
				draworder = CCW;
				cullface = FRONT;
				mask = RED | GREEN | BLUE | ALPHA;
				texfilter = ANISOTROPIC;
				texwrap = CLAMP;
				depthfunction = LEQUAL;
				stencilfunction = ALWAYS;				
			}

			bool operator == (CRenderState &o) {
				return (capabilities == o.capabilities
					&&	draworder == o.draworder
					&&	cullface == o.cullface
					&&	mask == o.mask
					&&	texfilter == o.texfilter
					&&	texwrap == o.texwrap
					&&	depthfunction == o.depthfunction
					&&  stencilfunction == o.stencilfunction
					);
			}

			bool operator != (CRenderState &o) {
				return !(capabilities == o.capabilities
					&&	draworder == o.draworder
					&&	cullface == o.cullface
					&&	mask == o.mask
					&&	texfilter == o.texfilter
					&&	texwrap == o.texwrap
					&&	depthfunction == o.depthfunction
					&&  stencilfunction == o.stencilfunction
					);
			}
		



			draw_order_			draworder;
			cull_face_			cullface;
			texture_filtering_	texfilter;
			texture_wrap_		texwrap;
			comp_function_		depthfunction;
			comp_function_		stencilfunction;
			std::int32_t		capabilities;
			std::int32_t		mask;

		};

	}
}


#endif