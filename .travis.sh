function generate() {
    cmake -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -G "Unix Makefiles"
}

generate \
&& make -j4 \
&& sudo make install \
&& sudo ldconfig \
&& (cd test/unit; ../../bin/osre_unittest) \
