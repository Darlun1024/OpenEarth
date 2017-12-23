#define GLM_ENABLE_EXPERIMENTAL

#include "earth_renderer.hpp"
#include "sphere.hpp"
#include <memory>
#include <GLES3/gl3.h>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <android/bitmap.h>
#include <glm/geometric.hpp>
#include "../util/assets_file_reader.hpp"
#include "tile.hpp"
#include "earth.hpp"
#include "opengl_project.hpp"
#include "transform.hpp"
#include "../util/util.hpp"
#include "geometry/geometry_util.hpp"
#include "../shaders/raster_shader.hpp"

#define  DEFAULT_EYE_HEIGHT 1.0f

namespace OpenEarth {
    static const char *const JavaClassName = "com/geocompass/openearth/sdk/earth/EarthRenderer";
    static const char *const TAG = "earth_renderer_cpp";
    static const int const X_AXIS = 0;
    static const int const Y_AXIS = 1;
    static const int const Z_AXIS = 2;
    std::unique_ptr<OpenEarth::Sphere> sphere;
    std::shared_ptr<OpenEarth::OpenGLProject> gProject;
    std::unique_ptr<OpenEarth::Transform> gTransform;
    Tile *tile1;
    Tile *tile2;
    GLuint d_glprogram;
    int aPositionLocaiton;
    int aTextureLocation;
    int uTextureUnitLocation;
    int uProjectionLocation;

    AAssetManager *aAssetManager;

    glm::vec2 screenSize;
    glm::mat4x4 gModelMatrix;
    glm::mat4x4 gViewMatrix;
    glm::mat4x4 gProjectionMatrix;
    glm::mat4x4 gMvpMatrix;

    float earthRotateX = 0.0f;
    float earthRotateY = 0.0f;
    float earthRotateZ = 0.0f;


    //函数声明
    void drawEarth();

    float cameraTargetCenterY = 0.0f;

    //构造和析构函数
    OpenEarth::EarthRenderer::EarthRenderer() {
        glm::unProject(glm::vec3(1), glm::mat4(1), glm::mat4(1), glm::vec4(1));

    }

    OpenEarth::EarthRenderer::~EarthRenderer() {
        if (sphere)
            sphere.reset();

        if (tile1) {
            delete tile1;
        }

        if (tile2)
            delete tile2;

        delete aAssetManager;
    }

