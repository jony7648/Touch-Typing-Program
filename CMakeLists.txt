set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.21)
project(App CXX)



find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
include_directories(".")
include_directories("./libraries")
set(CORE_DIR "core")


set (GC_DIR "gc_widgets")
set (LIBRARY_DIR "libraries")

set (PROJECT_DIR "project")
set (SCENE_DIR "${PROJECT_DIR}/scenes")

set(LIBRAIES
)


set(PROJECT_SRC
	"${PROJECT_DIR}/text_loader.c"
)

set(SCENES
	"${SCENE_DIR}/typing_scene.c"
)

set(CORE_C 
	"${CORE_DIR}/types.c"
	"${CORE_DIR}/util.c"
	"${CORE_DIR}/file_util.c"
	"${CORE_DIR}/str_view.c"
	"${CORE_DIR}/cstr.c"
	"${CORE_DIR}/arena.c" 
	"${CORE_DIR}/slice.c"
	"${CORE_DIR}/time_componet.c"
	"${CORE_DIR}/keyboard.c" 
	"${CORE_DIR}/listener.c"
	"${CORE_DIR}/timer.c"
	"${CORE_DIR}/app.c" 
	"${CORE_DIR}/window.c"
	"${CORE_DIR}/scene.c"
	"${CORE_DIR}/global_manager.c"
)

set (GC
	"${GC_DIR}/gc_widget.c"
	"${GC_DIR}/button.c"
	"${GC_DIR}/container.c"
	"${GC_DIR}/text_field.c"
	"${GC_DIR}/text_view.c"
	"${GC_DIR}/scroll_window.c"
	"${GC_DIR}/gc_list.c"
)

add_executable(
	app 
	main.c 
	${LIBRAIES} 
	${CORE_C} 
	${GC} 
	${SCENES} 
	${PROJECT_SRC}
)

set_target_properties(app PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

project(app LANGUAGES C)
target_compile_options(app PRIVATE ${GTK4_CFLAGS})
target_link_libraries(app PkgConfig::GTK4)




