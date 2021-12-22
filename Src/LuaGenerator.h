#ifndef _LUA_GENERATOR_H_
#define _LUA_GENETATOR_H_

#include "HelloLua.h"
#include "ProcessKit.h"
#include "GUIKit.h"
#include "Log.h"

BEGIN_DECLARE_LUA_WRAPPER(ProcessKit, 0, 0, 4, 0)
DECLARE_EMPTY_CONSTRUCTOR()
DECLARE_EMPTY_DESTRUCTOR()
BEGIN_DECLARE_MEMBER()
DECLARE_METHOD(Start, int, const char*, const char*, const char*, const char*, long, bool)
DECLARE_METHOD(Stop, void)
DECLARE_METHOD(GetOutput, std::string)
DECLARE_METHOD(GetError, std::string)
END_DECLARE_MEMBER()
END_DECLARE_LUA_WRAPPER()

BEGIN_DECLARE_LUA_WRAPPER(GUIKit, 0, 0, 23, 0)
DECLARE_EMPTY_CONSTRUCTOR()
DECLARE_EMPTY_DESTRUCTOR()
BEGIN_DECLARE_MEMBER()
DECLARE_METHOD(BeginTabWidget, void, const char*)
DECLARE_METHOD(EndTabWidget, void)
DECLARE_METHOD(BeginTab, void, const char*, int, const char*)
DECLARE_METHOD(EndTab, void)
DECLARE_METHOD(BeginLayout, void, int, int, const char*)
DECLARE_METHOD(EndLayout, void)
DECLARE_METHOD(Button, void, const char*, const char*, int, const char*)
DECLARE_METHOD(Label, void, const char*, const char*, int, const char*)
DECLARE_METHOD(TextField, void, const char*, const char*, bool, int, const char*)
DECLARE_METHOD(LineField, void, const char*, const char*, bool, int, const char*)
DECLARE_METHOD(SetLineFieldText, void, const char*, const char*)
DECLARE_METHOD(GetLineFieldText, std::string, const char*)
DECLARE_METHOD(ComboBox, void, const char*, const char*, int, int, const char*)
DECLARE_METHOD(SetComboBoxItems, void, const char*, const char*)
DECLARE_METHOD(OpenFileDialog, std::string, const char*, const char*, const char*)
DECLARE_METHOD(OpenDirDialog, std::string, const char*, const char*)
DECLARE_METHOD(RadioGroup, void, const char*, const char*, const char*, int, int, int, const char*)
DECLARE_METHOD(SetSizePolicy, void, const char*, int, int)
END_DECLARE_MEMBER()
END_DECLARE_LUA_WRAPPER()

BEGIN_DECLARE_LUA_WRAPPER(Log, 3, 0, 0, 0)
DECLARE_EMPTY_CONSTRUCTOR()
DECLARE_EMPTY_DESTRUCTOR()
BEGIN_DECLARE_MEMBER()
DECLARE_STATIC_METHOD(LogD, void, const char*)
DECLARE_STATIC_METHOD(LogW, void, const char*)
DECLARE_STATIC_METHOD(LogE, void, const char*)
END_DECLARE_MEMBER()
END_DECLARE_LUA_WRAPPER()

#endif