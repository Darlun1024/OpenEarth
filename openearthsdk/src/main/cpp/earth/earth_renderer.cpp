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
#include "../util/assets_file_reader.hpp"
extern "C"{
    #include "../util/image.h"
}



namespace OpenEarth {
    static const char *const JavaClassName = "com/geocompass/openearth/sdk/earth/EarthRenderer";
    static const char *const TAG = "earth_renderer_cpp";
    std::unique_ptr<OpenEarth::Sphere> sphere;
    GLuint d_glprogram;
    glm::mat4x4 mModelMatrix;
    glm::mat4x4 mViewMatrix;
    glm::mat4x4 mProjectionMaxrix;
    glm::mat4x4 mMvpMatrix;

    JNIEnv* mEnv;
    jobject  mJavaObject; //java 的EarthRenderer类对象
    //构造和析构函数
    OpenEarth::EarthRenderer::EarthRenderer() {

    }

    OpenEarth::EarthRenderer::~EarthRenderer() {
        if (sphere)
            sphere.reset();
    }




    /**
 * 加载图片
 */
    extern "C"
    void loadTexture(){
        //获取类
        jclass objectClass  =  mEnv->GetObjectClass(mJavaObject);
        //获取函数句柄
        jmethodID methodID  =  mEnv->GetMethodID(objectClass , "getAssetManager","()Landroid/content/res/AssetManager;");
        //从Java获取AssetManager
        jobject  javaAssetManager = mEnv->CallObjectMethod(mJavaObject,methodID);
        AAssetManager* mgr  = AAssetManager_fromJava(mEnv,javaAssetManager);
        const char *eastImg = "east.jpeg";
        const char *westImg = "west.jpeg";

        AAsset* asset = AAssetManager_open(mgr, eastImg,AASSET_MODE_BUFFER);
        AAsset* asset1 = AAssetManager_open(mgr, westImg,AASSET_MODE_BUFFER);
        off_t length  = AAsset_getLength(asset);
        GLuint textureId[] = {0};
        GLuint textureId1[] = {0};
        glGenTextures(1,textureId);
        glGenTextures(1,textureId1);
        glBindTexture(GL_TEXTURE0,textureId[0]);
        glBindTexture(GL_TEXTURE1,textureId1[0]);

        FileData fileData = OpenEarth::util::AssetsFileReader::get_asset_data(eastImg,mgr);

        RawImageData data = get_raw_image_data_from_png(fileData.data,(int)fileData.data_length);


//        glTexImage2D()
//        gltex
         LOGE(TAG,"east size %ld",length);
    }



    void surfaceCreated(JNIEnv *env, jobject instance) {
        mEnv  = env;
        mJavaObject = instance;
        sphere = std::make_unique<OpenEarth::Sphere>(1.0f);
        GLuint glProgram;
        GLuint vertexShader;
        GLuint fragmentShader;
        //shader code
        const char *shader_vertex = "uniform mediump mat4 MODELVIEWPROJECTIONMATRIX;\n"
                "uniform mat4 u_MVPMatrix; \n"
                "attribute vec4 POSITION;\n"
                "void main(){\n"
                "  gl_Position = u_MVPMatrix*POSITION;\n"
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

        glEnable(GL_TEXTURE0);
        glEnable(GL_TEXTURE1);
        loadTexture();
    }

    void surfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
        mModelMatrix = glm::mat4(1.0f);
        mViewMatrix  = glm::lookAt(
                glm::vec3(1.0f,1.0f,1.0f), // Camera is at (4,3,3), in World Space
                glm::vec3(0.0f,0.0f,0.0f), // and looks at the origin
                glm::vec3(0.0f,1.0f,0.0f) // Head is up (set to 0,-1,0 to look upside-down)
        );


        glViewport(0, 0, width, height);
        const GLfloat ratio = (GLfloat) width / height;
        const GLfloat left   = width < height ? -1.0f:-1.0f*ratio;
        const GLfloat right  = width < height ? 1.0f:1.0f*ratio;
        const GLfloat bottom = width < height? -1.0f/ratio : -1.0f;
        const GLfloat top    = width < height? 1.0f/ratio : 1.0f;
        const GLfloat near = 1.0f;
        const GLfloat far = 10.0f;

        mProjectionMaxrix  =  glm::ortho (left, right, bottom, top, near, far);
        mMvpMatrix = mProjectionMaxrix * mViewMatrix * mModelMatrix;



    }

    void render(JNIEnv *env, jobject instance) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(d_glprogram);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, sphere->getVertexArray());
        glEnableVertexAttribArray(0);
        int projectLocation = glGetUniformLocation(d_glprogram, "u_MVPMatrix");
        glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(mMvpMatrix));
        int size = sphere->getVertextSize();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
    }

    void setLight(){
        GLfloat ambientLight[]={0.2f,0.2f,0.2f,1.0f};//白色主光源环境光
        GLfloat diffuseLight[]={0.8f,0.8f,0.8f,1.0f};//白色主光源漫反射
        GLfloat specularLight[]={1.0f,1.0f,1.0f,1.0f};//白色主光源镜面光
        GLfloat lightPos[]={50.0f,120.0f,120.0f,1.0f};  //光源位置
        GLfloat spotLightPos[]={0.0f,0.0f,200.0f,1.0f}; //射灯位置
        GLfloat spotDir[]={0.0f,0.0f,-1.0f};            //射灯方向

        GLfloat redLight[]={1.0f,0.0f,0.0f,1.0f};       //红色光源
        GLfloat greenLight[]={0.0f,1.0f,0.0f,1.0f};     //绿色光源
        GLfloat blueLight[]={0.0f,0.0f,1.0f,1.0f};      //蓝色光源


//        glEnable(GL_LIGHTING);                          //启用光照
//        glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);   //设置环境光源
//        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);   //设置漫反射光源
//        glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight); //设置镜面光源
//        glLightfv(GL_LIGHT0,GL_POSITION,lightPos);      //设置灯光位置
//        glEnable(GL_LIGHT0);                            //打开白色主光源

    }




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
