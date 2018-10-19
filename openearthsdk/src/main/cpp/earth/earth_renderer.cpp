#define GLM_ENABLE_EXPERIMENTAL

#include "earth_renderer.hpp"
#include "Sphere.hpp"
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
#include "../programs/program.hpp"
#include "../texture/texture.hpp"
#include "TileManager.hpp"
#include "tile_management.hpp"
#include "source/source.hpp"
#include "../shaders/line_shader.hpp"
#include "../shaders/light_raster_shader.hpp"

#define  DEFAULT_EYE_HEIGHT 1.0f

namespace OpenEarth {
    static const char *const JavaClassName = "com/geocompass/openearth/sdk/earth/EarthRenderer";
    static const char *const TAG = "earth_renderer_cpp";
    std::unique_ptr<OpenEarth::Sphere> sphere;
    std::shared_ptr<OpenEarth::OpenGLProject> gProject;
    std::unique_ptr<OpenEarth::Transform> gTransform;
    std::unique_ptr<OpenEarth::TileManagement> tileManager;
    Tile *tile1;
    Tile *tile2;
    GLuint d_glprogram;
    GLuint line_program;

    int aPositionLocation;
    int aTextureLocation;
    int uTextureUnitLocation;
    int uProjectionLocation;
    int uMvMatrixLocation;
    int uLightPosition;
    int aNormalPosition;



    AAssetManager *aAssetManager;

    glm::vec2 screenSize;
    glm::mat4x4 gModelMatrix;
    glm::mat4x4 gViewMatrix;
    glm::mat4x4 gProjectionMatrix;
    glm::mat4x4 gMvpMatrix;
    glm::mat4x4 gMvMatrix;
    glm::vec3 gLightPosition; //光照位置
    glm::vec3 gNormal; //

    OpenEarth::Texture *textureManager;

    //函数声明
    void drawEarth(JNIEnv *env);

