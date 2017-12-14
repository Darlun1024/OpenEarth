#include "earth_renderer.hpp"
#include "sphere.hpp"
#include <memory>
#include <GLES3/gl3.h>
#include <glm/ext.hpp>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <android/bitmap.h>
#include <glm/gtx/extended_min_max.inl>
#include "../util/assets_file_reader.hpp"
#include "tile.hpp"
#include "../logging.hpp"
#include "earth.hpp"

extern "C" {
    #include "../util/png_reader.h"
}

#define  DEFAULT_EYE_HEIGHT 1.0f
#define  DEFAULT_EARTH_RADIUS 1.0f

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
    int aPositionLocaiton;
    int aTextureLocation;
    int uTextureUnitLocation;
    int uProjectionLocation;

    AAssetManager* aAssetManager;

    glm::vec2  screenSize;
    glm::mat4x4 gModelMatrix;
    glm::mat4x4 gViewMatrix;
    glm::mat4x4 gProjectionMatrix;
    glm::mat4x4 gMvpMatrix;

    float earthRotateX = 0.0f;
    float earthRotateY = 0.0f;
    float earthRotateZ = 0.0f;
    float earthScale   = 1.0f;
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

        delete aAssetManager;
    }

    void initialize(){
        gModelMatrix = glm::mat4(1.0f);  //模型矩阵
        gModelMatrix = glm::translate(gModelMatrix,glm::vec3(0,0,-OpenEarth::Earth::getRadius()*earthScale));
        gViewMatrix  = glm::lookAt(
                glm::vec3(0.0f, 0.0f, DEFAULT_EYE_HEIGHT), //眼睛位置
                glm::vec3(0.0f, 0.0f, 0.0f), //瞄准的点
                glm::vec3(0.0f, 1.0f, 0.0f)  //头顶方向的法向量
        );


    }
    /**
     * 这里采用调整视角的方式旋转
     * @param env
     * @param instance
     * @param axis
     * @param radian
     */
    void rotateEarth(JNIEnv *env, jobject instance, jint axis, jfloat radian){
        switch(axis){
            case X_AXIS:
                earthRotateX += radian;
                gModelMatrix = glm::rotate(gModelMatrix,radian,glm::vec3(1.0f,0.0f,0.0f));
                break;
            case Y_AXIS:
                earthRotateY +=radian;
                gModelMatrix = glm::rotate(gModelMatrix,radian,glm::vec3(0.0f,1.0f,0.0f));
                break;
            case Z_AXIS:
                earthRotateZ +=radian;
                gModelMatrix = glm::rotate(gModelMatrix,radian,glm::vec3(0.0f,0.0f,1.0f));
                break;
        }
    }

    void updateModelMatrix(){
        gModelMatrix = glm::mat4(1.0f);  //模型矩阵
        gModelMatrix = glm::translate(gModelMatrix,glm::vec3(0,0,-OpenEarth::Earth::getRadius()*earthScale)); //移动地心，使得地球总在显示范围内部
        //设置旋转
        gModelMatrix = glm::rotate(gModelMatrix,earthRotateX,glm::vec3(1.0f,0.0f,0.0f));
        gModelMatrix = glm::rotate(gModelMatrix,earthRotateY,glm::vec3(0.0f,1.0f,0.0f));
        gModelMatrix = glm::rotate(gModelMatrix,earthRotateZ,glm::vec3(0.0f,0.0f,1.0f));
    }

    //重新绘制地球
    void updateEarth(){
        updateModelMatrix();
        tile1->reset();
        tile2->reset();
    }

    //设置球体的各个参数
    void setScale(JNIEnv *env, jobject instance,jfloat scale){

    }

    void setZoom(JNIEnv *env, jobject instance,jfloat zoom){
        earthScale = 1.0f;
        OpenEarth::Earth::setRadius(OpenEarth::DEFAULT_RADIUS * pow(2,zoom-1));
        updateEarth();
    }
    void setTilt(JNIEnv *env, jobject instance,jfloat tilt){

    }


    void surfaceCreated(JNIEnv *env, jobject instance) {
        jclass objectClass = env->GetObjectClass(instance);
        //获取函数句柄
        jmethodID methodID = env->GetMethodID(objectClass, "getAssetManager",
                                               "()Landroid/content/res/AssetManager;");
        //从Java获取AssetManager
        jobject javaAssetManager = env->CallObjectMethod(instance, methodID);
        aAssetManager = AAssetManager_fromJava(env, javaAssetManager);
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
        screenSize = glm::vec2(width,height);
        const GLfloat ratio  = (GLfloat) width / height;
        const GLfloat left   = width < height ? -1.0f : -1.0f * ratio;
        const GLfloat right  = width < height ? 1.0f : 1.0f * ratio;
        const GLfloat bottom = width < height ? -1.0f / ratio : -1.0f;
        const GLfloat top    = width < height ? 1.0f / ratio : 1.0f;
        const GLfloat near   = 1.0f;
        const GLfloat far    = 10.0f;

//        mProjectionMatrix = glm::ortho(left, right, bottom, top, near, far); //正交投影
        gProjectionMatrix = glm::perspective(90.0f,ratio,near,far); //透视投影

        //屏幕坐标转为归一化设备坐标
//         float normalizeX = width/2;
//         float normalizeY = height/2;
         float normalizeX = 0;
         float normalizeY = 0;
//        glm::mat4 invVP = glm::inverse(mProjectionMatrix * mViewMatrix);
//        glm::vec4 screenPos = glm::vec4(normalizeX, normalizeY, 1.0f, 1.0f);
//        glm::vec4 worldPos = invVP * screenPos;

//        glm::vec3 dir = glm::normalize(glm::vec3(worldPos));


}


void render(JNIEnv *env, jobject instance) {
        drawEarth();
    }

    void drawEarth(){
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glUseProgram(d_glprogram);

        aPositionLocaiton = glGetAttribLocation(d_glprogram, "POSITION");
        aTextureLocation = glGetAttribLocation(d_glprogram, "a_TextureCoordinates");
        uTextureUnitLocation = glGetUniformLocation(d_glprogram, "u_TextureUnit");
        uProjectionLocation = glGetUniformLocation(d_glprogram, "u_MVPMatrix");

        glUniform1i(uTextureUnitLocation, 0);

        gMvpMatrix = gProjectionMatrix * gViewMatrix * gModelMatrix;
        glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, glm::value_ptr(gMvpMatrix));

        tile1->draw(aPositionLocaiton,aTextureLocation,aAssetManager,"west.jpeg");
        tile2->draw(aPositionLocaiton,aTextureLocation,aAssetManager,"east.jpeg");

    }


    static JNINativeMethod gMethods[] = {
            {"nativeSurfaceCreated", "()V",   (void *) surfaceCreated},
            {"nativeSurfaceChanged", "(II)V", (void *) surfaceChanged},
            {"nativeRender",         "()V",   (void *) render},
            {"nativeRotateEarth",   "(IF)V",  (void *) rotateEarth},
            {"nativeInitialize",   "()V",     (void *) initialize},
            {"nativeSetScale",   "(F)V",     (void *) setScale},
            {"nativeSetTilt",   "(F)V",     (void *) setTilt},
            {"nativeSetZoom",   "(F)V",     (void *) setZoom}
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
