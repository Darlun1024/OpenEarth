#include "earth_renderer.hpp"
#include "sphere.hpp"
#include <memory>
#include <GLES3/gl3.h>


namespace OpenEarth {
    static const char *const JavaClassName = "com/geocompass/openearth/sdk/earth/EarthRenderer";
    std::unique_ptr<OpenEarth::Sphere> sphere;
    GLuint d_glprogram;
    GLfloat gModelMatrix[16] = {0.0f};
    GLfloat gViewMatrix[16] = {0.0f};
    GLfloat gProjectMatrix[16] = {0.0f};
    GLfloat gMVPMatrix[16] = {0.0f};

    //构造和析构函数
    OpenEarth::EarthRenderer::EarthRenderer() {

    }

    OpenEarth::EarthRenderer::~EarthRenderer() {
        if (sphere)
            sphere.reset();
    }


    void surfaceCreated(JNIEnv *env, jobject instance) {
        sphere = std::make_unique<OpenEarth::Sphere>(1.0f);
        GLuint glProgram;
        GLuint vertexShader;
        GLuint fragmentShader;
        //shader code
        const char *shader_vertex = "uniform mediump mat4 MODELVIEWPROJECTIONMATRIX;\n"
                "uniform mat4 u_MVPMatrix; \n"
                "attribute vec4 POSITION;\n"
                "void main(){\n"
                "  gl_Position = POSITION;\n"
                "}";
        const char *shader_fragment = "precision mediump float;\n"
                "void main(){\n"
                "   gl_FragColor = vec4(0,0,1,1);\n"
                "}";
        glProgram = glCreateProgram();


        if (glProgram == 0) {
            return;
        }

        d_glprogram = glProgram;

//    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

        //vertexShader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &shader_vertex, NULL);

        //fragmentShader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &shader_fragment, NULL);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        glAttachShader(glProgram, vertexShader);
        glAttachShader(glProgram, fragmentShader);

        glLinkProgram(glProgram);
    }

    void surfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
        GLfloat aspect = width / height;
        glViewport(0, 0, width, height);
        const GLfloat ratio = (GLfloat) width / height;
        const GLfloat left = -ratio;
        const GLfloat right = ratio;
        const GLfloat bottom = -1.0f;
        const GLfloat top = 1.0f;
        const GLfloat near = 1.0f;
        const GLfloat far = 10.0f;

//        glm::frustum(left, right, bottom, top, near, far);
    }

    void render(JNIEnv *env, jobject instance) {

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //vertex array
//        GLfloat vertexs[] = {
//                0.0f, 1.0f, 0.0f,
//                -1.0f, -1.0f, 0.0f,
//                1.0f, -1.0f, 0.0f
//        };

        glUseProgram(d_glprogram);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, sphere->getVertexArray());
        glEnableVertexAttribArray(0);
        int projectLocation = glGetUniformLocation(d_glprogram, "u_MVPMatrix");
        glUniformMatrix4fv(projectLocation, 1, GL_FALSE, gMVPMatrix);
        int size = sphere->getVertextSize();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
    }


// {"getStringFromJNI", "()Ljava/lang/String;", (jstring*)getString}
//三个参数分别为 Java中的方法名，参数和返回类型，C++中的函数名
    static JNINativeMethod gMethods[] = {
            {"nativeSurfaceCreated", "()V",   (void *) surfaceCreated},
            {"nativeSurfaceChanged", "(II)V", (void *) surfaceChanged},
            {"nativeRender",         "()V",   (void *) render}
    };

//注册native 方法
    static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                     int numMethods) {
        jclass clazz;
        clazz = env->FindClass(className);
        if (clazz == NULL) {
            return JNI_FALSE;
        }
        if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
            return JNI_FALSE;
        }
        return JNI_TRUE;
    }

    void OpenEarth::EarthRenderer::registerNative(JNIEnv *env) {
        registerNativeMethods(env, JavaClassName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
    }
}