    float cameraTargetCenterY = 0.0f;
    static const float MAX_TARGET_CENTER_Y = 1.0f;
    static const float MIN_TARGET_CENTER_Y = 0.0f;

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
    }

    void initialize() {
        textureManager = new Texture();
        OpenEarth::Earth::initialize();
        gModelMatrix = OpenEarth::Earth::getModelMatrix();

        //设置镜头位置
        gViewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 1.0f), //眼睛位置
                glm::vec3(0.0f, cameraTargetCenterY, 0.0f), //瞄准的点
                glm::vec3(0.0f, 1.0f, 0.0f)  //头顶方向的法向量
        );

        //投影
        gProjectionMatrix = glm::mat4(1.0f);


        screenSize = glm::vec2(0, 0);

        //投影工具
        gProject = std::make_shared<OpenEarth::OpenGLProject>(gViewMatrix, gProjectionMatrix,
                                                              screenSize);
        //坐标转换工具
        gTransform = std::make_unique<OpenEarth::Transform>(gModelMatrix, gProject);

        //纹理管理工具
        tileManager = std::make_unique<OpenEarth::TileManagement>();

        //光源位置
        gLightPosition = glm::vec3(0.0f,0.0f,2.0f); //镜头后方

        //法向量
        gNormal = glm::vec3(0.0f,0.0f,1.0f);
    }

    //可见的地球的坐标范围
    OpenEarth::Geometry::Bounds getViewMapBounds() {
        //考虑到旋转的情况，四个角的坐标都要求
        glm::vec2 nw = gTransform->screenPointToLatlng(glm::vec2(0, 0));
        glm::vec2 se = gTransform->screenPointToLatlng(screenSize);
        glm::vec2 ne = gTransform->screenPointToLatlng(glm::vec2(screenSize[0], 0));
        glm::vec2 sw = gTransform->screenPointToLatlng(glm::vec2(0, screenSize[1]));
        using namespace std;
        float left = fmax(fmin(fmin(fmin(nw[1], se[1]), ne[1]), sw[1]), -180);
        float right = fmin(fmax(fmax(fmax(nw[1], se[1]), ne[1]), sw[1]), 180);
        float bottom = fmax(fmin(fmin(fmin(nw[0], se[0]), ne[0]), sw[0]), -90);
        float top = fmin(fmax(fmax(fmax(nw[0], se[0]), ne[0]), sw[0]), 90);
        if(left > 180 ) left = -180;
        if(bottom > 90) bottom = -90;
        return OpenEarth::Geometry::Bounds(left,bottom,right,top);

    }

    /**
     *  更新模型矩阵
     *  模型矩阵更新后，坐标转换工具和纹理管理都有对应的更新
     * @return
     */
    void updateModelMatrix() {
        OpenEarth::Earth::updateModelMatrix();
        gModelMatrix = OpenEarth::Earth::getModelMatrix();
        gTransform->setModelMatrix(gModelMatrix);
        OpenEarth::Geometry::Bounds bounds = getViewMapBounds();
        tileManager->update(OpenEarth::Earth::getZoom(),bounds);
    }

    /**
     * 自由旋转球体
     * @param env
     * @param instance
     * @param screenPoint1
     * @param screenPoint2
     */
    void rotateWithScreenPoint(JNIEnv *env, jobject instance, jfloatArray screenPoint1,
                               jfloatArray screenPoint2) {
        jboolean isCopy = true;
        jfloat *array1 = env->GetFloatArrayElements(screenPoint1, &isCopy);
        jfloat *array2 = env->GetFloatArrayElements(screenPoint2, &isCopy);
        glm::vec2 latlng1 = gTransform->screenPointToLatlng(glm::vec2(array1[0], array1[1]));
        glm::vec2 latlng2 = gTransform->screenPointToLatlng(glm::vec2(array2[0], array2[1]));
        if (!gTransform->isValidLatlng(latlng1) || !gTransform->isValidLatlng(latlng2))
            return;
        float deltaLat = latlng2[0] - latlng1[0];
        float deltaLon = latlng2[1] - latlng1[1];
        if (deltaLat * (array2[1] - array1[1]) > 0) deltaLat = -deltaLat;  //在两级附近
        //TODO 处理子午线上的情况
        OpenEarth::Earth::rotate(deltaLat, deltaLon);
        updateModelMatrix();
    }


    void rotateWithLatLng(JNIEnv *env, jobject instance, jfloat deltaLat, jfloat deltaLon) {
        OpenEarth::Earth::rotate(deltaLat, deltaLon);
        updateModelMatrix();
    }


    /**
     * 设置中心点坐标
     * @param env
     * @param instance
     * @param latlng
     */
    void setCenter(JNIEnv *env, jobject instance, jfloatArray latlng) {
        //默认地图中心是 (0,0)
        jboolean isCopy = true;
        jfloat *array = env->GetFloatArrayElements(latlng, &isCopy);
        OpenEarth::Earth::setCenterLatLng(new LatLng(array[0], array[1]));
        updateModelMatrix();
    }

    /**
     * 获取中心坐标
     * @param env
     * @param instance
     * @return
     */
    jfloatArray getCenter(JNIEnv *env, jobject instance) {
        LatLng *latLng = OpenEarth::Earth::getCenterLatLng();
        float array1[] = {latLng->lat, latLng->lon};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray, 0, 2, array1);
        latLng = nullptr;
        return floatArray;
    }


    //重新绘制地球
    void updateEarth() {
        updateModelMatrix();
        tile1->reset();
        tile2->reset();
    }

    //设置球体的缩放比例
    void setScale(JNIEnv *env, jobject instance, jfloat scale) {
        //TODO 修改实现方法，这里的scale要限制在[1.0 2.0)之内
        if (OpenEarth::Earth::setScale(scale))
            updateEarth();
        else
            updateModelMatrix();
    }

    /**
     * 缩放地球
     * 如果缩放的比例累积大于2.0或者小于0.5，需要修改Zoom,更新模型矩阵
     * @param env
     * @param instance
     * @param scale 缩放比例
     */
    void scale(JNIEnv *env, jobject instance, jfloat scale) {
        if (OpenEarth::Earth::setScale(scale))
            updateEarth();
        else
            updateModelMatrix();
    }

    /**获取球体的缩放比例(1.0 2.0)*/
    jfloat getScale() {
        return OpenEarth::Earth::getScale();
    }


    //设置球体的显示级别,不同的级别会采用不用的纹理
    void setZoom(JNIEnv *env, jobject instance, jfloat zoom) {
        if (OpenEarth::Earth::setZoom(zoom))
            updateEarth();
    }

    void zoomIn(JNIEnv *env, jobject instance) {
        int zoom = OpenEarth::Earth::getZoom();
        if (OpenEarth::Earth::setZoom(zoom + 1))
            updateEarth();
    }

    void zoomOut(JNIEnv *env, jobject instance) {
        int zoom = OpenEarth::Earth::getZoom();
        if (OpenEarth::Earth::setZoom(zoom - 1))
            updateEarth();
    }

    //获取球体的显示级别
    jint getZoom() {
        return OpenEarth::Earth::getZoom();
    }

    void setCameraTargetCenterY(float tilt) {
        if (tilt > MAX_TARGET_CENTER_Y) tilt = MAX_TARGET_CENTER_Y;
        if (tilt < MIN_TARGET_CENTER_Y) tilt = MIN_TARGET_CENTER_Y;
        cameraTargetCenterY = tilt;
        gViewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, DEFAULT_EYE_HEIGHT), //眼睛位置
                glm::vec3(0.0f, cameraTargetCenterY, 0.0f), //瞄准的点
                glm::vec3(0.0f, 1.0f, 0.0f)  //头顶方向的法向量
        );
        gProject->setViewMatrix(gViewMatrix);
    }


    //修改摄像头瞄准的点，实现地图倾斜效果
    void setTilt(JNIEnv *env, jobject instance, jfloat tilt) {
        setCameraTargetCenterY(tilt);
    }

    void tilt(JNIEnv *env, jobject instance, jfloat tilt) {
        setCameraTargetCenterY(cameraTargetCenterY + tilt);
    }


    /**
     * 屏幕坐标转世界坐标
     * 这里转换的世界坐标是在 z=0 时的坐标
     * @param env
     * @param instance
     * @param point
     * @return
     */
    jfloatArray screen2World(JNIEnv *env, jobject instance, jfloatArray point) {
        jboolean isCopy = true;
        jfloat *array = env->GetFloatArrayElements(point, &isCopy);
        glm::vec3 world = gProject->unProject(glm::vec2(array[0], array[1]), 0);
        jfloat array1[3] = {world[0], world[1], world[2]};
        jfloatArray floatArray = env->NewFloatArray(3);
        env->SetFloatArrayRegion(floatArray, 0, 3, array1);
        return floatArray;
    }

    /**
     * 世界坐标转屏幕坐标
     * @param env
     * @param instance
     * @param point
     * @return
     */
    jfloatArray world2Screen(JNIEnv *env, jobject instance, jfloatArray point) {
        jboolean isCopy = true;
        float *array = env->GetFloatArrayElements(point, &isCopy);
        glm::vec2 screen = gProject->project(glm::vec3(array[0], array[1], array[2]));
        jfloat array1[2] = {screen[0], screen[1]};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray, 0, 2, array1);
        return floatArray;
    }

    /**
     * 屏幕坐标转经纬度
     * 返回球面可见部门与屏幕交点的坐标
     */
    jfloatArray screen2LatLng(JNIEnv *env, jobject instance, jfloatArray point) {
        jboolean isCopy = true;
        jfloat *array = env->GetFloatArrayElements(point, &isCopy);
        glm::vec2 latlng = gTransform->screenPointToLatlng(glm::vec2(array[0], array[1]));
        jfloat array1[2] = {latlng[0], latlng[1]};
        jfloatArray floatArray = env->NewFloatArray(3);
        env->SetFloatArrayRegion(floatArray, 0, 2, array1);
        return floatArray;
    }

    /**
     * 经纬度转屏幕坐标
     * @param env
     * @param instance
     * @param latlng
     * @return
     */
    jfloatArray latLng2Screen(JNIEnv *env, jobject instance, jfloatArray latlng) {
        jboolean isCopy = true;
        float *array = env->GetFloatArrayElements(latlng, &isCopy);
        glm::vec2 screen = gTransform->latLngToScreenPoint(new LatLng(array[0], array[1]));
        jfloat array1[2] = {screen[0], screen[1]};
        jfloatArray floatArray = env->NewFloatArray(2);
        env->SetFloatArrayRegion(floatArray, 0, 2, array1);
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

        //shader code
        const char *shader_vertex = OpenEarth::Shaders::RasterShader::veterxShader;
        const char *shader_fragment = OpenEarth::Shaders::RasterShader::fragmentShader;
        d_glprogram = OpenEarth::Programs::Program::createProgram(shader_vertex, shader_fragment);

        //line shader
        const char *line_shader_vertex = OpenEarth::Shaders::LineShader::veterxShader;
        const char *line_shader_fragment = OpenEarth::Shaders::LineShader::fragmentShader;
        line_program = OpenEarth::Programs::Program::createProgram(line_shader_vertex, line_shader_fragment);


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
        gTransform->setProject(gProject);
    }

