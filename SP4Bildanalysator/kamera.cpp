#include "kamera.h"

Kamera::Kamera(const char* pfad)
{
    pwd = pfad;
    id = 0;
    gamma = 1.0;
///    videostream = cv::VideoCapture (id); // 0 = standardkamera
}
/**
 * @brief Kamera::nehmeAuf
 * @param pfad
 * @return < 0 wenn nicht erfolgreich. = 0 wenn erfolgreich
 */
int Kamera::nehmeAuf(const char* pfad)
{
    // Das Zielpfad konstruieren
    //cv::String ziel = cv::String (pwd) + cv::String(pfad);
    cv::String ziel = cv::String(pfad);

    // Open camera for this shot
    if(!videostream.open(id))
    {
        std::cerr << "Kamera konnte nicht geoeffnet werden (ID " << id << ")\n";
        return -1;
    }

    // Request resolution (camera / backend may ignore!)
    videostream.set(cv::CAP_PROP_FRAME_WIDTH,frame_width_);
    videostream.set(cv::CAP_PROP_FRAME_HEIGHT,frame_height_);

    // small warmup delay and discard some frames
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    bool grapped_any=false;
    for (int i = 0; i < warmup_frames_; ++i)
    {
        if(videostream.grab())
          grapped_any = true;
  }

    /** grabs, decodes and returns the next video frame **/
    if(!videostream.read(bild) || bild.empty())
    {
        std::cerr << "Bild konnte nicht erfasst werden\n";
        videostream.release();
        return -2;
    }
    cv::Mat out;
    //bild.convertTo(out,-1,1.1,15);
    out = gammaCorrect(bild,gamma);
    // Das Bild im angegebenen Pfad Speichern
    if( !cv::imwrite(ziel,out) )
    {
        std::cerr << "Bild konnte nicht am Ziel gespeichert werden\n";
        videostream.release();
        return -3;
    }

    videostream.release();
    return 0;
}
/**
 * @brief Kamera::nehmeAufTest überspringt die Kamera und übergibt direkt ein Bild aus den Trainingsdaten. nur für Testzwecke.
 * @param pfad
 */
void Kamera::nehmeAufTest(const char *pfad)
{
    cv::String ziel = cv::String (pwd) + cv::String(pfad);
    ziel = cv::String(pfad);
    bild = cv::imread("/home/student/git/SP4/SweetPicker4/Trainingsdaten-bunt/2.jpg");
    if( !cv::imwrite(ziel,bild) )
    {
        printf("Bild konnte nicht am Ziel gespeichert werden\n");
    }
}
/**
 * @brief Kamera::setzeKameraID Methode zum Kamera ändern. Wenn die angegebene ID zu keiner Kamera führt wird die ID verworfen und die vorherige ID weiter verwendet.
 * @param idnew - neue Kamera ID
 * @return < 0 wenn nicht erfolgreich. = 0 wenn erfolgreich.
 */
int Kamera::setzeKameraID(int idnew)
{

  std::cout << "setzeKameraID " << idnew << " (alte ID " << id << ")\n";
  if(idnew == id)
    return 0;

  // Probe first to avoid breaking the existing camera if new ID is invalid.
  cv::VideoCapture probe;
  if (!probe.open(idnew, backend_)) {
    std::cout << idnew << " ist keine gültige Kamera" << std::endl;
    return -1;
  }

  // Apply same requested settings to the probe
  probe.set(cv::CAP_PROP_FRAME_WIDTH, frame_width_);
  probe.set(cv::CAP_PROP_FRAME_HEIGHT, frame_height_);

  // Warmup the probe a little
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  for (int i = 0; i < warmup_frames_; ++i) {
    probe.grab();
  }

  // Switch safely (no manual destructor!)
  if (videostream.isOpened()) {
    videostream.release();
  }
  videostream = std::move(probe);
  id = idnew;

  return 0;
}

int Kamera::setzeGamma(double g)
{
  if(g>0.5 && g<1.5)
  {
    std::cout << "setzeGamma: " << g << std::endl;
    gamma = g;
    return 0;
  }else
  {
    std::cout << "setzeGamma: out of range" << std::endl;
    return -1;

  }
}

cv::Mat Kamera::gammaCorrect(const cv::Mat &image, double gamma)
{
  CV_Assert(gamma>0.0);
  cv::Mat out;
  cv::Mat lut(1,256,CV_8U);
  for (int i=0;i<256;++i)
  {
    lut.at<uchar>(i) = cv::saturate_cast<uchar>(std::pow(i/255.0,gamma)*255.0);
  }
  cv::LUT(image,lut,out);
  return out;
}
