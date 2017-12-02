#include "earth_renderer.hpp"
#include "sphere.hpp"
#include <memory>
#include <GLES3/gl3.h>
#include "../deps/glm/ext.hpp"
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "../logging.hpp"
#include <android/log.h>
#include <android/bitmap.h>
#include <gtx/extended_min_max.inl>
#include "../util/assets_file_reader.hpp"
#include "tile.hpp"

extern "C" {
#include "../util/image.h"
}

#define BUFFER_OFFSET(i) ((void*)(i))
namespace OpenEarth {
    static const char *const JavaClassName = "com/geocompass/openearth/sdk/earth/EarthRenderer";
    static const char *const TAG = "earth_renderer_cpp";
    static const int const X_AXIS = 0;
    static const int const Y_AXIS = 1;
    static const int const Z_AXIS = 2;
    std::unique_ptr<OpenEarth::Sphere> sphere;
    Tile *tile1;
    Tile *tile2;
    GLuint d_glprogram;
    int aPostionLocaiton;
    int aTextureLocation;
    int uTextureUnitLocation;
    int uProjectionLocation;
    glm::mat4x4 mModelMatrix;
    glm::mat4x4 mViewMatrix;
    glm::mat4x4 mProjectionMatrix;
    glm::mat4x4 mMvpMatrix;

    AAssetManager* mAssetManager;
    //函数声明
    void drawEarth();

    //构造和析构函数
    OpenEarth::EarthRenderer::EarthRenderer() {

    }

    OpenEarth::EarthRenderer::~EarthRenderer() {
        if (sphere)
            sphere.reset();

        if (tile1) {
            delete tile1;
        }

        if(tile2)
            delete tile2;

        delete mAssetManager;
    }

    void initialize(){
        mModelMatrix = glm::mat4(1.0f);

        mViewMatrix  = glm::lookAt(
                glm::vec3(1.0f, 0.0f, 1.0f), //
                glm::vec3(0.0f, 0.0f, 0.0f), //
                glm::vec3(0.0f, 1.0f, 0.0f)  //
        );

    }

    void rotateEarth(JNIEnv *env, jobject instance, jint axis, jfloat radian){
        switch(axis){
            case X_AXIS:
                mModelMatrix = glm::rotate(mModelMatrix,radian,glm::vec3(1.0f,0.0f,0.0f));
                break;
            case Y_AXIS:
                mModelMatrix = glm::rotate(mModelMatrix,radian,glm::vec3(0.0f,1.0f,0.0f));
                break;
            case Z_AXIS:
                mModelMatrix = glm::rotate(mModelMatrix,radian,glm::vec3(0.0f,0.0f,1.0f));
                break;
        }

    }

    /**
 * 加载图片
 */
    GLuint loadTexture(const char* name) {
        //获取类
        FileData fileData = OpenEarth::util::AssetsFileReader::get_asset_data(name, mAssetManager);

        RawImageData data = get_raw_image_data_from_png(fileData.data, (int) fileData.data_length);
        GLuint textureId;
        glGenTextures(1, &textureId);
        assert(textureId != 0);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
                GL_TEXTURE_2D, 0, data.gl_color_format, data.width, data.height, 0,
                data.gl_color_format, GL_UNSIGNED_BYTE, data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureId;
    }


