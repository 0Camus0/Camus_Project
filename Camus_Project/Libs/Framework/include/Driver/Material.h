#ifndef CA_MATERIAL_H
#define CA_MATERIAL_H

namespace hyperspace {
	namespace video {

	class Material_	{
	public:
		Material_(){}
		
		enum Kind_Map{
			AMBIENT_MAP,
			DIFFUSE_MAP,
			SPECULAR_MAP,
			GLOSS_MAP,
			NORMAL_MAP,
			LIGHT_MAP,
			SHADOW_MAP,
			SELF_ILLUMINATION_MAP,
			FRESNEL_MAP,
			REFLECTION_MAP,
			CUBE_REFLECTION_MAP,
			INVALID_MAP = -1
		};

		enum Kind_Color{
			AMBIENT_COLOR,
			DIFFUSE_COLOR,
			SPECULAR_COLOR,
			FRESNEL_COLOR,
			INVALID_COLOR = -1
		};

		enum Kind_Value{
			SPECULAR_POWER,
			OPACITY,
			SELF_ILLUMINATION_LEVEL,
			FRESNEL_POWER,
			FRESNEL_BIAS,
			FRESNEL_MULT,
			FRESNEL_MASK,
			INVALID_VALUE = -1
		};

		enum Property{
			USE_DIFFUSE_MAP			= (1 << 0),
			USE_SPECULAR_MAP		= (1 << 1), 
			USE_GLOSS_MAP			= (1 << 2),
			USE_NORMAL_MAP			= (1 << 3),
			USE_LIGHT_MAP			= (1 << 4),
			USE_ILLUMINATION_MAP	= (1 << 5),
			USE_FRESNEL_MAP			= (1 << 6),
			USE_REFLECTION_MAP		= (1 << 7),
			USE_CUBE_REFLECTION_MAP = (1 << 8),
			USE_SPECULAR_HIGHLIGHT	= (1 << 9),
			USE_FRESNEL_TERMN		= (1 << 10),
			USE_SHADOW_MAP			= (1 << 11),
			USE_FLIPPED_GREEN		= (1 << 12),
			USE_DUMMY_SHADER		= (1 << 13),
			USE_NO_LIGHTING			= (1 << 14),

			BE_SHADOW_CASTER		= (1 << 15),
			BE_PHYSICS_STATIC		= (1 << 16),
			BE_PHYSICS_DYNAMIC		= (1 << 17),

			IS_TRANSPARENT			= (1 << 18),
			IS_FOR_ALPHA_TEST		= (1 << 19),
		};

		struct Map_ {
			Map_() : kind(INVALID_MAP) {}
			int kind;
			int tex_id;
		};

		struct Color_{
			Color_() : kind(INVALID_COLOR) {}
			int kind;
			float rgba[4];
		};

		struct Value_{
			Value_() : Value_(INVALID_VALUE) {}
			int kind;
			float value;
		};

		struct MatUnit{
			Map_	map;
			Color_	color;
			Value_	value;
		};



	};

	}
}

#endif