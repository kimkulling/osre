function generate() {
    cmake -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -G "Unix Makefiles"
}

generate \
&& make -j4 \
&& cd bin \
&& ./osre_unittest \
