######################################################################
# Automatically generated by qmake (2.01a) sob. mar 10 10:49:58 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += Scripts/
QT += gui
# Input
HEADERS += Math/Vector2.h \
           Math/Vector3.h \
           Math/Vector4.h \
           Math/Ray.h \
           Math/Matrix4x4.h \
           Geometry/Sphere.h \
    Math/Plane.h \
    Geometry/Triangle.h \
    Geometry/Geometry.h \
    LightIntensity.h \
    Image.h \
    Camera.h \
    Lights/AmbientLight.h \
    Lights/PointLight.h \
    Material.h \
    Scene.h \
    DisplayWindow.h \
    MainWindow.h \
    Scripts/ScriptManager.h \
    Scripts/Script.h \
    Scripts/luabind/yield_policy.hpp \
    Scripts/luabind/wrapper_base.hpp \
    Scripts/luabind/weak_ref.hpp \
    Scripts/luabind/version.hpp \
    Scripts/luabind/value_wrapper.hpp \
    Scripts/luabind/typeid.hpp \
    Scripts/luabind/tag_function.hpp \
    Scripts/luabind/shared_ptr_converter.hpp \
    Scripts/luabind/scope.hpp \
    Scripts/luabind/return_reference_to_policy.hpp \
    Scripts/luabind/raw_policy.hpp \
    Scripts/luabind/prefix.hpp \
    Scripts/luabind/out_value_policy.hpp \
    Scripts/luabind/operator.hpp \
    Scripts/luabind/open.hpp \
    Scripts/luabind/object.hpp \
    Scripts/luabind/nil.hpp \
    Scripts/luabind/make_function.hpp \
    Scripts/luabind/lua_include.hpp \
    Scripts/luabind/luabind.hpp \
    Scripts/luabind/iterator_policy.hpp \
    Scripts/luabind/handle.hpp \
    Scripts/luabind/get_pointer.hpp \
    Scripts/luabind/get_main_thread.hpp \
    Scripts/luabind/function.hpp \
    Scripts/luabind/from_stack.hpp \
    Scripts/luabind/exception_handler.hpp \
    Scripts/luabind/error.hpp \
    Scripts/luabind/discard_result_policy.hpp \
    Scripts/luabind/dependency_policy.hpp \
    Scripts/luabind/copy_policy.hpp \
    Scripts/luabind/container_policy.hpp \
    Scripts/luabind/config.hpp \
    Scripts/luabind/class_info.hpp \
    Scripts/luabind/class.hpp \
    Scripts/luabind/back_reference_fwd.hpp \
    Scripts/luabind/back_reference.hpp \
    Scripts/luabind/adopt_policy.hpp \
    Scripts/lua/lzio.h \
    Scripts/lua/lvm.h \
    Scripts/lua/lundump.h \
    Scripts/lua/lualib.h \
    Scripts/lua/luaconf.h \
    Scripts/lua/lua.hpp \
    Scripts/lua/lua.h \
    Scripts/lua/ltm.h \
    Scripts/lua/ltable.h \
    Scripts/lua/lstring.h \
    Scripts/lua/lstate.h \
    Scripts/lua/lparser.h \
    Scripts/lua/lopcodes.h \
    Scripts/lua/lobject.h \
    Scripts/lua/lmem.h \
    Scripts/lua/llimits.h \
    Scripts/lua/llex.h \
    Scripts/lua/lgc.h \
    Scripts/lua/lfunc.h \
    Scripts/lua/ldo.h \
    Scripts/lua/ldebug.h \
    Scripts/lua/lcode.h \
    Scripts/lua/lauxlib.h \
    Scripts/lua/lapi.h \
    Scripts/luabind/detail/yes_no.hpp \
    Scripts/luabind/detail/typetraits.hpp \
    Scripts/luabind/detail/stack_utils.hpp \
    Scripts/luabind/detail/signature_match.hpp \
    Scripts/luabind/detail/ref.hpp \
    Scripts/luabind/detail/property.hpp \
    Scripts/luabind/detail/primitives.hpp \
    Scripts/luabind/detail/policy.hpp \
    Scripts/luabind/detail/pointee_typeid.hpp \
    Scripts/luabind/detail/pointee_sizeof.hpp \
    Scripts/luabind/detail/pcall.hpp \
    Scripts/luabind/detail/other.hpp \
    Scripts/luabind/detail/operator_id.hpp \
    Scripts/luabind/detail/open.hpp \
    Scripts/luabind/detail/object_rep.hpp \
    Scripts/luabind/detail/object_funs.hpp \
    Scripts/luabind/detail/object_call.hpp \
    Scripts/luabind/detail/most_derived.hpp \
    Scripts/luabind/detail/make_instance.hpp \
    Scripts/luabind/detail/link_compatibility.hpp \
    Scripts/luabind/detail/is_indirect_const.hpp \
    Scripts/luabind/detail/instance_holder.hpp \
    Scripts/luabind/detail/inheritance.hpp \
    Scripts/luabind/detail/has_get_pointer.hpp \
    Scripts/luabind/detail/garbage_collector.hpp \
    Scripts/luabind/detail/format_signature.hpp \
    Scripts/luabind/detail/enum_maker.hpp \
    Scripts/luabind/detail/deduce_signature.hpp \
    Scripts/luabind/detail/decorate_type.hpp \
    Scripts/luabind/detail/debug.hpp \
    Scripts/luabind/detail/convert_to_lua.hpp \
    Scripts/luabind/detail/conversion_storage.hpp \
    Scripts/luabind/detail/constructor.hpp \
    Scripts/luabind/detail/compute_score.hpp \
    Scripts/luabind/detail/class_rep.hpp \
    Scripts/luabind/detail/class_registry.hpp \
    Scripts/luabind/detail/class_cache.hpp \
    Scripts/luabind/detail/call_operator_iterate.hpp \
    Scripts/luabind/detail/call_member.hpp \
    Scripts/luabind/detail/call_function.hpp \
    Scripts/luabind/detail/call.hpp \
    Scripts/luabind/detail/calc_arity.hpp \
    Lights/AreaLight.h \
    Geometry/CornellBox.h \
    Texture.h \
    PhotonMap.h \
    KDTree.h \
    RayTracer.h \
    Geometry/MS3DModel.h \
    Geometry/Box.h \
    StreamPhotonMap.h

