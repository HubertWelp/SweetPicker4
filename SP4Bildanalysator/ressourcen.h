#ifndef RESSOURCEN_H
#define RESSOURCEN_H

/* Definition von Konstanten */
// #define PWD "/home/Student/git/SP4/SweetPicker3/SP4Bildanalysator"
#define BILD "aktuelleSzene.jpg"
#define ERKNT "gefundeneObjekte.jpg"
#define BILDABLAGE "/SP4Bilderkennung/"
#define AUSSCHNITT "Ausschnitt.jpg"
#define AUSSCHNITTSW "AusschnittSW.jpg"
#define TEXTABLAGE "/gefundeneObjekte.txt"
#define PROGNAME "/home/student/git/SP4/SweetPicker3/SP4Objekterkenner/SP4Objekterkenner.py"
#define PFAD "/home/student/Downloads/gefundeneObjekte.txt"
#define DCLASS "detection_classes"
#define DSCORE "detection_scores"
#define DBOX "detection_boxes"
#define MAXLESEN 100    // Anzahl der aus der Textdatei ins Array einzulesenden Elemente
#define ANZELEMENT 105  // Größe des Arrays (5 Elemente Puffer)
#define ANZSUCHE 10     // Maximale Anzahl der auszusuchenden Elemente (die Indexe der 10 zuerst auftauchenden Klassen werden ermittelt)
//#define BILDBRT 354     // Echte Bildbreite in mm
//#define BILDHHE 274     // Echte Bildhöhe in mm

/* Einbindung von Qt Bibliotheken */
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileInfo>
#include <QColor>

/* Einbindung von c Bibliotheken */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Directory */
#include <dirent.h>

/*SP4Admin*/
#include "../SP4Admin/konfig.h"

/* Einbindung von c++ Bibliotheken */
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <fstream>

/* Einbindung der opencv Bibliotheken */
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio.hpp>

/* Definition von Variablen */
namespace ressourcen
{
extern unsigned int BILDBRT;
extern unsigned int BILDHHE;
extern std::string PWD;
}
/* Selbstdefinierte Strukturen und Enums */
struct RBox {double a; double b; double c; double d;};
struct Punkt {int x;int y;int z;};
struct Punkte {double ymin; double xmin; double ymax; double xmax;};

#endif // RESSOURCEN_H
