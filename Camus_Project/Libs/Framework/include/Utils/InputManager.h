#ifndef T1000_INPUT_MANAGER_H
#define T1000_INPUT_MANAGER_H

#include <Config.h>

enum STDKEYS {
	T1000K_UNKNOWN		= 0,
	T1000K_FIRST		= 0,
	T1000K_BACKSPACE		= 8,
	T1000K_TAB		= 9,
	T1000K_CLEAR		= 12,
	T1000K_RETURN		= 13,
	T1000K_PAUSE		= 19,
	T1000K_ESCAPE		= 27,
	T1000K_SPACE		= 32,
	T1000K_EXCLAIM		= 33,
	T1000K_QUOTEDBL		= 34,
	T1000K_HASH		= 35,
	T1000K_DOLLAR		= 36,
	T1000K_AMPERSAND		= 38,
	T1000K_QUOTE		= 39,
	T1000K_LEFTPAREN		= 40,
	T1000K_RIGHTPAREN		= 41,
	T1000K_ASTERISK		= 42,
	T1000K_PLUS		= 43,
	T1000K_COMMA		= 44,
	T1000K_MINUS		= 45,
	T1000K_PERIOD		= 46,
	T1000K_SLASH		= 47,
	T1000K_0			= 48,
	T1000K_1			= 49,
	T1000K_2			= 50,
	T1000K_3			= 51,
	T1000K_4			= 52,
	T1000K_5			= 53,
	T1000K_6			= 54,
	T1000K_7			= 55,
	T1000K_8			= 56,
	T1000K_9			= 57,
	T1000K_COLON		= 58,
	T1000K_SEMICOLON		= 59,
	T1000K_LESS		= 60,
	T1000K_EQUALS		= 61,
	T1000K_GREATER		= 62,
	T1000K_QUESTION		= 63,
	T1000K_AT			= 64,
	T1000K_LEFTBRACKET	= 91,
	T1000K_BACKSLASH		= 92,
	T1000K_RIGHTBRACKET	= 93,
	T1000K_CARET		= 94,
	T1000K_UNDERSCORE		= 95,
	T1000K_BACKQUOTE		= 96,
	T1000K_a			= 97,
	T1000K_b			= 98,
	T1000K_c			= 99,
	T1000K_d			= 100,
	T1000K_e			= 101,
	T1000K_f			= 102,
	T1000K_g			= 103,
	T1000K_h			= 104,
	T1000K_i			= 105,
	T1000K_j			= 106,
	T1000K_k			= 107,
	T1000K_l			= 108,
	T1000K_m			= 109,
	T1000K_n			= 110,
	T1000K_o			= 111,
	T1000K_p			= 112,
	T1000K_q			= 113,
	T1000K_r			= 114,
	T1000K_s			= 115,
	T1000K_t			= 116,
	T1000K_u			= 117,
	T1000K_v			= 118,
	T1000K_w			= 119,
	T1000K_x			= 120,
	T1000K_y			= 121,
	T1000K_z			= 122,
	T1000K_DELETE		= 127,
	T1000K_WORLD_0		= 160,
	T1000K_WORLD_1		= 161,
	T1000K_WORLD_2		= 162,
	T1000K_WORLD_3		= 163,
	T1000K_WORLD_4		= 164,
	T1000K_WORLD_5		= 165,
	T1000K_WORLD_6		= 166,
	T1000K_WORLD_7		= 167,
	T1000K_WORLD_8		= 168,
	T1000K_WORLD_9		= 169,
	T1000K_WORLD_10		= 170,
	T1000K_WORLD_11		= 171,
	T1000K_WORLD_12		= 172,
	T1000K_WORLD_13		= 173,
	T1000K_WORLD_14		= 174,
	T1000K_WORLD_15		= 175,
	T1000K_WORLD_16		= 176,
	T1000K_WORLD_17		= 177,
	T1000K_WORLD_18		= 178,
	T1000K_WORLD_19		= 179,
	T1000K_WORLD_20		= 180,
	T1000K_WORLD_21		= 181,
	T1000K_WORLD_22		= 182,
	T1000K_WORLD_23		= 183,
	T1000K_WORLD_24		= 184,
	T1000K_WORLD_25		= 185,
	T1000K_WORLD_26		= 186,
	T1000K_WORLD_27		= 187,
	T1000K_WORLD_28		= 188,
	T1000K_WORLD_29		= 189,
	T1000K_WORLD_30		= 190,
	T1000K_WORLD_31		= 191,
	T1000K_WORLD_32		= 192,
	T1000K_WORLD_33		= 193,
	T1000K_WORLD_34		= 194,
	T1000K_WORLD_35		= 195,
	T1000K_WORLD_36		= 196,
	T1000K_WORLD_37		= 197,
	T1000K_WORLD_38		= 198,
	T1000K_WORLD_39		= 199,
	T1000K_WORLD_40		= 200,
	T1000K_WORLD_41		= 201,
	T1000K_WORLD_42		= 202,
	T1000K_WORLD_43		= 203,
	T1000K_WORLD_44		= 204,
	T1000K_WORLD_45		= 205,
	T1000K_WORLD_46		= 206,
	T1000K_WORLD_47		= 207,
	T1000K_WORLD_48		= 208,
	T1000K_WORLD_49		= 209,
	T1000K_WORLD_50		= 210,
	T1000K_WORLD_51		= 211,
	T1000K_WORLD_52		= 212,
	T1000K_WORLD_53		= 213,
	T1000K_WORLD_54		= 214,
	T1000K_WORLD_55		= 215,
	T1000K_WORLD_56		= 216,
	T1000K_WORLD_57		= 217,
	T1000K_WORLD_58		= 218,
	T1000K_WORLD_59		= 219,
	T1000K_WORLD_60		= 220,
	T1000K_WORLD_61		= 221,
	T1000K_WORLD_62		= 222,
	T1000K_WORLD_63		= 223,
	T1000K_WORLD_64		= 224,
	T1000K_WORLD_65		= 225,
	T1000K_WORLD_66		= 226,
	T1000K_WORLD_67		= 227,
	T1000K_WORLD_68		= 228,
	T1000K_WORLD_69		= 229,
	T1000K_WORLD_70		= 230,
	T1000K_WORLD_71		= 231,
	T1000K_WORLD_72		= 232,
	T1000K_WORLD_73		= 233,
	T1000K_WORLD_74		= 234,
	T1000K_WORLD_75		= 235,
	T1000K_WORLD_76		= 236,
	T1000K_WORLD_77		= 237,
	T1000K_WORLD_78		= 238,
	T1000K_WORLD_79		= 239,
	T1000K_WORLD_80		= 240,
	T1000K_WORLD_81		= 241,
	T1000K_WORLD_82		= 242,
	T1000K_WORLD_83		= 243,
	T1000K_WORLD_84		= 244,
	T1000K_WORLD_85		= 245,
	T1000K_WORLD_86		= 246,
	T1000K_WORLD_87		= 247,
	T1000K_WORLD_88		= 248,
	T1000K_WORLD_89		= 249,
	T1000K_WORLD_90		= 250,
	T1000K_WORLD_91		= 251,
	T1000K_WORLD_92		= 252,
	T1000K_WORLD_93		= 253,
	T1000K_WORLD_94		= 254,
	T1000K_WORLD_95		= 255,
	T1000K_KP0		= 256,
	T1000K_KP1		= 257,
	T1000K_KP2		= 258,
	T1000K_KP3		= 259,
	T1000K_KP4		= 260,
	T1000K_KP5		= 261,
	T1000K_KP6		= 262,
	T1000K_KP7		= 263,
	T1000K_KP8		= 264,
	T1000K_KP9		= 265,
	T1000K_KP_PERIOD		= 266,
	T1000K_KP_DIVIDE		= 267,
	T1000K_KP_MULTIPLY	= 268,
	T1000K_KP_MINUS		= 269,
	T1000K_KP_PLUS		= 270,
	T1000K_KP_ENTER		= 271,
	T1000K_KP_EQUALS		= 272,

