**# SP4Objekterkenner YOLO Modell POC**

Der SP4Objekterkenner bietet ein YOLO-basiertes Objekterkennungssystem als Ersatz für den ursprünglichen TensorFlow-basierten SP3Objekterkenner, während die vollständige Schnittstellenkompatibilität mit bestehenden Komponenten erhalten bleibt.

## Installation

### Voraussetzungen

- Python 3.10 ist gewährleistet, andere Versionen möglich
- Cuda Schnickschnack
- pip3
- virtualenv
- ~1 GB VRAM

### Einrichtung

1. Skript ausführbar machen:
   ```bash
   chmod +x setup_venv_SP4Objekterkenner.sh
   ```

2. YOLO-Umgebung einrichten:
   ```bash
   ./setup_venv_SP4Objekterkenner.sh
   ```


## Verwendung

### Ausführung mit dem Toggle-Skript

Alle launch Skripte des Sweetpickers welche
```bash
USE_YOLO=${1:-0}
```
im Header aufweisen Unterstützen das Schalten zwischen dem Orginalen Objekterkenner (mit VRAM Problemen) und dem Ersatzobjekterkenner.
Beispiel anhand catkin_ws\src\chatbot_lm\launch\launch_complete.sh:

Um das SP4-System mit dem ursprünglichen Objekterkenner zu starten:
```bash
./launch_complete.sh 0
```

Um das SP4-System mit dem YOLO-basierten Objekterkenner zu starten:
```bash
./launch_complete.sh 1
```

## Klassen-Mapping

Das YOLO-Modell verwendet leicht unterschiedliche Klassennamen im Vergleich zum ursprünglichen Modell. Die folgende Zuordnung wird für die Kompatibilität verwendet:

| YOLO Modell         | Ursprüngliches Modell | ID |
|---------------------|-----------------------|----|
| Maoam               | Maoam                 | 1  |
| Snickers            | Snickers              | 2  |
| MilkyWay            | Milkyway              | 3  |
| SchokoMilchRiegel   | Riegel                | 4  |

## Systemarchitektur

Die Implementierung gewährleistet vollständige Schnittstellenkompatibilität:

1. **Netzwerkschnittstelle**: 
   - UDP-Socket-Kommunikation auf localhost (127.0.0.1) Port 5850
   - Gleiches Nachrichtenprotokoll wie das ursprüngliche System

2. **Dateisystem-Schnittstelle**:
   - Liest Bilder von: `/home/student/git/SP4/SweetPicker4/SP4Bildanalysator/SP3Bilderkennung/aktuelleSzene.jpg`
   - Ausgabe an: 
     - `/home/student/git/SP4/SweetPicker4/SP4Bildanalysator/SP3Bilderkennung/gefundeneObjekte.jpg`
     - `/home/student/git/SP4/SweetPicker4/SP4Bildanalysator/SP3Bilderkennung/gefundeneObjekte.txt`

Das Ausgabeformat von `gefundeneObjekte.jpg` wird von dem Ursprünglichen System abweichen, da ein anderes Tool verwendet wird.