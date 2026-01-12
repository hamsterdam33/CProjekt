## ✅ TODO / Roadmap (mit Aufwand)

### Must-have (MVP abschließen)
- [ ] `config_free()` implementieren (S)
  - Speicher für `start_dirs` (strdup) sauber freigeben, ggf. später weitere dynamische Felder.
- [ ] `parse_arguments()` härten & finalisieren (S–M)
  - Default-Startdir `"."` wenn kein Startdir angegeben.
  - Saubere Fehlermeldungen (missing arg / invalid value / unknown option).
  - Optional: Mehrere Startdirs unterstützen und korrekt sammeln (falls noch nicht sauber final).
- [ ] `-mindepth` & `-maxdepth` korrekt integrieren (M)
  - `maxdepth`: Rekursion früh stoppen (Traversal-Abbruch).
  - `mindepth`: nur Ausgabe ab bestimmter Tiefe (Filter-/Print-Entscheidung).

### Weitere Filter / Optionen (je nach Aufgabenstellung)
- [ ] `-size` (M)
  - Parsing von Zahlen + Einheit (z.B. 512-byte blocks / bytes), Vergleichlogik.
- [ ] `-print` / Output-Format verbessern (S)
  - Aktuell immer `printf("%s\n", path)` – ggf. Optionen für Format / Baumdarstellung.
- [ ] Symlink-Verhalten definieren (S–M)
  - Aktuell: keine Follow-Symlinks (best-effort). Optional: `-L`/`-P` Verhalten.

### Robustheit / Edge Cases
- [ ] Fehlerbehandlung konsistent machen (S–M)
  - `opendir`/`lstat`/`snprintf` Fehler → stderr, Traversal läuft weiter.
- [ ] `PATH_MAX` / lange Pfade sauber behandeln (M)
  - Aktuell fester Buffer (`4096`). Optional: dynamische Pfadbildung.
- [ ] Unit-/Integration-Tests in `tests/` (M)
  - Test-fixtures + bash scripts, die erwartete Outputs prüfen.
- [ ] CI (GitHub Actions) für Build + Tests (M)
  - Build unter Ubuntu, Warnings anzeigen, Tests laufen lassen.

### Code-Qualität / Repo-Hygiene
- [ ] Warnings auf 0 bringen (S)
  - Unused Funktionen/Includes entfernen oder hinter Flags.
- [ ] `.gitignore` prüfen (S)
  - Keine Build-Artefakte committen (`*.o`, `mfind`, etc.).
- [ ] README final polish (S)
  - klare Usage Beispiele, Limitations, “Supported options” Tabelle.
