add_library(
	glm 
	SHARED
    # File source
    src/main/cpp/glm/vec2.hpp
    src/main/cpp/glm/vec3.hpp
    src/main/cpp/glm/vec4.hpp
    src/main/cpp/glm/mat2x2.hpp
    src/main/cpp/glm/mat2x3.hpp
    src/main/cpp/glm/mat2x4.hpp
    src/main/cpp/glm/mat3x2.hpp
    src/main/cpp/glm/mat3x3.hpp
    src/main/cpp/glm/mat3x4.hpp
    src/main/cpp/glm/mat4x2.hpp
    src/main/cpp/glm/mat4x3.hpp
    src/main/cpp/glm/mat4x4.hpp
    src/main/cpp/glm/matrix.hpp
    src/main/cpp/glm/gtc/matrix_transform.hpp
    src/main/cpp/glm/gtc/matrix_inverse.hpp
)




SET_TARGET_PROPERTIES(glm PROPERTIES LINKER_LANGUAGE CXX)
