# Entwicklungsprotokoll – mfind

Dieses Dokument dient als internes Entwicklungsprotokoll für das Projekt `mfind`.
Es hält fest, was bereits umgesetzt wurde, woran aktuell gearbeitet wird,
welche Aufgaben als Nächstes anstehen und wo es noch offene Fragen oder Probleme gibt.

---

## 📅 13.12.2025

### ✅ Erledigt
- Git-Repository initialisiert und Projektstruktur angelegt (`src/`, `include/`, `tests/`)
- Makefile erstellt und erfolgreich getestet (`make`, `make clean`)
- Entwicklungsumgebung mit WSL (Ubuntu) und GCC eingerichtet
- GitHub-Zugriff über SSH konfiguriert
- Build-Artefakte (`mfind`, `.o`-Dateien) per `.gitignore` ausgeschlossen
- README.md mit Projektbeschreibung, Build- und Usage-Anleitung erstellt

### 🛠 In Arbeit
- Planung der internen Konfigurationsstruktur (`config_t`)
- Konzeption des Argument-Parsings für Kommandozeilenoptionen
  (`-name`, `-type`, `-maxdepth`, `-mindepth`)

### ⏭ Nächste Schritte
- Implementierung von `args.c` / `args.h`
- Parsen und Validieren der Kommandozeilenargumente
- Testweise Ausgabe der geparsten Konfiguration (Debug-Ausgabe)
- Vorbereitung für die rekursive Verzeichnis-Traversierung

### ⚠ Bekannte Einschränkungen / Hinweise
- Es findet noch keine echte Verzeichnisdurchsuchung statt
- Filter-Logik und Traversal sind noch nicht implementiert
- Tests sind aktuell noch nicht vorhanden

---

## 🧠 Design-Entscheidungen
- Zunächst Single-Thread-Implementierung, Parallelisierung erst später
- Trennung von Traversierung und Filterlogik
- Best-Effort-Fehlerbehandlung (Fehler nach `stderr`, Programm läuft weiter)

---

## ❓ Offene Fragen
- Umgang mit symbolischen Links (ignorieren oder folgen?)
- Unterstützung mehrerer Startverzeichnisse im MVP?
- Zeitpunkt für die