SOURCES += Math/Vector2.cpp \
           Math/Vector3.cpp \
           Math/Vector4.cpp \
           Math/Ray.cpp \
           Math/Matrix4x4.cpp \
           main.cpp \
           Geometry/Sphere.cpp \
    Math/Plane.cpp \
    Geometry/Triangle.cpp \
    LightIntensity.cpp \
    Image.cpp \
    Camera.cpp \
    Lights/AmbientLight.cpp \
    Lights/PointLight.cpp \
    Material.cpp \
    Scene.cpp \
    DisplayWindow.cpp \
    MainWindow.cpp \
    Scripts/ScriptManager.cpp \
    Scripts/Script.cpp \
    Scripts/lua/print.c \
    Scripts/lua/lzio.c \
    Scripts/lua/lvm.c \
    Scripts/lua/lundump.c \
    Scripts/lua/ltm.c \
    Scripts/lua/ltablib.c \
    Scripts/lua/ltable.c \
    Scripts/lua/lstrlib.c \
    Scripts/lua/lstring.c \
    Scripts/lua/lstate.c \
    Scripts/lua/lparser.c \
    Scripts/lua/loslib.c \
    Scripts/lua/lopcodes.c \
    Scripts/lua/lobject.c \
    Scripts/lua/loadlib.c \
    Scripts/lua/lmem.c \
    Scripts/lua/lmathlib.c \
    Scripts/lua/llex.c \
    Scripts/lua/liolib.c \
    Scripts/lua/linit.c \
    Scripts/lua/lgc.c \
    Scripts/lua/lfunc.c \
    Scripts/lua/ldump.c \
    Scripts/lua/ldo.c \
    Scripts/lua/ldebug.c \
    Scripts/lua/ldblib.c \
    Scripts/lua/lcode.c \
    Scripts/lua/lbaselib.c \
    Scripts/lua/lauxlib.c \
    Scripts/lua/lapi.c \
    Scripts/luabind/src/wrapper_base.cpp \
    Scripts/luabind/src/weak_ref.cpp \
    Scripts/luabind/src/stack_content_by_name.cpp \
    Scripts/luabind/src/scope.cpp \
    Scripts/luabind/src/pcall.cpp \
    Scripts/luabind/src/open.cpp \
    Scripts/luabind/src/object_rep.cpp \
    Scripts/luabind/src/link_compatibility.cpp \
    Scripts/luabind/src/inheritance.cpp \
    Scripts/luabind/src/function.cpp \
    Scripts/luabind/src/exception_handler.cpp \
    Scripts/luabind/src/error.cpp \
    Scripts/luabind/src/create_class.cpp \
    Scripts/luabind/src/class_rep.cpp \
    Scripts/luabind/src/class_registry.cpp \
    Scripts/luabind/src/class_info.cpp \
    Scripts/luabind/src/class.cpp \
    Lights/AreaLight.cpp \
    Geometry/CornellBox.cpp \
    Texture.cpp \
    PhotonMap.cpp \
    KDTree.cpp \
    RayTracer.cpp \
    Geometry/MS3DModel.cpp \
    Geometry/Box.cpp \
    StreamPhotonMap.cpp

QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp

OTHER_FILES += \
    Scripts/luabind/src/src.pro.user



