    void surfaceCreated(JNIEnv *env, jobject instance) {
        jclass objectClass = env->GetObjectClass(instance);
        //获取函数句柄
        jmethodID methodID = env->GetMethodID(objectClass, "getAssetManager",
                                               "()Landroid/content/res/AssetManager;");
        //从Java获取AssetManager
        jobject javaAssetManager = env->CallObjectMethod(instance, methodID);
        mAssetManager = AAssetManager_fromJava(env, javaAssetManager);
//        sphere = std::make_unique<OpenEarth::Sphere>(1.0f);
        tile1 = new Tile(0, 0, 1);
        tile2 = new Tile(1, 0, 1);
        GLuint glProgram;
        GLuint vertexShader;
        GLuint fragmentShader;
        //shader code
        const char *shader_vertex =
                "uniform mat4 u_MVPMatrix; \n"
                        "attribute vec4 POSITION;\n"
                        "attribute vec2 a_TextureCoordinates;\n"
                        "varying vec2 v_TextureCoordinates;\n"
                        "void main(){\n"
                            "gl_Position = u_MVPMatrix*POSITION;\n"
                            "v_TextureCoordinates = a_TextureCoordinates;\n"
                        "}";
        const char *shader_fragment = "precision mediump float;\n"
                "uniform sampler2D u_TextureUnit;   \n"
                "varying vec2 v_TextureCoordinates; \n"
                "void main(){\n"
                "   gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);\n"
                "}";
        glProgram = glCreateProgram();


        if (glProgram == 0) {
            return;
        }


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
         int linkStatus ;
        glGetProgramiv(glProgram,GL_LINK_STATUS,&linkStatus);
        if(linkStatus==0){
            glDeleteProgram(glProgram);
        }
        d_glprogram = glProgram;

    }

    void surfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {

        glViewport(0, 0, width, height);
        const GLfloat ratio = (GLfloat) width / height;
        const GLfloat left = width < height ? -1.0f : -1.0f * ratio;
        const GLfloat right = width < height ? 1.0f : 1.0f * ratio;
        const GLfloat bottom = width < height ? -1.0f / ratio : -1.0f;
        const GLfloat top = width < height ? 1.0f / ratio : 1.0f;
        const GLfloat near = 1.0f;
        const GLfloat far = 10.0f;

        mProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);


    }

    GLuint create_vbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage) {
        assert(data != NULL);
        GLuint vbo_object;
        glGenBuffers(1, &vbo_object);
        assert(vbo_object != 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vbo_object;
    }

    void render(JNIEnv *env, jobject instance) {
        drawEarth();
    }

    void drawEarth(){
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        GLuint  textureId1 =  loadTexture("west.png");
        GLuint  textureId2 =  loadTexture("east.png");
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(d_glprogram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId1);
//        GLfloat * vertexArray = tile->getVertexArray();

        aPostionLocaiton = glGetAttribLocation(d_glprogram, "POSITION");
        aTextureLocation = glGetAttribLocation(d_glprogram, "a_TextureCoordinates");
        uTextureUnitLocation = glGetUniformLocation(d_glprogram, "u_TextureUnit");
        uProjectionLocation = glGetUniformLocation(d_glprogram, "u_MVPMatrix");

        glUniform1i(uTextureUnitLocation, 0);

        mMvpMatrix = mProjectionMatrix * mViewMatrix * mModelMatrix;
        glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, glm::value_ptr(mMvpMatrix));

//        GLuint buffer = create_vbo(sizeof(vertexArray),vertexArray,GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glVertexAttribPointer(aPostionLocaiton, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), tile1->getVertexArray());
        glEnableVertexAttribArray(aPostionLocaiton);
//        (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
        glVertexAttribPointer(aTextureLocation,2,GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),tile1->getTextureVertexArray());
        glEnableVertexAttribArray(aPostionLocaiton);
        glEnableVertexAttribArray(aTextureLocation);

        int size = tile1->getVertexArraySize();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, size);

        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId2);

        glVertexAttribPointer(aPostionLocaiton, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), tile2->getVertexArray());
        glEnableVertexAttribArray(aPostionLocaiton);
//        (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
        glVertexAttribPointer(aTextureLocation,2,GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),tile2->getTextureVertexArray());
        glEnableVertexAttribArray(aPostionLocaiton);
        glEnableVertexAttribArray(aTextureLocation);

        size = tile2->getVertexArraySize();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
    }




    static JNINativeMethod gMethods[] = {
            {"nativeSurfaceCreated", "()V",   (void *) surfaceCreated},
            {"nativeSurfaceChanged", "(II)V", (void *) surfaceChanged},
            {"nativeRender",         "()V",   (void *) render},
            {"nativeRotateEarth",   "(IF)V",  (void *) rotateEarth},
            {"nativeInitialize",   "()V",     (void *) initialize}
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
