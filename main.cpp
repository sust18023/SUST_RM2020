#include "Armor/Armor.h"
#include "Buff/Buff.h"
#include "Base.h"

Mat image;
static volatile unsigned int ProduceIndex=0;     // 图像生成序号，用于线程之间逻辑
static volatile unsigned int ConsumptionIndex=0; // 图像消耗序号
void CameraSolve(){
    VideoCapture capture;
    #if VIDEO_READ_IO
        #if ARMOR_OR_BUFF
            capture.open(BUFF_RATH);
        #else
            capture.open(ARMOR_PATH);
        #endif
    #else
        capture.open(CAMERA_PATH);
    #endif

    if(!capture.isOpened()){
        printf("can not open ...\n");
        return ;
    }
    while(1){
        capture>>image;
        ProduceIndex++;
        waitKey(1);
        imshow("image", image);
    }
}
void AimSolve(){
    ArmorDetector A;
    BuffDetector B;
    float AngleX, AngleY,Distance;
    while(1){
        while(ProduceIndex-ConsumptionIndex<IMAGE_CACHE);
        ConsumptionIndex=ProduceIndex;
        if(ARMOR_OR_BUFF){
            B.BuffDetectTask(image);
            B.GetAngle(AngleX, AngleY,Distance);
        }else{
            A.ArmorDetectTask(image);
            A.GetAngle(AngleX, AngleY,Distance);
        }
        cout<<AngleX<<"  "<<AngleY<<"  "<<Distance<<endl;
    }
    return ;
}


int main(){
    thread Camera(CameraSolve);
    thread Aim(AimSolve);
    Camera.detach();
    Aim.join();
    return 0;
}
