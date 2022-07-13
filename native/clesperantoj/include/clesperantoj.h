#include "clesperanto.hpp"

/**
 * @brief 
 * ObjectJ just wraps an CLIc Object
 * 
 * This is done so we can store Clic Objects on the java side
 * 
 * However the Object itself is private so the java side doesn't need to wrap all of Object (and thus much of cl.cpp)
 * 
 * Note we declare ClesperantoJ as a friend class so that ClesperantoJ can access the Object
 */
class ObjectJ
{
    friend class ClesperantoJ;    
    private:
        cle::Object obj;
};

class ClesperantoJ
{


private:
    cle::Clesperanto cle;

public:
    ClesperantoJ();
    ~ClesperantoJ() = default;

    void sayHello();

    template<typename T>
    ObjectJ create(int nx, int ny);

    template<typename T>
    ObjectJ push(T* in, int nx, int ny);

    template<typename T>
    void pull(T * out, ObjectJ obj);

    void gaussianBlur2d(ObjectJ in, ObjectJ out, float sx, float sy);

    // templated version
    template<typename T>
    void guassianBlur2dT(T*in, T*out, int nr, int nc, float sx, float sy);

};


template<typename T>
void ClesperantoJ::guassianBlur2dT(T*in, T*out, int nx, int ny, float sx, float sy)   {
    
    std::cout<<"test templated guassian blur\n"<<std::flush;

    // need to convert the input to a vector in order to push...
    // this step is wasteful, TODO:  
    // create version of Push that takes in pointer ??
    // or pass in cl_mem created in java and create object from that?
    std::vector<T> vecData;
    vecData.resize(nx*ny);

    T * vecPointer = vecData.data();

    for (int i=0;i<nx*ny;i++) {
        vecPointer[i]=in[i];
    }

    std::array<size_t,3> dimensions = {nx, ny, 1};
    auto inGPU = cle.Push<T>(vecData, dimensions, "image");
    auto outGPU = cle.Create<T>(dimensions,"image");
    
    cle.GaussianBlur(inGPU, outGPU, 3,3,1);
    std::vector<T> outVec=cle.Pull<T>(outGPU); 
    
    T * temp = outVec.data();

    for (int i=0;i<nx*ny;i++) {
        out[i]=temp[i];
    }
}

template<typename T>
ObjectJ ClesperantoJ::create(int nx, int ny) {

    std::array<size_t,3> dimensions = {nx, ny, 1};
    auto obj = cle.Create<T>(dimensions,"image");

    ObjectJ objJ = ObjectJ();
    objJ.obj = obj;

    return objJ;
}

template<typename T>
ObjectJ ClesperantoJ::push(T *in, int nx, int ny) {
    
    std::array<size_t,3> dimensions = {nx, ny, 1};
    std::vector<T> vecData;
    vecData.resize(nx*ny);

    T * vecPointer = vecData.data();

    for (int i=0;i<nx*ny;i++) {
        vecPointer[i]=in[i];
    }

    auto obj = cle.Push<T>(vecData, dimensions, "image");
 
    ObjectJ objJ = ObjectJ();
    objJ.obj = obj;
    
    return objJ;
}

template<typename T>
void ClesperantoJ::pull(T *out, ObjectJ obj) {
    std::vector<T> outVec=cle.Pull<T>(obj.obj); 
    
    T * temp = outVec.data();

    for (int i=0;i<outVec.size();i++) {
        out[i]=temp[i];
    }
}