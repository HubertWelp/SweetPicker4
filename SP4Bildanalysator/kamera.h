#ifndef KAMERA_H
#define KAMERA_H

#include "ressourcen.h"

class Kamera
{
public:
    explicit Kamera(const char* pfad);
    int nehmeAuf(const char* pfad);
    void nehmeAufTest(const char* pfad);
    int setzeKameraID(int idnew = 0);

private:
    cv::VideoCapture videostream;
    cv::Mat bild;
    const char* pwd;
    int id;

    // Tunables
    int warmup_frames_ = 10;
    int frame_width_ = 640;
    int frame_height_ = 480;
    int backend_ = cv::CAP_ANY;
};

#endif // KAMERA_H
