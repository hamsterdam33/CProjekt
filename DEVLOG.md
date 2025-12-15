# Entwicklungsprotokoll – mfind

Dieses Dokument dient als internes Entwicklungsprotokoll für das Projekt `mfind`.
Es hält fest, was bereits umgesetzt wurde, woran aktuell gearbeitet wird,
welche Aufgaben als Nächstes anstehen und wo es noch offene Fragen oder Probleme gibt.

---

## 📅 13.12.2025 14:24

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
>
---

## 📅 15.12.2025

### ✅ Erledigt
- `config_init()` implementiert und mit sinnvollen Default-Werten versehen
- Initialisierung der Konfiguration erfolgreich in `main.c` getestet
- Debug-Ausgaben zur Überprüfung der Default-Werte hinzugefügt
- Include-/Build-Fehler in `main.c` behoben (`#include` wurde vom Compiler nicht erkannt)

### 🧠 Erkenntnisse
- `#include`-Direktiven werden vom Preprocessor verarbeitet und sind kein normaler C-Code
- Editor-Anzeige kann vom tatsächlichen Dateiinhalt abweichen
- Terminal-Ausgaben sind maßgeblich für Debugging

### ⏭ Nächste Schritte
- Debug-Ausgaben wieder entfernen oder hinter ein Flag legen
- Entwurf und Implementierung von `parse_arguments(argc, argv, &cfg)`

## 📅 15.12.2025 19:42
### ✅ Erledigt
- `parse_arguments(argc, argv, &cfg)` schrittweise implementiert
- Unterstützung für folgende Optionen hinzugefügt:
> - `-name <pattern>`
> - `-type f|d`
> - `-mindepth <n>`
> - `-maxdepth <n>`
- Dynamisches Sammeln von Startverzeichnissen (start_dirs) mittels realloc
- Default-Startverzeichnis `"."` gesetzt, wenn keine Startdirs angegeben werden
- Konsistenzprüfung: mindepth darf nicht größer als maxdepth sein
- Robuste Integer-Validierung mit `strtol` umgesetzt
- Debug-Ausgaben genutzt, um Parsing-Logik und Werte korrekt zu verifizieren
- Mehrere typische C-Fehler identifiziert und behoben (Klammern, unreachable code, falsche Includes, Variablennamen)

### 🧠 Erkenntnisse
- Argument-Parsing in C erfordert explizite und defensive Programmierung
- argc/argv müssen systematisch und in klarer Reihenfolge verarbeitet werden
- continue ist essenziell, um Mehrfachverarbeitung von Argumenten zu vermeiden
- Dynamische Arrays (char **) benötigen sauberes Speicher-Management
- Fehlermeldungen früh auszugeben vereinfacht Debugging erheblich
- Kleine Tippfehler (z. B. falsche Variablennamen oder Klammern) können große Compilerfehler verursachen
- ASCII-Werte erklären, warum char-Variablen bei %d als Zahlen erscheinen

###⚠️ Offene Punkte / TODO
- `config_free()` implementieren, um allokierten Speicher `(strdup)` sauber freizugeben
- Debug-Ausgaben konsolidieren oder über ein zentrales Debug-Flag steuern
- Fehlerbehandlung ggf. vereinheitlichen (einheitliche Fehlermeldungen)
- Unit-Tests bzw. einfache Testfälle für Argument-Parsing definieren

### ⏭ Nächste Schritte
- Speicherfreigabe `(config_free)` implementieren
- Übergang von Argument-Parsing zu Traversal-Logik vorbereiten
- Beginn der Verzeichnisdurchquerung basierend auf `config_t`