//TODO refactor
    void testDrawLine(){
        glUseProgram(line_program);
        aPositionLocation    = glGetAttribLocation(line_program, "POSITION");
        uProjectionLocation  = glGetUniformLocation(line_program,"u_MVPMatrix");
        int  colorPosition   = glGetUniformLocation(line_program,"u_LineColor");
        glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, glm::value_ptr(gMvpMatrix));
        glm::vec4 color = glm::vec4(0.0f,1.0f,1.0f,0.6f);

        glUniform4fv(colorPosition,1,glm::value_ptr(color));
        float points[] = {0.0f,0.0f,-2.0f,1.0f,1.0f,-2.0f};
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),
                              points);
        glEnableVertexAttribArray(aPositionLocation);
        glDrawArrays(GL_LINE_STRIP,0,2);
    }

    void render(JNIEnv *env, jobject instance) {
        drawEarth(env);
        testDrawLine();
    }

    void drawEarth(JNIEnv *env) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glUseProgram(d_glprogram);

        aPositionLocation    = glGetAttribLocation(d_glprogram, "POSITION");
        aTextureLocation     = glGetAttribLocation(d_glprogram, "a_TextureCoordinates");
        uTextureUnitLocation = glGetUniformLocation(d_glprogram, "u_TextureUnit");
        uProjectionLocation  = glGetUniformLocation(d_glprogram, "u_MVPMatrix");

        //试验漫反射光照
