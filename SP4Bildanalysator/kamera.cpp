#include "kamera.h"

Kamera::Kamera(const char* pfad)
{
    pwd = pfad;
    id = 0;
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
        std::cerr << "Kamera konnte nicht geoeffnet werden (ID )" << id << ")\n";
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
    // Das Bild im angegebenen Pfad Speichern
    if( !cv::imwrite(ziel,bild) )
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