	T1000K_UP			= 273,
	T1000K_DOWN		= 274,
	T1000K_RIGHT		= 275,
	T1000K_LEFT		= 276,
	T1000K_INSERT		= 277,
	T1000K_HOME		= 278,
	T1000K_END		= 279,
	T1000K_PAGEUP		= 280,
	T1000K_PAGEDOWN		= 281,

	T1000K_F1			= 282,
	T1000K_F2			= 283,
	T1000K_F3			= 284,
	T1000K_F4			= 285,
	T1000K_F5			= 286,
	T1000K_F6			= 287,
	T1000K_F7			= 288,
	T1000K_F8			= 289,
	T1000K_F9			= 290,
	T1000K_F10		= 291,
	T1000K_F11		= 292,
	T1000K_F12		= 293,
	T1000K_F13		= 294,
	T1000K_F14		= 295,
	T1000K_F15		= 296,

	T1000K_NUMLOCK		= 300,
	T1000K_CAPSLOCK		= 301,
	T1000K_SCROLLOCK		= 302,
	T1000K_RSHIFT		= 303,
	T1000K_LSHIFT		= 304,
	T1000K_RCTRL		= 305,
	T1000K_LCTRL		= 306,
	T1000K_RALT		= 307,
	T1000K_LALT		= 308,
	T1000K_RMETA		= 309,
	T1000K_LMETA		= 310,
	T1000K_LSUPER		= 311,
	T1000K_RSUPER		= 312,
	T1000K_MODE		= 313,
	T1000K_COMPOSE		= 314,

