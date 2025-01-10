all : index.html

EMCC:=C:/Program\ Files/emsdk/upstream/emscripten/em++

IMGUI:=imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui.cpp
IMGUI_BACKENDS:=imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp
INCLUDES:=-Istb -Iimgui -Isrc
LIBS:=-lembind -lGL -lSDL2
STATICS:=-sAUTO_JS_LIBRARIES -sUSE_GLFW=3 -sUSE_SDL -sTEXTDECODER=2 -sMIN_CHROME_VERSION=80 -sABORTING_MALLOC=0 -sASSERTIONS=0 -sEXPORTED_RUNTIME_METHODS=ccall
FILES:=--embed-file assets --shell-file shell.html

index.html : src/main.cpp src/renderer/renderer.cpp src/sid-api/libcsidlight.cpp $(IMGUI) $(IMGUI_BACKENDS)
	$(EMCC) -Oz $(INCLUDES) -std=c++20 $(LIBS) $(FILES) $(STATICS) $^ -o $@

rebuild : clean index.html

clean : 
	del index.html;del index.data;del index.js;del index.wasm