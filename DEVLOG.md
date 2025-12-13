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
- Zeitpunkt für die Einführung von Threads (vor oder nach vollständigem MVP?)

---

## 📅 13.12.2025 (später am Tag)

### ✅ Erledigt
- Konfigurationsstruktur `config_t` entworfen
- Felder für Startverzeichnisse, Filter, Tiefenbegrenzung und Parallelisierung definiert
- Design an Kommandozeilenoptionen ausgerichtet (`-name`, `-type`, `-mindepth`, `-maxdepth`)
- Speicher- und Pointer-Modell (`char*`, `char**`) geklärt

### 🛠 In Arbeit
- Planung der Initialisierung der `config_t` (Default-Werte)
- Vorbereitung der Argument-Parsing-Logik in `args.c`

### ⏭ Nächste Schritte
- Implementierung von `config_init()`
- Parsen und Validieren der Kommandozeilenargumente
- Erste Debug-Ausgabe der Konfiguration

### 🧠 Notizen
- `config_t` enthält ausschließlich User-Konfiguration, keinen Traversal-Zustand
- Tiefe (`depth`) wird bewusst nicht in der Config gespeichert

 >In `config_t` werden unterschiedliche Pointer-Typen verwendet, um verschiedene Arten von Benutzereingaben abzubilden:
>
> - `int max_depth`  
> Enthält den **Wert selbst** (z. B. `2`).  
> Kein Pointer, da direkt mit der Zahl gearbeitet wird.
>
> - `char *name_pattern`  
> Zeigt auf den **Anfang eines Strings** im Speicher (z. B. `"*.c"`).  
> Der String selbst liegt an einer anderen Speicheradresse.
>
> Beispiel:
> name_pattern → "*.c"
>- `char **start_dirs`
>  Zeigt auf eine **Liste von String-Adressen** (mehrere Startverzeichnisse).
>  Jedes Element der Liste zeigt wiederum auf einen String.
>
> Beispiel:
> start_dirs
>
> +-----------+-----------+
>
> | "." | "/home" |
>
> +-----------+-----------+
>
> Faustregel:
> - kein `*`  → direkter Wert
> - ein `*`   → Adresse eines Werts
> - zwei `*`  → Adresse einer Liste von Adressen
