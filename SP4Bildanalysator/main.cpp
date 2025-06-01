#include <QCoreApplication>
#include "ressourcen.h"
#include "verwalter.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
  string ipAdressSPKoordinator="127.0.0.1";
  if(argc!=1 && argc!=3)
  {
    std::cout << "usage: SP4Bildanalysator [--IP <IP-Adress of SP4Koordinator (default: 127.0.0.1)>]" << std::endl;
  }else if(argc==3 && string(argv[1])=="--IP"){
      ipAdressSPKoordinator = string(argv[2]);
    }else {
      ipAdressSPKoordinator = string("127.0.0.1");
    }

  QCoreApplication a(argc, argv);
  Verwalter v(ipAdressSPKoordinator);
  return a.exec();
}
