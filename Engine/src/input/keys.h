#pragma once
// clang-format off
#define K_MOUSEL         0x00
#define K_MOUSEM         0x01
#define K_MOUSER         0x02
#define K_MOUSEMUP       0x03
#define K_MOUSEMDOWN     0x04
#define K_MOUSES0        0x05
#define K_MOUSES1        0x06

#define K_BACK           0x08
#define K_TAB            0x09

#define K_CLEAR          0x0C
#define K_RETURN         0x0D

#define K_SHIFT          0x10
#define K_CONTROL        0x11
#define K_MENU           0x12
#define K_PAUSE          0x13
#define K_CAPITAL        0x14

#define K_ESCAPE         0x1B

#define K_SPACE          0x20
#define K_PRIOR          0x21
#define K_NEXT           0x22
#define K_END            0x23
#define K_HOME           0x24
#define K_LEFT           0x25
#define K_UP             0x26
#define K_RIGHT          0x27
#define K_DOWN           0x28
#define K_SELECT         0x29
#define K_PRINT          0x2A
#define K_EXECUTE        0x2B
#define K_SNAPSHOT       0x2C
#define K_INSERT         0x2D
#define K_DELETE         0x2E
#define K_HELP           0x2F

#define K_A              0x41
#define K_B              0x42
#define K_C              0x43
#define K_D              0x44
#define K_E              0x45
#define K_F              0x46
#define K_G              0x47
#define K_H              0x48
#define K_I              0x49
#define K_J              0x4A
#define K_K              0x4B
#define K_L              0x4C
#define K_M              0x4D
#define K_N              0x4E
#define K_O              0x4F
#define K_P              0x50
#define K_Q              0x51
#define K_R              0x52
#define K_S              0x53
#define K_T              0x54
#define K_U              0x55
#define K_V              0x56
#define K_W              0x57
#define K_X              0x58
#define K_Y              0x59
#define K_Z              0x5A
#define K_BRACKETLEFT    0x5B
#define K_BACKSLASH      0x5C
#define K_BRACKETRIGHT   0x5D

#define K_SLEEP          0x5F

#define K_NUMPAD0        0x60
#define K_NUMPAD1        0x61
#define K_NUMPAD2        0x62
#define K_NUMPAD3        0x63
#define K_NUMPAD4        0x64
#define K_NUMPAD5        0x65
#define K_NUMPAD6        0x66
#define K_NUMPAD7        0x67
#define K_NUMPAD8        0x68
#define K_NUMPAD9        0x69
#define K_MULTIPLY       0x6A
#define K_ADD            0x6B
#define K_SEPARATOR      0x6C
#define K_SUBTRACT       0x6D
#define K_DECIMAL        0x6E
#define K_DIVIDE         0x6F
#define K_F1             0x70
#define K_F2             0x71
#define K_F3             0x72
#define K_F4             0x73
#define K_F5             0x74
#define K_F6             0x75
#define K_F7             0x76
#define K_F8             0x77
#define K_F9             0x78
#define K_F10            0x79
#define K_F11            0x7A
#define K_F12            0x7B
#define K_F13            0x7C
#define K_F14            0x7D
#define K_F15            0x7E
#define K_F16            0x7F
#define K_F17            0x80
#define K_F18            0x81
#define K_F19            0x82
#define K_F20            0x83
#define K_F21            0x84
#define K_F22            0x85
#define K_F23            0x86
#define K_F24            0x87

#define K_BACKQUOTE      0x88
#define K_1              0x89
#define K_2              0x8A
#define K_3              0x8B
#define K_4              0x8C
#define K_5              0x8D
#define K_6              0x8E
#define K_7              0x8F
#define K_8              0x90
#define K_9              0x91
#define K_0              0x92
#define K_MINUS          0x93
#define K_EQUAL          0x94
#define K_BACKSPACE      0x95
#define K_CAPSLOCK       0x96
#define K_ALT            0x97
#define K_ALTGR          0x98
#define K_SEMICOLON      0x99
#define K_QUOTE          0x9A
#define K_COMMA          0x9B
#define K_PUNT           0x9C
#define K_QUESTION       0x9D
#define K_LSHIFT         0x9E
#define K_RSHIFT         0x9F
#define K_LCONTROL       0xA0
#define K_RCONTROL       0xA1
#define K_LOS            0xA2
#define K_ROS            0xA3
#define K_CONTEXTMENU    0xA4
#define K_ARROWUP        0xA5
#define K_ARROWLEFT      0xA6
#define K_ARROWRIGHT     0xA7
#define K_ARROWDOWN      0xA8
#define K_PAGEUP         0xA9
#define K_PAGEDOWN       0xAA
#define K_SCROLLLOCK     0xAB
#define K_NUMLOCK        0xAC
#define K_SLASH          0xAD
#define K_UNDERSCORE     0xAE
#define K_NUMPADDEL      0xAF
#define K_NUMPADRETURN   0xB0
// clang-format on

const char *key_to_str(unsigned char key);
const char *mouse_to_str(unsigned char mouse);