//        uLightPosition       = glGetUniformLocation(d_glprogram,"u_LightPos");
//        uMvMatrixLocation    = glGetUniformLocation(d_glprogram,"u_MVMatrix");
//        aNormalPosition      = glGetAttribLocation(d_glprogram,"a_Normal");
//
//        gMvMatrix = gViewMatrix * gModelMatrix;
//        glUniformMatrix4fv(uMvMatrixLocation, 1, GL_FALSE, glm::value_ptr(gMvMatrix));
//        glUniform3fv(uLightPosition,1,glm::value_ptr(gLightPosition));

//        glActiveTexture()

        glUniform1i(uTextureUnitLocation, 0);  //这里的数字是设备纹理单元的编号，0 对应的是GL_TEXTURE0

        gMvpMatrix = gProjectionMatrix * gViewMatrix * gModelMatrix; //投影x视图x模型矩阵
        glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, glm::value_ptr(gMvpMatrix));
//        file:///storage/emulated/0/
//        Source::Source* source = new Source::Source("http://web2.geo-compass.com/wmts/wmts?layer=dzdt_mkt&style=default&tilematrixset=c&Service=WMTS&Request=GetTile&Version=1.0.0&Format=image/png&TileMatrix=17&TileCol=104023&TileRow=54198");
        Source::Source* source = new Source::Source("http://t3.tianditu.com/DataServer?T=img_c&x={x}&y={y}&l={z}");
//        Source::Source* source = new Source::Source("mbtile://path=/storage/emulated/0/img_beijing.db&x={x}&y={y}&l={z}");
        tileManager->draw(env,aPositionLocation,aTextureLocation,source,aAssetManager);
        testDrawLine();

    }



    static JNINativeMethod gMethods[] = {
            {"nativeSurfaceCreated",   "()V",     (void *) surfaceCreated},
            {"nativeSurfaceChanged",   "(II)V",   (void *) surfaceChanged},
            {"nativeRender",           "()V",     (void *) render},
            {"nativeRotateWithPoint",  "([F[F)V", (void *) rotateWithScreenPoint},
            {"nativeRotateWithLatLng", "(FF)V",   (void *) rotateWithLatLng},
            {"nativeInitialize",       "()V",     (void *) initialize},
            {"nativeSetScale",         "(F)V",    (void *) setScale},
            {"nativeScale",            "(F)V",    (void *) scale},
            {"nativeGetScale",         "()F",     (jfloat *) getScale},
            {"nativeSetTilt",          "(F)V",    (void *) setTilt},
            {"nativeTilt",             "(F)V",    (void *) tilt},
            {"nativeSetZoom",          "(F)V",    (void *) setZoom},
            {"nativeZoomIn",           "()V",     (void *) zoomIn},
            {"nativeZoomOut",          "()V",     (void *) zoomOut},
            {"nativeGetZoom",          "()I",     (jint *) getZoom},
            {"nativeSetCenter",        "([F)V",   (void *) setCenter},
            {"nativeScreen2World",     "([F)[F",  (jfloatArray *) screen2World},
            {"nativeWorld2Screen",     "([F)[F",  (jfloatArray *) world2Screen},
            {"nativeLatLng2Screen",    "([F)[F",  (jfloatArray *) latLng2Screen},
            {"nativeScreen2LatLng",    "([F)[F",  (jfloatArray *) screen2LatLng}

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