    void initialize() {
        OpenEarth::Earth::initialize();
        gModelMatrix =  OpenEarth::Earth::getModelMatrix();

        gViewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 1.0f), //眼睛位置
                glm::vec3(0.0f, cameraTargetCenterY, 0.0f), //瞄准的点
                glm::vec3(0.0f, 1.0f, 0.0f)  //头顶方向的法向量
        );
        gProjectionMatrix = glm::mat4(1.0f);
        screenSize = glm::vec2(0,0);
        gProject   = std::make_shared<OpenEarth::OpenGLProject>(gViewMatrix,gProjectionMatrix,screenSize);
        gTransform = std::make_unique<OpenEarth::Transform>(gModelMatrix,gProject);
    }

    void updateModelMatrix() {
        OpenEarth::Earth::updateModelMatrix();
        gModelMatrix = OpenEarth::Earth::getModelMatrix();
        gTransform -> setModelMatrix(gModelMatrix);
    }

    /**
     * 自由旋转球体
     * @param env
     * @param instance
     * @param screenPoint1
     * @param screenPoint2
     */
    void rotateEarth(JNIEnv *env, jobject instance, jfloatArray screenPoint1,jfloatArray screenPoint2){
        jboolean isCopy = true;
        jfloat* array1 = env->GetFloatArrayElements(screenPoint1,&isCopy);
        jfloat* array2 = env->GetFloatArrayElements(screenPoint2,&isCopy);
        glm::vec2 latlng1 = gTransform->screenPointToLatlng(glm::vec2(array1[0],array1[1]));
        glm::vec2 latlng2 = gTransform->screenPointToLatlng(glm::vec2(array2[0],array2[1]));
        if(!gTransform->isValidLatlng(latlng1) || ! gTransform->isValidLatlng(latlng2))
            return;
        float deltaLat = latlng2[0] - latlng1[0];
        float deltaLon = latlng2[1] - latlng1[1];
        if(deltaLat * (array2[1]-array1[1]) > 0) deltaLat = -deltaLat;  //在两级附近
        //TODO 处理子午线上的情况
        OpenEarth::Earth::rotate(deltaLat,deltaLon);
        updateModelMatrix();
    }


    /**
     * 设置中心点坐标
     * @param env
     * @param instance
     * @param latlng
     */
    void setCenter(JNIEnv *env, jobject instance, jfloatArray latlng){
        //默认地图中心是 (0,0)
        jboolean isCopy = true;
        jfloat* array = env->GetFloatArrayElements(latlng,&isCopy);
        OpenEarth::Earth::setCenterLatLng(new LatLng(array[0],array[1]));
        updateModelMatrix();
    }

    /**
     * 获取中心坐标
     * @param env
     * @param instance
     * @return
     */
    jfloatArray getCenter(JNIEnv *env, jobject instance){
        jfloat array1[2] = {-earthRotateX,-earthRotateY};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray,0,2,array1);
        return floatArray;
    }



    //重新绘制地球
    void updateEarth() {
        updateModelMatrix();
        tile1->reset();
        tile2->reset();
    }

    //设置球体的缩放比例
    void setScale(JNIEnv *env, jobject instance, jfloat scale){
        if(OpenEarth::Earth::setScale(scale))
            updateEarth();
        else
            updateModelMatrix();
    }

    /**获取球体的缩放比例(1.0 2.0)*/
    jfloat getScale(){
        return OpenEarth::Earth::getScale();
    }


    //设置球体的显示级别,不同的级别会采用不用的纹理
    void setZoom(JNIEnv *env, jobject instance, jfloat zoom) {
        if(OpenEarth::Earth::setZoom(zoom))
            updateEarth();
    }
    //获取球体的显示级别
    jint getZoom(){
        return OpenEarth::Earth::getZoom();
    }

    //修改摄像头瞄准的点
    void setTilt(JNIEnv *env, jobject instance, jfloat tilt) {
        //TODO 根据角度计算
        cameraTargetCenterY = tilt;
        gViewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, DEFAULT_EYE_HEIGHT), //眼睛位置
                glm::vec3(0.0f, cameraTargetCenterY, 0.0f), //瞄准的点
                glm::vec3(0.0f, 1.0f, 0.0f)  //头顶方向的法向量
        );
        gProject->setViewMatrix(gViewMatrix);
    }

    jfloatArray screen2World(JNIEnv *env, jobject instance, jfloatArray point){
        jboolean isCopy = true;
        jfloat* array = env->GetFloatArrayElements(point,&isCopy);
        glm::vec3 world = gProject->unProject(glm::vec2(array[0],array[1]),0);
        jfloat array1[3] = {world[0],world[1],world[2]};
        jfloatArray floatArray = env->NewFloatArray(3);
        env->SetFloatArrayRegion(floatArray,0,3,array1);
        return floatArray;
    }

    jfloatArray world2Screen(JNIEnv *env, jobject instance, jfloatArray point){
        jboolean isCopy = true;
        float* array = env->GetFloatArrayElements(point,&isCopy);
        glm::vec2 screen = gProject->project(glm::vec3(array[0],array[1],array[2]));
        jfloat array1[2] = {screen[0],screen[1]};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray,0,2,array1);
        return floatArray;
    }

    jfloatArray screen2LatLng(JNIEnv *env, jobject instance, jfloatArray point){
        jboolean isCopy = true;
        jfloat* array = env->GetFloatArrayElements(point,&isCopy);
        glm::vec2 latlng = gTransform->screenPointToLatlng(glm::vec2(array[0],array[1]));
        jfloat array1[2] = {latlng[0],latlng[1]};
        jfloatArray floatArray = env->NewFloatArray(3);
        env->SetFloatArrayRegion(floatArray,0,2,array1);
        return floatArray;
    }

    jfloatArray latLng2Screen(JNIEnv *env, jobject instance, jfloatArray latlng){
        jboolean isCopy = true;
        float* array = env->GetFloatArrayElements(latlng,&isCopy);
        glm::vec2 screen = gTransform->latLngToScreenPoint(new LatLng(array[0],array[1]));
        jfloat array1[2] = {screen[0],screen[1]};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray,0,2,array1);
        return floatArray;
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
        const char *shader_vertex = OpenEarth::Shader::RasterShader::veterxShader;
        const char *shader_fragment = OpenEarth::Shader::RasterShader::fragmentShader;
        glProgram = glCreateProgram();


        if (glProgram == 0) {
            return;
        }

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
        int linkStatus;
        glGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == 0) {
            glDeleteProgram(glProgram);
        }
        d_glprogram = glProgram;
    }


    void surfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
        glViewport(0, 0, width, height);
        screenSize = glm::vec2(width, height);
        const GLfloat ratio = (GLfloat) width / height;
        const GLfloat left = width < height ? -1.0f : -1.0f * ratio;
        const GLfloat right = width < height ? 1.0f : 1.0f * ratio;
        const GLfloat bottom = width < height ? -1.0f / ratio : -1.0f;
        const GLfloat top = width < height ? 1.0f / ratio : 1.0f;
        const GLfloat near = 1.0f;
        const GLfloat far = 3.0f;
//      mProjectionMatrix = glm::ortho(left, right, bottom, top, near, far); //正交投影
        gProjectionMatrix = glm::perspective(90.0f, ratio, near, far); //透视投影

        gProject->setProjectMatrix(gProjectionMatrix);
        gProject->setScreenSize(screenSize);
    }


    void render(JNIEnv *env, jobject instance){
        drawEarth();
    }

    void drawEarth() {
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

        tile1->draw(aPositionLocaiton, aTextureLocation, aAssetManager, "west.jpeg");
        tile2->draw(aPositionLocaiton, aTextureLocation, aAssetManager, "east.jpeg");
    }




    static JNINativeMethod gMethods[] = {
            {"nativeSurfaceCreated", "()V",   (void *) surfaceCreated},
            {"nativeSurfaceChanged", "(II)V", (void *) surfaceChanged},
            {"nativeRender",         "()V",   (void *) render},
            {"nativeRotateEarth",    "([F[F)V", (void *) rotateEarth},
            {"nativeInitialize",     "()V",   (void *) initialize},
            {"nativeSetScale",       "(F)V",  (void *) setScale},
            {"nativeGetScale",       "()F",   (jfloat*) getScale},
            {"nativeSetTilt",        "(F)V",  (void *) setTilt},
            {"nativeSetZoom",        "(F)V",  (void *) setZoom},
            {"nativeGetZoom",        "()I",   (jint *) getZoom},
            {"nativeSetCenter",     "([F)V",  (void *) setCenter},
            {"nativeScreen2World",  "([F)[F",  (jfloatArray *) screen2World},
            {"nativeWorld2Screen",  "([F)[F",  (jfloatArray *) world2Screen},
            {"nativeLatLng2Screen", "([F)[F",  (jfloatArray *) latLng2Screen},
            {"nativeScreen2LatLng", "([F)[F",  (jfloatArray *) screen2LatLng}

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