	T1000K_HELP		= 315,
	T1000K_PRINT		= 316,
	T1000K_SYSREQ		= 317,
	T1000K_BREAK		= 318,
	T1000K_MENU		= 319,
	T1000K_POWER		= 320,
	T1000K_EURO		= 321,
	T1000K_UNDO		= 322,
	T1000K_LAST
};

enum STDKEYS_ANDROID {
	AND_T1000K_UNKNOWN = 0,
	AND_T1000K_SOFT_LEFT = 1,
	AND_T1000K_SOFT_RIGHT = 2,
	AND_T1000K_HOME = 3,
	AND_T1000K_BACK = 4,
	AND_T1000K_CALL = 5,
	AND_T1000K_ENDCALL = 6,
	AND_T1000K_0 = 7,
	AND_T1000K_1 = 8,
	AND_T1000K_2 = 9,
	AND_T1000K_3 = 10,
	AND_T1000K_4 = 11,
	AND_T1000K_5 = 12,
	AND_T1000K_6 = 13,
	AND_T1000K_7 = 14,
	AND_T1000K_8 = 15,
	AND_T1000K_9 = 16,
	AND_T1000K_STAR = 17,
	AND_T1000K_POUND = 18,
	AND_T1000K_DPAD_UP = 19,
	AND_T1000K_DPAD_DOWN = 20,
	AND_T1000K_DPAD_LEFT = 21,
	AND_T1000K_DPAD_RIGHT = 22,
	AND_T1000K_DPAD_CENTER = 23,
	AND_T1000K_VOLUME_UP = 24,
	AND_T1000K_VOLUME_DOWN = 25,
	AND_T1000K_POWER = 26,
	AND_T1000K_CAMERA = 27,
	AND_T1000K_CLEAR = 28,
	AND_T1000K_A = 29,
	AND_T1000K_B = 30,
	AND_T1000K_C = 31,
	AND_T1000K_D = 32,
	AND_T1000K_E = 33,
	AND_T1000K_F = 34,
	AND_T1000K_G = 35,
	AND_T1000K_H = 36,
	AND_T1000K_I = 37,
	AND_T1000K_J = 38,
	AND_T1000K_K = 39,
	AND_T1000K_L = 40,
	AND_T1000K_M = 41,
	AND_T1000K_N = 42,
	AND_T1000K_O = 43,
	AND_T1000K_P = 44,
	AND_T1000K_Q = 45,
	AND_T1000K_R = 46,
	AND_T1000K_S = 47,
	AND_T1000K_T = 48,
	AND_T1000K_U = 49,
	AND_T1000K_V = 50,
	AND_T1000K_W = 51,
	AND_T1000K_X = 52,
	AND_T1000K_Y = 53,
	AND_T1000K_Z = 54,
	AND_T1000K_COMMA = 55,
	AND_T1000K_PERIOD = 56,
	AND_T1000K_ALT_LEFT = 57,
	AND_T1000K_ALT_RIGHT = 58,
	AND_T1000K_SHIFT_LEFT = 59,
	AND_T1000K_SHIFT_RIGHT = 60,
	AND_T1000K_TAB = 61,
	AND_T1000K_SPACE = 62,
	AND_T1000K_SYM = 63,
	AND_T1000K_EXPLORER = 64,
	AND_T1000K_ENVELOPE = 65,
	AND_T1000K_ENTER = 66,
	AND_T1000K_DEL = 67,
	AND_T1000K_GRAVE = 68,
	AND_T1000K_MINUS = 69,
	AND_T1000K_EQUALS = 70,
	AND_T1000K_LEFT_BRACKET = 71,
	AND_T1000K_RIGHT_BRACKET = 72,
	AND_T1000K_BACKSLASH = 73,
	AND_T1000K_SEMICOLON = 74,
	AND_T1000K_APOSTROPHE = 75,
	AND_T1000K_SLASH = 76,
	AND_T1000K_AT = 77,
	AND_T1000K_NUM = 78,
	AND_T1000K_HEADSETHOOK = 79,
	AND_T1000K_FOCUS = 80,
	AND_T1000K_PLUS = 81,
	AND_T1000K_MENU = 82,
	AND_T1000K_NOTIFICATION = 83,
	AND_T1000K_SEARCH = 84,
	AND_T1000K_MEDIA_PLAY_PAUSE = 85,
	AND_T1000K_MEDIA_STOP = 86,
	AND_T1000K_MEDIA_NEXT = 87,
	AND_T1000K_MEDIA_PREVIOUS = 88,
	AND_T1000K_MEDIA_REWIND = 89,
	AND_T1000K_MEDIA_FAST_FORWARD = 90,
	AND_T1000K_MUTE = 91,
	AND_T1000K_PAGE_UP = 92,
	AND_T1000K_PAGE_DOWN = 93,
	AND_T1000K_PICTSYMBOLS = 94,
	AND_T1000K_SWITCH_CHARSET = 95,
	AND_T1000K_BUTTON_A = 96,
	AND_T1000K_BUTTON_B = 97,
	AND_T1000K_BUTTON_C = 98,
	AND_T1000K_BUTTON_X = 99,
	AND_T1000K_BUTTON_Y = 100,
	AND_T1000K_BUTTON_Z = 101,
	AND_T1000K_BUTTON_L1 = 102,
	AND_T1000K_BUTTON_R1 = 103,
	AND_T1000K_BUTTON_L2 = 104,
	AND_T1000K_BUTTON_R2 = 105,
	AND_T1000K_BUTTON_THUMBL = 106,
	AND_T1000K_BUTTON_THUMBR = 107,
	AND_T1000K_BUTTON_START = 108,
	AND_T1000K_BUTTON_SELECT = 109,
	AND_T1000K_BUTTON_MODE = 110,
	AND_T1000K_ESCAPE = 111,
	AND_T1000K_FORWARD_DEL = 112,
	AND_T1000K_CTRL_LEFT = 113,
	AND_T1000K_CTRL_RIGHT = 114,
	AND_T1000K_CAPS_LOCK = 115,
	AND_T1000K_SCROLL_LOCK = 116,
	AND_T1000K_META_LEFT = 117,
	AND_T1000K_META_RIGHT = 118,
	AND_T1000K_FUNCTION = 119,
	AND_T1000K_SYSRQ = 120,
	AND_T1000K_BREAK = 121,
	AND_T1000K_MOVE_HOME = 122,
	AND_T1000K_MOVE_END = 123,
	AND_T1000K_INSERT = 124,
	AND_T1000K_FORWARD = 125,
	AND_T1000K_MEDIA_PLAY = 126,
	AND_T1000K_MEDIA_PAUSE = 127,
	AND_T1000K_MEDIA_CLOSE = 128,
	AND_T1000K_MEDIA_EJECT = 129,
	AND_T1000K_MEDIA_RECORD = 130,
	AND_T1000K_F1 = 131,
	AND_T1000K_F2 = 132,
	AND_T1000K_F3 = 133,
	AND_T1000K_F4 = 134,
	AND_T1000K_F5 = 135,
	AND_T1000K_F6 = 136,
	AND_T1000K_F7 = 137,
	AND_T1000K_F8 = 138,
	AND_T1000K_F9 = 139,
	AND_T1000K_F10 = 140,
	AND_T1000K_F11 = 141,
	AND_T1000K_F12 = 142,
	AND_T1000K_NUM_LOCK = 143,
	AND_T1000K_NUMPAD_0 = 144,
	AND_T1000K_NUMPAD_1 = 145,
	AND_T1000K_NUMPAD_2 = 146,
	AND_T1000K_NUMPAD_3 = 147,
	AND_T1000K_NUMPAD_4 = 148,
	AND_T1000K_NUMPAD_5 = 149,
	AND_T1000K_NUMPAD_6 = 150,
	AND_T1000K_NUMPAD_7 = 151,
	AND_T1000K_NUMPAD_8 = 152,
	AND_T1000K_NUMPAD_9 = 153,
	AND_T1000K_NUMPAD_DIVIDE = 154,
	AND_T1000K_NUMPAD_MULTIPLY = 155,
	AND_T1000K_NUMPAD_SUBTRACT = 156,
	AND_T1000K_NUMPAD_ADD = 157,
	AND_T1000K_NUMPAD_DOT = 158,
	AND_T1000K_NUMPAD_COMMA = 159,
	AND_T1000K_NUMPAD_ENTER = 160,
	AND_T1000K_NUMPAD_EQUALS = 161,
	AND_T1000K_NUMPAD_LEFT_PAREN = 162,
	AND_T1000K_NUMPAD_RIGHT_PAREN = 163,
	AND_T1000K_VOLUME_MUTE = 164,
	AND_T1000K_INFO = 165,
	AND_T1000K_CHANNEL_UP = 166,
	AND_T1000K_CHANNEL_DOWN = 167,
	AND_T1000K_ZOOM_IN = 168,
	AND_T1000K_ZOOM_OUT = 169,
	AND_T1000K_TV = 170,
	AND_T1000K_WINDOW = 171,
	AND_T1000K_GUIDE = 172,
	AND_T1000K_DVR = 173,
	AND_T1000K_BOOKMARK = 174,
	AND_T1000K_CAPTIONS = 175,
	AND_T1000K_SETTINGS = 176,
	AND_T1000K_TV_POWER = 177,
	AND_T1000K_TV_INPUT = 178,
	AND_T1000K_STB_POWER = 179,
	AND_T1000K_STB_INPUT = 180,
	AND_T1000K_AVR_POWER = 181,
	AND_T1000K_AVR_INPUT = 182,
	AND_T1000K_PROG_RED = 183,
	AND_T1000K_PROG_GREEN = 184,
	AND_T1000K_PROG_YELLOW = 185,
	AND_T1000K_PROG_BLUE = 186,
	AND_T1000K_APP_SWITCH = 187,
	AND_T1000K_BUTTON_1 = 188,
	AND_T1000K_BUTTON_2 = 189,
	AND_T1000K_BUTTON_3 = 190,
	AND_T1000K_BUTTON_4 = 191,
	AND_T1000K_BUTTON_5 = 192,
	AND_T1000K_BUTTON_6 = 193,
	AND_T1000K_BUTTON_7 = 194,
	AND_T1000K_BUTTON_8 = 195,
	AND_T1000K_BUTTON_9 = 196,
	AND_T1000K_BUTTON_10 = 197,
	AND_T1000K_BUTTON_11 = 198,
	AND_T1000K_BUTTON_12 = 199,
	AND_T1000K_BUTTON_13 = 200,
	AND_T1000K_BUTTON_14 = 201,
	AND_T1000K_BUTTON_15 = 202,
	AND_T1000K_BUTTON_16 = 203,
	AND_T1000K_LANGUAGE_SWITCH = 204,
	AND_T1000K_MANNER_MODE = 205,
	AND_T1000K_3D_MODE = 206,
	AND_T1000K_CONTACTS = 207,
	AND_T1000K_CALENDAR = 208,
	AND_T1000K_MUSIC = 209,
	AND_T1000K_CALCULATOR = 210,
	AND_T1000K_ZENKAKU_HANKAKU = 211,
	AND_T1000K_EISU = 212,
	AND_T1000K_MUHENKAN = 213,
	AND_T1000K_HENKAN = 214,
	AND_T1000K_KATAKANA_HIRAGANA = 215,
	AND_T1000K_YEN = 216,
	AND_T1000K_RO = 217,
	AND_T1000K_KANA = 218,
	AND_T1000K_ASSIST = 219,
	AND_T1000K_BRIGHTNESS_DOWN = 220,
	AND_T1000K_BRIGHTNESS_UP = 221,
	AND_T1000K_MEDIA_AUDIO_TRACK = 222,
	AND_T1000K_SLEEP = 223,
	AND_T1000K_WAKEUP = 224,
	AND_T1000K_PAIRING = 225,
	AND_T1000K_MEDIA_TOP_MENU = 226,
	AND_T1000K_11 = 227,
	AND_T1000K_12 = 228,
	AND_T1000K_LAST_CHANNEL = 229,
	AND_T1000K_TV_DATA_SERVICE = 230,
	AND_T1000K_VOICE_ASSIST = 231,
	AND_T1000K_TV_RADIO_SERVICE = 232,
	AND_T1000K_TV_TELETEXT = 233,
	AND_T1000K_TV_NUMBER_ENTRY = 234,
	AND_T1000K_TV_TERRESTRIAL_ANALOG = 235,
	AND_T1000K_TV_TERRESTRIAL_DIGITAL = 236,
	AND_T1000K_TV_SATELLITE = 237,
	AND_T1000K_TV_SATELLITE_BS = 238,
	AND_T1000K_TV_SATELLITE_CS = 239,
	AND_T1000K_TV_SATELLITE_SERVICE = 240,
	AND_T1000K_TV_NETWORK = 241,
	AND_T1000K_TV_ANTENNA_CABLE = 242,
	AND_T1000K_TV_INPUT_HDMI_1 = 243,
	AND_T1000K_TV_INPUT_HDMI_2 = 244,
	AND_T1000K_TV_INPUT_HDMI_3 = 245,
	AND_T1000K_TV_INPUT_HDMI_4 = 246,
	AND_T1000K_TV_INPUT_COMPOSITE_1 = 247,
	AND_T1000K_TV_INPUT_COMPOSITE_2 = 248,
	AND_T1000K_TV_INPUT_COMPONENT_1 = 249,
	AND_T1000K_TV_INPUT_COMPONENT_2 = 250,
	AND_T1000K_TV_INPUT_VGA_1 = 251,
	AND_T1000K_TV_AUDIO_DESCRIPTION = 252,
	AND_T1000K_TV_AUDIO_DESCRIPTION_MIX_UP = 253,
	AND_T1000K_TV_AUDIO_DESCRIPTION_MIX_DOWN = 254,
	AND_T1000K_TV_ZOOM_MODE = 255,
	AND_T1000K_TV_CONTENTS_MENU = 256,
	AND_T1000K_TV_MEDIA_CONTEXT_MENU = 257,
	AND_T1000K_TV_TIMER_PROGRAMMING = 258,
	AND_T1000K_HELP = 259,
	AND_T1000K_NAVIGATE_PREVIOUS = 260,
	AND_T1000K_NAVIGATE_NEXT = 261,
	AND_T1000K_NAVIGATE_IN = 262,
	AND_T1000K_NAVIGATE_OUT = 263,
	AND_T1000K_STEM_PRIMARY = 264,
	AND_T1000K_STEM_1 = 265,
	AND_T1000K_STEM_2 = 266,
	AND_T1000K_STEM_3 = 267,
	AND_T1000K_DPAD_UP_LEFT = 268,
	AND_T1000K_DPAD_DOWN_LEFT = 269,
	AND_T1000K_DPAD_UP_RIGHT = 270,
	AND_T1000K_DPAD_DOWN_RIGHT = 271,
	AND_T1000K_MEDIA_SKIP_FORWARD = 272,
	AND_T1000K_MEDIA_SKIP_BACKWARD = 273,
	AND_T1000K_MEDIA_STEP_FORWARD = 274,
	AND_T1000K_MEDIA_STEP_BACKWARD = 275,
	AND_T1000K_SOFT_SLEEP = 276,
	AND_T1000K_CUT = 277,
	AND_T1000K_COPY = 278,
	AND_T1000K_PASTE = 279,
	AND_T1000K_SYSTEM_NAVIGATION_UP = 280,
	AND_T1000K_SYSTEM_NAVIGATION_DOWN = 281,
	AND_T1000K_SYSTEM_NAVIGATION_LEFT = 282,
	AND_T1000K_SYSTEM_NAVIGATION_RIGHT = 283,
	AND_T1000K_ALL_APPS = 284,
	AND_T1000K_REFRESH = 285
};

#define MAXKEYS 512
#define MAXMOUSEBUTTONS 5


class InputManager {
public:
	InputManager();
	bool	PressedOnceKey(int key);
	bool	PressedOnceMouseButton(int mb);

	bool	PressedKey(int key);
	bool	PressedMouseButton(int mb);

	bool	KeyStates[2][MAXKEYS];
	bool	KeyAndroidStates[2][MAXKEYS];
	bool	MouseButtonStates[2][MAXMOUSEBUTTONS];

	int		xDelta;
	int 	yDelta;
};

#endif
